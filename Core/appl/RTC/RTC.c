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
  RTC_TimeTypeDef Time_handler = {0};
  RTC_DateTypeDef Date_handler = {0};

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
  TimeDate_Buffer[RTC_Weekday] = Date_handler.WeekDay;
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
  RTC_TimeTypeDef Time_handler = {0};
  RTC_DateTypeDef Date_handler = {0};

  uint8_t local_timeDate[RTC_Time_Date_Max] = {0};

  memcpy(local_timeDate, timebuffer_pu8, total_size_array);

  Time_handler.Hours   = local_timeDate[RTC_hours];
  Time_handler.Minutes = local_timeDate[RTC_minutes];
  Time_handler.Seconds = local_timeDate[RTC_seconds];
  Time_handler.TimeFormat = local_timeDate[RTC_time_format];

  Date_handler.Year  = local_timeDate[RTC_year];
  Date_handler.Month = local_timeDate[RTC_month];
  Date_handler.Date  = local_timeDate[RTC_day];
  Date_handler.WeekDay = local_timeDate[RTC_Weekday];

  HAL_RTC_SetTime(&hrtc, &Time_handler, RTC_FORMAT_BIN);
  HAL_RTC_SetDate(&hrtc, &Date_handler, RTC_FORMAT_BIN);

}


uint8_t SetAlarm(uint8_t * timebuffer_pu8, uint8_t max_array)
{
  RTC_AlarmTypeDef Alarm_handler = {0u};
  uint8_t alarmlocal[RTC_Alram_info_Max];
  HAL_StatusTypeDef alarm_status_config = HAL_ERROR;
  uint8_t alarm_enable_bl = 0x00u;

  if(NULL != timebuffer_pu8)
    {
      memcpy(alarmlocal, timebuffer_pu8, max_array);


      Alarm_handler.AlarmTime.Hours   = alarmlocal[RTC_Alarm_hours];
      Alarm_handler.AlarmTime.Minutes = alarmlocal[RTC_Alarm_minutes];
      Alarm_handler.AlarmTime.Seconds = 0x0;
      Alarm_handler.AlarmTime.SubSeconds = 0x0;
      Alarm_handler.AlarmTime.TimeFormat = alarmlocal[RTC_Alarm_format];
      Alarm_handler.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
      Alarm_handler.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
      Alarm_handler.AlarmMask = RTC_ALARMMASK_HOURS || RTC_ALARMMASK_MINUTES;
      Alarm_handler.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
      Alarm_handler.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
      Alarm_handler.AlarmDateWeekDay = RTC_WEEKDAY_SUNDAY;
      Alarm_handler.Alarm = RTC_ALARM_A;

      alarm_status_config = HAL_RTC_SetAlarm_IT(&hrtc, &Alarm_handler, RTC_FORMAT_BIN);

      if(HAL_OK == alarm_status_config)
	{
	  alarm_enable_bl = 0x01u;
	}
    }
  return alarm_enable_bl;
}

