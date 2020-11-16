#ifndef __COCO_TIMER_H_
#define __COCO_TIMER_H_


#include "chip.h"

#define TICKRATE_HZ 1000

#ifdef __cplusplus
extern "C" {
#endif

//Esta libreria usa el timer 0

void initTimer ( void );
bool delay ( long timeMs );
bool delayMicroseconds ( long timeUS );
double millisMicroseconds ( void );
uint32_t millis ( void );

#ifdef __cplusplus
}
#endif

#endif /* __COCO_TIMER_H_ */

