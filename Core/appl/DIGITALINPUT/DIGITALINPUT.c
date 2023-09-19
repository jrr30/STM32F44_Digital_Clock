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

#define FALSE     0x00u
#define TRUE      0x01u

#define OFFSATE   0x01u

/*User typedef------------------------------------------*/

typedef struct Button_Descriptor
{
	Input_Status Digital_Input_State;
	Input_Source Digital_Input_Source;

}Button_Container;

typedef enum input_source_private
{
	Increment_private = 1,
	Decrement_private,
	Set_private,
	Alarm_private,

	Source_max_private,
}Input_Source_Private;

Button_Container Inputs_Private[Source_max];
uint8_t Input_Bouncer[Source_max] = {0x00u};

/*Private variables definition--------------------------*/
static uint8_t last_known_u8   = (uint8_t)(Increment_private | Decrement_private | Set_private | Alarm_private);
static uint8_t last_index_u8   =  0x00u;

/*Private functions prototype--------------------------*/


/*Private functions definition-------------------------*/


/*Public functions definition--------------------------*/

void Io_read(void)
{
//	uint8_t source_index_u8 = 0x00u;

	if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,SET_BUTTON_Pin) && (Set_private == (last_known_u8 & Set_private)))
	{
		Input_Bouncer[Set] +=0x01u;
		last_known_u8 = Set_private;
	}
	else if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,SET_ALARM_BUTTON_Pin) && (Alarm_private == (last_known_u8 & Alarm_private)))
	{
		Input_Bouncer[Alarm] +=0x01u;
		last_known_u8 = Alarm_private;
	}
	else if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,INCREMENT_BUTTON_Pin) && (Increment_private == (last_known_u8 & Increment_private)))
	{
		Input_Bouncer[Increment] +=0x01u;
		last_known_u8 = Increment_private;
	}
	else if(GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOC,DECREMENT_BUTTON_Pin) && (Decrement_private == (last_known_u8 & Decrement_private)))
	{
		Input_Bouncer[Decrement] +=0x01u;
		last_known_u8 = Decrement_private;
	}
	else
	{
		Inputs_Private[last_index_u8].Digital_Input_State = Input_low;
		Input_Bouncer[last_index_u8] = RESET;
		last_known_u8 = (uint8_t)(Increment_private | Decrement_private | Set_private | Alarm_private);
	}

	if(DEBOUNCED == Input_Bouncer[last_known_u8 - OFFSATE])
	{
		Inputs_Private[last_known_u8 - OFFSATE].Digital_Input_State = Input_high;
		last_index_u8 = last_known_u8 - OFFSATE;
	}

}

Input_Status Io_Read(Input_Source InputSource)
{
	return Inputs_Private[InputSource].Digital_Input_State;
}

void Init_digital_input(void)
{
	uint8_t index = 0x00;

	for(index = Increment; index < Source_max; index++)
	{
		Input_Bouncer[Increment] = 0x00;
		Inputs_Private[index].Digital_Input_Source = index;
		Inputs_Private[index].Digital_Input_State = Input_low;
	}
}

#endif /* APPL_DIGITALINPUT_DIGITALINPUT_C_ */
