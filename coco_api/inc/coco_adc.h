#ifndef __COCO_ADC_H_
#define __COCO_ADC_H_


#include "chip.h"

#define A0 0 //ADC0/1_0
#define A1 1 //ADC0/1_1
#define A2 2 //ADC0/1_2
#define A3 3 //ADC0/1_3
#define A4 4 //ADC0/1_4
#define A5 5 //ADC0/1_5
#define A6 6 //ADC0/1_6
#define A7 7 //ADC0/1_7
#define ADC_SAMPLE_RATE ADC_MAX_SAMPLE_RATE //Si se desea, colocar valor en Hz en lugar de ADC_MAX_SAMPLE_RATE
#define ADC_RESOLUTION ADC_10BITS //Esta es la máxima resolución
#define ADC_STATE DISABLE


#ifdef __cplusplus
extern "C" {
#endif

void adc_init (bool channel);
uint16_t analogRead (uint8_t numberADC);

#ifdef __cplusplus
}
#endif

#endif /* __COCO_ADC_H_ */

