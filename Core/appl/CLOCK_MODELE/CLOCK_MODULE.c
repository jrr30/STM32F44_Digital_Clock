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

#define CLK_UNIT_ONE (0x01u)

#define TIMER_TICK   (0x01u)
#define EXPIRE_TIMER (0x00u)
#define START_TIMER  (0x04u)

#define YEAR_2000 (2000u)

#define ALARM_STATE_TABLE \
  ENTRY(State_Init_Alarm, Alarm_Set_Init)\
  ENTRY(State_Set_Hr, Alarm_Set_Hr)\
  ENTRY(State_Set_Mn, Alarm_Set_Mn)\
  ENTRY(State_Set_Time_Format, Alarm_Timet_Formart)\
  ENTRY(State_Set_Days, Alarm_Week_Days)\
  ENTRY(State_Exit_Alarm, Alarm_Set_Exit)\

#define TIMEDATE_STATE_TABLE \
  ENTRY(State_Init_TimeDate, TimeDate_Set_Init)\
  ENTRY(State_Set_Hr_TimeDate,TimeDate_Set_Hrs)\
  ENTRY(State_Set_Mn_TimeDate, TimeDate_Set_Mn)\
  ENTRY(State_Set_Time_Format_TimeDate, TimeDate_Set_Format)\
  ENTRY(State_Set_year_TimeDate, TimeDate_Set_Year)\
  ENTRY(State_Set_month_TimeDate, TimeDate_Set_Month)\
  ENTRY(State_Set_day_TimeDate, TimeDate_Set_Day)\
  ENTRY(State_Exit_TimeDate, TimeDate_Set_Exit)\


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

typedef enum alarm_availability_TAG
{

  alarm_enable = 0x01u,
  alarm_disable = 0xB0u

}E_alarm_availability;

typedef enum setting_menu_TAG
{
#define ENTRY(a,b) a,
  TIMEDATE_STATE_TABLE
#undef ENTRY

  max_state_TimeDate
}E_setting_menu_states;

typedef enum alarm_setttings_states_t
{

#define ENTRY(a,b) a,
  ALARM_STATE_TABLE
#undef ENTRY

  max_state_alarm
}E_alarm_setttings_states_T;


#define ENTRY(a,b) static void b(button_descriptor * button_increment, button_descriptor * button_decrement);
  ALARM_STATE_TABLE
#undef ENTRY

#define ENTRY(a,b) static void b(button_descriptor * button_increment, button_descriptor * button_decrement);
  TIMEDATE_STATE_TABLE
#undef ENTRY



/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
E_main_clock_states main_clock_state_e = init;
E_setting_menu_states settings_menu_state_e = State_Init_TimeDate;
E_alarm_setttings_states_T alarm_settings_state_e = State_Init_Alarm;
uint8_t delay_state = 0x00u;

static uint8_t timer_start_exit_alarm;

static uint8_t clk_temp_buffer_timeDate[CLK_timeDate_Max];
static uint8_t clk_temp_buffer_time_alarm[CLK_Alram_info_Max];

static uint16_t clk_translation_action_buffer_alarm[CLK_Alram_info_Max] =
    {
	Alarm_idle_Requested,
	Alarm_idle_Requested,
	Alarm_idle_Requested,
	Alarm_Monday_Requested,
	Alarm_Tusday_Requested,
	Alarm_Wensday_Requested,
	Alarm_Thursday_Requested,
	Alarm_Friday_Requested,
	Alarm_Saturday_Requested,
	Alarm_Sunday_Requested,
    };

/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void FSMLF_Menu_Config(void);
static void FSMLF_Alarm_Menu_Config(void);

static void CLKF_Print_Time_Date(void);

/******************************************************************************
* Function Definitions
*******************************************************************************/

