/*
 * DIGITALINPUT.h
 *
 *  Created on: Sep 16, 2023
 *      Author: Jesus
 */

#ifndef APPL_DIGITALINPUT_DIGITALINPUT_H_
#define APPL_DIGITALINPUT_DIGITALINPUT_H_

#include "stm32f4xx_hal.h"

/*Macros declaration-----------------------------------*/

/*User typede------------------------------------------*/

typedef enum input_status
{
	Input_low,
	Input_high,

	Input_max,
}Input_Status;

typedef enum input_source
{
	Increment,
	Decrement,
	Set,
	Alarm,

	Source_max,
}Input_Source;



/*Public functions-----------------------------------*/

void Io_read(void);
Input_Status Io_Read(Input_Source InputSource);

#endif /* APPL_DIGITALINPUT_DIGITALINPUT_H_ */
