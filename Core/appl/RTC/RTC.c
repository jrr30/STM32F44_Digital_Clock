/*
 * RTC.c
 *
 *  Created on: Aug 28, 2023
 *      Author: Jesus
 */

#include "../appl/RTC/RTC.h"
#include "../appl/LCD/LCD16.h"
#include <stdint.h>
#include <string.h>

RTC_HandleTypeDef hrtc;

static uint8_t TimeDate_Buffer[RTC_Time_Date_Max];

void RTC_updateTimeDate(void)
{
  RTC_TimeTypeDef Time_handler;
  RTC_DateTypeDef Date_handler;

  //Updating buffer of time
  (void) HAL_RTC_GetTime (&hrtc, &Time_handler, RTC_FORMAT_BIN);

  //Updating buffer of date
  (void) HAL_RTC_GetDate (&hrtc, &Date_handler, RTC_FORMAT_BIN);

  //Saving time in buffer
  TimeDate_Buffer[RTC_hours] = Time_handler.Hours;
  TimeDate_Buffer[RTC_minutes] = Time_handler.Minutes;
  TimeDate_Buffer[RTC_seconds] = Time_handler.Seconds;
  TimeDate_Buffer[RTC_time_format] = Time_handler.TimeFormat;

  //Saving date in buffer
  TimeDate_Buffer[RTC_year] = Date_handler.Year;
  TimeDate_Buffer[RTC_month] = Date_handler.Month;
  TimeDate_Buffer[RTC_day] = Date_handler.Date;
}


void Read_TimeDate(uint8_t * timebuffer_pu8, uint8_t total_size_array)
{
  if(NULL != timebuffer_pu8)
    {
      memcpy(timebuffer_pu8, TimeDate_Buffer, total_size_array);
    }
}

void Write_TimeDate(uint8_t * timebuffer_pu8, uint8_t total_size_array)
{
  RTC_TimeTypeDef Time_handler;
  RTC_DateTypeDef Date_handler;
  uint8_t local_timeDate[RTC_Time_Date_Max] = {0};

  memcpy(local_timeDate, timebuffer_pu8, total_size_array);

  Time_handler.Hours   = local_timeDate[RTC_hours];
  Time_handler.Minutes = local_timeDate[RTC_minutes];
  Time_handler.Seconds = local_timeDate[RTC_seconds];
  Time_handler.TimeFormat = local_timeDate[RTC_time_format];

  Date_handler.Year  = local_timeDate[RTC_year];
  Date_handler.Month = local_timeDate[RTC_month];
  Date_handler.Date  = local_timeDate[RTC_day];

  HAL_RTC_SetTime(&hrtc, &Time_handler, RTC_FORMAT_BIN);
  HAL_RTC_SetDate(&hrtc, &Date_handler, RTC_FORMAT_BIN);
}


void SetAlarm(uint8_t * timebuffer_pu8, uint8_t max_array)
{
  RTC_AlarmTypeDef Alarm_handler;
  uint8_t alarmlocal[RTC_Alram_info_Max];
  HAL_StatusTypeDef alarm_status_config = HAL_ERROR;

  if(NULL != timebuffer_pu8)
    {
      memcpy(alarmlocal, timebuffer_pu8, max_array);

      Alarm_handler.AlarmTime.Hours   = alarmlocal[RTC_Alarm_hours];
      Alarm_handler.AlarmTime.Minutes = alarmlocal[RTC_Alarm_minutes];
      Alarm_handler.AlarmTime.Seconds = 0u;
      Alarm_handler.AlarmTime.TimeFormat = alarmlocal[2u];
      Alarm_handler.AlarmDateWeekDaySel = alarmlocal[RTC_Alarm_Monday];

      alarm_status_config = HAL_RTC_SetAlarm_IT(&hrtc, &Alarm_handler, RTC_FORMAT_BIN);

      if(HAL_OK == alarm_status_config)
	{
	  LCDEF_Print_Custome_Char(Char_Bell_Custome, Row_1, Column_1);
	}
    }

}

