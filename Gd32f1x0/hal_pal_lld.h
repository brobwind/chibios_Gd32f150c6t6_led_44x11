#ifndef HAL_PAL_LLD_H
#define HAL_PAL_LLD_H

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

#undef PAL_MODE_RESET
#undef PAL_MODE_UNCONNECTED
#undef PAL_MODE_INPUT
#undef PAL_MODE_INPUT_PULLUP
#undef PAL_MODE_INPUT_PULLDOWN
#undef PAL_MODE_INPUT_ANALOG
#undef PAL_MODE_OUTPUT_PUSHPULL
#undef PAL_MODE_OUTPUT_OPENDRAIN

/**
 * @name    GD32-specific I/O mode flags
 * @{
 */
#define PAL_GD32_MODE_MASK             (3U << 0U)
#define PAL_GD32_MODE_INPUT            (0U << 0U)
#define PAL_GD32_MODE_OUTPUT           (1U << 0U)
#define PAL_GD32_MODE_ALTERNATE        (2U << 0U)
#define PAL_GD32_MODE_ANALOG           (3U << 0U)

#define PAL_GD32_OTYPE_MASK            (1U << 2U)
#define PAL_GD32_OTYPE_PUSHPULL        (0U << 2U)
#define PAL_GD32_OTYPE_OPENDRAIN       (1U << 2U)

#define PAL_GD32_OSPEED_MASK           (3U << 3U)
#define PAL_GD32_OSPEED_LOWEST         (0U << 3U)
#define PAL_GD32_OSPEED_MID            (1U << 3U)
#define PAL_GD32_OSPEED_HIGHEST        (3U << 3U)

#define PAL_GD32_PUPDR_MASK            (3U << 5U)
#define PAL_GD32_PUPDR_FLOATING        (0U << 5U)
#define PAL_GD32_PUPDR_PULLUP          (1U << 5U)
#define PAL_GD32_PUPDR_PULLDOWN        (2U << 5U)

#define PAL_GD32_ALTERNATE_MASK        (15U << 7U)
#define PAL_GD32_ALTERNATE(n)          ((n) << 7U)

/**
 * @brief   Alternate function.
 *
 * @param[in] n         alternate function selector
 */
#define PAL_MODE_ALTERNATE(n)           (PAL_GD32_MODE_ALTERNATE |         \
                                         PAL_GD32_ALTERNATE(n))
/** @} */

/**
 * @name    Standard I/O mode flags
 * @{
 */
/**
 * @brief   This mode is implemented as input.
 */
#define PAL_MODE_RESET                  PAL_GD32_MODE_INPUT

/**
 * @brief   This mode is implemented as input with pull-up.
 */
#define PAL_MODE_UNCONNECTED            PAL_MODE_INPUT_PULLUP

/**
 * @brief   Regular input high-Z pad.
 */
#define PAL_MODE_INPUT                  PAL_GD32_MODE_INPUT

/**
 * @brief   Input pad with weak pull up resistor.
 */
#define PAL_MODE_INPUT_PULLUP           (PAL_GD32_MODE_INPUT |             \
                                         PAL_GD32_PUPDR_PULLUP)

/**
 * @brief   Input pad with weak pull down resistor.
 */
#define PAL_MODE_INPUT_PULLDOWN         (PAL_GD32_MODE_INPUT |             \
                                         PAL_GD32_PUPDR_PULLDOWN)

/**
 * @brief   Analog input mode.
 */
#define PAL_MODE_INPUT_ANALOG           PAL_GD32_MODE_ANALOG

/**
 * @brief   Push-pull output pad.
 */
#define PAL_MODE_OUTPUT_PUSHPULL        (PAL_GD32_MODE_OUTPUT |            \
                                         PAL_GD32_OTYPE_PUSHPULL)

/**
 * @brief   Open-drain output pad.
 */
#define PAL_MODE_OUTPUT_OPENDRAIN       (PAL_GD32_MODE_OUTPUT |            \
                                         PAL_GD32_OTYPE_OPENDRAIN)
/** @} */

/* Discarded definitions from the ST headers, the PAL driver uses its own
   definitions in order to have an unified handling for all devices.
   Unfortunately the ST headers have no uniform definitions for the same
   objects across the various sub-families.*/
#undef GPIOA
#undef GPIOB
#undef GPIOC
#undef GPIOD
#undef GPIOE
#undef GPIOF
#undef GPIOG
#undef GPIOH
#undef GPIOI
#undef GPIOJ
#undef GPIOK

/**
 * @name    GPIO ports definitions
 * @{
 */
