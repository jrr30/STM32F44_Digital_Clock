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
  sec_setting,
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

static uint8_t clk_temp_buffer_time[CLK_MAX_TIME_BUFFER];
static uint8_t clk_temp_buffer_date[CLK_MAX_DATE_BUFFER];
/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void FSMLF_Menu_Config(void);
static void CLKLF_Set_Hour(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Minute(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Sec(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Year(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Day(button_descriptor * button_increment, button_descriptor * button_decrement);
static void CLKLF_Set_Month(button_descriptor * button_increment, button_descriptor * button_decrement);

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

  APPIFEF_Get_Button_Req(Set, &set_button);
  APPIFEF_Get_Button_Req(Alarm, &alarm_button);

  switch(main_clock_state_e)
  {
    case init:
      main_clock_state_e = print;

      break;
    case print:

      //reading time from RTC SW component
      ReadTime(clk_temp_buffer_time);
      //sending time to Appitf SW component
      APPIFEF_Send_Time_Display(clk_temp_buffer_time);

      //reading date from RTC SW component
      ReadDate(clk_temp_buffer_date);
      //sending date to Appitf SW component
      APPIFEF_Send_Date_Display(clk_temp_buffer_date);

      if(Setting_Enter_Requested == set_button.push_buttonuest_u16 && button_pushed == set_button.button_status)
	{
	  main_clock_state_e = set_time;
	  settings_menu_state_e = setting_init;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      else if(Alarm_max_Requested == alarm_button.push_buttonuest_u16 && button_pushed == alarm_button.button_status)
	{
	  main_clock_state_e = set_time;
	  APPIFEF_Set_Button_Status(Alarm, button_proccessed);
	}

      break;
    case set_time:

      FSMLF_Menu_Config();

      break;
    case set_alaram:

      if(Alarm_Exit_Requested == alarm_button.push_buttonuest_u16 && button_pushed == alarm_button.button_status)
	{
	  main_clock_state_e = print;
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

  APPIFEF_Get_Button_Req(Set, &set_button);
  APPIFEF_Get_Button_Req(Increment, &increment_button);
  APPIFEF_Get_Button_Req(Decrement, &decrement_button);

  switch(settings_menu_state_e)
  {
    case setting_init:

      delay_state += CLK_UNIT_ONE;
      APPIFEF_Send_String_Settings();

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
	  settings_menu_state_e = sec_setting;
	  APPIFEF_Set_Button_Status(Set, button_proccessed);
	}
      break;
    case sec_setting:

      CLKLF_Set_Sec(&increment_button, &decrement_button);

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
      APPIFEF_Send_String_Saving();

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

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_time[CLK_HRS] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_time[CLK_HRS] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_time[CLK_HRS] <= 0x00u)
    {
      clk_temp_buffer_time[CLK_HRS] = 0x00u;
    }
  else if (clk_temp_buffer_time[CLK_HRS] >= 12)
    {
      clk_temp_buffer_time[CLK_HRS] = 12;
    }

  APPIFEF_Send_Setting(clk_temp_buffer_time[CLK_HRS], 3);
}

static void CLKLF_Set_Minute(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_time[CLK_MIN] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_time[CLK_MIN] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_time[CLK_MIN] <= 0x00u)
    {
      clk_temp_buffer_time[CLK_MIN] = 0x00u;
    }
  else if (clk_temp_buffer_time[CLK_MIN] >= 60)
    {
      clk_temp_buffer_time[CLK_MIN] = 60;
    }

  APPIFEF_Send_Setting(clk_temp_buffer_time[CLK_MIN], 4);
}

static void CLKLF_Set_Sec(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_time[CLK_SEC] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_time[CLK_SEC] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_time[CLK_SEC] <= 0x00u)
    {
      clk_temp_buffer_time[CLK_SEC] = 0x00u;
    }
  else if (clk_temp_buffer_time[CLK_SEC] >= 60)
    {
      clk_temp_buffer_time[CLK_SEC] = 60;
    }

  APPIFEF_Send_Setting(clk_temp_buffer_time[CLK_SEC], 5);
}

static void CLKLF_Set_Year(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_date[CLK_YEAR] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_date[CLK_YEAR] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_date[CLK_YEAR] <= 0x00u)
    {
      clk_temp_buffer_date[CLK_YEAR] = 0x00u;
    }
  else if (clk_temp_buffer_date[CLK_YEAR] >= 99u)
    {
      clk_temp_buffer_date[CLK_YEAR] = 99;
    }

  APPIFEF_Send_Setting(clk_temp_buffer_date[CLK_YEAR], 8);
}

static void CLKLF_Set_Day(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      clk_temp_buffer_date[CLK_DAY] += CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_date[CLK_DAY] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }
  else
    {

    }

  if (clk_temp_buffer_date[CLK_DAY] <= 0x00u)
    {
      clk_temp_buffer_date[CLK_DAY] = 0x00u;
    }
  else if (clk_temp_buffer_date[CLK_DAY] >= 30)
    {
      clk_temp_buffer_date[CLK_DAY] = 30;
    }

  APPIFEF_Send_Setting(clk_temp_buffer_date[CLK_DAY], 6);
}

static void CLKLF_Set_Month(button_descriptor * button_increment, button_descriptor * button_decrement)
{

  if (Incrementing_Requested_Requested == button_increment->push_buttonuest_u16 && button_pushed == button_increment->button_status)
    {
      APPIFEF_Set_Button_Status(Increment, button_proccessed);
      APPIFEF_Clear_push_button(Increment);
      clk_temp_buffer_date[CLK_MOUNTH] += CLK_UNIT_ONE;
    }
  else if (Decrementing_Requested_Requested == button_decrement->push_buttonuest_u16 && button_pushed == button_decrement->button_status)
    {
      clk_temp_buffer_date[CLK_MOUNTH] -= CLK_UNIT_ONE;
      APPIFEF_Set_Button_Status(Decrement, button_proccessed);
      APPIFEF_Clear_push_button(Decrement);
    }

  if (clk_temp_buffer_date[CLK_MOUNTH] <= 0x00u)
    {
      clk_temp_buffer_date[CLK_MOUNTH] = 0x00u;
    }
  else if (clk_temp_buffer_date[CLK_MOUNTH] >= 12u)
    {
      clk_temp_buffer_date[CLK_MOUNTH] = 12u;
    }

  APPIFEF_Send_Setting(clk_temp_buffer_date[CLK_MOUNTH], 7);
}


/*************** END OF FUNCTIONS ***************************************************************************/
