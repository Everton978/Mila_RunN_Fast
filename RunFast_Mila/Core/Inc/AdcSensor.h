/*
 * AdcSensor.h
 *
 *  Created on: 26 de ago. de 2026
 *      Author: USER
 */

#ifndef INC_ADCSENSOR_H_
#define INC_ADCSENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "main.h"

void AdcSensor(void);


#ifdef __cplusplus
}
#endif

#endif /* INC_ADCSENSOR_H_ */
