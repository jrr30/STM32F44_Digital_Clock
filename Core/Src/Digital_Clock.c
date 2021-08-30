/*
 * Digital_Clock.c
 *
 *  Created on: Oct 20, 2020
 *      Author: Jesus
 */

#include "Digital_Clock.h"
#include <string.h>
#include <stdio.h>
#include "LCD_1602.h"
#include "RTC.h"
#include "stm32f4xx_hal.h"
#include "BSP_STM32F44E.h"

#define MENU_ITEMS_CONFIG 0X07u
#define REFRESH_TIME_SETTINGS 100000

static uint8_t Buffer_date[11] = {0};
static uint8_t Buffer_time[11] = {0};
static uint8_t Buffer[16] = {0};

static RTC_TimeTypeDef Gglobal_time;
static RTC_DateTypeDef Gglobal_date;
static RTC_TimeTypeDef STime;
static RTC_DateTypeDef SDate;

extern RTC_HandleTypeDef hrtc;
extern uint8_t *str[2];
extern uint8_t global_counter;
extern G_Flags_T G_Status_F;

void (*date_config[MENU_ITEMS_CONFIG])(uint8_t * G_counter) = {NULL};


static void setting_hours(uint8_t * hours)
{
	*hours = STime.Hours;
	while(G_Status_F.shifting_parameter == 0x02)
	{
		if(12 < *hours)
		{
			*hours = ZERO_VALUE;
		}
		else
		{
			Clear();
			Set_Cursor(Column_1, Row_1);
			sprintf((char *)Buffer, "Setting Hrs:%d", global_counter);
			print_string(Buffer);
			delay_us(REFRESH_TIME_SETTINGS);
		}
	}
	STime.Hours = *hours;
	Clear();
}


static void setting_minutes(uint8_t * minutes)
{
	*minutes = STime.Minutes;
	while(G_Status_F.shifting_parameter == 0x04)
	{
		if(60 < *minutes)
		{
			*minutes = ZERO_VALUE;
		}
		else
		{
			Clear();
			Set_Cursor(Column_1, Row_1);
			sprintf((char *)Buffer, "Setting Min:%d", global_counter);
			print_string(Buffer);
			delay_us(REFRESH_TIME_SETTINGS);
		}
	}
	STime.Minutes = *minutes;
	Clear();
}

static void setting_seconds(uint8_t * seconds)
{
	*seconds = STime.Seconds;
	while(G_Status_F.shifting_parameter == 0x08)
	{
		if(60 < *seconds)
		{
			*seconds = ZERO_VALUE;
		}
		else
		{
			Clear();
			Set_Cursor(Column_1, Row_1);
			sprintf((char *)Buffer, "Setting Sec:%d", global_counter);
			print_string(Buffer);
			delay_us(REFRESH_TIME_SETTINGS);
		}
	}
	STime.Seconds = *seconds;
	Clear();
}

static void setting_period(uint8_t * period)
{
	*period = ZERO_VALUE;
	while(G_Status_F.shifting_parameter == 0x10)
	{
		if(1 < *period)
		{
			*period = ZERO_VALUE;
		}
		else
		{
			Clear();
			Set_Cursor(Column_1, Row_1);
			sprintf((char *)Buffer, "Sel Period:%s", str[*period]);
			print_string(Buffer);
			delay_us(REFRESH_TIME_SETTINGS);
		}
	}
	if(0x00 == *period)
	{
		STime.TimeFormat = RTC_HOURFORMAT12_AM;
	}
	else
	{
		STime.TimeFormat = RTC_HOURFORMAT12_PM;
	}
	Clear();
}

static void setting_day(uint8_t * day)
{
	*day = SDate.WeekDay;
	while(G_Status_F.shifting_parameter == 0x20)
	{
		if(31 < *day)
		{
			*day = ZERO_VALUE;
		}
		else
		{
			Clear();
			Set_Cursor(Column_1, Row_1);
			sprintf((char *)Buffer, "Setting Day:%d", global_counter);
			print_string(Buffer);
			delay_us(REFRESH_TIME_SETTINGS);
		}
	}
	SDate.Date = *day;
	Clear();
}