#define GPIOA                           ((Gd32_gpio_t *)GPIOA_BASE)
#define GPIOB                           ((Gd32_gpio_t *)GPIOB_BASE)
#define GPIOC                           ((Gd32_gpio_t *)GPIOC_BASE)
#define GPIOD                           ((Gd32_gpio_t *)GPIOD_BASE)
#define GPIOE                           ((Gd32_gpio_t *)GPIOE_BASE)
#define GPIOF                           ((Gd32_gpio_t *)GPIOF_BASE)
#define GPIOG                           ((Gd32_gpio_t *)GPIOG_BASE)
#define GPIOH                           ((Gd32_gpio_t *)GPIOH_BASE)
#define GPIOI                           ((Gd32_gpio_t *)GPIOI_BASE)
#define GPIOJ                           ((Gd32_gpio_t *)GPIOJ_BASE)
#define GPIOK                           ((Gd32_gpio_t *)GPIOK_BASE)
/** @} */

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @name    Port related definitions
 * @{
 */
/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 16

/**
 * @brief   Whole port mask.
 * @details This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ((ioportmask_t)0xFFFF)
/** @} */

/**
 * @name    Line handling macros
 * @{
 */
/**
 * @brief   Forms a line identifier.
 * @details A port/pad pair are encoded into an @p ioline_t type. The encoding
 *          of this type is platform-dependent.
 * @note    In this driver the pad number is encoded in the lower 4 bits of
 *          the GPIO address which are guaranteed to be zero.
 */
#define PAL_LINE(port, pad)                                                 \
  ((ioline_t)((uint32_t)(port)) | ((uint32_t)(pad)))

/**
 * @brief   Decodes a port identifier from a line identifier.
 */
#define PAL_PORT(line)                                                      \
  ((Gd32_gpio_t *)(((uint32_t)(line)) & 0xFFFFFFF0U))

/**
 * @brief   Decodes a pad identifier from a line identifier.
 */
#define PAL_PAD(line)                                                       \
  ((uint32_t)((uint32_t)(line) & 0x0000000FU))

/**
 * @brief   Value identifying an invalid line.
 */
#define PAL_NOLINE                      0U
/** @} */

/**
 * @brief   GD32 GPIO registers block.
 */
typedef struct {

  volatile uint32_t     MODER;
  volatile uint32_t     OTYPER;
  volatile uint32_t     OSPEEDR;
  volatile uint32_t     PUPDR;
  volatile uint32_t     IDR;
  volatile uint32_t     ODR;
  volatile union {
    uint32_t            W;
    struct {
      uint16_t          set;
      uint16_t          clear;
    } H;
  } BSRR;
  volatile uint32_t     LCKR;
  volatile uint32_t     AFRL;
  volatile uint32_t     AFRH;
  volatile uint32_t     BRR;
} Gd32_gpio_t;

/**
 * @brief   GPIO port setup info.
 */
typedef struct {
  /** Initial value for MODER register.*/
  uint32_t              moder;
  /** Initial value for OTYPER register.*/
  uint32_t              otyper;
  /** Initial value for OSPEEDR register.*/
  uint32_t              ospeedr;
  /** Initial value for PUPDR register.*/
  uint32_t              pupdr;
  /** Initial value for ODR register.*/
  uint32_t              odr;
  /** Initial value for AFRL register.*/
  uint32_t              afrl;
  /** Initial value for AFRH register.*/
  uint32_t              afrh;
} Gd32_gpio_setup_t;

/**
 * @brief   GD32 GPIO static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialize the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
#if GD32_HAS_GPIOA || defined(__DOXYGEN__)
  /** @brief Port A setup data.*/
  Gd32_gpio_setup_t    PAData;
#endif
#if GD32_HAS_GPIOB || defined(__DOXYGEN__)
  /** @brief Port B setup data.*/
  Gd32_gpio_setup_t    PBData;
#endif
#if GD32_HAS_GPIOC || defined(__DOXYGEN__)
  /** @brief Port C setup data.*/
  Gd32_gpio_setup_t    PCData;
#endif
#if GD32_HAS_GPIOD || defined(__DOXYGEN__)
  /** @brief Port D setup data.*/
  Gd32_gpio_setup_t    PDData;
#endif
#if GD32_HAS_GPIOE || defined(__DOXYGEN__)
  /** @brief Port E setup data.*/
  Gd32_gpio_setup_t    PEData;
#endif
#if GD32_HAS_GPIOF || defined(__DOXYGEN__)
  /** @brief Port F setup data.*/
  Gd32_gpio_setup_t    PFData;
#endif
#if GD32_HAS_GPIOG || defined(__DOXYGEN__)
  /** @brief Port G setup data.*/
  Gd32_gpio_setup_t    PGData;
#endif
#if GD32_HAS_GPIOH || defined(__DOXYGEN__)
  /** @brief Port H setup data.*/
  Gd32_gpio_setup_t    PHData;
#endif
#if GD32_HAS_GPIOI || defined(__DOXYGEN__)
  /** @brief Port I setup data.*/
  Gd32_gpio_setup_t    PIData;
#endif
#if GD32_HAS_GPIOJ || defined(__DOXYGEN__)
  /** @brief Port I setup data.*/
  Gd32_gpio_setup_t    PJData;
#endif
#if GD32_HAS_GPIOK || defined(__DOXYGEN__)
  /** @brief Port I setup data.*/
  Gd32_gpio_setup_t    PKData;
#endif
} PALConfig;

