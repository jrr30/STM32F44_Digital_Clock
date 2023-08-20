/*
 * SchM_DigitalClock.c
 *
 *  Created on: Jul 2, 2023
 *      Author: Jesus
 */

#include "SchM_DigitalClock.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


TaskHandle_t Get_Time_Handler = NULL;
TaskHandle_t Print_Time_Handler = NULL;
TaskHandle_t Process_Rx_Data_Handler = NULL;
TaskHandle_t Set_Time_Handler = NULL;

QueueHandle_t Time_Queue_Handler = NULL;
QueueHandle_t User_Input_Time_Date_Queue_Handler = NULL;


void Task_Generation(void)
{

	xTaskCreate(Get_RTC_Runnable, "Get Time ", 100, NULL, 2, &Get_Time_Handler);
	xTaskCreate(Print_Time_Runnable, "Print Time", 100, NULL, 2, &Print_Time_Handler);
	xTaskCreate(Process_UART_Data_Runnable, "UART Rx", 100, NULL, 2, &Process_Rx_Data_Handler);

}

void Get_RTC_Runnable(void * parameters)
{
	for(;;)
	{
		taskYIELD();
	}
}


/**
  * @brief  This task prints out data time in Nextion screen.
  *
  * @note   This task is executed in cooperation mode. This task uses a Queue in order to get data.
  *         Also This task calls the UART-Tx to send info.
  *
  * @param  None
  * @retval None
  */

void Print_Time_Runnable(void * parameters)
{
	for(;;)
	{
		taskYIELD();
	}
}

void Process_UART_Data_Runnable(void * parameters)
{

	for(;;)
	{
		taskYIELD();
	}
}

