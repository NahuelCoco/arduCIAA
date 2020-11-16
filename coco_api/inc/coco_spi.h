#ifndef __COCO_SPI_H_
#define __COCO_SPI_H_

#include "chip.h"

#define SPI_MODE0 SSP_CLOCK_CPHA0_CPOL0
#define SPI_MODE1 SSP_CLOCK_CPHA1_CPOL0
#define SPI_MODE2 SSP_CLOCK_CPHA0_CPOL1
#define SPI_MODE3 SSP_CLOCK_CPHA1_CPOL1

#define SPI_CLOCK_DIV2 8000000
#define SPI_CLOCK_DIV4 4000000
#define SPI_CLOCK_DIV8 2000000
#define SPI_CLOCK_DIV16 1000000
#define SPI_CLOCK_DIV32 500000
#define SPI_CLOCK_DIV64 250000
#define SPI_CLOCK_DIV128 125000

#ifdef __cplusplus
extern "C" {
#endif

class SPI
{
	public:
		void begin( void );

		void setDataMode( uint32_t mode );

		void setClockDivider( uint32_t divider );

		uint8_t transfer( uint8_t val );
};

extern SPI spi;

#ifdef __cplusplus
}
#endif

#endif /* __COCO_SPI_H_ */

