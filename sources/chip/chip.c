/*
 * chip.c
 *
 *  Created on: Sep 3, 2023
 *      Author: taras.karpliak
 */

#include "sources/chip/chip_private.h"


// ------------------------------------------------------------------
void CHIP_Init(void)
{
  CHIP_ClockInit();
  CHIP_NvicInit();
  CHIP_TimerInit();
  CHIP_GpioInit();

  SysTick_Config(218857); //(SystemCoreClock / 1000u + 12);
}


// ------------------------------------------------------------------
static void CHIP_ClockInit(void)
{
  // Disable the PLL to configure it
  RCC->CR &= ~RCC_CR_PLLON;

  // Wait until PLLRDY bit is cleared
  while ((RCC->CR & RCC_CR_PLLRDY) != 0);

  // Configure the PLL multiplication factor and division factors
  RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ)) |
                 ((RCC_PLLCFGR_PLLSRC_HSI) |
                  (16U << RCC_PLLCFGR_PLLM_Pos) |  // HSI frequency is divided by 2
                  (432U << RCC_PLLCFGR_PLLN_Pos) | // Desired HCLK = 2 * PLLN = 216MHz
                  (0u << RCC_PLLCFGR_PLLP_Pos) |
                  (9u << RCC_PLLCFGR_PLLQ_Pos));

  // Enable the PLL
  RCC->CR |= RCC_CR_PLLON;

  // Wait until PLLRDY bit is set
  while ((RCC->CR & RCC_CR_PLLRDY) == 0);

  // Configure the Flash latency and prefetch
  FLASH->ACR &= ~FLASH_ACR_LATENCY;
  FLASH->ACR |= FLASH_ACR_LATENCY_7WS;
  FLASH->ACR |= FLASH_ACR_PRFTEN;

  // Configure the AHB, APB1, and APB2 prescalers
  RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
  RCC->CFGR |= (RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2);

  // Select the HSI as input clock for MCO1 pin
  RCC->CFGR &= ~RCC_CFGR_MCO1PRE;
  RCC->CFGR &= ~RCC_CFGR_MCO1;

  // Select the PLL as the system clock source
  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |= RCC_CFGR_SW_PLL;

  // Wait until the PLL is used as the system clock source
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

  RCC->DCKCFGR2 |= RCC_DCKCFGR2_USART1SEL_0; // System clock is selected as USART 1 clock

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

  LL_RCC_ClocksTypeDef clocks = {0};
  LL_RCC_GetSystemClocksFreq(&clocks);

  ASSERT(clocks.SYSCLK_Frequency == 216000000u);
  ASSERT(clocks.HCLK_Frequency   == 216000000u);
  ASSERT(clocks.PCLK1_Frequency  == 54000000u);
  ASSERT(clocks.PCLK2_Frequency  == 108000000u);

  SystemCoreClockUpdate();
}


// ------------------------------------------------------------------
static void CHIP_GpioInit(void)
{
	GPIOB->MODER |= GPIO_MODER_MODER0_0;
  GPIOB->MODER |= GPIO_MODER_MODER7_0;
  GPIOB->MODER |= GPIO_MODER_MODER14_0;

  GPIOE->MODER |= GPIO_MODER_MODER10_0;
  GPIOE->MODER |= GPIO_MODER_MODER11_0;

  // Configure the alternate function for PA9 (USART1_TX)
  GPIOA->AFR[1] &= ~(0xF << (4 * (9 - 8))); // Clear AF bits
  GPIOA->AFR[1] |= (7 << (4 * (9 - 8)));    // Set AF7 for USART1_TX
  GPIOA->MODER |= GPIO_MODER_MODER9_1;      // Set as alternate function
  GPIOA->OTYPER |= GPIO_OTYPER_OT9;         // Open-drain

  // Configure the alternate function for PA8 (MCO1)
  GPIOA->AFR[1] &= ~(0xF << (4 * (8 - 8))); // Clear AF bits
  GPIOA->AFR[1] |= (0 << (4 * (8 - 8)));    // Set AF0 for MCO1
  GPIOA->MODER |= GPIO_MODER_MODER8_1;      // Set as alternate function
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT8;        // Push-pull

  // Configure PE9 (TIM1_CH1) as an alternate function
  // Choose AF1 (TIM1) for PE9
  GPIOE->AFR[1] &= ~(0xF << (4 * (9 - 8))); // Clear AF bits
  GPIOE->AFR[1] |= (1 << (4 * (9 - 8)));
  GPIOE->MODER |= GPIO_MODER_MODER9_1; // Alternate function mode
  GPIOE->OTYPER &= ~GPIO_OTYPER_OT9;        // Push-pull

  GPIOA->MODER |= GPIO_MODER_MODER0_0;  // Alternate function mode
  GPIOA->OTYPER &= ~GPIO_OTYPER_OT0;     // Push-pull
}


