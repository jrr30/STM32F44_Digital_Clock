/*
 * BSP_STM32F44E.h
 *
 *  Created on: Jan 22, 2021
 *      Author: Jesus
 */

#ifndef INC_BSP_STM32F44E_H_
#define INC_BSP_STM32F44E_H_

#include "stm32f4xx_hal.h"

#define ZERO_VALUE 0x00u
#define ONE_VALUE 0x001u
#define TRESSHOLD_SWITCH_DEBOUNCE 2500

typedef struct G_Flags_t
{
	uint16_t shifting_parameter;
	uint8_t Setting_Time_Date:1;
	uint8_t global_counter;
}G_Flags_T;

typedef struct G_Variables_t
{
	uint32_t counter_us_debound;
}G_Variables_T;

void delay_us(uint32_t microseconds);
void Push_Beep_Button(void);


#endif /* INC_BSP_STM32F44E_H_ */


/*
 *  Date          USER_ID            Brief Description
 *  22/01/2021    Jesus Rodriguez    This change implements in the GPIO ISR Callback a mechanism to avoid Switch debounce
 *
 * */
