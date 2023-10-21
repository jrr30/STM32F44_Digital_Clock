/*
 * INPUTIF.h
 *
 *  Created on: Sep 18, 2023
 *      Author: Jesus
 */

#ifndef APPL_INPUTIF_INPUTIF_H_
#define APPL_INPUTIF_INPUTIF_H_

#include "../appl/DIGITALINPUT/DIGITALINPUT.h"

/*Macros declaration------------------------------------*/

/*User typedef------------------------------------------*/

typedef enum Button_Set_Requests_TAG
{
	Setting_Idle_Requested   = 0x0001,
	Setting_Enter_Requested  = 0x0002,
	Setting_Hour_Requested   = 0x0004,
	Setting_Min_Requested    = 0x0008,
	Setting_Sec_Requested    = 0x0010,
	Setting_Year_Requested   = 0x0020,
	Setting_Month_Requested  = 0x0040,
	Setting_Day_Requested    = 0x0080,
	Setting_Exit_Requested   = 0x0100,

	Set_max_status_Requested = 0x0200,
}E_Button_Set_Requests;

typedef enum Button_Alarm_Requests_TAG
{
	Alarm_idle_Requested  = 0x0001,
	Alarm_Ente_Requested  = 0x0002,
	Alarm_Exit_Requested  = 0x0004,

	Alarm_max_Requested   = 0x0008,
}E_Button_Alarm_Requests;

typedef enum Button_Increment_Requests_TAG
{
	Incrementing_Idle_Requested       = 0x0001,
	Incrementing_Requested_Requested  = 0x0002,

	Incrementing_max_status_Requested = 0x0004,
}E_Button_Increment_Requests;

typedef enum Button_Decrement_Requests_TAG
{

	Decrementing_Idle_Requested          = 0x0001,
	Decrementing_Requested_Requested     = 0x0002,

	Decrementing_max_status_Requested    = 0x0004,
}E_Button_Decrement_Requests;

/*Public functions--------------------------------------*/

void INTFEF_Thread(void);
void INTFEF_Init(void);
uint16_t INTFEF_Get_Button_Req(Input_Source source_e);

#endif /* APPL_INPUTIF_INPUTIF_H_ */
