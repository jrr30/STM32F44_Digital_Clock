/*
 * SchM_DigitalClock.h
 *
 *  Created on: Jul 2, 2023
 *      Author: Jesus
 */

#ifndef INC_SCHM_DIGITALCLOCK_H_
#define INC_SCHM_DIGITALCLOCK_H_

void Get_RTC_Runnable(void * parameters);
void Print_Time_Runnable(void * parameters);
void Process_UART_Data_Runnable(void * parameters);
void Task_Generation(void);

#endif /* INC_SCHM_DIGITALCLOCK_H_ */
