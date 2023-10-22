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
#define CLK_MAX_TIME_BUFFER 0x03u
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
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
E_main_clock_states main_clock_state_e = init;

static uint8_t clk_time_buffer[CLK_MAX_TIME_BUFFER];
/******************************************************************************
* Function Prototypes
*******************************************************************************/

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

      ReadTime(clk_time_buffer);
      APPIFEF_Send_Time_Display(clk_time_buffer);

      if(Setting_Enter_Requested == APPIFEF_Get_Button_Req(Set))
	{
	  main_clock_state_e = set_time;
	}
      else if(Alarm_max_Requested == APPIFEF_Get_Button_Req(Alarm))
	{
	  main_clock_state_e = set_time;
	}

      break;
    case set_time:

      if(Setting_Exit_Requested == APPIFEF_Get_Button_Req(Set))
	{
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

/*************** END OF FUNCTIONS ***************************************************************************/
