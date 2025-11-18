/**
  ******************************************************************************
  * @file    system_stm32l4xx.c
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device Peripheral Access Layer System Source File
  *
  *   This file provides two functions and one global variable to be called from
  *   user application:
  *      - SystemInit(): This function is called at startup just after reset and
  *                      before branch to main program. This call is made inside
  *                      the "startup_stm32l4xx.s" file.
  *
  *      - SystemCoreClock variable: Contains the core clock (HCLK), it can be used
  *                                  by the user application to setup the SysTick
  *                                  timer or configure other parameters.
  *
  *      - SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
  *                                 be called whenever the core clock is changed
  *                                 during program execution.
  *
  ******************************************************************************
  */

#include <stdint.h>

/* STM32L4xx specific defines */
#define RCC_BASE              0x40021000UL
#define FLASH_BASE            0x40022000UL
#define PWR_BASE              0x40007000UL

#define RCC_CR                (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_ICSCR             (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_CFGR              (*(volatile uint32_t *)(RCC_BASE + 0x08))
#define RCC_PLLCFGR           (*(volatile uint32_t *)(RCC_BASE + 0x0C))
#define RCC_CIER              (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR1          (*(volatile uint32_t *)(RCC_BASE + 0x58))
#define RCC_APB2ENR           (*(volatile uint32_t *)(RCC_BASE + 0x60))

#define FLASH_ACR             (*(volatile uint32_t *)(FLASH_BASE + 0x00))

#define PWR_CR1               (*(volatile uint32_t *)(PWR_BASE + 0x00))

/* RCC_CR bits */
#define RCC_CR_MSION          (1UL << 0)
#define RCC_CR_MSIRDY         (1UL << 1)
#define RCC_CR_HSION          (1UL << 8)
#define RCC_CR_HSIRDY         (1UL << 10)
#define RCC_CR_HSEON          (1UL << 16)
#define RCC_CR_HSERDY         (1UL << 17)
#define RCC_CR_PLLON          (1UL << 24)
#define RCC_CR_PLLRDY         (1UL << 25)

/* RCC_CFGR bits */
#define RCC_CFGR_SW_MSI       (0x0UL << 0)
#define RCC_CFGR_SW_HSI       (0x1UL << 0)
#define RCC_CFGR_SW_HSE       (0x2UL << 0)
#define RCC_CFGR_SW_PLL       (0x3UL << 0)
#define RCC_CFGR_SWS_MSI      (0x0UL << 2)
#define RCC_CFGR_SWS_HSI      (0x1UL << 2)
#define RCC_CFGR_SWS_HSE      (0x2UL << 2)
#define RCC_CFGR_SWS_PLL      (0x3UL << 2)

/* FLASH_ACR bits */
#define FLASH_ACR_LATENCY_4WS (4UL << 0)
#define FLASH_ACR_PRFTEN      (1UL << 8)
#define FLASH_ACR_ICEN        (1UL << 9)
#define FLASH_ACR_DCEN        (1UL << 10)

/* PWR_CR1 bits */
#define PWR_CR1_VOS_Range1    (0x1UL << 9)

/* PLL configuration for 80MHz from MSI 4MHz */
#define RCC_PLLCFGR_PLLSRC_MSI  (0x1UL << 0)
#define RCC_PLLCFGR_PLLM_DIV1   (0x0UL << 4)  /* PLLM = 1 */
#define RCC_PLLCFGR_PLLN_MUL40  (40UL << 8)   /* PLLN = 40 */
#define RCC_PLLCFGR_PLLR_DIV2   (0x0UL << 25) /* PLLR = 2 */
#define RCC_PLLCFGR_PLLREN      (1UL << 24)   /* Enable PLLR output */

/* MSI Clock Range */
#define RCC_ICSCR_MSIRANGE_4MHz (0x6UL << 13)

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */

/**
  * @brief  Clock variable for system core clock (HCLK)
  * The SystemCoreClock variable contains the core clock (HCLK), it can
  * be used by the user application to setup the SysTick timer or configure
  * other parameters. It may also be used by the HAL/LL drivers.
  */
uint32_t SystemCoreClock = 4000000; /* Default to 4 MHz MSI */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the FPU setting, vector table location and the PLL configuration
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
  /* FPU settings ------------------------------------------------------------*/
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    /* Enable CP10 and CP11 coprocessors (FPU) */
    *((volatile uint32_t *)0xE000ED88) |= ((3UL << 10*2) | (3UL << 11*2));
  #endif

  /* Reset the RCC clock configuration to the default reset state ------------*/
  /* Set MSION bit */
  RCC_CR |= RCC_CR_MSION;

  /* Reset CFGR register */
  RCC_CFGR = 0x00000000;

  /* Reset HSEON, CSSON , HSION, and PLLON bits */
  RCC_CR &= ~(RCC_CR_HSEON | RCC_CR_HSION | RCC_CR_PLLON);

  /* Reset PLLCFGR register */
  RCC_PLLCFGR = 0x00001000;

  /* Disable all interrupts */
  RCC_CIER = 0x00000000;

  /* Configure the Vector Table location add offset address ------------------*/
