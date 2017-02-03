#ifndef HAL_ST_LLD_H
#define HAL_ST_LLD_H

#include "mcuconf.h"
#include "Gd32_registry.h"
#include "Gd32_tim.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SysTick timer IRQ priority.
 */
#if !defined(GD32_ST_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ST_IRQ_PRIORITY               8
#endif

/**
 * @brief   TIMx unit (by number) to be used for free running operations.
 * @note    You must select a 32 bits timer if a 32 bits @p systick_t type
 *          is required.
 * @note    Timers 2, 3, 4 and 5 are supported.
 */
#if !defined(GD32_ST_USE_TIMER) || defined(__DOXYGEN__)
#define GD32_ST_USE_TIMER                  2
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if GD32_ST_USE_TIMER == 2
#if !GD32_HAS_TIM2
#error "TIM2 not present"
#endif
#define GD32_ST_TIM                              GD32_TIM2

#elif GD32_ST_USE_TIMER == 3
#if !GD32_HAS_TIM3
#error "TIM3 not present"
#endif
#define GD32_ST_TIM                              GD32_TIM3

#else
#error "GD32_ST_USE_TIMER specifies an unsupported timer"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void st_lld_init(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Returns the time counter value.
 *
 * @return              The counter value.
 *
 * @notapi
 */
static inline systime_t st_lld_get_counter(void) {

  return (systime_t)GD32_ST_TIM->CNT;
}

/**
 * @brief   Starts the alarm.
 * @note    Makes sure that no spurious alarms are triggered after
 *          this call.
 *
 * @param[in] time      the time to be set for the first alarm
 *
 * @notapi
 */
static inline void st_lld_start_alarm(systime_t time) {

  GD32_ST_TIM->CCR[0] = (uint32_t)time;
  GD32_ST_TIM->SR     = 0;
  GD32_ST_TIM->DIER   = GD32_TIM_DIER_CC1IE;
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
static inline void st_lld_stop_alarm(void) {

  GD32_ST_TIM->DIER = 0;
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] time      the time to be set for the next alarm
 *
 * @notapi
 */
static inline void st_lld_set_alarm(systime_t time) {

  GD32_ST_TIM->CCR[0] = (uint32_t)time;
}

/**
 * @brief   Returns the current alarm time.
 *
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t st_lld_get_alarm(void) {

  return (systime_t)GD32_ST_TIM->CCR[0];
}

/**
 * @brief   Determines if the alarm is active.
 *
 * @return              The alarm status.
 * @retval false        if the alarm is not active.
 * @retval true         is the alarm is active
 *
 * @notapi
 */
static inline bool st_lld_is_alarm_active(void) {

  return (bool)((GD32_ST_TIM->DIER & GD32_TIM_DIER_CC1IE) != 0);
}

#endif /* HAL_ST_LLD_H */


