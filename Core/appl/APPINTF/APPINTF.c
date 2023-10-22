/*
 * INPUTIF.c
 *
 *  Created on: Sep 18, 2023
 *      Author: Jesus
 */


#include "../appl/APPINTF/APPINTF.h"
#include "../appl/DIGITALINPUT/DIGITALINPUT.h"

#include <string.h>
#include <stdio.h>

/*Macros declaration------------------------------------*/
#define INTF_VALUE          (0x00u)
#define INTF_SHIFT_VALUE    (0x01u)
#define APPIF_MAX_LCD_DIGIT   (16u)

#define APPIF_HRS (0x00u)
#define APPIF_MIN (0x01u)
#define APPIF_SEC (0x02u)

#define APPIF_YEAR   (0x00u)
#define APPIF_MOUNTH (0x01u)
#define APPIF_DAY    (0x02u)
/*User typedef------------------------------------------*/


/*Private variables definition--------------------------*/

static uint16_t button_req_status_u16[Source_max];
static uint8_t appif_time_buffer_u8[APPIF_MAX_LCD_DIGIT];
static uint8_t appif_date_buffer_u8[APPIF_MAX_LCD_DIGIT];


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

void APPIFEF_Thread(void)
{
	uint8_t button_index_u8 = INTF_VALUE;
	Input_Status button_status = Input_low;

	for(button_index_u8 = Increment; button_index_u8 < Source_max; button_index_u8++)
	{
		button_status = Io_Read(button_index_u8);
		INTFLF_Button_translate_request(button_status, button_index_u8);
	}

}

void APPIFEF_Init(void)
{
	button_req_status_u16[Set]         = Setting_Idle_Requested;
	button_req_status_u16[Alarm]       = Alarm_idle_Requested;
	button_req_status_u16[Increment]   = Incrementing_Idle_Requested;
	button_req_status_u16[Decrement]   = Decrementing_Idle_Requested;
}

uint16_t APPIFEF_Get_Button_Req(Input_Source source_e)
{
	return button_req_status_u16[source_e];
}

void APPIFEF_Send_Time_Display(uint8_t * time_buffer_pu8)
{
  sprintf((char *)appif_time_buffer_u8, "%02d:%02d:%02d", time_buffer_pu8[APPIF_HRS], time_buffer_pu8[APPIF_MIN], time_buffer_pu8[APPIF_SEC]);

}
void APPIFEF_Send_Date_Display(uint8_t * date_buffer_pu8)
{
  sprintf((char *)appif_date_buffer_u8, "%02d/%02d/%4d", date_buffer_pu8[APPIF_DAY], date_buffer_pu8[APPIF_MOUNTH], date_buffer_pu8[APPIF_YEAR]);
}

void APPIFEF_Get_Time(uint8_t * time_buffer_pu8)
{
  memcpy(time_buffer_pu8, appif_time_buffer_u8, sizeof(appif_time_buffer_u8));
}
void APPIFEF_Get_Date(uint8_t * date_buffer_pu8)
{
  date_buffer_pu8 = appif_date_buffer_u8;
}

