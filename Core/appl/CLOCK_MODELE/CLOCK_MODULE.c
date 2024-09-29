/*******************************************************************************
* Title                 :   System Initialization
* Filename              :   CLOCK_MODULE
* Author                :   Jesus
* Origin Date           :   Oct 21, 2023
* Version               :   1.0.0
* Compiler              :   Microchip C30 v3.30c
* Target                :   PIC24FJ64GB004
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description 
*  Oct 21, 2023    XXXXXXXXXXX         JWB      Module Created.
*
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/

#include "../appl/CLOCK_MODELE/CLOCK_MODULE.h"
#include "../appl/RTC/RTC.h"
#include "../appl/LCD/LCD16.h"
#include "../appl/DIGITALINPUT/DIGITALINPUT.h"
#include "../appl/APPINTF/APPINTF.h"

#include <string.h>
#include <stdio.h>

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/
/**
 *Doxygen tag for documenting variables and constants
 */
#define   CONSTANT					5

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define CLK_MAX_TIME_BUFFER (0x03u)
#define CLK_MAX_DATE_BUFFER (0x03u)

#define CLK_HRS    (0x00u)
#define CLK_MIN    (0x01u)
#define CLK_SEC    (0x02u)

#define CLK_YEAR   (0x00u)
#define CLK_MOUNTH (0x01u)
#define CLK_DAY    (0x02u)

#define CLK_UNIT_ONE (0x01)

#define YEAR_2000 (2000u)
/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum main_clock_TAG
{
  init,
  print,
  set_time,
  set_alaram

}E_main_clock_states;

typedef enum setting_menu_TAG
{
  setting_init,
  hour_setting,
  min_setting,
  time_formart_setting,
  year_setting,
  month_setting,
  day_setting,
  exit_setting

}E_setting_menu_states;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
E_main_clock_states main_clock_state_e = init;
E_setting_menu_states settings_menu_state_e = setting_init;
uint8_t delay_state = 0x00u;

