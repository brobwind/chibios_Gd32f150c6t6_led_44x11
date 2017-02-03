#ifndef GD32_REGISTRY_H
#define GD32_REGISTRY_H

#if !defined(GD32F1X0) || defined(__DOXYGEN__)
#define GD32F1X0
#endif

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    GD32F1xx capabilities
 * @{
 */
/*===========================================================================*/
/* GD32F150x6                                                                */
/*===========================================================================*/
#if defined(GD32F150x6)

/* Common identifier of all GD32F030 devices.*/
#define GD32F150


/* EXTI attributes.*/
#define GD32_EXTI_NUM_LINES                20
#define GD32_EXTI_IMR_MASK                 0xFFF50000U

/* GPIO attributes.*/
#define GD32_HAS_GPIOA                     TRUE
#define GD32_HAS_GPIOB                     TRUE
#define GD32_HAS_GPIOC                     TRUE
#define GD32_HAS_GPIOD                     FALSE
#define GD32_HAS_GPIOE                     FALSE
#define GD32_HAS_GPIOF                     TRUE
#define GD32_HAS_GPIOG                     FALSE
#define GD32_HAS_GPIOH                     FALSE
#define GD32_HAS_GPIOI                     FALSE
#define GD32_HAS_GPIOJ                     FALSE
#define GD32_HAS_GPIOK                     FALSE

#define GD32_GPIO_EN_MASK                  (RCC_AHBCCR_PAEN |            \
                                             RCC_AHBCCR_PBEN |           \
                                             RCC_AHBCCR_PCEN |           \
                                             RCC_AHBCCR_PFEN)

/* TIM attributes.*/
#define GD32_TIM_MAX_CHANNELS              4

#define GD32_HAS_TIM2                      TRUE
#define GD32_TIM2_IS_32BITS                TRUE
#define GD32_TIM2_CHANNELS                 4

#define GD32_HAS_TIM3                      TRUE
#define GD32_TIM3_IS_32BITS                FALSE
#define GD32_TIM3_CHANNELS                 4

#else
#error "GD32F1x0 device not specified"
#endif

#endif /* GD32_REGISTRY_H */