static void setting_month(uint8_t * month)
{
	*month = SDate.Month;
	while(G_Status_F.shifting_parameter == 0x40)
	{
		if(12 < *month)
		{
			*month = ZERO_VALUE;
		}
		else
		{
			Clear();
			Set_Cursor(Column_1, Row_1);
			sprintf((char *)Buffer, "Setting MTH:%d", global_counter);
			print_string(Buffer);
			delay_us(REFRESH_TIME_SETTINGS);
		}
	}
	SDate.Month = *month;
	Clear();
}

static void setting_year(uint8_t * year)
{
	*year = SDate.Year;
	while(G_Status_F.shifting_parameter == 0x80)
	{
		if(99 < *year)
		{
			*year = ZERO_VALUE;
		}
		else
		{
			Clear();
			Set_Cursor(Column_1, Row_1);
			sprintf((char *)Buffer, "Setting Yr:%d", (2000 +global_counter));
			print_string(Buffer);
			delay_us(REFRESH_TIME_SETTINGS);
		}
	}
	SDate.Year = *year;
	Clear();
}

void setting_time(uint8_t * gobla_counter_Prt)
{

	Get_Time_Date(&Gglobal_time, &Gglobal_date, &hrtc);
	STime = Gglobal_time;
	SDate = Gglobal_date;

	uint8_t iter_timer = 0x00;

	date_config[0] = &setting_hours;
	date_config[1] = &setting_minutes;
	date_config[2] = &setting_seconds;
	date_config[3] = &setting_period;
	date_config[4] = &setting_day;
	date_config[5] = &setting_month;
	date_config[6] = &setting_year;

	for(iter_timer = ZERO_VALUE; iter_timer <= 6; iter_timer++)
	{
		(date_config[iter_timer])(gobla_counter_Prt);
	}

	STime.Seconds = ZERO_VALUE; // Hardcoding value since seconds are not being set

	if (HAL_RTC_SetTime(&hrtc, &STime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_RTC_SetDate(&hrtc, &SDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

void setting_alarm(uint8_t * gobla_counter_Prt)
{

	Get_Time_Date(&Gglobal_time, &Gglobal_date, &hrtc);
	STime = Gglobal_time;
	SDate = Gglobal_date;

	uint8_t iter_timer = 0x00;

	date_config[0] = &setting_hours;
	date_config[1] = &setting_minutes;
	date_config[2] = &setting_seconds;
	date_config[3] = &setting_period;

	for(iter_timer = 0x00; iter_timer <= 3; iter_timer++)
	{
		(date_config[iter_timer])(gobla_counter_Prt);
	}

	if (HAL_RTC_SetTime(&hrtc, &STime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

	if (HAL_RTC_SetDate(&hrtc, &SDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

void Print_Time(void)
{
	uint8_t str_am_pm[3] = {0};
	Get_Time_Date(&Gglobal_time, &Gglobal_date, &hrtc);
	if(RTC_HOURFORMAT12_AM == Gglobal_time.TimeFormat)
	{
		memcpy(str_am_pm, str[0], 0x03);
	}
	else
	{
		memcpy(str_am_pm, str[1], 0x03);
	}
	sprintf((char *)Buffer_time, "%02d:%02d:%02d %s", Gglobal_time.Hours, Gglobal_time.Minutes, Gglobal_time.Seconds, str_am_pm);
	Set_Cursor(Row_1, Column_4);
	print_string(Buffer_time);
}
void Print_Date(void)
{
	Get_Time_Date(&Gglobal_time, &Gglobal_date, &hrtc);
	sprintf((char *)Buffer_date, "%02d/%02d/%4d", Gglobal_date.Month, Gglobal_date.Date, (2000 + Gglobal_date.Year));
	Set_Cursor(Row_2, Column_4);
	print_string(Buffer_date);
}


