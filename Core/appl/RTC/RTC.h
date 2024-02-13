/*
 * RTC.h
 *
 *  Created on: Aug 28, 2023
 *      Author: Jesus
 */

#ifndef APPL_RTC_RTC_H_
#define APPL_RTC_RTC_H_

#include "stm32f4xx_hal.h"

/*Macros declaration-----------------------------------*/
#define HRS 0x00u
#define MIN 0x01u
#define SEC 0x02u

#define YEAR   0x00u
#define MOUNTH 0x01u
#define DAY    0x02u

/*Public functions-----------------------------------*/
void RTC_updateTimeDate(void);
void ReadTime(uint8_t * ptrtimebuffer);
void ReadDate(uint8_t * Ptrdatebuffer);
void WriteTime(uint8_t * timebuffer_pu8);
void WriteDate(uint8_t * datebuffer_pu8);


#endif /* APPL_RTC_RTC_H_ */
