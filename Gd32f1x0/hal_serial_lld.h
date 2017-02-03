#ifndef HAL_SERIAL_LLD_H
#define HAL_SERIAL_LLD_H

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Advanced buffering support switch.
 * @details This constants enables the advanced buffering support in the
 *          low level driver, the queue buffer is no more part of the
 *          @p SerialDriver structure, each driver can have a different
 *          queue size.
 */
#define SERIAL_ADVANCED_BUFFERING_SUPPORT   TRUE

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   USART2 interrupt priority level setting.
 */
#if !defined(GD32_SERIAL_USART2_PRIORITY) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART2_PRIORITY        12
#endif

/**
 * @brief   Input buffer size for USART2.
 */
#if !defined(GD32_SERIAL_USART2_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART2_IN_BUF_SIZE     SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for USART2.
 */
#if !defined(GD32_SERIAL_USART2_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART2_OUT_BUF_SIZE    SERIAL_BUFFERS_SIZE
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   GD32 Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct {
  /**
   * @brief Bit rate.
   */
  uint32_t                  speed;
  /* End of the mandatory fields.*/
  /**
   * @brief Initialization value for the CR1 register.
   */
  uint32_t                  cr1;
  /**
   * @brief Initialization value for the CR2 register.
   */
  uint32_t                  cr2;
  /**
   * @brief Initialization value for the CR3 register.
   */
  uint32_t                  cr3;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  input_queue_t             iqueue;                                         \
  /* Output queue.*/                                                        \
  output_queue_t            oqueue;                                         \
  /* End of the mandatory fields.*/                                         \
  /* Pointer to the USART registers block.*/                                \
  USART_TypeDef             *usart;                                         \
  /* Clock frequency for the associated USART/UART.*/                       \
  uint32_t                  clock;                                          \
  /* Mask to be applied on received frames.*/                               \
  uint8_t                   rxmask;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * Extra USARTs definitions here (missing from the ST header file).
 */
#define USART_CTLR2_STOP1_BITS    (0 << 12)   /**< @brief CR2 1 stop bit value.*/
#define USART_CTLR2_STOP0P5_BITS  (1 << 12)   /**< @brief CR2 0.5 stop bit value.*/
#define USART_CTLR2_STOP2_BITS    (2 << 12)   /**< @brief CR2 2 stop bit value.*/
#define USART_CTLR2_STOP1P5_BITS  (3 << 12)   /**< @brief CR2 1.5 stop bit value.*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_SERIAL_USE_USART2 && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL */

#endif /* HAL_SERIAL_LLD_H */
