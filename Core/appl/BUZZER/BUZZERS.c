/*******************************************************************************
* Title                 :   System Initialization
* Filename              :   BUZZERS
* Author                :   Jesus
* Origin Date           :   Feb 17, 2024
* Version               :   1.0.0
* Compiler              :   Microchip C30 v3.30c
* Target                :   PIC24FJ64GB004
* Notes                 :   None
*******************************************************************************/
/*************** MODULE REVISION LOG ******************************************
*
*    Date    Software Version    Initials   Description 
*  Feb 17, 2024    XXXXXXXXXXX         JWB      Module Created.
*
*******************************************************************************/
/******************************************************************************
* Includes
*******************************************************************************/
#include "../appl/BUZZER/BUZZER.h"
#include "SchM_DigitalClock.h"

#include <main.h>
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

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
/******************************************************************************
* Function : Pwm_Init()
*
*
* @bfiref:  This function is used to 
*
* @param:   None.
*
* @return:  None.
*
*
*******************************************************************************/
void buzzer_beep_on(void)
{
  delay_buzzer_start();
}

/*************** END OF FUNCTIONS ***************************************************************************/