#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void initgpio(Gd32_gpio_t *gpiop, const Gd32_gpio_setup_t *config) {

  gpiop->OTYPER  = config->otyper;
  gpiop->OSPEEDR = config->ospeedr;
  gpiop->PUPDR   = config->pupdr;
  gpiop->ODR     = config->odr;
  gpiop->AFRL    = config->afrl;
  gpiop->AFRH    = config->afrh;
  gpiop->MODER   = config->moder;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   GD32 I/O ports configuration.
 * @details Ports A-D(E, F, G, H) clocks enabled.
 *
 * @param[in] config    the GD32 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

  /*
   * Enables the GPIO related clocks.
   */
  rccEnableAHB(GD32_GPIO_EN_MASK, TRUE);

  /*
   * Initial GPIO setup.
   */
#if GD32_HAS_GPIOA
  initgpio(GPIOA, &config->PAData);
#endif
#if GD32_HAS_GPIOB
  initgpio(GPIOB, &config->PBData);
#endif
#if GD32_HAS_GPIOC
  initgpio(GPIOC, &config->PCData);
#endif
#if GD32_HAS_GPIOD
  initgpio(GPIOD, &config->PDData);
#endif
#if GD32_HAS_GPIOE
  initgpio(GPIOE, &config->PEData);
#endif
#if GD32_HAS_GPIOF
  initgpio(GPIOF, &config->PFData);
#endif
#if GD32_HAS_GPIOG
  initgpio(GPIOG, &config->PGData);
#endif
#if GD32_HAS_GPIOH
  initgpio(GPIOH, &config->PHData);
#endif
#if GD32_HAS_GPIOI
  initgpio(GPIOI, &config->PIData);
#endif
#if GD32_HAS_GPIOJ
  initgpio(GPIOJ, &config->PJData);
#endif
#if GD32_HAS_GPIOK
  initgpio(GPIOK, &config->PKData);
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull at minimum
 *          speed.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {

  uint32_t moder   = (mode & PAL_GD32_MODE_MASK) >> 0;
  uint32_t otyper  = (mode & PAL_GD32_OTYPE_MASK) >> 2;
  uint32_t ospeedr = (mode & PAL_GD32_OSPEED_MASK) >> 3;
  uint32_t pupdr   = (mode & PAL_GD32_PUPDR_MASK) >> 5;
  uint32_t altr    = (mode & PAL_GD32_ALTERNATE_MASK) >> 7;
  uint32_t bit     = 0;
  while (true) {
    if ((mask & 1) != 0) {
      uint32_t altrmask, m1, m2, m4;

      altrmask = altr << ((bit & 7) * 4);
      m1 = 1 << bit;
      m2 = 3 << (bit * 2);
      m4 = 15 << ((bit & 7) * 4);
      port->OTYPER  = (port->OTYPER & ~m1) | otyper;
      port->OSPEEDR = (port->OSPEEDR & ~m2) | ospeedr;
      port->PUPDR   = (port->PUPDR & ~m2) | pupdr;
      if (moder == PAL_GD32_MODE_ALTERNATE) {
        /* If going in alternate mode then the alternate number is set
           before switching mode in order to avoid glitches.*/
        if (bit < 8)
          port->AFRL = (port->AFRL & ~m4) | altrmask;
        else
          port->AFRH = (port->AFRH & ~m4) | altrmask;
        port->MODER   = (port->MODER & ~m2) | moder;
      }
      else {
        /* If going into a non-alternate mode then the mode is switched
           before setting the alternate mode in order to avoid glitches.*/
        port->MODER   = (port->MODER & ~m2) | moder;
        if (bit < 8)
          port->AFRL = (port->AFRL & ~m4) | altrmask;
        else
          port->AFRH = (port->AFRH & ~m4) | altrmask;
      }
    }
    mask >>= 1;
    if (!mask)
      return;
    otyper <<= 1;
    ospeedr <<= 2;
    pupdr <<= 2;
    moder <<= 2;
    bit++;
  }
}

#endif /* HAL_USE_PAL */

