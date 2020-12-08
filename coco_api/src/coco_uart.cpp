#include "coco_uart.h"
#include "coco_gpio.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
uart0 aux0;
uart1 aux1;
uart2 aux2;
String val0;
String val1;
String val2;


/*
 * Función:			void uart0::begin ( uint32_t br )
 *
 * Uso:				Con esta función inicializaremos el UART0 el cual corresponde al etiquetado RS485 en la EDUCIAA
 * 					y al USART0 en el microcontrolador.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-br: Velocidad en baudios con la que se inicializará el UART0.
 *
 */

void uart0::begin ( uint32_t br )
{
	Chip_SCU_PinMuxSet(9, 5, (SCU_MODE_INACT | SCU_MODE_FUNC7)); //Tx
	Chip_SCU_PinMuxSet(9, 6, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC7)); //Rx
	Chip_UART_Init(LPC_USART0); //Inicializo USART0
	Chip_UART_SetBaud(LPC_USART0, br); //Configuro el baudrate de la USART0
	Chip_UART_ConfigData(LPC_USART0, UART_LCR_WLEN8 | UART_LCR_PARITY_DIS | UART_LCR_SBS_1BIT); //Configuro USART0 con 8 bits de data 1 de stop y sin paridad
	Chip_UART_TXEnable(LPC_USART0); //Habilito el envio de datos
}

/*
 * Función:			bool uart0::available ( void )
 *
 * Uso:				Con esta función conoceremos si existe información disponible en el buffer
 * 					de la UART0.
 *
 * Return:			Devuelve un 1 en caso de haber información y un 0 en caso contrario.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

bool uart0::available ( void )
{
	int cont = 0; //Variable auxiliar de contador
	char aux[100]; //Reservo 8 bytes * 100
	if(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_RDR)
	{
		for (cont = 0; cont < 100; cont++)
		{
			aux[cont] = '\0'; //Limpio vector de caracteres
		}
		cont = 0;
		while(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_RDR)
		{
			aux[cont] = (const char) Chip_UART_ReadByte(LPC_USART0);
			cont++;
		}
		val2 = aux; //Guardamos lo leido en la variable global val2 para luego si se desea, ser utilizada en la función read
		if(aux[0] == '\n' || aux[0] == '\r' )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return 0;
}

/*
 * Función:			void uart0::write ( const char *word )
 *
 * Uso:				Con esta función podremos enviar una cadena de caracteres por el UART0.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Cadena de caracteres que se desea enviar.
 *
 */

void uart0::write ( const char *word )
{
	while( *word != '\0' ) //Detecto el fin de la palabra
	{
		while ( !(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE));
		Chip_UART_SendByte(LPC_USART0, (uint8_t) *word++);
	}
}
/*
 * Función:			void uart0::write ( char c )
 *
 * Uso:				Con esta función podremos enviar 1 byte por el UART0.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Byte que se desea enviar.
 *
 */

/*
 * Función:			void uart0::write ( int number )
 *
 * Uso:				Con esta función podremos enviar un número entero por el UART0.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Cadena de caracteres que se desea enviar.
 *
 */

void uart0::write( int number )
{
	//int arrayNumbers[6];
	int aux;
	int div=1;
	char i=0;
	char j=0;
	if (number >= 0)
	{
		aux = number;
		while(aux)
		{
			aux = aux / 10;
			div = div*10;
			i++;
		}
		i++;
		char arrayNumbers[i];

		i--;
		j=i;
		i=0;
		while(div>1)
		{
			div = div/10;
			arrayNumbers[i] = (number / div) + 48;
			number = number % div;
			i++;
		}

		arrayNumbers[i]='\0';
		const char *word = arrayNumbers;

		while( *word != '\0' ) //Detecto el fin de la palabra
			{
				while ( !(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE));
				Chip_UART_SendByte(LPC_USART0, (uint8_t) *word++);
			}
	}
	else
	{
		i = 1;
		j = 1;
		number = (-1)*number;
		aux = number;
		while(aux)
		{
			aux = aux / 10;
			div = div*10;
			i++;
		}
		i++;
		char arrayNumbers[i];

		i--;
		j=i;
		i=1;
		while(div>1)
		{
			div = div/10;
			arrayNumbers[i] = (number / div) + 48;
			number = number % div;
			i++;
		}

		arrayNumbers[i]='\0';
		arrayNumbers[0]='-';
		const char *word = arrayNumbers;

		while( *word != '\0' ) //Detecto el fin de la palabra
			{
				while ( !(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE));
				Chip_UART_SendByte(LPC_USART0, (uint8_t) *word++);
			}
	}

}