// ------------------------------------------------------------------
 void CHIP_TimerInit(void)
{
  LL_TIM_InitTypeDef tim_init =
  {
    .Prescaler = 0,
    .CounterMode = LL_TIM_COUNTERMODE_UP,
    .Autoreload = 216u,
    .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
    .RepetitionCounter = 0x00,
  };
  ASSERT(LL_TIM_Init(TIM1, &tim_init) == SUCCESS)

  LL_TIM_OC_InitTypeDef oc_init =
  {
    .OCMode = LL_TIM_OCMODE_PWM1,
    .OCState = LL_TIM_OCSTATE_ENABLE,
    .OCNState = LL_TIM_OCSTATE_DISABLE,
    .CompareValue = tim_init.Autoreload / 2u,
    .OCPolarity = LL_TIM_OCPOLARITY_HIGH,
    .OCNPolarity = LL_TIM_OCPOLARITY_LOW,
    .OCIdleState = LL_TIM_OCIDLESTATE_LOW,
    .OCNIdleState = LL_TIM_OCIDLESTATE_HIGH,
  };
  ASSERT(LL_TIM_OC_Init(TIM1, LL_TIM_CHANNEL_CH1, &oc_init) == SUCCESS)

  LL_TIM_EnableIT_CC1(TIM1);
  LL_TIM_EnableAllOutputs(TIM1);
}


// ------------------------------------------------------------------
static void CHIP_NvicInit(void)
{
  NVIC_SetPriority(DMA2_Stream7_IRQn, 0); // Set Priority for DMA2
  NVIC_EnableIRQ(DMA2_Stream7_IRQn);      // Enable interrupt

  NVIC_SetPriority(DMA2_Stream2_IRQn, 0); // Set Priority for DMA2
  NVIC_EnableIRQ(DMA2_Stream2_IRQn);      // Enable interrupt

  NVIC_SetPriority(TIM1_CC_IRQn, 1); // Set Priority for DMA2
  NVIC_EnableIRQ(TIM1_CC_IRQn);      // Enable interrupt
}



#define MOTTOR_FULL_STEP_ANGLE_0_1_DEGREES     ((uint32_t) 18u)     // [0.1 degrees] Angle of one full step
#define MOTTOR_PULLEY_OUTER_DIAMETER_MM        ((uint32_t) 16u)     // [mm] Outer diameter of the motor pulley

#define TMC2209_MICROSTEP_PRESCALER            ((uint32_t) 256u)      // Micro-step resolution

#define PI_MUL_BY_10_POW_6                     ((uint32_t) 3141592u) // 3.141592 represented in fixed-point format with 7 decimal places
#define HALF_CIRCLE_ANGLE_DEGREES              ((uint32_t) 180u)
#define MAX_DISTANCE_0_1_MM                    ((uint32_t) (30u * 10u * 10u))

static uint32_t steps_left = 0u;

// ------------------------------------------------------------------
void CHIP_MoveForward(uint16_t distance_0_1_mm, uint16_t speed_mm_per_s)
{
  // L [nm] = (PI * r [nm] * α) / 180 = (PI * 10^6 * d [mm] * 10^6 * α * 10) / 10^6 / 2 / 180 / 10 = (PI * 10^6 * d [mm] * α * 10) / 2 / 180 / 10
  uint32_t one_step_length_nm = (uint32_t)((PI_MUL_BY_10_POW_6 * MOTTOR_FULL_STEP_ANGLE_0_1_DEGREES * MOTTOR_PULLEY_OUTER_DIAMETER_MM) /
                                (2u * 10 * HALF_CIRCLE_ANGLE_DEGREES * TMC2209_MICROSTEP_PRESCALER));

  if (distance_0_1_mm <= MAX_DISTANCE_0_1_MM)
  {
    uint32_t num_of_steps =  (distance_0_1_mm * 100000u * TMC2209_MICROSTEP_PRESCALER) / one_step_length_nm;

    if ((distance_0_1_mm * 100000u * TMC2209_MICROSTEP_PRESCALER) % one_step_length_nm >= 5u)
    {
      num_of_steps++;
    }
    steps_left = num_of_steps;
  }

  uint32_t step_frequency_khz = (uint32_t)(((uint64_t)1000000000u * (uint64_t)speed_mm_per_s) / one_step_length_nm);

  LL_TIM_SetPrescaler(TIM1, (uint32_t)(1000000000u / step_frequency_khz) - 1u);
  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableCounter(TIM1);
}


void TIM1_CC_IRQHandler(void)
{
  if (LL_TIM_IsActiveFlag_CC1(TIM1) != 0u)
  {
    LL_TIM_ClearFlag_CC1(TIM1);

    if (steps_left > 0u)
    {
      steps_left--;
    }
    else
    {
      LL_TIM_DisableCounter(TIM1);
      LL_TIM_CC_DisableChannel(TIM1, LL_TIM_CHANNEL_CH1);
    }
  }
}

// ------------------------------------------------------------------
void CHIP_SetGreenLedPin(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BS0;
}


// ------------------------------------------------------------------
void CHIP_ResetGreenLedPin(void)
{
	GPIOB->BSRR |= GPIO_BSRR_BR0;
}


// ------------------------------------------------------------------
uint32_t CHIP_GetSysCoreClockHz(void)
{
  return SystemCoreClock;
}


// ------------------------------------------------------------------
void CHIP_SetDirPin(void)
{
  GPIOE->BSRR |= GPIO_BSRR_BS10;
}


// ------------------------------------------------------------------
void CHIP_ResetDirPin(void)
{
  GPIOE->BSRR |= GPIO_BSRR_BR10;
}
// ------------------------------------------------------------------
void CHIP_SetStepPin(void)
{
  GPIOE->BSRR |= GPIO_BSRR_BS11;
}


// ------------------------------------------------------------------
void CHIP_ResetStepPin(void)
{
  GPIOE->BSRR |= GPIO_BSRR_BR11;
}