static uint8_t clk_temp_buffer_time[CLK_time_info_Max];
static uint8_t clk_temp_buffer_date[CLK_date_info_Max];
/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void FSMLF_Menu_Config(void);
static void CLKLF_Set_Hour(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Minute(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Time_Format(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Year(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Day(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Month(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKF_Print_Time_Date(void);

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function : FSMEF_Clock_Thread()
*
*
* @brief:  This function is the main function that will coordinate the main state machine of the clock
*
* @param:   None.
*
* @return:  None.
*
*
*******************************************************************************/
void FSMEF_Clock_Thread(void)
{
  button_descriptor set_button;
  button_descriptor alarm_button;
  LCD_Out_Buffer_T local_LCD_str;

  APPIFEF_Get_Button_Req(Set, &set_button);
  APPIFEF_Get_Button_Req(Alarm, &alarm_button);

  switch(main_clock_state_e)
  {
    case init:
      main_clock_state_e = print;

      break;
    case print:

      CLKF_Print_Time_Date();

      if(Setting_Enter_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  main_clock_state_e = set_time;
	  settings_menu_state_e = setting_init;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      else if(Alarm_Ente_Requested == alarm_button.push_buttonuest_u16 && button_pushed == alarm_button.button_status)
	{
	  main_clock_state_e = set_alaram;
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}

      break;
    case set_time:

      FSMLF_Menu_Config();

      break;
    case set_alaram:

      strncpy((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Working on it :)", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

      strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

      APPIFEF_Send_LCD(&local_LCD_str);

      if(Alarm_Exit_Requested == alarm_button.push_buttonuest_u16 && button_pushed == alarm_button.button_status)
	{
	  main_clock_state_e = print;
	  APPIFEF_Clear();
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}
      break;
    default:

      break;
  }
}

/******************************************************************************
* Function : FSMLF_Menu_Config()
*
*
* @brief:  This function is the main function that will coordinate the main state machine of the clock
*
* @param:   None.
*
* @return:  None.
*
*
*******************************************************************************/
static void FSMLF_Menu_Config(void)
{
  button_descriptor set_button;
  button_descriptor increment_button;
  button_descriptor decrement_button;
  LCD_Out_Buffer_T local_LCD_str;

  APPIFEF_Get_Button_Req(Set, &set_button);
  APPIFEF_Get_Button_Req(Increment, &increment_button);
  APPIFEF_Get_Button_Req(Decrement, &decrement_button);

  switch(settings_menu_state_e)
  {
    case setting_init:

      delay_state += CLK_UNIT_ONE;

      strncpy((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Settings        ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

      strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

      APPIFEF_Send_LCD(&local_LCD_str);

      if(0x04 == delay_state)
	{
	  settings_menu_state_e = hour_setting;
	  delay_state = 0x00u;
	}

      break;
    case hour_setting:

      CLKLF_Set_Hour(&increment_button, &decrement_button);

      if(Setting_Hour_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = min_setting;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case min_setting:

      CLKLF_Set_Minute(&increment_button, &decrement_button);

      if(Setting_Min_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = time_formart_setting;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case time_formart_setting:

      CLKLF_Set_Time_Format(&increment_button, &decrement_button);

      if(Setting_Sec_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = year_setting;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;

    case year_setting:

      CLKLF_Set_Year(&increment_button, &decrement_button);
      if(Setting_Year_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = month_setting;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case month_setting:

      CLKLF_Set_Month(&increment_button, &decrement_button);
      if(Setting_Month_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = day_setting;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case day_setting:

      CLKLF_Set_Day(&increment_button, &decrement_button);
      if(Setting_Day_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = exit_setting;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    default:

      delay_state += CLK_UNIT_ONE;

      strncpy((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Saving Settings ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Up_Row_Buffer.colum_position = Column_1;


      strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

      APPIFEF_Send_LCD(&local_LCD_str);

      if(0x04 == delay_state)
	{
	  main_clock_state_e = print;
	  delay_state = 0x00u;
	  WriteDate(clk_temp_buffer_date);
	  WriteTime(clk_temp_buffer_time);
	  APPIFEF_Clear();
	}

      break;
  }
}

static void CLKLF_Set_Hour(button_descriptor * button_increment, button_descriptor * button_decrement)
{
  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_time[CLK_hours] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_time[CLK_hours] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_time[CLK_hours] <= 0x00u)
    {
      clk_temp_buffer_time[CLK_hours] = 0x00u;
    }
  else if (clk_temp_buffer_time[CLK_hours] >= 12)
    {
      clk_temp_buffer_time[CLK_hours] = 12;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Hour:%02d ", clk_temp_buffer_time[CLK_hours]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);


}

static void CLKLF_Set_Minute(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_time[CLK_minutes] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_time[CLK_minutes] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_time[CLK_minutes] <= 0x00u)
    {
      clk_temp_buffer_time[CLK_minutes] = 0x00u;
    }
  else if (clk_temp_buffer_time[CLK_minutes] >= 60)
    {
      clk_temp_buffer_time[CLK_minutes] = 60;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Min:%02d  ", clk_temp_buffer_time[CLK_minutes]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void CLKLF_Set_Time_Format(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;
  uint8_t am_pm_txt[3];

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_time[CLK_time_format] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_time[CLK_time_format] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_time[CLK_time_format] <= 0x00u)
    {
      clk_temp_buffer_time[CLK_time_format] = 0x00u;
      strcpy((char *)am_pm_txt, "am");
    }
  else if (clk_temp_buffer_time[CLK_time_format] >= 1)
    {
      clk_temp_buffer_time[CLK_time_format] = 1;
      strcpy((char *)am_pm_txt, "pm");
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Time: %s     ", am_pm_txt);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void CLKLF_Set_Year(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_date[CLK_year] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_date[CLK_year] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_date[CLK_year] <= 0x00u)
    {
      clk_temp_buffer_date[CLK_year] = 0x00u;
    }
  else if (clk_temp_buffer_date[CLK_year] >= 99u)
    {
      clk_temp_buffer_date[CLK_year] = 99;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Year:20%02d", clk_temp_buffer_date[CLK_year]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void CLKLF_Set_Day(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_date[CLK_day] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_date[CLK_day] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_date[CLK_day] <= 0x00u)
    {
      clk_temp_buffer_date[CLK_day] = 0x00u;
    }
  else if (clk_temp_buffer_date[CLK_day] >= 30)
    {
      clk_temp_buffer_date[CLK_day] = 30;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Day:%02d   ", clk_temp_buffer_date[CLK_day]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void CLKLF_Set_Month(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
      clk_temp_buffer_date[CLK_month] += CLK_UNIT_ONE;
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_date[CLK_month] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }

  if (clk_temp_buffer_date[CLK_month] <= 0x00u)
    {
      clk_temp_buffer_date[CLK_month] = 0x00u;
    }
  else if (clk_temp_buffer_date[CLK_month] >= 12u)
    {
      clk_temp_buffer_date[CLK_month] = 12u;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Month:%02d ", clk_temp_buffer_date[CLK_month]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}


static void CLKF_Print_Time_Date(void)
{
  uint8_t am_pm_txt[3];
  LCD_Out_Buffer_T local_LCD_str;

  //reading time from CLK SW component
  ReadTime(clk_temp_buffer_time);

  //reading date from RTC SW component
  ReadDate(clk_temp_buffer_date);

  //sending time to Appitf SW component
  if(0u  == clk_temp_buffer_time[CLK_time_format])
    {
      strcpy((char *)am_pm_txt, "am");
    }
  else
    {
      strcpy((char *)am_pm_txt, "pm");
    }
  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "%02d:%02d:%02d %s", clk_temp_buffer_time[CLK_hours], clk_temp_buffer_time[CLK_minutes], clk_temp_buffer_time[CLK_seconds], (char *)am_pm_txt);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_4;

  sprintf((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "%02d/%02d/%4d", clk_temp_buffer_date[CLK_month], clk_temp_buffer_date[CLK_day], YEAR_2000 + clk_temp_buffer_date[CLK_year]);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_4;

  //sending date to Appitf SW component
  APPIFEF_Send_LCD(&local_LCD_str);
}

/*************** END OF FUNCTIONS ***************************************************************************/