void uart0::write ( char c )
{
	while ( !(Chip_UART_ReadLineStatus(LPC_USART0) & UART_LSR_THRE));
	Chip_UART_SendByte(LPC_USART0, (uint8_t) c);
}

/*
 * Función:			const char uart0::read ( void )
 *
 * Uso:				Con esta función podremos leer 1 byte por el UART0.
 *
 * Return:			Devuelve 1 byte obtenido del buffer del UART0.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

const char uart0::read ( void )
{
	return (const char) Chip_UART_ReadByte(LPC_USART0);
}

/*
 * Función:			String uart0::readString ( void )
 *
 * Uso:				Con esta función podremos leer una cadena de caracteres por el UART0.
 *
 * Return:			Devuelve la cadena de caracteres disponible en el buffer del UART0.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

String uart0::readString ( void )
{
	return val0;
}


/*
 * Función:			void uart1::begin ( uint32_t br )
 *
 * Uso:				Con esta función inicializaremos el UART1 el cual corresponde al etiquetado RS232 en la EDUCIAA
 * 					y al USART3 en el microcontrolador.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-br: Velocidad en baudios con la que se inicializará el UART1.
 *
 */

void uart1::begin ( uint32_t br )
{
	Chip_SCU_PinMuxSet(2, 3, (SCU_MODE_INACT | SCU_MODE_FUNC2));
	Chip_SCU_PinMuxSet(2, 4, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC2));
	Chip_UART_Init(LPC_USART3); //Inicializo USART3
	Chip_UART_SetBaud(LPC_USART3, br); //Configuro el baudrate de la USART3
	Chip_UART_ConfigData(LPC_USART3, UART_LCR_WLEN8 | UART_LCR_PARITY_DIS | UART_LCR_SBS_1BIT); //Configuro USART3 con 8 bits de data 1 de stop y sin paridad
	Chip_UART_TXEnable(LPC_USART3); //Habilito el envio de datos
}

/*
 * Función:			String uart1::readString ( void )
 *
 * Uso:				Con esta función podremos leer una cadena de caracteres por el UART1.
 *
 * Return:			Devuelve la cadena de caracteres disponible en el buffer del UART1.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

String uart1::readString( void )
{
	return val1;
}

/*
 * Función:			void uart1::write ( const char *word )
 *
 * Uso:				Con esta función podremos enviar una cadena de caracteres por el UART1.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Cadena de caracteres que se desea enviar.
 *
 */

void uart1::write( const char *word )
{
	while( *word != '\0' ) //Detecto el fin de la palabra
	{
		while ( !(Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_THRE));
		Chip_UART_SendByte(LPC_USART3, (uint8_t) *word++);
	}
}

/*
 * Función:			void uart1::write ( int number )
 *
 * Uso:				Con esta función podremos enviar un número entero por el UART3.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Cadena de caracteres que se desea enviar.
 *
 */

