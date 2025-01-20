/*
 * RTC.h
 *
 *  Created on: Aug 28, 2023
 *      Author: Jesus
 */

#ifndef APPL_RTC_RTC_H_
#define APPL_RTC_RTC_H_

#include "stm32f4xx_hal.h"

/*Macros declaration-----------------------------------*/

typedef enum RTC_time_info_t
{
	RTC_hours,
	RTC_minutes,
	RTC_seconds,
	RTC_time_format,
	RTC_year,
	RTC_month,
	RTC_Weekday,
	RTC_day,

	RTC_Time_Date_Max
}RTC_time_info_T;

typedef enum RTC_time_Alarm_info_t
{
	RTC_Alarm_hours,
	RTC_Alarm_minutes,
	RTC_Alarm_format,
	RTC_Alarm_Monday,
	RTC_Alarm_Tuesday,
	RTC_Alarm_Wensday,
	RTC_Alarm_Thursday,
	RTC_Alarm_Friday,
	RTC_Alarm_Saturday,
	RTC_Alarm_Sunday,

	RTC_Alram_info_Max
}RCT_time_Alarm_info_T;

#define HRS 0x00u
#define MIN 0x01u
#define SEC 0x02u

#define YEAR   0x00u
#define MOUNTH 0x01u
#define DAY    0x02u

/*Public functions-----------------------------------*/
void RTC_updateTimeDate(void);
void Read_TimeDate(uint8_t * timebuffer_pu8, uint8_t total_size_array);
void Write_TimeDate(uint8_t * timebuffer_pu8, uint8_t total_size_array);
uint8_t SetAlarm(uint8_t * timebuffer_pu8, uint8_t max_array);


#endif /* APPL_RTC_RTC_H_ */
