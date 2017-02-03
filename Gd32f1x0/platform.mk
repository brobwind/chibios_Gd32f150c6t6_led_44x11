# List of all the GD32F1x0 platform files.
ifeq ($(USE_SMART_BUILD),yes)
HALCONF := $(strip $(shell cat halconf.h | egrep -e "\#define"))

PLATFORMSRC := $(CHIBIOS)/os/hal/ports/common/ARMCMx/nvic.c \
				Gd32f1x0/hal_lld.c \
				Gd32f1x0/Gd32_dma.c \
				Gd32f1x0/hal_st_lld.c
			
ifneq ($(findstring HAL_USE_EXT TRUE,$(HALCONF)),)
PLATFORMSRC += Gd32f1x0/hal_ext_lld.c
PLATFORMSRC += Gd32f1x0/hal_ext_lld_isr.c
endif
ifneq ($(findstring HAL_USE_PAL TRUE,$(HALCONF)),)
PLATFORMSRC += Gd32f1x0/hal_pal_lld.c
endif
ifneq ($(findstring HAL_USE_GPT TRUE,$(HALCONF)),)
PLATFORMSRC += Gd32f1x0/hal_gpt_lld.c
endif
ifneq ($(findstring HAL_USE_SERIAL TRUE,$(HALCONF)),)
PLATFORMSRC += Gd32f1x0/hal_serial_lld.c
endif
ifneq ($(findstring HAL_USE_UART TRUE,$(HALCONF)),)
PLATFORMSRC += Gd32f1x0/hal_uart_lld.c
endif
ifneq ($(findstring HAL_USE_USB TRUE,$(HALCONF)),)
PLATFORMSRC += Gd32f1x0/hal_usb_lld.c
PLATFORMSRC += $(CHIBIOS)/os/hal/ports/STM32/LLD/USBv1/hal_usb_lld.c
endif

endif

# Required include directories
PLATFORMINC := $(CHIBIOS)/os/hal/ports/common/ARMCMx
PLATFORMINC += Gd32f1x0
