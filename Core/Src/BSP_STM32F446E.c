/*
 * BSP_STM32F446E.c
 *
 *  Created on: Jan 22, 2021
 *      Author: Jesus
 */

#include "BSP_STM32F44E.h"
#include "main.h"
#include <stdint.h>

TIM_HandleTypeDef htim5;

/**
  * @brief  Sets a timer in microseconds.
  *
  * @note   This function is intended to generate a delay with a time-base in microseconds,
  *         it is importat to remark that each count depends on the clock speed. E.g 16 MHz and prescalar 16, so we get 1MHz = 1us
  *
  * @param  delay time in microseconds. Max value is up to 4294967295
  *
  *
  * @retval None
  */

void delay_us(uint32_t microseconds)
{
	__HAL_TIM_SET_COUNTER(&htim5, 0);

	while( __HAL_TIM_GET_COUNTER(&htim5) < microseconds);
}

/**
  * @brief  Generate a Beep sound.
  *
  * @note   This function is intended to generate pulse to make a sound using the Buzzer
  *
  * @param  none
  *
  *
  * @retval None
  */

void Push_Beep_Button(void)
{
	HAL_GPIO_WritePin(GPIOA, BUZZER_OUT_Pin, GPIO_PIN_SET);
	delay_us(5000);
	HAL_GPIO_WritePin(GPIOA, BUZZER_OUT_Pin, GPIO_PIN_RESET);
}


/*
 *  Date          USER_ID            Brief Description
 *  22/01/2021    Jesus Rodriguez    Creation of file
 *  03/20/2021    Jesus Rodriguez    Documentation of some functions
 *
 * */
