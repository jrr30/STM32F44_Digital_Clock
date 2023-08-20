/*
 * RTC.c
 *
 *  Created on: Oct 17, 2020
 *      Author: Jesus
 */

#include "RTC.h"
#include "main.h"

const uint8_t str_am [3] = {"am"};
const uint8_t str_pm [3] = {"pm"};
const uint8_t *str[2] = {str_am, str_pm};


void Set_Time_Date(RTC_TimeTypeDef * Global_time_setting, RTC_DateTypeDef * Global_date_setting, RTC_HandleTypeDef * Ghrtc)
{
	RTC_TimeTypeDef local_time_data_struct = {0};
	RTC_DateTypeDef local_date_data_struct = {0};

	local_time_data_struct.Hours = Global_time_setting->Hours;
	local_time_data_struct.Minutes = Global_time_setting->Minutes;
	local_time_data_struct.Seconds = Global_time_setting->Seconds;
	local_time_data_struct.DayLightSaving = Global_time_setting->DayLightSaving;
	local_time_data_struct.StoreOperation = Global_time_setting->StoreOperation;
	if (HAL_RTC_SetTime(Ghrtc, &local_time_data_struct, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

	local_date_data_struct.WeekDay = Global_date_setting->WeekDay;
	local_date_data_struct.Month = Global_date_setting->Month;
	local_date_data_struct.Date = Global_date_setting->Date;
	local_date_data_struct.Year = Global_date_setting->Year;

	if (HAL_RTC_SetDate(Ghrtc, &local_date_data_struct, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

}
void Get_Time_Date(RTC_TimeTypeDef * Global_time_getting, RTC_DateTypeDef * Global_date_getting, RTC_HandleTypeDef * Ghrtc)
{
	(void)HAL_RTC_GetTime(Ghrtc, Global_time_getting, RTC_FORMAT_BIN);
	(void)HAL_RTC_GetDate(Ghrtc, Global_date_getting, RTC_FORMAT_BIN);
}

void Set_Alarm(RTC_HandleTypeDef * Ghrtc)
{
	RTC_AlarmTypeDef local_alarm = {0};
	  /** Enable the Alarm A
	  */
	local_alarm.AlarmTime.Hours = 0x0A;
	local_alarm.AlarmTime.Minutes = 0x02;
	local_alarm.AlarmTime.Seconds = 0x0;
	local_alarm.AlarmTime.SubSeconds = 0x0;
	local_alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
	local_alarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	local_alarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	local_alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	local_alarm.AlarmSubSecondMask = RTC_ALARMMASK_SECONDS;
	local_alarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	local_alarm.AlarmDateWeekDay = RTC_WEEKDAY_MONDAY
			                       |RTC_WEEKDAY_TUESDAY
								   |RTC_WEEKDAY_WEDNESDAY
								   |RTC_WEEKDAY_THURSDAY
								   |RTC_WEEKDAY_FRIDAY
								   |RTC_WEEKDAY_SATURDAY
								   |RTC_WEEKDAY_SUNDAY;
	local_alarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(Ghrtc, &local_alarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

void Alarm_sound(void)
{
	uint8_t iter_alarm_count = 0;

	for(iter_alarm_count = 0; iter_alarm_count <= BUZZER_OUT_Pin; iter_alarm_count++)
	{
		HAL_GPIO_TogglePin(GPIOA, BUZZER_OUT_Pin);
		delay_us(1000000);
	}
	HAL_GPIO_WritePin(GPIOA, BUZZER_OUT_Pin, GPIO_PIN_RESET);
}
