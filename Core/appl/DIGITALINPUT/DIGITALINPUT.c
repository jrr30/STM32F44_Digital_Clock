/*
 * DIGITALINPUT.c
 *
 *  Created on: Sep 16, 2023
 *      Author: Jesus
 */

#ifndef APPL_DIGITALINPUT_DIGITALINPUT_C_
#define APPL_DIGITALINPUT_DIGITALINPUT_C_

#include "../appl/DIGITALINPUT/DIGITALINPUT.h"
#include <main.h>

/*Macros declaration------------------------------------*/
#define DEBOUNCED 0x03u
#define RESET     0x00u

/*User typedef------------------------------------------*/

typedef struct Button_Descriptor
{
	Input_Status Digital_Input_State;
	Input_Source Digital_Input_Source;

}Button_Container;

Button_Container Inputs_Private[Source_max];
uint8_t Input_Bouncer[Source_max] = {0x00u};

/*Private variables definition--------------------------*/
static uint8_t last_index_u8   = Source_max;

/*Private functions prototype--------------------------*/


/*Private functions definition-------------------------*/


/*Public functions definition--------------------------*/

void Io_read(void)
{
	uint8_t source_index_u8        = 0x00u;

	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,SET_BUTTON_Pin))
	{
		Input_Bouncer[Set] +=1;
		last_index_u8 = Set;
	}
	else if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,SET_ALARM_BUTTON_Pin))
	{
		Input_Bouncer[Alarm] +=1;
		last_index_u8 = Alarm;
	}
	else if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,INCREMENT_BUTTON_Pin))
	{
		Input_Bouncer[Increment] +=1;
		last_index_u8 = Increment;
	}
	else if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,DECREMENT_BUTTON_Pin))
	{
		Input_Bouncer[Decrement] +=1;
		last_index_u8 = Decrement;
	}
	else
	{
		Inputs_Private[last_index_u8].Digital_Input_State = Input_low;
		Input_Bouncer[last_index_u8] = RESET;
		last_index_u8 = Source_max;
	}

	for(source_index_u8 = Increment; source_index_u8 <= Source_max; source_index_u8++)
	{
		if(DEBOUNCED == Input_Bouncer[source_index_u8])
		{
			Inputs_Private[source_index_u8].Digital_Input_State = Input_high;
		}
	}
}

Input_Status Io_Read(Input_Source InputSource)
{
	return Inputs_Private[InputSource].Digital_Input_State;
}

#endif /* APPL_DIGITALINPUT_DIGITALINPUT_C_ */
