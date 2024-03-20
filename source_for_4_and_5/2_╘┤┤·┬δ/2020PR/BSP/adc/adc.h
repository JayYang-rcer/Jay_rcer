#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx.h"

#define ADC_PROCESSING_VALUE_SUM 100

extern float RANGING_DISTANCE;

void adc_adc3_init(void);
void adc_update_ranging_distance(void);

#endif
