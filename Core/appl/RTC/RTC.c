/*
 * RTC.c
 *
 *  Created on: Aug 28, 2023
 *      Author: Jesus
 */

#include "../appl/RTC/RTC.h"
#include <stdint.h>
#include <string.h>

#define MAX_TIME_BUFFER 0x03u
#define MAX_DATE_BUFFER 0x03u

RTC_HandleTypeDef hrtc;

static uint8_t TimeBuffer[MAX_TIME_BUFFER];
static uint8_t DateBuffer[MAX_DATE_BUFFER];
static RTC_TimeTypeDef Time_handler;
static RTC_DateTypeDef Date_handler;
static RTC_AlarmTypeDef Alarm_handler;

typedef enum format_t
{
	twelve,
	twenty_four
}format_T;

static void Change_formarttwelve(uint8_t * source_timebuffer, uint8_t * des_timebuffer);

static void Change_formarttwelve(uint8_t * source_timebuffer, uint8_t * des_timebuffer)
{
	uint8_t timeb_u8[MAX_TIME_BUFFER] = {0x00u};
	memcpy(timeb_u8, source_timebuffer,MAX_TIME_BUFFER);

	if(timeb_u8[HRS] > 12u)
	{
		timeb_u8[HRS] = timeb_u8[HRS] - 12u;
	}
	memcpy(des_timebuffer, timeb_u8, MAX_TIME_BUFFER);
}

void RTC_updateTimeDate(void)
{
        //Updating buffer of time
	(void)HAL_RTC_GetTime(&hrtc, &Time_handler, RTC_FORMAT_BIN);

	//Updating buffer of date
	(void)HAL_RTC_GetDate(&hrtc, &Date_handler, RTC_FORMAT_BIN);

	//Saving time in buffer
	TimeBuffer[HRS] = Time_handler.Hours;
	TimeBuffer[MIN] = Time_handler.Minutes;
	TimeBuffer[SEC] = Time_handler.Seconds;

	//Saving date in buffer
	DateBuffer[YEAR]   = Date_handler.Year;
	DateBuffer[MOUNTH] = Date_handler.Month;
	DateBuffer[DAY]    = Date_handler.Date;
}


void ReadDate(uint8_t * timebuffer_pu8)
{
	memcpy(timebuffer_pu8, DateBuffer, MAX_DATE_BUFFER);
}

void ReadTime(uint8_t * timebuffer_pu8)
{
	uint8_t localtime[MAX_TIME_BUFFER] = {0};

	Change_formarttwelve(TimeBuffer, localtime);
	memcpy(timebuffer_pu8, localtime, MAX_TIME_BUFFER);
}

void WriteTime(uint8_t * timebuffer_pu8)
{
  uint8_t localtime[MAX_TIME_BUFFER] = {0};

  memcpy(localtime, timebuffer_pu8, MAX_DATE_BUFFER);

  Time_handler.Hours   = localtime[HRS];
  Time_handler.Minutes = localtime[MIN];
  Time_handler.Seconds = localtime[SEC];


  HAL_RTC_SetTime(&hrtc, &Time_handler, RTC_FORMAT_BIN);
}

void WriteDate(uint8_t * datebuffer_pu8)
{
  uint8_t localdate[MAX_DATE_BUFFER] = {0};

  memcpy(localdate, datebuffer_pu8, MAX_DATE_BUFFER);

  Date_handler.Year  = localdate[YEAR];
  Date_handler.Month = localdate[MOUNTH];
  Date_handler.Date  = localdate[DAY];


  HAL_RTC_SetDate(&hrtc, &Date_handler, RTC_FORMAT_BIN);
}

void SetAlarm(uint8_t * timebuffer_pu8)
{
  uint8_t localtime[MAX_TIME_BUFFER] = {0};

  memcpy(localtime, timebuffer_pu8, MAX_DATE_BUFFER);

  Alarm_handler.AlarmTime.Hours   = localtime[HRS];;
  Alarm_handler.AlarmTime.Minutes = localtime[MIN];
  Alarm_handler.AlarmTime.Seconds = localtime[SEC];


  HAL_RTC_SetAlarm_IT(&hrtc, &Alarm_handler, RTC_FORMAT_BIN);
}

