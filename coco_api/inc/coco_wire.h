#ifndef __COCO_WIRE_H_
#define __COCO_WIRE_H_

#include "chip.h"


#define maxRead 20
#define CLOCK_100KHz 100000
#define CLOCK_400KHz 400000

#ifdef __cplusplus
extern "C" {
#endif

class Wire
{
	public:
		void manager( uint8_t devAddr,uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize );

		void begin ( void );

		void beginTransmission( uint8_t address );

		char endTransmission( void );

		char write( uint8_t value );

		char write( uint8_t data[], int length );

		char requestFrom( uint8_t address, uint8_t quantity );

		uint8_t available( void );

		uint8_t read( void );

		void setClock ( long clockFrequency );

		volatile uint8_t globalQuantity = 0;

		volatile uint8_t globalAuxiliar = 0;

		volatile uint8_t globalReceived[maxRead];

		volatile uint8_t iam;

};

extern Wire wire;

#ifdef __cplusplus
}
#endif

#endif /* __COCO_WIRE_H_ */

