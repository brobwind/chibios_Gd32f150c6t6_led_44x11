#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* For compatibility for those devices without LIN support in the USARTs.*/
#if !defined(USART_STR_LBDF)
#define USART_STR_LBDF                      0
#endif

#if !defined(USART_CTLR2_LBDIE)
#define USART_CTLR2_LBDIE                   0
#endif

/* GD32L0xx/GD32F7xx ST headers difference.*/
#if !defined(USART_STR_LBDF)
#define USART_STR_LBDF                      USART_STR_LBD
#endif

/* Handling differences in frame size bits.*/
#if !defined(USART_CTLR1_M_0)
#define USART_CTLR1_M_0                     (1 << 12)
#endif

#if !defined(USART_CTLR1_M_1)
#define USART_CTLR1_M_1                     (1 << 28)
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USART2 serial driver identifier.*/
#if GD32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config =
{
  SERIAL_DEFAULT_BITRATE,
  0,
  USART_CTLR2_STOP1_BITS,
  0
};

#if GD32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
/** @brief Input buffer for SD2.*/
static uint8_t sd_in_buf2[GD32_SERIAL_USART2_IN_BUF_SIZE];

/** @brief Output buffer for SD2.*/
static uint8_t sd_out_buf2[GD32_SERIAL_USART2_OUT_BUF_SIZE];
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart_init(SerialDriver *sdp, const SerialConfig *config) {
  USART_TypeDef *u = sdp->usart;

  /* Baud rate setting.*/
  u->BRR = (uint32_t)(sdp->clock / config->speed);

  /* Note that some bits are enforced.*/
  u->CTLR2 = config->cr2 | USART_CTLR2_LBDIE;
  u->CTLR3 = config->cr3 | USART_CTLR3_ERIE;
  u->CTLR1 = config->cr1 | USART_CTLR1_UEN | USART_CTLR1_PEIE |
                         USART_CTLR1_RBNEIE | USART_CTLR1_TEN |
                         USART_CTLR1_REN;
  u->SCR = 0xFFFFFFFFU;

  /* Deciding mask to be applied on the data register on receive, this is
     required in order to mask out the parity bit.*/
  if ((config->cr1 & USART_CTLR1_PCEN) != 0U) {
    switch (config->cr1 & (USART_CTLR1_M_1 | USART_CTLR1_M_0)) {
    case 0:
      sdp->rxmask = 0x7F;
      break;
    case USART_CTLR1_M_1:
      sdp->rxmask = 0x3F;
      break;
    default:
      sdp->rxmask = 0xFF;
    }
  }
  else {
    sdp->rxmask = 0xFF;
  }
}

/**
 * @brief   USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] u         pointer to an USART I/O block
 */
static void usart_deinit(USART_TypeDef *u) {

  u->CTLR1 = 0;
  u->CTLR2 = 0;
  u->CTLR3 = 0;
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] isr       USART ISR register value
 */
static void set_error(SerialDriver *sdp, uint32_t isr) {
  eventflags_t sts = 0;

  if (isr & USART_STR_ORE)
    sts |= SD_OVERRUN_ERROR;
  if (isr & USART_STR_PE)
    sts |= SD_PARITY_ERROR;
  if (isr & USART_STR_FE)
    sts |= SD_FRAMING_ERROR;
  if (isr & USART_STR_NE)
    sts |= SD_NOISE_ERROR;
  osalSysLockFromISR();
  chnAddFlagsI(sdp, sts);
  osalSysUnlockFromISR();
}

/**
 * @brief   Common IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the USART
 */
static void serve_interrupt(SerialDriver *sdp) {
  USART_TypeDef *u = sdp->usart;
  uint32_t cr1 = u->CTLR1;
  uint32_t isr;

  /* Reading and clearing status.*/
  isr = u->STR;
  u->SCR = isr;

  /* Error condition detection.*/
  if (isr & (USART_STR_ORE | USART_STR_NE | USART_STR_FE  | USART_STR_PE))
    set_error(sdp, isr);

  /* Special case, LIN break detection.*/
  if (isr & USART_STR_LBDF) {
    osalSysLockFromISR();
    chnAddFlagsI(sdp, SD_BREAK_DETECTED);
    osalSysUnlockFromISR();
  }

  /* Data available.*/
  if (isr & USART_STR_RBNE) {
    osalSysLockFromISR();
    sdIncomingDataI(sdp, (uint8_t)u->RDTR & sdp->rxmask);
    osalSysUnlockFromISR();
  }

  /* Transmission buffer empty.*/
  if ((cr1 & USART_CTLR1_TBEIE) && (isr & USART_STR_TBE)) {
    msg_t b;
    osalSysLockFromISR();
    b = oqGetI(&sdp->oqueue);
    if (b < MSG_OK) {
      chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
      u->CTLR1 = (cr1 & ~USART_CTLR1_TBEIE) | USART_CTLR1_TCIE;
    }
    else
      u->TDTR = b;
    osalSysUnlockFromISR();
  }

  /* Physical transmission end.*/
  if (isr & USART_STR_TC) {
    osalSysLockFromISR();
    if (oqIsEmptyI(&sdp->oqueue))
      chnAddFlagsI(sdp, CHN_TRANSMISSION_END);
    u->CTLR1 = cr1 & ~USART_CTLR1_TCIE;
    osalSysUnlockFromISR();
  }
}

#if GD32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
static void notify2(io_queue_t *qp) {

  (void)qp;
  USART2->CTLR1 |= USART_CTLR1_TBEIE;
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if GD32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
#if !defined(GD32_USART2_HANDLER)
#error "GD32_USART2_HANDLER not defined"
#endif
/**
 * @brief   USART2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USART2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  serve_interrupt(&SD2);

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {
#if GD32_SERIAL_USE_USART2
  sdObjectInit(&SD2);
  iqObjectInit(&SD2.iqueue, sd_in_buf2, sizeof sd_in_buf2, NULL, &SD2);
  oqObjectInit(&SD2.oqueue, sd_out_buf2, sizeof sd_out_buf2, notify2, &SD2);
  SD2.usart = USART2;
  SD2.clock = GD32_USART2CLK;
#if defined(GD32_USART2_NUMBER)
  nvicEnableVector(GD32_USART2_NUMBER, GD32_SERIAL_USART2_PRIORITY);
#endif
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL)
    config = &default_config;

  if (sdp->state == SD_STOP) {
#if GD32_SERIAL_USE_USART2
    if (&SD2 == sdp) {
      rccEnableUSART2(FALSE);
    }
#endif
  }
  usart_init(sdp, config);
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
    /* UART is de-initialized then clocks are disabled.*/
    usart_deinit(sdp->usart);

#if GD32_SERIAL_USE_USART2
    if (&SD2 == sdp) {
      rccDisableUSART2(FALSE);
      return;
    }
#endif
  }
}

#endif /* HAL_USE_SERIAL */