void uart1::write( int number )
{
	//int arrayNumbers[6];
	int aux;
	int div=1;
	char i=0;
	char j=0;
	if (number >= 0)
	{
		aux = number;
		while(aux)
		{
			aux = aux / 10;
			div = div*10;
			i++;
		}
		i++;
		char arrayNumbers[i];

		i--;
		j=i;
		i=0;
		while(div>1)
		{
			div = div/10;
			arrayNumbers[i] = (number / div) + 48;
			number = number % div;
			i++;
		}

		arrayNumbers[i]='\0';
		const char *word = arrayNumbers;

		while( *word != '\0' ) //Detecto el fin de la palabra
			{
				while ( !(Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_THRE));
				Chip_UART_SendByte(LPC_USART3, (uint8_t) *word++);
			}
	}
	else
	{
		i = 1;
		j = 1;
		number = (-1)*number;
		aux = number;
		while(aux)
		{
			aux = aux / 10;
			div = div*10;
			i++;
		}
		i++;
		char arrayNumbers[i];

		i--;
		j=i;
		i=1;
		while(div>1)
		{
			div = div/10;
			arrayNumbers[i] = (number / div) + 48;
			number = number % div;
			i++;
		}

		arrayNumbers[i]='\0';
		arrayNumbers[0]='-';
		const char *word = arrayNumbers;

		while( *word != '\0' ) //Detecto el fin de la palabra
			{
				while ( !(Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_THRE));
				Chip_UART_SendByte(LPC_USART3, (uint8_t) *word++);
			}
	}

}


/*
 * Función:			void uart1::write ( char c )
 *
 * Uso:				Con esta función podremos enviar 1 byte por el UART1.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Byte que se desea enviar.
 *
 */

void uart1::write ( char c )
{
	while ( !(Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_THRE));
	Chip_UART_SendByte(LPC_USART3, (uint8_t) c);
}

/*
 * Función:			const char uart1::read ( void )
 *
 * Uso:				Con esta función podremos leer 1 byte por el UART1.
 *
 * Return:			Devuelve 1 byte obtenido del buffer del UART1.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

const char uart1::read ( void )
{
	return (const char) Chip_UART_ReadByte(LPC_USART3);
}

/*
 * Función:			bool uart1::available ( void )
 *
 * Uso:				Con esta función conoceremos si existe información disponible en el buffer
 * 					de la UART1.
 *
 * Return:			Devuelve un 1 en caso de haber información y un 0 en caso contrario.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

bool uart1::available ( void )
{
	int cont = 0; //Variable auxiliar de contador
	char aux[100]; //Reservo 8 bytes * 100
	if(Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_RDR)
	{
		for (cont = 0; cont < 100; cont++)
		{
			aux[cont] = '\0'; //Limpio vector de caracteres
		}
		cont = 0;
		while(Chip_UART_ReadLineStatus(LPC_USART3) & UART_LSR_RDR)
		{
			aux[cont] = (const char) Chip_UART_ReadByte(LPC_USART3);
			cont++;
		}
		val2 = aux; //Guardamos lo leido en la variable global val2 para luego si se desea, ser utilizada en la función read
		if(aux[0] == '\n' || aux[0] == '\r' )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return 0;
}


/*
 * Función:			void uart2::begin ( uint32_t br )
 *
 * Uso:				Con esta función inicializaremos el UART2 el cual corresponde al etiquetado USB en la EDUCIAA
 * 					y al USART2 en el microcontrolador.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-br: Velocidad en baudios con la que se inicializará el UART0.
 *
 */

void uart2::begin ( uint32_t br )
{
	Chip_SCU_PinMuxSet(7, 1, (SCU_MODE_INACT | SCU_MODE_FUNC6));
	Chip_SCU_PinMuxSet(7, 2, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC6));
	Chip_UART_Init(LPC_USART2); //Inicializo USART2
	Chip_UART_SetBaud(LPC_USART2, br); //Configuro el baudrate de la USART2
	Chip_UART_ConfigData(LPC_USART2, UART_LCR_WLEN8 | UART_LCR_PARITY_DIS | UART_LCR_SBS_1BIT); //Configuro USART2 con 8 bits de data 1 de stop y sin paridad
	Chip_UART_TXEnable(LPC_USART2); //Habilito el envio de datos
}

