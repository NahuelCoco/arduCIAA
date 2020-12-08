#ifndef __COCO_PWM_H_
#define __COCO_PWM_H_

#include "chip.h"

#define frec_init 50 //Hz
#define SCT_PWM            LPC_SCT

#ifdef __cplusplus
extern "C" {
#endif

long freq = 50;

void pwm_init ( void );

void pwm_changeFrec ( long frec );

bool analogWrite ( uint8_t pin, uint8_t value );

bool analogWriteUS ( uint8_t pin, int value );

#ifdef __cplusplus
}
#endif

#endif /* __COCO_PWM_H_ */

