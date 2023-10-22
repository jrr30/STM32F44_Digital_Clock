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
#include "../appl/CLOCK_MODELE/CLOCK_MODULE.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

TaskHandle_t Task_5ms_Handler = NULL;
TaskHandle_t Task_20ms_Handler = NULL;
TaskHandle_t Task_40ms_Handler = NULL;
TaskHandle_t Task_100ms_Handler = NULL;

QueueHandle_t Time_Queue_Handler = NULL;
QueueHandle_t User_Input_Time_Date_Queue_Handler = NULL;

static void Task_5ms(void * parameters);
static void Task_20ms(void * parameters);
static void Task_40ms(void * parameters);
static void Task_100ms(void * parameters);


void Task_Generation(void)
{

	xTaskCreate(Task_5ms, "5ms", 100, NULL, 1, &Task_5ms_Handler);
	xTaskCreate(Task_20ms, "20ms", 100, NULL, 1, &Task_20ms_Handler);
	xTaskCreate(Task_40ms, "40ms", 100, NULL, 3, &Task_40ms_Handler);
	xTaskCreate(Task_100ms, "100ms", 100, NULL, 4, &Task_100ms_Handler);

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
	    APPIFEF_Thread();
	    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(20));
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

void Task_40ms(void * parameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
	    FSMEF_Clock_Thread();
	    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(40));
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
  uint8_t row_turn = 0x00u;
  for(;;)
    {
      row_turn++;
      if(row_turn == 4)
	{
	  row_turn = 0x00;
	  LCDEF_Send_TimeRow();
	}
      vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(100));
    }
}

