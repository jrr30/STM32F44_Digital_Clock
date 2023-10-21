/*
 * INPUTIF.c
 *
 *  Created on: Sep 18, 2023
 *      Author: Jesus
 */


#include "../appl/INPUTIF/INPUTIF.h"
#include "../appl/DIGITALINPUT/DIGITALINPUT.h"

#include <string.h>

/*Macros declaration------------------------------------*/
#define INTF_VALUE 0x00u
#define INTF_SHIFT_VALUE 0x01u
/*User typedef------------------------------------------*/


/*Private variables definition--------------------------*/

static uint16_t button_req_status_u16[Source_max];


static void INTFLF_Button_translate_request(Input_Status status_e, Input_Source source_e);


static void INTFLF_Button_translate_request(Input_Status status_e, Input_Source source_e)
{

	if(Increment == source_e && status_e == Input_high)
	{
		button_req_status_u16[Increment] <<= INTF_SHIFT_VALUE;

		if(button_req_status_u16[Increment] == Incrementing_max_status_Requested)
		{
			button_req_status_u16[Increment]  = Incrementing_Idle_Requested;
		}
	}
	else if(Decrement == source_e && status_e == Input_high)
	{
		button_req_status_u16[Decrement] <<= INTF_SHIFT_VALUE;

		if(button_req_status_u16[Decrement] == Decrementing_max_status_Requested)
		{
			button_req_status_u16[Decrement] = Decrementing_Idle_Requested;
		}
	}
	else if(Set == source_e && status_e == Input_high)
	{
		button_req_status_u16[Set] <<= INTF_SHIFT_VALUE;

		if(button_req_status_u16[Set]  == Set_max_status_Requested)
		{
			button_req_status_u16[Set]  = (uint16_t)Setting_Idle_Requested;
		}
	}
	else if(Alarm == source_e && status_e == Input_high)
	{
		button_req_status_u16[Alarm] <<= INTF_SHIFT_VALUE;

		if(button_req_status_u16[Alarm] == Decrementing_max_status_Requested)
		{
			button_req_status_u16[Alarm] = (uint16_t)Decrementing_Idle_Requested;
		}
	}

}

/*Public functions--------------------------------------*/

void INTFEF_Thread(void)
{
	uint8_t button_index_u8 = INTF_VALUE;
	Input_Status button_status = Input_low;

	for(button_index_u8 = Increment; button_index_u8 < Source_max; button_index_u8++)
	{
		button_status = Io_Read(button_index_u8);
		INTFLF_Button_translate_request(button_status, button_index_u8);
	}

}

void INTFEF_Init(void)
{
	button_req_status_u16[Set]         = Setting_Idle_Requested;
	button_req_status_u16[Alarm]       = Alarm_idle_Requested;
	button_req_status_u16[Increment]   = Incrementing_Idle_Requested;
	button_req_status_u16[Decrement]   = Decrementing_Idle_Requested;
}

uint16_t INTFEF_Get_Button_Req(Input_Source source_e)
{
	return button_req_status_u16[source_e];
}


