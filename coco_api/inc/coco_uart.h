#ifndef __COCO_UART_H_
#define __COCO_UART_H_

#include "chip.h"
#include "coco_timer.h"
#define uart_config ( UART_LCR_WLEN8 | UART_LCR_PARITY_DIS | UART_LCR_SBS_1BIT )

typedef char *String;

#ifdef __cplusplus
extern "C" {
#endif

class uart0
{
	public:
		void begin ( uint32_t br );

		void write ( const char *word );

		void write( int number );

		void write ( char c );

		bool available ( void );

		const char read ( void );

		String readString ( void );
};

class uart1
{
	public:
		void begin ( uint32_t br );

		void write ( const char *word );

		void write( int number );

		void write ( char c );

		bool available ( void );

		const char read ( void );

		String readString ( void );
};

class uart2
{
	public:
		void begin ( uint32_t br );

		void write ( const char *word );

		void write( int number );

		void write ( char c );

		bool available ( void );

		const char read ( void );

		String readString ( void );
};

extern uart0 aux0;
extern uart1 aux1;
extern uart2 aux2;

#ifdef __cplusplus
}
#endif

#endif /* __COCO_UART_H_ */

