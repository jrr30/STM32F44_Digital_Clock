/*
 * SchM_DigitalClock.c
 *
 *  Created on: Jul 2, 2023
 *      Author: Jesus
 */

#include "SchM_DigitalClock.h"
#include "../appl/RTC/RTC.h"
#include "../appl/LCD/LCD16.h"
#include "../appl/DIGITALINPUT/DIGITALINPUT.h"
#include "../appl/APPINTF/APPINTF.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t localtest[3] = {0};

TaskHandle_t Task_5ms_Handler = NULL;
TaskHandle_t Task_20ms_Handler = NULL;
TaskHandle_t Task_100ms_Handler = NULL;

QueueHandle_t Time_Queue_Handler = NULL;
QueueHandle_t User_Input_Time_Date_Queue_Handler = NULL;

Input_Source source_e = Increment;
uint16_t button_req_status_u16[Source_max] = {0};

static void Task_20ms(void * parameters);
static void Task_5ms(void * parameters);
static void Task_100ms(void * parameters);


void Task_Generation(void)
{

	xTaskCreate(Task_5ms, "5ms", 100, NULL, 1, &Task_5ms_Handler);
	xTaskCreate(Task_20ms, "20ms", 100, NULL, 1, &Task_20ms_Handler);
	xTaskCreate(Task_100ms, "100ms", 100, NULL, 2, &Task_100ms_Handler);

}


/**
  * @brief  This task run every 5ms by the scheduler.
  *
  * @note   This task will handle all activities that need to run at 5ms
  *
  * @param  None
  * @retval None
  */

void Task_5ms(void * parameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{

		Io_Thread();
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(5));
	}
}

/**
  * @brief  This task run every 20ms by the scheduler.
  *
  * @note   This task will handle all activities that need to run at 20ms
  *
  * @param  None
  * @retval None
  */

void Task_20ms(void * parameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
		RTC_updateTimeDate();
		ReadTime(localtest);
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(20));
	}
}

/**
  * @brief  This task run every 100ms by the scheduler.
  *
  * @note   This task will handle all activities that need to run at 100ms
  *
  * @param  None
  * @retval None
  */

void Task_100ms(void * parameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
//	uint8_t counter = 0;
//	uint8_t counter2 = 0;
	for(;;)
	{
//		counter2 ++;
//		if(counter2 == 20)
//		{
//			counter++;
//			uint8_t Buffer_time[2];
//			sprintf((char *)Buffer_time, "%02d", counter);
//			Set_Cursor(Row_1, Column_9);
//			print_string(Buffer_time);
//			counter2 = 0;
//		}
	        APPIFEF_Thread();
		button_req_status_u16[source_e] = APPIFEF_Get_Button_Req(source_e);
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(100));
	}
}

