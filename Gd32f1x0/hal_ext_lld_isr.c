#include "hal.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

#include "hal_ext_lld_isr.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   EXTI[0]...EXTI[1] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector54) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PD;
  pr &= ((1U << 0) | (1U << 1));
  EXTI->PD = pr;
  if (pr & (1U << 0))
    EXTD1.config->channels[0].cb(&EXTD1, 0);
  if (pr & (1U << 1))
    EXTD1.config->channels[1].cb(&EXTD1, 1);

  OSAL_IRQ_EPILOGUE();
}
#endif /* HAL_USE_EXT */

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables EXTI IRQ sources.
 *
 * @notapi
 */
void ext_lld_exti_irq_enable(void) {

  nvicEnableVector(EXTI0_1_IRQn, GD32_EXT_EXTI0_1_IRQ_PRIORITY);
}

/**
 * @brief   Disables EXTI IRQ sources.
 *
 * @notapi
 */
void ext_lld_exti_irq_disable(void) {

  nvicDisableVector(EXTI0_1_IRQn);
}

#endif /* HAL_USE_EXT */