#ifdef VECT_TAB_SRAM
  *((volatile uint32_t *)0xE000ED08) = 0x20000000 | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM */
#else
  *((volatile uint32_t *)0xE000ED08) = 0x08000000 | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
#endif
}

/**
  * @brief  Update SystemCoreClock variable according to Clock Register Values.
  *         The SystemCoreClock variable contains the core clock (HCLK), it can
  *         be used by the user application to setup the SysTick timer or configure
  *         other parameters.
  *
  * @note   Each time the core clock (HCLK) changes, this function must be called
  *         to update SystemCoreClock variable value. Otherwise, any configuration
  *         based on this variable will be incorrect.
  *
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  uint32_t tmp, msirange, pllvco, pllr, pllsource, pllm;

  /* Get SYSCLK source */
  tmp = RCC_CFGR & 0x0C;

  switch (tmp)
  {
    case 0x00:  /* MSI used as system clock source */
      msirange = (RCC_CR >> 4) & 0x0F;
      /* MSI frequency ranges from 100 kHz to 48 MHz */
      static const uint32_t MSIRangeTable[12] = {
        100000, 200000, 400000, 800000, 1000000, 2000000,
        4000000, 8000000, 16000000, 24000000, 32000000, 48000000
      };
      SystemCoreClock = MSIRangeTable[msirange];
      break;

    case 0x04:  /* HSI used as system clock source */
      SystemCoreClock = 16000000;
      break;

    case 0x08:  /* HSE used as system clock source */
      SystemCoreClock = 8000000; /* Assuming 8MHz HSE */
      break;

    case 0x0C:  /* PLL used as system clock source */
      pllsource = (RCC_PLLCFGR & 0x03);
      pllm = ((RCC_PLLCFGR >> 4) & 0x07) + 1;

      if (pllsource == 0x02)  /* HSE used as PLL clock source */
      {
        pllvco = (8000000 / pllm);
      }
      else if (pllsource == 0x03)  /* HSI used as PLL clock source */
      {
        pllvco = (16000000 / pllm);
      }
      else  /* MSI used as PLL clock source */
      {
        msirange = (RCC_CR >> 4) & 0x0F;
        static const uint32_t MSIRangeTable[12] = {
          100000, 200000, 400000, 800000, 1000000, 2000000,
          4000000, 8000000, 16000000, 24000000, 32000000, 48000000
        };
        pllvco = MSIRangeTable[msirange] / pllm;
      }

      pllvco = pllvco * ((RCC_PLLCFGR >> 8) & 0x7F);
      pllr = (((RCC_PLLCFGR >> 25) & 0x03) + 1) * 2;
      SystemCoreClock = pllvco / pllr;
      break;

    default:
      SystemCoreClock = 4000000;
      break;
  }
}

/**
  * @brief  Configure the system clock to 80 MHz using PLL with MSI as source
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable Power Control clock */
  RCC_APB1ENR1 |= (1UL << 28);

  /* Voltage scaling - Select Range 1 (up to 80 MHz) */
  PWR_CR1 |= PWR_CR1_VOS_Range1;

  /* Enable MSI oscillator and wait for it to be ready */
  RCC_CR |= RCC_CR_MSION;
  while ((RCC_CR & RCC_CR_MSIRDY) == 0);

  /* Configure MSI to 4 MHz */
  RCC_CR = (RCC_CR & ~(0xF << 4)) | RCC_ICSCR_MSIRANGE_4MHz;

  /* Configure Flash latency for 80 MHz (4 wait states for 3.3V) */
  FLASH_ACR = FLASH_ACR_LATENCY_4WS | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN;

  /* Configure PLL: MSI as source, M=1, N=40, R=2 for 80 MHz
   * VCO = MSI * N / M = 4 MHz * 40 / 1 = 160 MHz
   * SYSCLK = VCO / R = 160 MHz / 2 = 80 MHz
   */
  RCC_PLLCFGR = RCC_PLLCFGR_PLLSRC_MSI |
                RCC_PLLCFGR_PLLM_DIV1 |
                RCC_PLLCFGR_PLLN_MUL40 |
                RCC_PLLCFGR_PLLR_DIV2 |
                RCC_PLLCFGR_PLLREN;

  /* Enable PLL */
  RCC_CR |= RCC_CR_PLLON;

  /* Wait for PLL to be ready */
  while ((RCC_CR & RCC_CR_PLLRDY) == 0);

  /* Select PLL as system clock source */
  RCC_CFGR = (RCC_CFGR & ~0x03) | RCC_CFGR_SW_PLL;

  /* Wait for PLL to be used as system clock source */
  while ((RCC_CFGR & 0x0C) != RCC_CFGR_SWS_PLL);

  /* Update SystemCoreClock variable */
  SystemCoreClock = 80000000;
}
