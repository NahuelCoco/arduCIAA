#include "coco.h"


int main(void) {

	_coco();
	pinMode(39,OUTPUT);

    while(1)
    {
    	digitalToggle(39);
    	delayMicroseconds(5000000);
    }
    return 0 ;
}