/*
 * Función:			String uart2::readString ( void )
 *
 * Uso:				Con esta función podremos leer una cadena de caracteres por el UART2.
 *
 * Return:			Devuelve la cadena de caracteres disponible en el buffer del UART2.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

String uart2::readString( void )
{
	return val2;
}

/*
 * Función:			void uart2::write ( const char *word )
 *
 * Uso:				Con esta función podremos enviar una cadena de caracteres por el UART2.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Cadena de caracteres que se desea enviar.
 *
 */

void uart2::write( const char *word )
{
	while( *word != '\0' ) //Detecto el fin de la palabra
	{
		while ( !(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_THRE));
		Chip_UART_SendByte(LPC_USART2, (uint8_t) *word++);
	}
}

/*
 * Función:			void uart2::write ( int number )
 *
 * Uso:				Con esta función podremos enviar un número entero por el UART2.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Cadena de caracteres que se desea enviar.
 *
 */

void uart2::write( int number )
{
	//int arrayNumbers[6];
	int aux;
	int div=1;
	char i=0;
	char j=0;
	if (number >= 0)
	{
		aux = number;
		while(aux)
		{
			aux = aux / 10;
			div = div*10;
			i++;
		}
		i++;
		char arrayNumbers[i];

		i--;
		j=i;
		i=0;
		while(div>1)
		{
			div = div/10;
			arrayNumbers[i] = (number / div) + 48;
			number = number % div;
			i++;
		}

		arrayNumbers[i]='\0';
		const char *word = arrayNumbers;

		while( *word != '\0' ) //Detecto el fin de la palabra
			{
				while ( !(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_THRE));
				Chip_UART_SendByte(LPC_USART2, (uint8_t) *word++);
			}
	}
	else
	{
		i = 1;
		j = 1;
		number = (-1)*number;
		aux = number;
		while(aux)
		{
			aux = aux / 10;
			div = div*10;
			i++;
		}
		i++;
		char arrayNumbers[i];

		i--;
		j=i;
		i=1;
		while(div>1)
		{
			div = div/10;
			arrayNumbers[i] = (number / div) + 48;
			number = number % div;
			i++;
		}

		arrayNumbers[i]='\0';
		arrayNumbers[0]='-';
		const char *word = arrayNumbers;

		while( *word != '\0' ) //Detecto el fin de la palabra
			{
				while ( !(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_THRE));
				Chip_UART_SendByte(LPC_USART2, (uint8_t) *word++);
			}
	}

}

/*
 * Función:			void uart2::write ( char c )
 *
 * Uso:				Con esta función podremos enviar 1 byte por el UART2.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-word: Byte que se desea enviar.
 *
 */

void uart2::write( char c )
{
	while ( !(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_THRE));
	Chip_UART_SendByte(LPC_USART2, (uint8_t) c);
}

/*
 * Función:			const char uart2::read ( void )
 *
 * Uso:				Con esta función podremos leer 1 byte por el UART2.
 *
 * Return:			Devuelve 1 byte obtenido del buffer del UART2.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

const char uart2::read ( void )
{
	return (const char) Chip_UART_ReadByte(LPC_USART2);
}

/*
 * Función:			bool uart2::available ( void )
 *
 * Uso:				Con esta función conoceremos si existe información disponible en el buffer (16 bytes FIFO)
 * 					de la UART2.
 *
 * Return:			Devuelve un 1 en caso de haber información y un 0 en caso contrario.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

bool uart2::available ( void )
{
	int cont = 0; //Variable auxiliar de contador
	char aux[100]; //Reservo 8 bytes * 100
	if(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_RDR)
	{
		for (cont = 0; cont < 100; cont++)
		{
			aux[cont] = '\0'; //Limpio vector de caracteres
		}
		cont = 0;
		while(Chip_UART_ReadLineStatus(LPC_USART2) & UART_LSR_RDR)
		{
			aux[cont] = (const char) Chip_UART_ReadByte(LPC_USART2);
			cont++;
		}
		val2 = aux; //Guardamos lo leido en la variable global val2 para luego si se desea, ser utilizada en la función read
		if(aux[0] == '\n' || aux[0] == '\r' )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return 0;
}