/**
 * @brief   Type of digital I/O port sized unsigned integer.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Type of digital I/O modes.
 */
typedef uint32_t iomode_t;

/**
 * @brief   Type of an I/O line.
 */
typedef uint32_t ioline_t;

/**
 * @brief   Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef Gd32_gpio_t * ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver wraps the definitions already present in the GD32   */
/* firmware library.                                                         */
/*===========================================================================*/

/**
 * @brief   GPIO port A identifier.
 */
#if GD32_HAS_GPIOA || defined(__DOXYGEN__)
#define IOPORT1         GPIOA
#endif

/**
 * @brief   GPIO port B identifier.
 */
#if GD32_HAS_GPIOB || defined(__DOXYGEN__)
#define IOPORT2         GPIOB
#endif

/**
 * @brief   GPIO port C identifier.
 */
#if GD32_HAS_GPIOC || defined(__DOXYGEN__)
#define IOPORT3         GPIOC
#endif

/**
 * @brief   GPIO port D identifier.
 */
#if GD32_HAS_GPIOD || defined(__DOXYGEN__)
#define IOPORT4         GPIOD
#endif

/**
 * @brief   GPIO port E identifier.
 */
#if GD32_HAS_GPIOE || defined(__DOXYGEN__)
#define IOPORT5         GPIOE
#endif

/**
 * @brief   GPIO port F identifier.
 */
#if GD32_HAS_GPIOF || defined(__DOXYGEN__)
#define IOPORT6         GPIOF
#endif

/**
 * @brief   GPIO port G identifier.
 */
#if GD32_HAS_GPIOG || defined(__DOXYGEN__)
#define IOPORT7         GPIOG
#endif

/**
 * @brief   GPIO port H identifier.
 */
#if GD32_HAS_GPIOH || defined(__DOXYGEN__)
#define IOPORT8         GPIOH
#endif

/**
 * @brief   GPIO port I identifier.
 */
#if GD32_HAS_GPIOI || defined(__DOXYGEN__)
#define IOPORT9         GPIOI
#endif

/**
 * @brief   GPIO port J identifier.
 */
#if GD32_HAS_GPIOJ || defined(__DOXYGEN__)
#define IOPORT10        GPIOJ
#endif

/**
 * @brief   GPIO port K identifier.
 */
#if GD32_HAS_GPIOK || defined(__DOXYGEN__)
#define IOPORT11        GPIOK
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   GPIO ports subsystem initialization.
 *
 * @notapi
 */
#define pal_lld_init(config) _pal_lld_init(config)

/**
 * @brief   Reads an I/O port.
 * @details This function is implemented by reading the GPIO IDR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the application
 *          code.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) ((port)->IDR)

/**
 * @brief   Reads the output latch.
 * @details This function is implemented by reading the GPIO ODR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the application
 *          code.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 *
 * @notapi
 */
#define pal_lld_readlatch(port) ((port)->ODR)

/**
 * @brief   Writes on a I/O port.
 * @details This function is implemented by writing the GPIO ODR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits) ((port)->ODR = (bits))

/**
 * @brief   Sets a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be ORed on the specified port
 *
 * @notapi
 */
#define pal_lld_setport(port, bits) ((port)->BSRR.H.set = (uint16_t)(bits))

/**
 * @brief   Clears a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be cleared on the specified port
 *
 * @notapi
 */
#define pal_lld_clearport(port, bits) ((port)->BSRR.H.clear = (uint16_t)(bits))

/**
 * @brief   Writes a group of bits.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    the group bit offset within the port
 * @param[in] bits      bits to be written. Values exceeding the group
 *                      width are masked.
 *
 * @notapi
 */
#define pal_lld_writegroup(port, mask, offset, bits)                        \
  ((port)->BSRR.W = ((~(bits) & (mask)) << (16U + (offset))) |              \
                     (((bits) & (mask)) << (offset)))

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @notapi
 */
#define pal_lld_setgroupmode(port, mask, offset, mode)                      \
  _pal_lld_setgroupmode(port, mask << offset, mode)

/**
 * @brief   Writes a logical state on an output pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @notapi
 */
#define pal_lld_writepad(port, pad, bit) pal_lld_writegroup(port, 1, pad, bit)

extern const PALConfig pal_default_config;

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_init(const PALConfig *config);
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* HAL_PAL_LLD_H */


