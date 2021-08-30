/*
 * RTC.h
 *
 *  Created on: Oct 17, 2020
 *      Author: Jesus
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32f4xx_hal.h"

extern void Error_Handler(void);
extern void delay_us(uint32_t microseconds);

void Set_Time_Date(RTC_TimeTypeDef * Global_time_setting, RTC_DateTypeDef * Global_date_setting, RTC_HandleTypeDef * Ghrtc);
void Get_Time_Date(RTC_TimeTypeDef * Global_time_getting, RTC_DateTypeDef * Global_date_getting, RTC_HandleTypeDef * Ghrtc);
void Set_Alarm(RTC_HandleTypeDef * Ghrtc);
void Alarm_sound(void);

#endif /* INC_RTC_H_ */
