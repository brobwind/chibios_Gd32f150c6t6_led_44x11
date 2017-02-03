#include "hal.h"

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

/**
 * @brief   Initializes the backup domain.
 * @note    WARNING! Changing clock source impossible without resetting
 *          of the whole BKP domain.
 */
static void hal_lld_backup_domain_init(void) {
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* Reset of all peripherals.*/
  rccResetAHB(0xFFFFFFFF);
  rccResetAPB1(0xFFFFFFFF);

  /* PWR clock enabled.*/
  rccEnablePWRInterface(FALSE);

  /* Initializes the backup domain.*/
  hal_lld_backup_domain_init();

#if defined(GD32_DMA_REQUIRED)
  dmaInit();
#endif
}

/**
 * @brief   GD32 clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
void Gd32_clock_init(void) {
#if !GD32_NO_INIT
	/* Set HSIEN bit */
	RCC->GCCR |= RCC_GCCR_HSIEN;

	/* Reset SCS[1:0], AHBPS[3:0], APB1PS[2:0], APB2PS[2:0]
	ADCPS[1:0], USBPS[1:0], CKOTUSRC[2:0], CKOUTDIV, and PLLDV bits */
	#define RCC_GCFGR_FREQ_DIVIDE_RESET		0x083F000C

	/* Reset HSEEN, CKMEN and PLLEN bits */
	#define RCC_GCCR_CLK_ENABLE_RESET		0xFEF6FFFF

	/* Reset HSEBPS bit */
	#define RCC_GCCR_HSEBPS_RESET			0xFFFBFFFF

	/* Reset PLLSEL, PLLPREDV and PLLMF[3:0] bits */
	#define RCC_GCFGR_CLK_SELECT_RESET		0xF7C0FFFF

	/* Reset HSEPREDV1[3:0] bits */
	#define RCC_GCFGR2_HSEPREDV1_RESET		0xFFFFFFF0

	/* Reset USARTSEL[1:0], I2CSEL, CECSEL and ADCSEL bits */
	#define RCC_GCFGR3_RESET				0xFFFFFEBC

	/* Reset HSI14 bit */
	#define RCC_GCCR2_HSI14_RESET			0xFFFFFFFE

	/* Disable all interrupts */
	#define RCC_GCIR_DISABLE				0x00000000

	/* Reset SCS[1:0], AHBPS[3:0], APB1PS[2:0], APB2PS[2:0],
	   ADCPS[1:0], USBPS[1:0], CKOTUSRC[2:0], CKOUTDIV, and PLLDV bits */
	RCC->GCFGR &= RCC_GCFGR_FREQ_DIVIDE_RESET;

	/* Reset HSEEN, CKMEN and PLLEN bits */
	RCC->GCCR &= RCC_GCCR_CLK_ENABLE_RESET;

	/* Reset HSEBPS bit */
	RCC->GCCR &= RCC_GCCR_HSEBPS_RESET;

	/* Reset PLLSEL, PLLPREDV and PLLMF[3:0] bits */
	RCC->GCFGR &= RCC_GCFGR_CLK_SELECT_RESET;

	/* Reset HSEPREDV1[3:0] bits */
	RCC->GCFGR2 &= RCC_GCFGR2_HSEPREDV1_RESET;

	/* Reset USARTSEL[1:0], CECSEL and ADCSEL bits */
	RCC->GCFGR3 &= RCC_GCFGR3_RESET;

	/* Reset HSI14 bit */
	RCC->GCCR2 &= RCC_GCCR2_HSI14_RESET;

	/* Disable all interrupts */
	RCC->GCIR = RCC_GCIR_DISABLE;

	/* ======================================================================= */
	/* Configures the system clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
	/* ======================================================================= */

	/* AHB = SYSCLK not divided */
	RCC->GCFGR |= RCC_GCFGR_AHBPS_DIV1;

	/* APB2 = AHB not divided */
	RCC->GCFGR |= RCC_GCFGR_APB2PS_DIV1;

	/* APB1 = AHB not divided */
	RCC->GCFGR |= RCC_GCFGR_APB1PS_DIV1;

	/* PLL multiplication factor is 18, PLL = HSI_DIV2 * 18 = 72MHz */
	RCC->GCFGR &= ~(RCC_GCFGR_PLLSEL | RCC_GCFGR_PLLPREDV | RCC_GCFGR_PLLMF);
	RCC->GCFGR |= RCC_GCFGR_PLLSEL_HSI_DIV2 | RCC_GCFGR_PLLPREDV_PREDIV1 | RCC_GCFGR_PLLMF18;

	/* PLL Enable */
	RCC->GCCR |= RCC_GCCR_PLLEN;

	/* Wait until PLL is ready */
	while (!(RCC->GCCR & RCC_GCCR_PLLSTB)) {}

	/* PLL is choosen as system clock source */
	RCC->GCFGR &= ~RCC_GCFGR_SCS;
	RCC->GCFGR |= RCC_GCFGR_SCS_PLL;

	/* Wait until PLL is chosen as system clock source */
	while ((RCC->GCFGR & RCC_GCFGR_SCSS) != RCC_GCFGR_SCSS_PLL) {}
#endif /* !GD32_NO_INIT */
}
