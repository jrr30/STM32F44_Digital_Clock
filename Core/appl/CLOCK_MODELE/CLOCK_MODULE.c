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

#define CLK_YEAR   (0x03u)
#define CLK_MOUNTH (0x04u)
#define CLK_DAY    (0x05u)

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
uint16_t buton_request_u16 = 0x00;

static uint8_t clk_temp_buffer_time[CLK_MAX_TIME_BUFFER];
static uint8_t clk_temp_buffer_date[CLK_MAX_DATE_BUFFER];
/******************************************************************************
* Function Prototypes
*******************************************************************************/
static void FSMLF_Menu_Config(uint16_t buton_rqst);
static void CLKLF_Set_Hour(void);
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

      if(Setting_Enter_Requested == APPIFEF_Get_Button_Req(Set))
	{
	  main_clock_state_e = set_time;
	  settings_menu_state_e = setting_init;
	}
      else if(Alarm_max_Requested == APPIFEF_Get_Button_Req(Alarm))
	{
	  main_clock_state_e = set_time;
	}

      break;
    case set_time:

      buton_request_u16 = APPIFEF_Get_Button_Req(Set);

      FSMLF_Menu_Config(buton_request_u16);

      if(Setting_Exit_Requested == buton_request_u16)
	{
	  APPIFEF_Clear();
	  main_clock_state_e = print;
	}

      break;
    case set_alaram:

      if(Alarm_Exit_Requested == APPIFEF_Get_Button_Req(Alarm))
	{
	  main_clock_state_e = print;
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
static void FSMLF_Menu_Config(uint16_t buton_rqst)
{
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

      CLKLF_Set_Hour();
      APPIFEF_Send_Setting(clk_temp_buffer_time[CLK_HRS], 3);

      if(Setting_Min_Requested == buton_rqst)
	{
	  settings_menu_state_e = min_setting;
	  APPIFEF_Clear();
	}
      break;
    case min_setting:

      APPIFEF_Send_Setting(clk_temp_buffer_time[CLK_MIN], 4);

      if(Setting_Sec_Requested == buton_rqst)
	{
	  settings_menu_state_e = sec_setting;
	  APPIFEF_Clear();
	}
      break;
    case sec_setting:


      APPIFEF_Send_Setting(clk_temp_buffer_time[CLK_SEC], 5);

      if(Setting_Year_Requested == buton_rqst)
	{
	  settings_menu_state_e = year_setting;
	  APPIFEF_Clear();
	}
      break;

    case year_setting:

      APPIFEF_Send_Setting(clk_temp_buffer_date[CLK_YEAR], 8);

      if(Setting_Month_Requested == buton_rqst)
	{
	  settings_menu_state_e = month_setting;
	  APPIFEF_Clear();
	}
      break;
    case month_setting:

      APPIFEF_Send_Setting(clk_temp_buffer_date[CLK_MOUNTH], 7);
      if(Setting_Day_Requested == buton_rqst)
	{
	  settings_menu_state_e = day_setting;
	  APPIFEF_Clear();
	}
      break;
    case day_setting:

      APPIFEF_Send_Setting(clk_temp_buffer_date[CLK_DAY], 6);

      if(Setting_Exit_Requested == buton_rqst)
	{
	  settings_menu_state_e = exit_setting;
	  APPIFEF_Clear();
	}
      break;
    default:

      break;
  }
}

static void CLKLF_Set_Hour(void)
{
  if (clk_temp_buffer_time[CLK_HRS] <= 0x00u)
    {
      clk_temp_buffer_time[CLK_HRS] = 0x00u;
    }
  else if (clk_temp_buffer_time[CLK_HRS] >= 0x0Cu)
    {
      clk_temp_buffer_time[CLK_HRS] = 0x0Cu;
    }


  if (Incrementing_Requested_Requested == APPIFEF_Get_Button_Req (Increment))
    {
      clk_temp_buffer_time[CLK_HRS] += CLK_UNIT_ONE;
      APPIFEF_Clear_Button_Req(Increment);
    }
  else if (Incrementing_Requested_Requested== APPIFEF_Get_Button_Req (Decrement))
    {
      clk_temp_buffer_time[CLK_HRS] -= CLK_UNIT_ONE;
      APPIFEF_Clear_Button_Req(Decrement);
    }
}

/*************** END OF FUNCTIONS ***************************************************************************/
