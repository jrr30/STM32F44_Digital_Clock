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
#include "../appl/BUZZER/BUZZER.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "main.h"

TaskHandle_t Task_50ms_Handler = NULL;
TaskHandle_t Task_200ms_Handler = NULL;
TaskHandle_t Task_400ms_Handler = NULL;
TaskHandle_t Task_500ms_Handler = NULL;

TimerHandle_t buzzer_timer = NULL;

QueueHandle_t Time_Queue_Handler = NULL;
QueueHandle_t User_Input_Time_Date_Queue_Handler = NULL;

static void Task_50ms(void * parameters);
static void Task_200ms(void * parameters);
static void Task_400ms(void * parameters);
static void Task_500ms(void * parameters);

static void BuzzerTimerCallback( TimerHandle_t xTimer);

void Task_Generation(void)
{

	xTaskCreate(Task_50ms,   "50ms", 200, NULL, 1, &Task_50ms_Handler );
	xTaskCreate(Task_200ms, "200ms", 200, NULL, 1, &Task_200ms_Handler);
	xTaskCreate(Task_400ms, "400ms", 400, NULL, 2, &Task_400ms_Handler);
	xTaskCreate(Task_500ms, "500ms", 200, NULL, 3, &Task_500ms_Handler);

	buzzer_timer = xTimerCreate("Buzzer_Timer", pdMS_TO_TICKS(BUZZER_TIME_ON), pdFALSE, ( void * ) 0, BuzzerTimerCallback);

}

void delay_buzzer_start(void)
{

  if( xTimerStart( buzzer_timer, 0) != pdPASS )
    {

    }
  else
    {
      HAL_GPIO_WritePin(GPIOA, BUZZER_OUT_Pin, GPIO_PIN_SET);
    }
}

void BuzzerTimerCallback( TimerHandle_t xTimer)
{
  HAL_GPIO_WritePin(GPIOA, BUZZER_OUT_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  This task run every 5ms by the scheduler.
  *
  * @note   This task will handle all activities that need to run at 5ms
  *
  * @param  None
  * @retval None
  */

void Task_50ms(void * parameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{

		Io_Thread();
		vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(50));
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

void Task_200ms(void * parameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
	    RTC_updateTimeDate();
	    APPIFEF_Thread();
	    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(200));
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

void Task_400ms(void * parameters)
{
	TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	for(;;)
	{
	    FSMEF_Clock_Thread();
	    vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(400));
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

void Task_500ms(void * parameters)
{
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  uint8_t switch_row = 0x01u;
  for(;;)
    {
      if(switch_row == ROW_UP)
	{
	  LCDEF_frist_Row();
	  switch_row = ROW_DOWN;
	}
      else if (switch_row == ROW_DOWN)
	{
	  LCDEF_Second_Row();
	  switch_row = ROW_UP;
	}
      else
	{
	  Set_Cursor(Row_1, Column_1);
	  print_string((unsigned char *)"Error task send");
	}

      vTaskDelayUntil( &xLastWakeTime, pdMS_TO_TICKS(500));
    }
}

