/*
 * LCD16.h
 *
 *  Created on: Sep 8, 2023
 *      Author: Jesus
 */

#ifndef APPL_LCD_LCD16_H_
#define APPL_LCD_LCD16_H_

#include <stdint.h>

extern void delay_us(uint32_t microseconds);

#define ROW_UP   1u
#define ROW_DOWN 2u


#define DATA_D7_Pin GPIO_PIN_15
#define DATA_D7_GPIO_Port GPIOA

#define DATA_D6_Pin GPIO_PIN_10
#define DATA_D6_GPIO_Port GPIOC

#define DATA_D5_Pin GPIO_PIN_11
#define DATA_D5_GPIO_Port GPIOC

#define DATA_D4_Pin GPIO_PIN_12
#define DATA_D4_GPIO_Port GPIOC

#define ENABLE_Pin GPIO_PIN_7
#define ENABLE_GPIO_Port GPIOB

#define RW_Pin GPIO_PIN_8
#define RW_GPIO_Port GPIOB

#define RS_Pin GPIO_PIN_9
#define RS_GPIO_Port GPIOB

/*
 * Macros for PINs in order to control RS, Enable, RW interfaced with specific port
 */
#define PIN_CONTROL_PORT GPIOB

#define RS RS_Pin
#define ENABLE ENABLE_Pin
#define RW RW_Pin

/*
 * Macros for PINs in order to control RS, Enable, RW interfaced with specific port
 */
#define DATA_D4 DATA_D4_Pin
#define DATA_D5 DATA_D5_Pin
#define DATA_D6 DATA_D6_Pin
#define DATA_D7 DATA_D7_Pin

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

/*Public functions-----------------------------------*/
void Write_Data(uint8_t data);
void Set_Cursor(uint8_t row, uint8_t column);
void LCD_Config(void);
void Clear(void);
void Home(void);
void print_string(uint8_t * lcd_str);
void LCDEF_Print_Str(void);


#endif /* APPL_LCD_LCD16_H_ */
