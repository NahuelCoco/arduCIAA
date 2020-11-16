#include "coco.h"


int main(void) {

	_coco();
	pinMode(LED1,OUTPUT);
	pinMode(TEC1, INPUT);


    while(1)
    {
    	if(!digitalRead(TEC1))
    		digitalWrite(LED1, HIGH);

    	if(digitalRead(TEC1))
    		digitalWrite(LED1, LOW);
    }
    return 0 ;
}
