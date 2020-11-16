#ifndef __COCO_TIMER_H_
#define __COCO_TIMER_H_


#include "chip.h"

#define TICKRATE_HZ_MS 1000
#define TICKRATE_HZ_US 1000000

#ifdef __cplusplus
extern "C" {
#endif

//Esta libreria usa el timer 0 y el 1

void initTimer ( void );
bool delay ( long timeMs );
bool delayMicroseconds ( long timeUS );
double long millis ( void );

#ifdef __cplusplus
}
#endif

#endif /* __COCO_TIMER_H_ */