/******************************************************************************
* Function : FSMEF_Clock_Init()
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
void FSMEF_Clock_Init(void)
{
    memset (clk_temp_buffer_time_alarm, 0x00u, sizeof(clk_temp_buffer_time_alarm));
    memset (clk_temp_buffer_timeDate, 0x00u, sizeof(clk_temp_buffer_timeDate));

    timer_start_exit_alarm = START_TIMER;
}


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
//  LCD_Out_Buffer_T local_LCD_str;

  APPIFEF_Get_Button_Req(Set, &set_button);
  APPIFEF_Get_Button_Req(Alarm, &alarm_button);

  switch(main_clock_state_e)
  {
    case init:

      main_clock_state_e = print;

      break;
    case print:

      CLKF_Print_Time_Date();

      if(Setting_Init_Requested == set_button.push_button_action_u16 && button_pushed == set_button.button_status)
	{
	  main_clock_state_e = set_time;
	}
      else if(Alarm_Init_Requested == alarm_button.push_button_action_u16 && button_pushed == alarm_button.button_status)
	{
	  main_clock_state_e = set_alaram;
	}

      break;
    case set_time:

      FSMLF_Menu_Config();

      break;
    case set_alaram:

      FSMLF_Alarm_Menu_Config();
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

  APPIFEF_Get_Button_Req(Set, &set_button);
  APPIFEF_Get_Button_Req(Increment, &increment_button);
  APPIFEF_Get_Button_Req(Decrement, &decrement_button);

  switch(settings_menu_state_e)
  {
    case State_Init_TimeDate:

      TimeDate_Set_Init(&increment_button, &decrement_button);

      break;
    case State_Set_Hr_TimeDate:

      TimeDate_Set_Hrs(&increment_button, &decrement_button);

      if(Setting_Min_Requested == set_button.push_button_action_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = State_Set_Mn_TimeDate;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case State_Set_Mn_TimeDate:

      TimeDate_Set_Mn(&increment_button, &decrement_button);

      if(Setting_Format_Requested == set_button.push_button_action_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = State_Set_Time_Format_TimeDate;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case State_Set_Time_Format_TimeDate:

      TimeDate_Set_Format(&increment_button, &decrement_button);

      if(Setting_Year_Requested == set_button.push_button_action_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = State_Set_year_TimeDate;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;

    case State_Set_year_TimeDate:

      TimeDate_Set_Year(&increment_button, &decrement_button);

      if(Setting_Month_Requested == set_button.push_button_action_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = State_Set_month_TimeDate;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case State_Set_month_TimeDate:

      TimeDate_Set_Month(&increment_button, &decrement_button);

      if(Setting_Day_Requested == set_button.push_button_action_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = State_Set_day_TimeDate;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case State_Set_day_TimeDate:

      TimeDate_Set_Day(&increment_button, &decrement_button);

      if(Setting_Exit_Requested == set_button.push_button_action_u16 && button_pushed == set_button.button_status)
	{
	  settings_menu_state_e = State_Exit_TimeDate;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    default:

      TimeDate_Set_Exit(&increment_button, &decrement_button);
      break;
  }
}

/******************************************************************************
* Function : TimeDate_Set_Init
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
static void TimeDate_Set_Init(button_descriptor * button_increment, button_descriptor * button_decrement)
{
  if(NULL != button_increment && NULL != button_decrement)
    {
      LCD_Out_Buffer_T local_LCD_str;

      if(EXPIRE_TIMER == timer_start_exit_alarm)
	{

	  timer_start_exit_alarm = START_TIMER;
	  settings_menu_state_e = State_Set_Hr_TimeDate;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);

	}
      else
	{
	  timer_start_exit_alarm -= TIMER_TICK;
	}

      strncpy((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Setting Time    ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

      strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

      APPIFEF_Send_LCD(&local_LCD_str);

    }
}

static void TimeDate_Set_Hrs(button_descriptor * button_increment, button_descriptor * button_decrement)
{
  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_timeDate[CLK_hours] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_timeDate[CLK_hours] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_timeDate[CLK_hours] <= 0x00u)
    {
      clk_temp_buffer_timeDate[CLK_hours] = 0x00u;
    }
  else if (clk_temp_buffer_timeDate[CLK_hours] >= 12)
    {
      clk_temp_buffer_timeDate[CLK_hours] = 12;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Hour:%02d ", clk_temp_buffer_timeDate[CLK_hours]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);


}

static void TimeDate_Set_Mn(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_timeDate[CLK_minutes] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_timeDate[CLK_minutes] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_timeDate[CLK_minutes] <= 0x00u)
    {
      clk_temp_buffer_timeDate[CLK_minutes] = 0x00u;
    }
  else if (clk_temp_buffer_timeDate[CLK_minutes] >= 60)
    {
      clk_temp_buffer_timeDate[CLK_minutes] = 60;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Min:%02d  ", clk_temp_buffer_timeDate[CLK_minutes]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void TimeDate_Set_Format(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;
  uint8_t am_pm_txt[3];

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_timeDate[CLK_time_format] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_timeDate[CLK_time_format] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_timeDate[CLK_time_format] <= 0x00u)
    {
      clk_temp_buffer_timeDate[CLK_time_format] = 0x00u;
      strcpy((char *)am_pm_txt, "am");
    }
  else if (clk_temp_buffer_timeDate[CLK_time_format] >= 1)
    {
      clk_temp_buffer_timeDate[CLK_time_format] = 1;
      strcpy((char *)am_pm_txt, "pm");
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Time: %s     ", am_pm_txt);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void TimeDate_Set_Year(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_timeDate[CLK_year] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_timeDate[CLK_year] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_timeDate[CLK_year] <= 0x00u)
    {
      clk_temp_buffer_timeDate[CLK_year] = 0x00u;
    }
  else if (clk_temp_buffer_timeDate[CLK_year] >= 99u)
    {
      clk_temp_buffer_timeDate[CLK_year] = 99;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Year:20%02d", clk_temp_buffer_timeDate[CLK_year]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void TimeDate_Set_Day(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_timeDate[CLK_day] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_timeDate[CLK_day] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_timeDate[CLK_day] <= 0x00u)
    {
      clk_temp_buffer_timeDate[CLK_day] = 0x00u;
    }
  else if (clk_temp_buffer_timeDate[CLK_day] >= 30)
    {
      clk_temp_buffer_timeDate[CLK_day] = 30;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Day:%02d   ", clk_temp_buffer_timeDate[CLK_day]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

static void TimeDate_Set_Month(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
      clk_temp_buffer_timeDate[CLK_month] += CLK_UNIT_ONE;
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_timeDate[CLK_month] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }

  if (clk_temp_buffer_timeDate[CLK_month] <= 0x00u)
    {
      clk_temp_buffer_timeDate[CLK_month] = 0x00u;
    }
  else if (clk_temp_buffer_timeDate[CLK_month] >= 12u)
    {
      clk_temp_buffer_timeDate[CLK_month] = 12u;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter Month:%02d ", clk_temp_buffer_timeDate[CLK_month]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

/******************************************************************************
* Function : TimeDate_Set_Exit
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
void TimeDate_Set_Exit(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  if(NULL != button_increment && NULL != button_decrement)
    {
      LCD_Out_Buffer_T local_LCD_str;

      strncpy((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Saving Settings ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

      strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

      APPIFEF_Send_LCD(&local_LCD_str);

      if(EXPIRE_TIMER == timer_start_exit_alarm)
	{
	  Write_TimeDate(clk_temp_buffer_timeDate, CLK_timeDate_Max);

	  settings_menu_state_e = State_Init_TimeDate;
	  main_clock_state_e = print;

	  APPIFEF_Set_Button_Status (Set, button_proccessed);
	  APPIFEF_Clear_push_button (Set);

	  APPIFEF_Clear();

	  timer_start_exit_alarm = START_TIMER;
	}
      else
	{
	  timer_start_exit_alarm -= TIMER_TICK;
	}

    }
}


static void CLKF_Print_Time_Date(void)
{
  uint8_t am_pm_txt[3];
  LCD_Out_Buffer_T local_LCD_str;

  Read_TimeDate(clk_temp_buffer_timeDate, CLK_timeDate_Max);

  //sending time to Appitf SW component
  if(0u  == clk_temp_buffer_timeDate[CLK_time_format])
    {
      strcpy((char *)am_pm_txt, "am");
    }
  else
    {
      strcpy((char *)am_pm_txt, "pm");
    }
  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "%02d:%02d:%02d %s", clk_temp_buffer_timeDate[CLK_hours], clk_temp_buffer_timeDate[CLK_minutes], clk_temp_buffer_timeDate[CLK_seconds], (char *)am_pm_txt);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_4;

  sprintf((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "%02d/%02d/%4d", clk_temp_buffer_timeDate[CLK_month], clk_temp_buffer_timeDate[CLK_day], YEAR_2000 + clk_temp_buffer_timeDate[CLK_year]);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_4;

  //sending date to Appitf SW component
  APPIFEF_Send_LCD(&local_LCD_str);
}


/******************************************************************************
* Function : FSMLF_Alarm_Menu_Config
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
static void FSMLF_Alarm_Menu_Config(void)
{
  button_descriptor alarm_button;
  button_descriptor increment_button;
  button_descriptor decrement_button;

  APPIFEF_Get_Button_Req(Alarm, &alarm_button);
  APPIFEF_Get_Button_Req(Increment, &increment_button);
  APPIFEF_Get_Button_Req(Decrement, &decrement_button);

  switch (alarm_settings_state_e)
  {
    case State_Init_Alarm:

      Alarm_Set_Init(&increment_button, &decrement_button);

      break;

    case State_Set_Hr:

      Alarm_Set_Hr(&increment_button, &decrement_button);

      if(Alarm_Min_Requested == alarm_button.push_button_action_u16 && button_pushed == alarm_button.button_status)
	{
	  alarm_settings_state_e = State_Set_Mn;
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}

      break;

    case State_Set_Mn:

      Alarm_Set_Mn(&increment_button, &decrement_button);

      if(Alarm_Format_Requested == alarm_button.push_button_action_u16 && button_pushed == alarm_button.button_status)
	{
	  alarm_settings_state_e = State_Set_Time_Format;
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}

      break;

    case State_Set_Time_Format:

      Alarm_Timet_Formart(&increment_button, &decrement_button);

      if(Alarm_Monday_Requested == alarm_button.push_button_action_u16 && button_pushed == alarm_button.button_status)
	{
	  alarm_settings_state_e = State_Set_Days;
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}

      break;

    case State_Set_Days:

      Alarm_Week_Days(&increment_button, &decrement_button);

      if(Alarm_Exit_Requested == alarm_button.push_button_action_u16 && button_pushed == alarm_button.button_status)
	{
	  alarm_settings_state_e = State_Exit_Alarm;
	}

      break;

    default:

      Alarm_Set_Exit(&increment_button, &decrement_button);

      break;
  }
}

/******************************************************************************
* Function : Alarm_Set_Init
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
static void Alarm_Set_Init(button_descriptor * button_increment, button_descriptor * button_decrement)
{
  if(NULL != button_increment && NULL != button_decrement)
    {
      LCD_Out_Buffer_T local_LCD_str;

      if(EXPIRE_TIMER == timer_start_exit_alarm)
	{

	  timer_start_exit_alarm = START_TIMER;
	  alarm_settings_state_e = State_Set_Hr;
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}
      else
	{
	  timer_start_exit_alarm -= TIMER_TICK;
	}

      strncpy((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Setting Alarm   ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

      strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

      APPIFEF_Send_LCD(&local_LCD_str);
    }
}

/******************************************************************************
* Function : Alarm_Set_Hr
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
static void Alarm_Set_Hr(button_descriptor * button_increment, button_descriptor * button_decrement)
{
  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
      clk_temp_buffer_time_alarm[CLK_Alarm_hours] += CLK_UNIT_ONE;
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
      clk_temp_buffer_time_alarm[CLK_Alarm_hours] -= CLK_UNIT_ONE;
    }

  if (clk_temp_buffer_time_alarm[CLK_Alarm_hours] <= 0x00u)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_hours] = 0x00u;
    }
  else if (clk_temp_buffer_time_alarm[CLK_Alarm_hours] >= 12u)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_hours] = 12u;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter hour:%02d ", clk_temp_buffer_time_alarm[CLK_Alarm_hours]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

/******************************************************************************
* Function : Alarm_Set_Mn
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
static void Alarm_Set_Mn(button_descriptor * button_increment, button_descriptor * button_decrement)
{
  LCD_Out_Buffer_T local_LCD_str;

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
      clk_temp_buffer_time_alarm[CLK_Alarm_minutes] += CLK_UNIT_ONE;
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
      clk_temp_buffer_time_alarm[CLK_Alarm_minutes] -= CLK_UNIT_ONE;
    }

  if (clk_temp_buffer_time_alarm[CLK_Alarm_minutes] <= 0x00u)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_minutes] = 0x00u;
    }
  else if (clk_temp_buffer_time_alarm[CLK_Alarm_minutes] >= 12u)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_minutes] = 12u;
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Enter min:%02d ", clk_temp_buffer_time_alarm[CLK_Alarm_minutes]);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

/******************************************************************************
* Function : Alarm_Timet_Formart
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
static void Alarm_Timet_Formart(button_descriptor * button_increment, button_descriptor * button_decrement)
{
  LCD_Out_Buffer_T local_LCD_str;
  uint8_t am_pm_txt[3];

  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_format] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_format] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_time_alarm[CLK_Alarm_format] <= 0x00u)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_format] = 0x00u;
      strcpy((char *)am_pm_txt, "am");
    }
  else if (clk_temp_buffer_time_alarm[CLK_Alarm_format] >= 1)
    {
      clk_temp_buffer_time_alarm[CLK_Alarm_format] = 1;
      strcpy((char *)am_pm_txt, "pm");
    }

  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Time: %s     ", am_pm_txt);
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

/******************************************************************************
* Function : Alarm_Week_Days
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
static void Alarm_Week_Days(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  LCD_Out_Buffer_T local_LCD_str;
  button_descriptor alarm_button;
  CLK_time_Alarm_info_T iter_day = CLK_Alarm_Monday;

  APPIFEF_Get_Button_Req(Alarm, &alarm_button);

  for(iter_day = CLK_Alarm_Monday; iter_day < CLK_Alram_info_Max; iter_day++)
    {
      if(clk_translation_action_buffer_alarm[iter_day] == alarm_button.push_button_action_u16)
	{
	  if (Incrementing_Requested_Requested == button_increment->push_button_action_u16 && button_pushed == button_increment->button_status)
	    {
	      clk_temp_buffer_time_alarm[iter_day] = alarm_enable;
	      APPIFEF_Set_Button_Status(Increment, button_proccessed);
	      APPIFEF_Clear_push_button(Increment);
	    }
	  else if (Decrementing_Requested_Requested == button_decrement->push_button_action_u16 && button_pushed == button_decrement->button_status)
	    {
	      clk_temp_buffer_time_alarm[iter_day] = alarm_disable;
	      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
	      APPIFEF_Clear_push_button(Decrement);
	    }
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}
    }



  sprintf((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, " M T W T F S S");
  local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

  sprintf((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, " %c %c %c %c %c %c %c",
	  clk_temp_buffer_time_alarm[CLK_Alarm_Monday],
	  clk_temp_buffer_time_alarm[CLK_Alarm_Tuesday],
	  clk_temp_buffer_time_alarm[CLK_Alarm_Wensday],
	  clk_temp_buffer_time_alarm[CLK_Alarm_Thursday],
	  clk_temp_buffer_time_alarm[CLK_Alarm_Friday],
	  clk_temp_buffer_time_alarm[CLK_Alarm_Saturday],
	  clk_temp_buffer_time_alarm[CLK_Alarm_Sunday]
	  );
  local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

  APPIFEF_Send_LCD(&local_LCD_str);
}

/******************************************************************************
* Function : Alarm_Set_Exit
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
void Alarm_Set_Exit(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  if(NULL != button_increment && NULL != button_decrement)
    {
      LCD_Out_Buffer_T local_LCD_str;

      strncpy((char *)local_LCD_str.Up_Row_Buffer.appif_out_buffer_u8, "Saving Alarm    ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Up_Row_Buffer.colum_position = Column_1;

      strncpy((char *)local_LCD_str.Down_Row_Buffer.appif_out_buffer_u8, "                ", APPIF_MAX_LCD_DIGIT);
      local_LCD_str.Down_Row_Buffer.colum_position = Column_1;

      APPIFEF_Send_LCD(&local_LCD_str);

      if(EXPIRE_TIMER == timer_start_exit_alarm)
	{
	  SetAlarm (clk_temp_buffer_time_alarm, CLK_Alram_info_Max);
	  alarm_settings_state_e = State_Init_Alarm;
	  main_clock_state_e = print;

	  APPIFEF_Set_Button_Status (Alarm, button_proccessed);
	  APPIFEF_Clear_push_button (Alarm);

	  APPIFEF_Clear ();

	  timer_start_exit_alarm = START_TIMER;

	}
      else
	{
	  timer_start_exit_alarm -= TIMER_TICK;
	}

    }
}


/*************** END OF FUNCTIONS ***************************************************************************/
