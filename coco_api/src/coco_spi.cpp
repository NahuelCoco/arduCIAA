#include "coco_spi.h"
/*
 * Funcionamiento de SPI:
 * Tenemos 4 pines:
 * MOSI - Master Out Slave In: Información del maestro al esclavo
 * MISO - Master In Slave Out: Información del esclavo al maestro (Recordemos que es bidireccional)
 * CLK - Clock
 * SS - Slave Select: Habilita y selecciona un esclavo
 * Hay que conocer que SPI tiene 4 modos de trabajo:
 * Modo 0: CPOL = 0 y CPHA = 0. Modo en el cual el estado del reloj permanece en estado lógico bajo y la información se envía en cada transición de bajo a alto, es decir alto activo.
 * Modo 1: CPOL = 0 y CPHA = 1. Modo en el cual el estado del reloj permanece en estado lógico bajo y la información se envía en cada transición de alto a bajo, es decir bajo activo.
 * Modo 2: CPOL = 1 y CPHA = 0. Modo en el cual el estado del reloj permanece en estado lógico alto y la información se envía en cada transición de bajo a alto, es decir alto activo.
 * Modo 3: CPOL = 1 y CPHA = 1. Modo en el cual el estado del reloj permanece en estado lógico alto y la información se envía en cada transición de alto a bajo, es decir bajo activo.
 *
 * Cada esclavo puede trabajar en cualquier modo y por ende el maestro será el que se adapte a cada uno de los dispositivos
 * a conectar.
 *
 * La conexión de los esclavos se puede hacer de dos maneras, de forma encadenada o paralela.
 *
 *
 */




/*
 * Función:			void SPI::begin ( void )
 *
 * Uso:				Con esta función inicializaremos el módulo SPI lo cual incluye, configurar los pines
 * 					que se relacionan con el mismo.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void SPI::begin ( void )
{
    Chip_SCU_PinMuxSet(0xf, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); //Clock
    Chip_SCU_PinMuxSet(0x1, 3, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); //Configuramos MiSo
    Chip_SCU_PinMuxSet(0x1, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC5)); //Configuramos MoSi

    Chip_SCU_PinMuxSet(0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); //CS1 - GPIO0
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);

    // Iniciamos SSP
    Chip_SSP_Init( LPC_SSP1 );
    Chip_SSP_Enable( LPC_SSP1 );
}

/*
 * Función:			void SPI::setDataMode ( uint32_t mode )
 *
 * Uso:				Con esta función setearemos uno de los 4 posibles para SPI trabajando siempre en 8 bits.
 *
 * Return:			No devuelve ningún parámetro
 *
 * Parámetros:		-mode: Seteará uno de los 4 modos definidos en coco_spi.h.
 *
 */

void SPI::setDataMode ( uint32_t mode )
{
	Chip_SSP_SetFormat( LPC_SSP1, SSP_BITS_8, SSP_FRAMEFORMAT_SPI, mode);
}

/*
 * Función:			void SPI::setClockDivider ( uint32_t divider )
 *
 * Uso:				Con esta función setearemos el divisor para configurar la frecuencia de trabajo de SPI.
 *
 * Return:			No devuelve ningún parámetro
 *
 * Parámetros:		-divider: Seteará uno de los divisores de frecuencia definidos en coco_spi.h.
 *
 */

void SPI::setClockDivider ( uint32_t divider )
{
	Chip_SSP_SetBitRate(LPC_SSP1, divider);
}

/*
 * Función:			uint8_t SPI::transfer ( uint8_t val )
 *
 * Uso:				Con esta función podremos enviar y recibir datos utilizando protocolo SPI.
 *
 * Return:			Devolverá un valor de 8 bits en caso de estar disponible en el buffer.
 *
 * Parámetros:		-val: Valor que se desea transferir por SPI. Si solo desea leer, indicar NULL este valor.
 *
 */

uint8_t SPI::transfer ( uint8_t val )
{
	uint8_t bufferTx[1];
	uint8_t bufferRx[1];
	bufferTx[0] = val;
    Chip_SSP_DATA_SETUP_T xferConfig;

	xferConfig.tx_data = bufferTx;
	xferConfig.tx_cnt  = 0;
	xferConfig.rx_data = bufferRx;
	xferConfig.rx_cnt  = 0;
	xferConfig.length  = 1;

	Chip_SSP_RWFrames_Blocking( LPC_SSP1, &xferConfig );

	return bufferRx[0];
}

