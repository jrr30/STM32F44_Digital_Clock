/****************************************************************************
* Title                 :   ADC Application    
* Filename              :   CLOCK_MODULE
* Author                :   JWB
* Origin Date           :   Oct 21, 2023
* Version               :   1.0.0
* Compiler              :   Microchip C30 v3.30c
* Target                :   PIC24FJ64GB004
* Notes                 :   None
*****************************************************************************/
/*************** INTERFACE CHANGE LIST **************************************
*
*    Date        Software Version    Initials   Description
*  Oct 21, 2023    XXXXXXXXXXX         JWB      Interface Created.
*
*****************************************************************************/
#ifndef APPL_CLOCK_MODELE_CLOCK_MODULE_H_
#define APPL_CLOCK_MODELE_CLOCK_MODULE_H_

/******************************************************************************
* Includes
*******************************************************************************/


/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/**
 * This constant is
 */


/******************************************************************************
* Configuration Constants
*******************************************************************************/


/******************************************************************************
* Macros
*******************************************************************************/


	
/******************************************************************************
* Typedefs
*******************************************************************************/
typedef enum CLK_time_info_t
{
	CLK_hours,
	CLK_minutes,
	CLK_seconds,
	CLK_time_format,
	CLK_year,
	CLK_month,
	CLK_day,

	CLK_timeDate_Max
}CLK_time_info_T;

typedef enum CLK_time_Alarm_info_t
{
	CLK_Alarm_hours,
	CLK_Alarm_minutes,
	CLK_Alarm_format,
	CLK_Alarm_Monday,
	CLK_Alarm_Tuesday,
	CLK_Alarm_Wensday,
	CLK_Alarm_Thursday,
	CLK_Alarm_Friday,
	CLK_Alarm_Saturday,
	CLK_Alarm_Sunday,

	CLK_Alram_info_Max
}CLK_time_Alarm_info_T;

/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/

void FSMEF_Clock_Thread(void);
void FSMEF_Clock_Init(void);

#endif
/*** End of File **************************************************************/
