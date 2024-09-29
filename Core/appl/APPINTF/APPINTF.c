/*
 * INPUTIF.c
 *
 *  Created on: Sep 18, 2023
 *      Author: Jesus
 */


#include "../appl/APPINTF/APPINTF.h"
#include "../appl/DIGITALINPUT/DIGITALINPUT.h"
#include "../appl/BUZZER/BUZZER.h"

#include <string.h>
#include <stdio.h>

/*Macros declaration------------------------------------*/
#define INTF_VALUE          (0x00u)
#define INTF_SHIFT_VALUE    (0x01u)

#define APPIF_HRS (0x00u)
#define APPIF_MIN (0x01u)
#define APPIF_SEC (0x02u)

#define APPIF_YEAR   (0x00u)
#define APPIF_MOUNTH (0x01u)
#define APPIF_DAY    (0x02u)

/*User typedef------------------------------------------*/

typedef enum APPIF_Date_info_t
{
	APPIF_time_txt,
	APPIF_date_txt,
	APPIF_setting_Enter_txt,
	APPIF_set_hours_txt,
	APPIF_set_minutes_txt,
	APPIF_set_time_format_txt,
	APPIF_set_day_txt,
	APPIF_set_month_txt,
	APPIF_set_year_txt,
	APPIF_set_empty_txt,
	APPIF_saving_settings_txt,
	APPIF_working_on_txt,

	APPIF_Txt_Max
}APPIF_Date_info_T;

typedef enum APPIF_time_info_t
{
        APPIF_hours,
	APPIF_minutes,
	APPIF_seconds,
	APPIF_time_format,

	APPIF_time_info_Max
}APPIF_time_info_T;
/*Private variables definition--------------------------*/

//static volatile uint16_t push_button_status_u16[Source_max];

static button_descriptor push_button_status_u16[Source_max];

static LCD_Out_Buffer_T APPIF_LCD_Out_Buffer;

static void INTFLF_Button_translate_request(Input_Status status_e, Input_Source source_e);

static void INTFLF_Button_translate_request(Input_Status status_e, Input_Source source_e)
{

	if(Increment == source_e && Input_high == status_e && (button_idle == push_button_status_u16[Increment].button_status || button_proccessed == push_button_status_u16[Increment].button_status ))
	{
	    push_button_status_u16[Increment].push_buttonuest_u16 <<= INTF_SHIFT_VALUE;
	    push_button_status_u16[Increment].button_status = button_pushed;

		if(push_button_status_u16[Increment].push_buttonuest_u16 == Incrementing_max_status_Requested)
		{
		    push_button_status_u16[Increment].push_buttonuest_u16  = Incrementing_Idle_Requested;
			push_button_status_u16[Increment].button_status = button_idle;
		}
	}
	else if(Decrement == source_e && Input_high == status_e && (button_idle == push_button_status_u16[Decrement].button_status || button_proccessed == push_button_status_u16[Decrement].button_status))
	{
	    push_button_status_u16[Decrement].push_buttonuest_u16 <<= INTF_SHIFT_VALUE;
	    push_button_status_u16[Decrement].button_status = button_pushed;

		if(push_button_status_u16[Decrement].push_buttonuest_u16 == Decrementing_max_status_Requested)
		{
		    push_button_status_u16[Decrement].push_buttonuest_u16  = Decrementing_Idle_Requested;
			push_button_status_u16[Decrement].button_status = button_idle;
		}
	}
	else if(Set == source_e && Input_high == status_e && (button_idle == push_button_status_u16[Set].button_status || button_proccessed == push_button_status_u16[Set].button_status))
	{

		push_button_status_u16[Set].push_buttonuest_u16 <<= INTF_SHIFT_VALUE;
		push_button_status_u16[Set].button_status = button_pushed;

		if(push_button_status_u16[Set].push_buttonuest_u16 == Setting_Exit_Requested)
		{
		    push_button_status_u16[Set].push_buttonuest_u16  = Setting_Idle_Requested;
			push_button_status_u16[Set].button_status = button_idle;
		}
	}
	else if(Alarm == source_e && status_e == Input_high)
	{

		push_button_status_u16[Alarm].push_buttonuest_u16 <<= INTF_SHIFT_VALUE;
		push_button_status_u16[Alarm].button_status = button_pushed;

		if(push_button_status_u16[Alarm].push_buttonuest_u16 == Alarm_max_Requested)
		{
		    push_button_status_u16[Alarm].push_buttonuest_u16  = Alarm_idle_Requested;
			push_button_status_u16[Alarm].button_status = button_idle;
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

  push_button_status_u16[Increment].push_buttonuest_u16 = Incrementing_Idle_Requested;
  push_button_status_u16[Increment].button_status = button_idle;

  push_button_status_u16[Decrement].push_buttonuest_u16 = Decrementing_Idle_Requested;
  push_button_status_u16[Decrement].button_status = button_idle;

  push_button_status_u16[Set].push_buttonuest_u16 = Setting_Idle_Requested;
  push_button_status_u16[Set].button_status = button_idle;

  push_button_status_u16[Alarm].push_buttonuest_u16 = Alarm_idle_Requested;
  push_button_status_u16[Alarm].button_status = button_idle;

  APPIF_LCD_Out_Buffer.Up_Row_Buffer.colum_position = INTF_VALUE;
  memset (APPIF_LCD_Out_Buffer.Up_Row_Buffer.appif_out_buffer_u8, INTF_VALUE,
	  sizeof(APPIF_LCD_Out_Buffer.Up_Row_Buffer.appif_out_buffer_u8));

  APPIF_LCD_Out_Buffer.Down_Row_Buffer.colum_position = INTF_VALUE;
  memset (APPIF_LCD_Out_Buffer.Down_Row_Buffer.appif_out_buffer_u8, INTF_VALUE,
	  sizeof(APPIF_LCD_Out_Buffer.Down_Row_Buffer.appif_out_buffer_u8));
}

void APPIFEF_Get_Button_Req(Input_Source source_e, button_descriptor * out_data)
{
  out_data->button_status = push_button_status_u16[source_e].button_status;
  out_data->push_buttonuest_u16 = push_button_status_u16[source_e].push_buttonuest_u16;
}

void APPIFEF_Set_Button_Status(Input_Source source_e, button_status button_status_e)
{
  push_button_status_u16[source_e].button_status = button_status_e;
  buzzer_beep_on();
}

void APPIFEF_Clear_push_button(Input_Source source_e)
{
  if(Increment == source_e)
    {
      push_button_status_u16[Increment].push_buttonuest_u16 = Incrementing_Idle_Requested;
    }
  else if(Decrement == source_e)
    {
      push_button_status_u16[Decrement].push_buttonuest_u16 = Decrementing_Idle_Requested;
    }
}

void APPIFEF_Get_OutBuffer(LCD_Out_Buffer_T * prt_outbuffer)
{
  memcpy(prt_outbuffer, &APPIF_LCD_Out_Buffer, sizeof(APPIF_LCD_Out_Buffer));
}


void APPIFEF_Send_LCD(LCD_Out_Buffer_T * ptr_str)
{
  memcpy( APPIF_LCD_Out_Buffer.Up_Row_Buffer.appif_out_buffer_u8, ptr_str->Up_Row_Buffer.appif_out_buffer_u8, sizeof(APPIF_LCD_Out_Buffer.Up_Row_Buffer.appif_out_buffer_u8));
  APPIF_LCD_Out_Buffer.Up_Row_Buffer.colum_position = ptr_str->Up_Row_Buffer.colum_position;

  memcpy(APPIF_LCD_Out_Buffer.Down_Row_Buffer.appif_out_buffer_u8, ptr_str->Down_Row_Buffer.appif_out_buffer_u8, sizeof(APPIF_LCD_Out_Buffer.Down_Row_Buffer.appif_out_buffer_u8));
  APPIF_LCD_Out_Buffer.Down_Row_Buffer.colum_position = ptr_str->Down_Row_Buffer.colum_position;
}


void APPIFEF_Clear(void)
{
  memset(APPIF_LCD_Out_Buffer.Up_Row_Buffer.appif_out_buffer_u8, ' ', sizeof(APPIF_LCD_Out_Buffer.Up_Row_Buffer.appif_out_buffer_u8));
  memset(APPIF_LCD_Out_Buffer.Down_Row_Buffer.appif_out_buffer_u8, ' ', sizeof(APPIF_LCD_Out_Buffer.Up_Row_Buffer.appif_out_buffer_u8));
  Clear();
}




