/*
 * INPUTIF.h
 *
 *  Created on: Sep 18, 2023
 *      Author: Jesus
 */

#ifndef APPL_APPINTF_APPINTF_H_
#define APPL_APPINTF_APPINTF_H_

#include "../appl/DIGITALINPUT/DIGITALINPUT.h"
#include "../appl/LCD/LCD16.h"

/*Macros declaration------------------------------------*/
#define APPIF_MAX_LCD_DIGIT   (17u)
/*User typedef------------------------------------------*/

typedef enum button_status_TAG
{
  button_idle,
  button_pushed,
  button_proccessed,

  max_button
}button_status;

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


typedef struct Print_Texts_LCD_TAG
{
  Row_lcd row_position;
  Column_lcd colum_position;
  uint8_t * texts_lcd;
}S_Print_Texts_LCD;


typedef struct Texts_LCD_Status_TAG
{

  Row_lcd row_position;
  Column_lcd colum_position;
  uint8_t appif_out_buffer_u8[APPIF_MAX_LCD_DIGIT];

}S_Texts_LCD_Status;

typedef struct
{
  uint16_t push_buttonuest_u16;
  button_status button_status;
}button_descriptor;

/*Public functions--------------------------------------*/

void APPIFEF_Thread(void);
void APPIFEF_Init(void);

void APPIFEF_Get_Button_Req(Input_Source source_e, button_descriptor * out_data);
void APPIFEF_Set_Button_Status(Input_Source source_e, button_status button_status_e);
void APPIFEF_Clear_push_button(Input_Source source_e);

void APPIFEF_Send_Time_Display(uint8_t * time_buffer_pu8);
void APPIFEF_Send_Date_Display(uint8_t * date_buffer_pu8);

void APPIFEF_Get_Time(S_Texts_LCD_Status * outbuffer_pS);
void APPIFEF_Get_Date(S_Texts_LCD_Status * outbuffer_pS);

void APPIFEF_Send_String_Settings(void);
void APPIFEF_Send_String_Saving(void);
void APPIFEF_Send_Setting(uint8_t  date_buffer_pu8, uint8_t index_string_u8);
void APPIFEF_Send_String_Alarm(void);

void APPIFEF_Clear(void);


#endif /* APPL_INPUTIF_INPUTIF_H_ */
