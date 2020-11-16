#ifndef __COCO_TIMER_H_
#define __COCO_TIMER_H_


#include "chip.h"


#ifdef __cplusplus
extern "C" {
#endif

//Esta libreria usa el timer 0

bool delay ( long timeMs );
bool delayMicroseconds ( unsigned int timeUS );
double millisMicroseconds ( void );
uint32_t millis ( void );

#ifdef __cplusplus
}
#endif

#endif /* __COCO_TIMER_H_ */

