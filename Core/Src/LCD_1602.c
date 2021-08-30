/*
 * LCD_1602.c
 *
 *  Created on: Oct 11, 2020
 *      Author: Jesus
 */

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "LCD_1602.h"


static void Enable_Pulse(void)
{
	HAL_GPIO_WritePin(PIN_CONTROL_PORT, ENABLE, GPIO_PIN_SET);
	delay_us(1);
	HAL_GPIO_WritePin(PIN_CONTROL_PORT, ENABLE, GPIO_PIN_RESET);
	delay_us(1);
}

/**
  * @brief  Write Data at low level as GPIO.
  *
  * @note   This function uses GPIO_HAL library to write 1 or 0 to the specific GPIO pin.
  *
  * @param  Command specifies the raw data in 1 byte.
  * @param  Rs specifies if the value to be wroten is either a command or data.
  *          This parameter can be one of the rs enum values:
  *            @arg COMMAND: to write a command in the LCD
  *            @arg DATA: to write data to the LCD.
  * @retval None
  */
static void Write_Data_Command_GPIO(uint8_t command, uint8_t rs)
{
	 HAL_GPIO_WritePin(PIN_CONTROL_PORT, RS|RW|ENABLE, GPIO_PIN_RESET);

#if 0
	uint8_t iter = 0x00u;
	uint8_t aux_variable = 0x00u;
	uint16_t bits_value = 0x00u;
	uint16_t GPIOC_PINs [MODE_DATA_4] = {GPIO_PIN_5,GPIO_PIN_6,GPIO_PIN_8,GPIO_PIN_9};
	uint8_t Offset_shiftting [MODE_DATA_4] = {0x05u,0x06u,0x08u,0x09u};
#endif

	if(rs == COMMAND)
	{
		HAL_GPIO_WritePin(PIN_CONTROL_PORT, RS, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(PIN_CONTROL_PORT, RS, GPIO_PIN_SET);
	}

#if 0
	for(iter = 0; iter <= MODE_DATA_4; iter++)
	{
		aux_variable = (0x01u & (command >> iter));
		bits_value  |= (GPIOC_PINs[iter] & (aux_variable<<Offset_shiftting[iter]));
	}

	GPIOC->BSRR = bits_value;
#endif

	HAL_GPIO_WritePin(DATA_PORT,DATA_D4,((command>>0)&TRUE_VALUE));
	HAL_GPIO_WritePin(DATA_PORT,DATA_D5,((command>>1)&TRUE_VALUE));
	HAL_GPIO_WritePin(DATA_PORT,DATA_D6,((command>>2)&TRUE_VALUE));
	HAL_GPIO_WritePin(DATA_PORT,DATA_D7,((command>>3)&TRUE_VALUE));

	Enable_Pulse();
	delay_us(1);
	HAL_GPIO_WritePin(DATA_PORT, DATA_D4|DATA_D5|DATA_D6|DATA_D7, GPIO_PIN_RESET);
}

/**
  * @brief  Write command
  *
  * @note   This function is specific to write a single command to control the LCD.
  *
  * @param  Command specifies the command to be executed.
  *           @arg command: this instruction can be taken from the macros "Set of commands to operate the LCD"
  * @retval None
  */
void Write_Command(uint8_t com)
{
	uint8_t command_lcd = COMMAND;
	Write_Data_Command_GPIO(((MASK_HIGH&com)>>4), command_lcd);
	Write_Data_Command_GPIO( (MASK_LOW&com), command_lcd);
}

/**
  * @brief  Write character
  *
  * @note   This function is specific to write a single character in the display.
  *
  * @param  Command specifies the raw as single character
  * @retval None
  */
void Write_Data(uint8_t data)
{
	uint8_t data_lcd = DATA;
	Write_Data_Command_GPIO(((MASK_HIGH&data)>>4), data_lcd);
	Write_Data_Command_GPIO( MASK_LOW&data, data_lcd);
}

/**
  * @brief  Specific function to perform the initial configuration.
  *
  * @note   This function do the configuration as 4 bit mode.
  *           if 8 bit mode is needed, this must be done.
  * @retval None
  */
void LCD_Config(void)
{
	delay_us(20000);
	Write_Data_Command_GPIO(0x03u, COMMAND);
	delay_us(7000);
	Write_Data_Command_GPIO(0x03u, COMMAND);
	delay_us(100);
	Write_Data_Command_GPIO(0x03u, COMMAND);
	delay_us(7000);

	Write_Data_Command_GPIO(0x02u,COMMAND);
	delay_us(40);

	Write_Command(M4BIT|M2LINE|M5_8_Font);

	Write_Command(ONDisp|OFFCursor|OFFBlink);

	Write_Command(CLEAR_SCREEN);
	delay_us(2000);

	Write_Command(IncAdd);
	Write_Command(CURSOR_RETURN);
	delay_us(2000);

}

/**
  * @brief  Displays a string in the LCD.
  *
  * @note   This function shows in the LCD a string.
  * @param  row string can be in ASCII format but always in a array.
  * @retval None
  */
void print_string(uint8_t * lcd_str)
{
	  uint8_t iter = 0;

	  while(lcd_str[iter] != '\0')
	  {
		  Write_Data((uint8_t)lcd_str[iter]);
		  iter++;
	  }
}

/**
  * @brief  Set cursor at the location given.
  *
  * @note   This function Set the cursor in the display at any column and row.
  *           Column and row must be provided.
  * @param  row sets wich row information will be display.
  *          This parameter can be one of the Column_lcd enum values:
  *            @arg Row_1: Display cursor in the first row.
  *            @arg Row_2: Display cursor in the second row.
  * @param  column placed cursor in the specific column.
  *          This parameter can be one of the Column_lcd enum values:
  *            @arg Column_lcd: Columns staring in 1 up to 16.
  * @retval None
  */
void Set_Cursor(Row_lcd row, uint8_t column)
{
	if (Row_1 == row)
	{
		Write_Command((ROW_ONE_OFFSET<<4)|column);
	}
	else
	{
		Write_Command((ROW_TWO_OFFSET<<4)|column);
	}
}

/**
  * @brief  Clear Screen
  *
  * @note   This function will clear information displayed in the screen, but this will not set cursor at home position.
  * @retval None
  */
void Clear(void)
{
	Write_Command(CLEAR_SCREEN);
	delay_us(2000);

}

/**
  * @brief  Set cursor in Home position.
  *
  * @note   This function will set the cursor to the home address in the LCD Screen which is Column 1 and Row 1
  * @retval None
  */
void Home(void)
{
	Write_Command(CURSOR_RETURN);
	delay_us(2000);
}
/*
 * Data Log
 *
 * User.....................Date.............Brief description..........................................
 * Jesus Rodriguez          10/17/20         Edit Write_Data_Command_GPIO function. Funtions Clear and Home were added up as well.
 *                                           Creating documentation for each funtion.
 */
