/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum SystemError_t SystemError_t;
enum SystemError_t
{
	SYS_ERROR_SRAM_FAIL,
	SYS_ERROR_SDIO_FAIL,
	SYS_ERROR_WIFI_FAIL,
};

typedef struct System_t System_t;
struct System_t
{
	// virtual comport status
	uint8_t vcom_open;

	// hardware functionality states
	uint8_t sram_working;
	uint8_t sdio_working;
	uint8_t wifi_working;

	// error state
	SystemError_t error;
};
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define WIFI_ENABLE_Pin GPIO_PIN_8
#define WIFI_ENABLE_GPIO_Port GPIOB
#define SDMMC_SW_Pin GPIO_PIN_15
#define SDMMC_SW_GPIO_Port GPIOA
#define WIFI_UART_RX_Pin GPIO_PIN_7
#define WIFI_UART_RX_GPIO_Port GPIOB
#define WIFI_UART_TX_Pin GPIO_PIN_6
#define WIFI_UART_TX_GPIO_Port GPIOB
#define USB_HOST_VBUF_Pin GPIO_PIN_2
#define USB_HOST_VBUF_GPIO_Port GPIOI
#define LED3_Pin GPIO_PIN_6
#define LED3_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOG
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOG
#define STATUS_LED_Pin GPIO_PIN_7
#define STATUS_LED_GPIO_Port GPIOF

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
