/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI2_CS_Pin GPIO_PIN_1
#define SPI2_CS_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define BUZZER_OUT_Pin GPIO_PIN_8
#define BUZZER_OUT_GPIO_Port GPIOC
#define DECREMENT_BUTTON_Pin GPIO_PIN_9
#define DECREMENT_BUTTON_GPIO_Port GPIOA
#define INCREMENT_BUTTON_Pin GPIO_PIN_10
#define INCREMENT_BUTTON_GPIO_Port GPIOA
#define SET_ALARM_BUTTON_Pin GPIO_PIN_11
#define SET_ALARM_BUTTON_GPIO_Port GPIOA
#define SET_BUTTON_Pin GPIO_PIN_12
#define SET_BUTTON_GPIO_Port GPIOA
#define DATA_D7_Pin GPIO_PIN_15
#define DATA_D7_GPIO_Port GPIOA
#define DATA_D6_Pin GPIO_PIN_10
#define DATA_D6_GPIO_Port GPIOC
#define DATA_D5_Pin GPIO_PIN_11
#define DATA_D5_GPIO_Port GPIOC
#define DATA_D4_Pin GPIO_PIN_12
#define DATA_D4_GPIO_Port GPIOC
#define ENABLE_Pin GPIO_PIN_7
#define ENABLE_GPIO_Port GPIOB
#define RW_Pin GPIO_PIN_8
#define RW_GPIO_Port GPIOB
#define RS_Pin GPIO_PIN_9
#define RS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
