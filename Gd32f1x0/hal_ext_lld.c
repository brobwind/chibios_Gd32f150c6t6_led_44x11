#include "hal.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EXTD1 driver identifier.
 */
EXTDriver EXTD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level EXT driver initialization.
 *
 * @notapi
 */
void ext_lld_init(void) {

  /* Driver initialization.*/
  extObjectInit(&EXTD1);
}

/**
 * @brief   Configures and activates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_start(EXTDriver *extp) {
  expchannel_t line;

  if (extp->state == EXT_STOP)
    ext_lld_exti_irq_enable();

  /* Configuration of automatic channels.*/
  for (line = 0; line < EXT_MAX_CHANNELS; line++)
    if (extp->config->channels[line].mode & EXT_CH_MODE_AUTOSTART)
      ext_lld_channel_enable(extp, line);
    else
      ext_lld_channel_disable(extp, line);
}

/**
 * @brief   Deactivates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_stop(EXTDriver *extp) {

  if (extp->state == EXT_ACTIVE)
    ext_lld_exti_irq_disable();

  EXTI->EER = 0;
  EXTI->IER = GD32_EXTI_IMR_MASK;
  EXTI->PD  = ~GD32_EXTI_IMR_MASK;
}

/**
 * @brief   Enables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be enabled
 *
 * @notapi
 */
void ext_lld_channel_enable(EXTDriver *extp, expchannel_t channel) {
  uint32_t cmask = (1 << (channel & 0x1F));

  /* Setting the associated GPIO for external channels.*/
  if (channel < 16) {
    uint32_t n = channel >> 2;
    uint32_t mask = ~(0xF << ((channel & 3) * 4));
    uint32_t port = ((extp->config->channels[channel].mode &
                      EXT_MODE_GPIO_MASK) >>
                     EXT_MODE_GPIO_OFF) << ((channel & 3) * 4);

    SYSCFG->EXTISS[n] = (SYSCFG->EXTISS[n] & mask) | port;
  }

    /* Masked out lines must not be touched by this driver.*/
    if ((cmask & GD32_EXTI_IMR_MASK) != 0U) {
      return;
    }

    /* Programming edge registers.*/
    if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
      EXTI->RTE |= cmask;
    else
      EXTI->RTE &= ~cmask;
    if (extp->config->channels[channel].mode & EXT_CH_MODE_FALLING_EDGE)
      EXTI->FTE |= cmask;
    else
      EXTI->FTE &= ~cmask;

    /* Programming interrupt and event registers.*/
    if (extp->config->channels[channel].cb != NULL) {
      EXTI->IER |= cmask;
      EXTI->EER &= ~cmask;
    }
    else {
      EXTI->EER |= cmask;
      EXTI->IER &= ~cmask;
    }
}

/**
 * @brief   Disables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be disabled
 *
 * @notapi
 */
void ext_lld_channel_disable(EXTDriver *extp, expchannel_t channel) {
  uint32_t cmask = (1 << (channel & 0x1F));

  (void)extp;

    EXTI->IER  &= ~cmask;
    EXTI->EER  &= ~cmask;
    EXTI->RTE &= ~cmask;
    EXTI->FTE &= ~cmask;
    EXTI->PD    =  cmask;
}

#endif /* HAL_USE_EXT */


