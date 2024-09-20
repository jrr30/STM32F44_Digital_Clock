/*
 * RTC.c
 *
 *  Created on: Aug 28, 2023
 *      Author: Jesus
 */

#include "../appl/RTC/RTC.h"
#include <stdint.h>
#include <string.h>

#define MAX_TIME_BUFFER 0x04u
#define MAX_DATE_BUFFER 0x03u

RTC_HandleTypeDef hrtc;

static uint8_t TimeBuffer[RTC_time_info_Max];
static uint8_t DateBuffer[RTC_date_info_Max];
static RTC_TimeTypeDef Time_handler;
static RTC_DateTypeDef Date_handler;
static RTC_AlarmTypeDef Alarm_handler;

typedef enum format_t
{
	twelve,
	twenty_four
}format_T;


void RTC_updateTimeDate(void)
{
        //Updating buffer of time
	(void)HAL_RTC_GetTime(&hrtc, &Time_handler, RTC_FORMAT_BIN);

	//Updating buffer of date
	(void)HAL_RTC_GetDate(&hrtc, &Date_handler, RTC_FORMAT_BIN);

	//Saving time in buffer
	TimeBuffer[RTC_hours] = Time_handler.Hours;
	TimeBuffer[RTC_minutes] = Time_handler.Minutes;
	TimeBuffer[RTC_seconds] = Time_handler.Seconds;
	TimeBuffer[RTC_time_format] = Time_handler.TimeFormat;

	//Saving date in buffer
	DateBuffer[RTC_year]   = Date_handler.Year;
	DateBuffer[RTC_month] = Date_handler.Month;
	DateBuffer[RTC_day]    = Date_handler.Date;
}


void ReadDate(uint8_t * timebuffer_pu8)
{
	memcpy(timebuffer_pu8, DateBuffer, sizeof(DateBuffer));
}

void ReadTime(uint8_t * timebuffer_pu8)
{
	memcpy(timebuffer_pu8, TimeBuffer, sizeof(TimeBuffer));
}

void WriteTime(uint8_t * timebuffer_pu8)
{
  uint8_t localtime[RTC_time_info_Max] = {0};

  memcpy(localtime, timebuffer_pu8, MAX_DATE_BUFFER);

  Time_handler.Hours   = localtime[RTC_hours];
  Time_handler.Minutes = localtime[RTC_minutes];
  Time_handler.Seconds = localtime[RTC_seconds];
  Time_handler.TimeFormat = localtime[RTC_time_format];


  HAL_RTC_SetTime(&hrtc, &Time_handler, RTC_FORMAT_BIN);
}

void WriteDate(uint8_t * datebuffer_pu8)
{
  uint8_t localdate[RTC_date_info_Max] = {0};

  memcpy(localdate, datebuffer_pu8, MAX_DATE_BUFFER);

  Date_handler.Year  = localdate[RTC_year];
  Date_handler.Month = localdate[RTC_month];
  Date_handler.Date  = localdate[RTC_day];


  HAL_RTC_SetDate(&hrtc, &Date_handler, RTC_FORMAT_BIN);
}

void SetAlarm(uint8_t * timebuffer_pu8)
{
  uint8_t localtime[RTC_time_info_Max] = {0};

  memcpy(localtime, timebuffer_pu8, MAX_DATE_BUFFER);

  Alarm_handler.AlarmTime.Hours   = localtime[RTC_hours];
  Alarm_handler.AlarmTime.Minutes = localtime[RTC_minutes];
  Alarm_handler.AlarmTime.Seconds = localtime[RTC_seconds];
  Alarm_handler.AlarmTime.TimeFormat = localtime[RTC_time_format];


  HAL_RTC_SetAlarm_IT(&hrtc, &Alarm_handler, RTC_FORMAT_BIN);
}

