/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "global_constants.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hw_gpio.hpp"
#include "mw_motor_controller.hpp"
#include "stdio.h"
#include "hw_pwm.hpp"
#include "hw_spi.hpp"
#include "car_control.hpp"
#include "mw_transceiver_rf.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint16_t timer = 0;
volatile uint8_t button_flag = 0;

uint8_t tx_data[NRF24L01P_PAYLOAD_LENGTH] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t rx_data[NRF24L01P_PAYLOAD_LENGTH] = { 0, };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern "C" int __io_putchar (int ch)
{
  while (!LL_USART_IsActiveFlag_TXE(USART2));
  LL_USART_TransmitData8(USART2, (char)ch);
  return ch;
}
/*
extern "C" int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        while (!LL_USART_IsActiveFlag_TXE(USART2));
        LL_USART_TransmitData8(USART2, ptr[i]);
    }
    return len;
}*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/
  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  // Add to some timing class ?
  LL_Init1msTick(SystemCoreClock);
  LL_SYSTICK_EnableIT();  // Enable SysTick interrupt
  
  // Add to PWM ?
  LL_TIM_EnableIT_UPDATE(TIM2);
  LL_TIM_EnableIT_UPDATE(TIM3);


  /* GPIO for motor controller, enable and sleep. And also LED.*/
  Gpio mc_sleep(GPIOA, LL_GPIO_PIN_4); // Example GPIO pin for LED
  Gpio mc_enable(GPIOB, LL_GPIO_PIN_0); // Example GPIO pin for Button
  Gpio led(GPIOA, LD2_Pin); // Example GPIO pin for LED
  Gpio mc_spi_cs_motor(GPIOC, LL_GPIO_PIN_0); // Example GPIO pin for SPI CS

  Gpio rf_t_spi_ncs(GPIOB, LL_GPIO_PIN_12); // Example GPIO pin for SPI CS
  Gpio rf_t_enable(GPIOB, LL_GPIO_PIN_13); // Example GPIO pin for RF CE

  const uint16_t servo_positions[] = {900, 1500, 2100};

  PWM servo_pwm(TIM3);
  PWM mc_pwm(TIM2);

  Spi mc_spi(SPI2, mc_spi_cs_motor);
  Spi rf_t_spi(SPI3, rf_t_spi_ncs);
  
  mc_sleep.reset();
  mc_enable.reset();

  MotorController mc_TB9054FTG(mc_spi, mc_pwm, mc_enable, mc_sleep);
  
  CarControl CarControl(mc_TB9054FTG, servo_pwm);

  TransceiverRF rf_t_nRF24L01(rf_t_spi, rf_t_enable);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    if(timer>1000) {
      led.toggle();
      timer=0;
    }
    else if(nrf_it_flag) {
      if(rf_t_nRF24L01.get_mode() == TransceiverRF::Mode::RX) {
        rf_t_nRF24L01.rx_irq(rx_data);
      }
      else if(rf_t_nRF24L01.get_mode() == TransceiverRF::Mode::TX) {
        rf_t_nRF24L01.tx_irq();
      }
      nrf_it_flag = 0;
    }else if (button_flag) {
      button_flag = 0;
      rf_t_nRF24L01.tx_transmit(tx_data);
    }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
  LL_PWR_DisableOverDriveMode();
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_16, 336, LL_RCC_PLLP_DIV_4);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(84000000);
  LL_SetSystemCoreClock(84000000);
  LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_TWICE);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
