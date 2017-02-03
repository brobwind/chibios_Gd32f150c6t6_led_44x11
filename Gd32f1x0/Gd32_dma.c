#include "hal.h"

/* The following macro is only defined if some driver requiring DMA services
   has been enabled.*/
#if defined(GD32_DMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   STM32 DMA helper initialization.
 *
 * @init
 */
void dmaInit(void) {
#if 0 /* TODO */
  int i;

  dma_streams_mask = 0U;
  for (i = 0; i < STM32_DMA_STREAMS; i++) {
    _stm32_dma_streams[i].channel->CCR = 0U;
    _stm32_dma_isr_redir[i].dma_func = NULL;
  }
  DMA1->IFCR = 0xFFFFFFFFU;
#if STM32_DMA2_NUM_CHANNELS > 0
  DMA2->IFCR = 0xFFFFFFFFU;
#endif
#endif /* TODO */
}

#endif /* GD32_DMA_REQUIRED */
