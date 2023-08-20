/*
 * LCD_1602.h
 *
 *  Created on: Oct 11, 2020
 *      Author: Jesus
 */


#ifndef THIRD_PARTY_LCD_1602__INC_LCD_1602_H_
#define THIRD_PARTY_LCD_1602__INC_LCD_1602_H_

extern void delay_us(uint32_t microseconds);

/*
 * Macros for PINs in order to control RS, Enable, RW interfaced with specific port
 */
#define PIN_CONTROL_PORT GPIOB
#define RS GPIO_PIN_6
#define ENABLE GPIO_PIN_8
#define RW GPIO_PIN_9

/*
 * Macros for PINs in order to control RS, Enable, RW interfaced with specific port
 */
#define DATA_PORT GPIOC
#define DATA_D4 GPIO_PIN_5
#define DATA_D5 GPIO_PIN_6
#define DATA_D6 GPIO_PIN_8
#define DATA_D7 GPIO_PIN_9

/*
 * General Macros for Driver
 */
#define MASK_HIGH 0xF0u
#define MASK_LOW  0x0Fu
#define MODE_DATA_4 0x04u
#define COMMAND 0x00u
#define DATA 0x01u
#define TRUE_VALUE 0x01
#define FALSE_VALUE 0x00
#define ROW_ONE_OFFSET 0x08
#define ROW_TWO_OFFSET 0x0C

/*
 * Set of commands to operate the LCD.
 */
#define CLEAR_SCREEN 0x01u
#define CURSOR_RETURN 0x02u
#define IncAdd 0x06u
#define DecAdd 0x04u
#define NSDisp 0x04u
#define SDisp 0x05u
#define ONDisp 0x0Cu
#define OFFDisp 0x08u
#define ONCursor 0x0Au
#define OFFCursor 0x08u
#define ONBlink 0x09u
#define OFFBlink 0x08u
#define SRDisp 0x1Cu
#define SLDisp 0x18u
#define CRDisp 0x14u
#define CLDisp 0x10u
#define M8BIT 0x30u
#define M4BIT 0x20u
#define M1LINE 0x20u
#define M2LINE 0x28u
#define M5_8_Font 0x20u
#define M5_11_Font 0x24u

typedef enum
{
	Column_1,
	Column_2,
	Column_3,
	Column_4,
	Column_5,
	Column_6,
	Column_7,
	Column_8,
	Column_9,
	Column_10,
	Column_11,
	Column_12,
	Column_13,
	Column_14,
	Column_15,
	Column_16
} Column_lcd;

typedef enum
{
	Row_1,
	Row_2
} Row_lcd;

/*
 *  APIs to interface with LCD Liquid Crystal (1602a)
 */
void Write_Command(uint8_t com);
void Write_Data(uint8_t data);
void Set_Cursor(uint8_t row, uint8_t column);
void LCD_Config(void);
void Clear(void);
void Home(void);
void print_string(uint8_t * lcd_str);


#endif /* THIRD_PARTY_LCD_1602__INC_LCD_1602_H_ */

/*
 * Data Log
 *
 * User.....................Date.............Brief description..........................................
 * Jesus Rodriguez          10/17/2020       Including functions Clear and Home.
 */
