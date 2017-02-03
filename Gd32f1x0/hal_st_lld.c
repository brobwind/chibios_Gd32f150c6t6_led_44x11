#include "hal.h"

#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
#if (OSAL_ST_RESOLUTION == 32)
#define ST_ARR_INIT                         0xFFFFFFFF
#else
#define ST_ARR_INIT                         0x0000FFFF
#endif

#if GD32_ST_USE_TIMER == 2
#if (OSAL_ST_RESOLUTION == 32) && !GD32_TIM2_IS_32BITS
#error "TIM2 is not a 32bits timer"
#endif

#if defined(GD32_TIM2_IS_USED)
#error "ST requires TIM2 but the timer is already used"
#else
#define GD32_TIM2_IS_USED
#endif

#define ST_HANDLER                          GD32_TIM2_HANDLER
#define ST_NUMBER                           GD32_TIM2_NUMBER
#define ST_CLOCK_SRC                        GD32_TIMCLK1
#define ST_ENABLE_CLOCK()                   rccEnableTIM2(FALSE)
#define ST_ENABLE_STOP()                    MCUDBG->APB1FZ |= MCUDBG_APB1_FZ_DBG_TIMER2_STOP

#elif GD32_ST_USE_TIMER == 3
#if (OSAL_ST_RESOLUTION == 32) && !GD32_TIM3_IS_32BITS
#error "TIM3 is not a 32bits timer"
#endif

#if defined(GD32_TIM3_IS_USED)
#error "ST requires TIM3 but the timer is already used"
#else
#define GD32_TIM3_IS_USED
#endif

#define ST_HANDLER                          GD32_TIM3_HANDLER
#define ST_NUMBER                           GD32_TIM3_NUMBER
#define ST_CLOCK_SRC                        GD32_TIMCLK1
#define ST_ENABLE_CLOCK()                   rccEnableTIM3(FALSE)
#define ST_ENABLE_STOP()                    MCUDBG->APB1FZ |= MCUDBG_APB1_FZ_DBG_TIMER3_STOP
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC

#if GD32_HCLK % OSAL_ST_FREQUENCY != 0
#error "the selected ST frequency is not obtainable because integer rounding"
#endif

#if (GD32_HCLK / OSAL_ST_FREQUENCY) - 1 > 0xFFFFFF
#error "the selected ST frequency is not obtainable because SysTick timer counter limits"
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)
/**
 * @brief   TIM2 interrupt handler.
 * @details This interrupt is used for system tick in free running mode.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(ST_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Note, under rare circumstances an interrupt can remain latched even if
     the timer SR register has been cleared, in those cases the interrupt
     is simply ignored.*/
  if ((GD32_ST_TIM->SR & GD32_TIM_SR_CC1IF) != 0U) {
    GD32_ST_TIM->SR = 0U;

    osalSysLockFromISR();
    osalOsTimerHandlerI();
    osalSysUnlockFromISR();
  }

  OSAL_IRQ_EPILOGUE();
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) || defined(__DOXYGEN__)
/**
 * @brief   System Timer vector.
 * @details This interrupt is used for system tick in periodic mode.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SysTick_Handler) {

  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GD driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
  /* Free running counter mode.*/

  /* Enabling timer clock.*/
  ST_ENABLE_CLOCK();

  /* Enabling the stop mode during debug for this timer.*/
  ST_ENABLE_STOP();

  /* Initializing the counter in free running mode.*/
  GD32_ST_TIM->PSC    = (ST_CLOCK_SRC / OSAL_ST_FREQUENCY) - 1;
  GD32_ST_TIM->ARR    = ST_ARR_INIT;
  GD32_ST_TIM->CCMR1  = 0;
  GD32_ST_TIM->CCR[0] = 0;
  GD32_ST_TIM->DIER   = 0;
  GD32_ST_TIM->CR2    = 0;
  GD32_ST_TIM->EGR    = GD32_TIM_EGR_UG;
  GD32_ST_TIM->CR1    = GD32_TIM_CR1_CEN;

  /* IRQ enabled.*/
  nvicEnableVector(ST_NUMBER, GD32_ST_IRQ_PRIORITY);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
  /* Periodic systick mode, the Cortex-Mx internal systick timer is used
     in this mode.*/
  SysTick->LOAD = (GD32_HCLK / OSAL_ST_FREQUENCY) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* IRQ enabled.*/
  nvicSetSystemHandlerPriority(HANDLER_SYSTICK, GD32_ST_IRQ_PRIORITY);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
}

#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */
