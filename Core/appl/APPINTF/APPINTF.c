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

#define APPIF_HRS (0x00u)
#define APPIF_MIN (0x01u)
#define APPIF_SEC (0x02u)

#define APPIF_YEAR   (0x00u)
#define APPIF_MOUNTH (0x01u)
#define APPIF_DAY    (0x02u)

#define APPIF_MAX_MESSAGES (0x0Cu)
/*User typedef------------------------------------------*/


/*Private variables definition--------------------------*/

//static volatile uint16_t push_button_status_u16[Source_max];

static button_descriptor push_button_status_u16[Source_max];

static S_Texts_LCD_Status appif_uprow_buffer_LCD;
static S_Texts_LCD_Status appif_downrow_buffer_LCD;

static S_Print_Texts_LCD menu_texts_formart_s[APPIF_MAX_MESSAGES] =
    {
	{Row_1, Column_4, (unsigned char *)"%02d:%02d:%02d"  }, //Time message      Index -> 0
	{Row_2, Column_4, (unsigned char *)"%02d/%02d/%4d"   }, //Date message      Index -> 1
	{Row_1, Column_1, (unsigned char *)"Settings        "}, //Settings Enter    Index -> 2
	{Row_1, Column_1, (unsigned char *)"Enter Hour:%02d "}, //Set Hours         Index -> 3
	{Row_1, Column_1, (unsigned char *)"Enter Min:%02d  "}, //Set Minutes       Index -> 4
	{Row_1, Column_1, (unsigned char *)"Enter Sec:%02d  "}, //Set Seconds       Index -> 5
	{Row_1, Column_1, (unsigned char *)"Enter Day:%02d   "}, //Set Day          Index -> 6
	{Row_1, Column_1, (unsigned char *)"Enter Month:%02d  "}, //Set Month       Index -> 7
	{Row_1, Column_1, (unsigned char *)"Enter Year:20%02d"}, //Set Year         Index -> 8
	{Row_2, Column_1, (unsigned char *)"                "}, //Set Year          Index -> 9
	{Row_1, Column_1, (unsigned char *)"Saving Settings "}, //Set Year          Index -> 10
    };

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

  appif_uprow_buffer_LCD.colum_position = INTF_VALUE;
  appif_uprow_buffer_LCD.row_position = INTF_VALUE;
  memset (appif_uprow_buffer_LCD.appif_out_buffer_u8, INTF_VALUE,
	  sizeof(appif_uprow_buffer_LCD.appif_out_buffer_u8));

  appif_downrow_buffer_LCD.colum_position = INTF_VALUE;
  appif_downrow_buffer_LCD.row_position = INTF_VALUE;
  memset (appif_downrow_buffer_LCD.appif_out_buffer_u8, INTF_VALUE,
	  sizeof(appif_downrow_buffer_LCD.appif_out_buffer_u8));
}

void APPIFEF_Get_Button_Req(Input_Source source_e, button_descriptor * out_data)
{
  out_data->button_status = push_button_status_u16[source_e].button_status;
  out_data->push_buttonuest_u16 = push_button_status_u16[source_e].push_buttonuest_u16;
}

void APPIFEF_Set_Button_Status(Input_Source source_e, button_status button_status_e)
{
  push_button_status_u16[source_e].button_status = button_status_e;
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

void APPIFEF_Get_Time(S_Texts_LCD_Status * outbuffer_pS)
{
  memcpy(outbuffer_pS, &appif_uprow_buffer_LCD, sizeof(appif_uprow_buffer_LCD));
}

void APPIFEF_Get_Date(S_Texts_LCD_Status * outbuffer_pS)
{
  memcpy(outbuffer_pS, &appif_downrow_buffer_LCD, sizeof(appif_downrow_buffer_LCD));
}

void APPIFEF_Send_Time_Display(uint8_t * time_buffer_pu8)
{
    sprintf((char *)appif_uprow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[0].texts_lcd, time_buffer_pu8[APPIF_HRS], time_buffer_pu8[APPIF_MIN], time_buffer_pu8[APPIF_SEC]);
    appif_uprow_buffer_LCD.colum_position = menu_texts_formart_s[0].colum_position;
    appif_uprow_buffer_LCD.row_position = menu_texts_formart_s[0].row_position;

}

void APPIFEF_Send_Date_Display(uint8_t * date_buffer_pu8)
{
  sprintf((char *)appif_downrow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[1].texts_lcd, date_buffer_pu8[APPIF_DAY], date_buffer_pu8[APPIF_MOUNTH], 2000 + date_buffer_pu8[APPIF_YEAR]);
  appif_downrow_buffer_LCD.colum_position = menu_texts_formart_s[1].colum_position;
  appif_downrow_buffer_LCD.row_position = menu_texts_formart_s[1].row_position;
}

void APPIFEF_Send_String_Settings(void)
{
  //Sending Settings on screen
  sprintf((char *)appif_uprow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[2].texts_lcd);
  appif_uprow_buffer_LCD.colum_position = menu_texts_formart_s[2].colum_position;
  appif_uprow_buffer_LCD.row_position = menu_texts_formart_s[2].row_position;

  //Clearing low row
  sprintf((char *)appif_downrow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[9].texts_lcd);
  appif_downrow_buffer_LCD.colum_position = menu_texts_formart_s[9].colum_position;
  appif_downrow_buffer_LCD.row_position = menu_texts_formart_s[9].row_position;
}

void APPIFEF_Send_String_Saving(void)
{
  //Sending Settings on screen
  sprintf((char *)appif_uprow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[10].texts_lcd);
  appif_uprow_buffer_LCD.colum_position = menu_texts_formart_s[2].colum_position;
  appif_uprow_buffer_LCD.row_position = menu_texts_formart_s[2].row_position;

  //Clearing low row
  sprintf((char *)appif_downrow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[9].texts_lcd);
  appif_downrow_buffer_LCD.colum_position = menu_texts_formart_s[9].colum_position;
  appif_downrow_buffer_LCD.row_position = menu_texts_formart_s[9].row_position;
}

void APPIFEF_Send_Setting(uint8_t date_buffer_pu8, uint8_t index_string_u8)
{
  //Sending Settings on screen

  sprintf((char *)appif_uprow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[index_string_u8].texts_lcd, date_buffer_pu8);
  appif_uprow_buffer_LCD.colum_position = menu_texts_formart_s[index_string_u8].colum_position;
  appif_uprow_buffer_LCD.row_position = menu_texts_formart_s[index_string_u8].row_position;

  //Clearing low row
  sprintf((char *)appif_downrow_buffer_LCD.appif_out_buffer_u8, (char *)menu_texts_formart_s[9].texts_lcd);
  appif_downrow_buffer_LCD.colum_position = menu_texts_formart_s[9].colum_position;
  appif_downrow_buffer_LCD.row_position = menu_texts_formart_s[9].row_position;
}


void APPIFEF_Clear(void)
{
  Clear();
}



