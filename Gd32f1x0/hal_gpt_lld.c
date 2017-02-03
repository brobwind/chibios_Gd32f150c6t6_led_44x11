#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD2 driver identifier.
 * @note    The driver GPTD2 allocates the timer TIM2 when enabled.
 */
#if GD32_GPT_USE_TIM2 || defined(__DOXYGEN__)
GPTDriver GPTD2;
#endif

/**
 * @brief   GPTD3 driver identifier.
 * @note    The driver GPTD3 allocates the timer TIM3 when enabled.
 */
#if GD32_GPT_USE_TIM3 || defined(__DOXYGEN__)
GPTDriver GPTD3;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if GD32_GPT_USE_TIM2 || defined(__DOXYGEN__)
#if !defined(GD32_TIM2_SUPPRESS_ISR)
#if !defined(GD32_TIM2_HANDLER)
#error "GD32_TIM2_HANDLER not defined"
#endif
/**
 * @brief   TIM2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIM2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD2);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIM2_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIM2 */

#if GD32_GPT_USE_TIM3 || defined(__DOXYGEN__)
#if !defined(GD32_TIM3_SUPPRESS_ISR)
#if !defined(GD32_TIM3_HANDLER)
#error "GD32_TIM3_HANDLER not defined"
#endif
/**
 * @brief   TIM3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIM3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD3);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIM3_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIM3 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GPT driver initialization.
 *
 * @notapi
 */
void gpt_lld_init(void) {

#if GD32_GPT_USE_TIM2
  /* Driver initialization.*/
  GPTD2.tim = GD32_TIM2;
  gptObjectInit(&GPTD2);
#endif

#if GD32_GPT_USE_TIM3
  /* Driver initialization.*/
  GPTD3.tim = GD32_TIM3;
  gptObjectInit(&GPTD3);
#endif
}

/**
 * @brief   Configures and activates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_start(GPTDriver *gptp) {
  uint16_t psc;

  if (gptp->state == GPT_STOP) {
    /* Clock activation.*/
#if GD32_GPT_USE_TIM2
    if (&GPTD2 == gptp) {
      rccEnableTIM2(FALSE);
      rccResetTIM2();
#if !defined(GD32_TIM2_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIM2_NUMBER, GD32_GPT_TIM2_IRQ_PRIORITY);
#endif
#if defined(GD32_TIM2CLK)
      gptp->clock = GD32_TIM2CLK;
#else
      gptp->clock = GD32_TIMCLK1;
#endif
    }
#endif

#if GD32_GPT_USE_TIM3
    if (&GPTD3 == gptp) {
      rccEnableTIM3(FALSE);
      rccResetTIM3();
#if !defined(GD32_TIM3_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIM3_NUMBER, GD32_GPT_TIM3_IRQ_PRIORITY);
#endif
#if defined(GD32_TIM3CLK)
      gptp->clock = GD32_TIM3CLK;
#else
      gptp->clock = GD32_TIMCLK1;
#endif
    }
#endif
  }

  /* Prescaler value calculation.*/
  psc = (uint16_t)((gptp->clock / gptp->config->frequency) - 1);
  osalDbgAssert(((uint32_t)(psc + 1) * gptp->config->frequency) == gptp->clock,
                "invalid frequency");

  /* Timer configuration.*/
  gptp->tim->CR1  = 0;                          /* Initially stopped.       */
  gptp->tim->CR2  = gptp->config->cr2;
  gptp->tim->PSC  = psc;                        /* Prescaler value.         */
  gptp->tim->SR   = 0;                          /* Clear pending IRQs.      */
  gptp->tim->DIER = gptp->config->dier &        /* DMA-related DIER bits.   */
                    ~GD32_TIM_DIER_IRQ_MASK;
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop(GPTDriver *gptp) {

  if (gptp->state == GPT_READY) {
    gptp->tim->CR1  = 0;                        /* Timer disabled.          */
    gptp->tim->DIER = 0;                        /* All IRQs disabled.       */
    gptp->tim->SR   = 0;                        /* Clear pending IRQs.      */

#if GD32_GPT_USE_TIM2
    if (&GPTD2 == gptp) {
#if !defined(GD32_TIM2_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIM2_NUMBER);
#endif
      rccDisableTIM2(FALSE);
    }
#endif

#if GD32_GPT_USE_TIM3
    if (&GPTD3 == gptp) {
#if !defined(GD32_TIM3_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIM3_NUMBER);
#endif
      rccDisableTIM3(FALSE);
    }
#endif
  }
}

/**
 * @brief   Starts the timer in continuous mode.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  period in ticks
 *
 * @notapi
 */
void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t interval) {

  gptp->tim->ARR   = (uint32_t)(interval - 1);  /* Time constant.           */
  gptp->tim->EGR   = GD32_TIM_EGR_UG;          /* Update event.            */
  gptp->tim->CNT   = 0;                         /* Reset counter.           */

  /* NOTE: After generating the UG event it takes several clock cycles before
     SR bit 0 goes to 1. This is because the clearing of CNT has been inserted
     before the clearing of SR, to give it some time.*/
  gptp->tim->SR    = 0;                         /* Clear pending IRQs.      */
  if (NULL != gptp->config->callback)
    gptp->tim->DIER |= GD32_TIM_DIER_UIE;      /* Update Event IRQ enabled.*/
  gptp->tim->CR1 = GD32_TIM_CR1_ARPE | GD32_TIM_CR1_URS | GD32_TIM_CR1_CEN;
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop_timer(GPTDriver *gptp) {

  gptp->tim->CR1   = 0;                         /* Initially stopped.       */
  gptp->tim->SR    = 0;                         /* Clear pending IRQs.      */

  /* All interrupts disabled.*/
  gptp->tim->DIER &= ~GD32_TIM_DIER_IRQ_MASK;
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 * @details This function specifically polls the timer waiting for completion
 *          in order to not have extra delays caused by interrupt servicing,
 *          this function is only recommended for short delays.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @notapi
 */
void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval) {

  gptp->tim->ARR  = (uint32_t)(interval - 1);   /* Time constant.           */
  gptp->tim->EGR  = GD32_TIM_EGR_UG;           /* Update event.            */
  gptp->tim->SR   = 0;                          /* Clear pending IRQs.      */
  gptp->tim->CR1  = GD32_TIM_CR1_OPM | GD32_TIM_CR1_URS | GD32_TIM_CR1_CEN;
  while (!(gptp->tim->SR & GD32_TIM_SR_UIF))
    ;
  gptp->tim->SR   = 0;                          /* Clear pending IRQs.      */
}

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_serve_interrupt(GPTDriver *gptp) {

  gptp->tim->SR = 0;
  if (gptp->state == GPT_ONESHOT) {
    gptp->state = GPT_READY;                /* Back in GPT_READY state.     */
    gpt_lld_stop_timer(gptp);               /* Timer automatically stopped. */
  }
  gptp->config->callback(gptp);
}

#endif /* HAL_USE_GPT */
