/*
 * Digital_Clock.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Jesus
 */

#ifndef INC_DIGITAL_CLOCK_H_
#define INC_DIGITAL_CLOCK_H_

#include <stdint.h>

void Print_Time(void);
void Print_Date(void);

void setting_time(uint8_t * gobla_counter_Prt);
void setting_alarm(uint8_t * gobla_counter_Prt);

//void setting_day(uint8_t * day);
//void setting_month(uint8_t * month);
//void setting_year(uint8_t * year);

#endif /* INC_DIGITAL_CLOCK_H_ */
