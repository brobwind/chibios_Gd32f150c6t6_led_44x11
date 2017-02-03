#ifndef HAL_LLD_H
#define HAL_LLD_H

#include "Gd32_registry.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Platform identification macros
 * @{
 */
#if defined(GD32F150x6) || defined(__DOXYGEN__)
#define PLATFORM_NAME           "GD32F150x6 Value Line Low Density devices"
#else
#error "GD32F1x0 device not specified"
#endif
/** @} */

/**
 * @brief   AHB frequency.
 */
#define GD32_HCLK                  72000000

/**
 * @brief   APB frequency.
 */
#define GD32_PCLK                  (GD32_HCLK / 1)

/**
 * @brief   USART2 frequency.
 */
#define GD32_USART2CLK             GD32_PCLK

/**
 * @brief   Timers clock.
 */
#define GD32_TIMCLK1               (GD32_PCLK * 1)


/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Various helpers.*/
#include "nvic.h"
#include "Gd32_isr.h"
#include "Gd32_dma.h"
#include "Gd32_rcc.h"

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
  void Gd32_clock_init(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

