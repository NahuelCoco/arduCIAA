#include "coco_wire.h"
I2CM_XFER_T  i2cmXferRec;

/*
 * Función:			void Wire::manager ( uint8_t devAddr,uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize )
 *
 * Uso:				Esta función será utilizada por las demás para el envio y recepción de datos por protocolo I2C.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-devAddr: Address del dispositivo slave I2C.
 * 					-txBuffPtr: Puntero donde está la información a enviar. NULL si solo se quiere leer
 * 					-txSize: Cantidad de bytes a enviar. 0 si solo se quiere leer.
 * 					-rxBuffPtr: Puntero donde se quiere almacenar la información leida.
 * 					-rxSize: Cantidad de bytes que se desean leer.
 *
 */

void Wire::manager ( uint8_t devAddr,uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize )
{
	i2cmXferRec.slaveAddr = devAddr; //Address del slave I2C
	i2cmXferRec.options = 0;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize; //Tamaño del dato a enviar en cantidad de bytes
	i2cmXferRec.rxSz = rxSize; //Tamaño de los datos que se recibirán en cantidad de bytes
	i2cmXferRec.txBuff = txBuffPtr; //Buffer de transmisión
	i2cmXferRec.rxBuff = rxBuffPtr; //Buffer de recepción
	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cmXferRec); //Ejecución
}

/*
 * Función:			void Wire::begin ( void )
 *
 * Uso:				Inicializa protocolo I2C con Pull-Up y a 400KHz. Se puede cambiar la velocidad con setClock
 * 					o desde esta función.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void Wire::begin ( void )
{
    Chip_SCU_I2C0PinConfig(I2C0_STANDARD_FAST_MODE); //Configura pines SDA y SCL con Pull-Up
    Chip_I2C_Init(I2C0); //Inicializa I2C0
    Chip_I2C_SetClockRate(I2C0, CLOCK_400KHz); //Setea velocidad de 400 KHz
    Chip_I2C_SetMasterEventHandler( I2C0, Chip_I2C_EventHandlerPolling ); //Deshabilita interrupciones
}

/*
 * Función:			void Wire::beginTransmission ( uint8_t address )
 *
 * Uso:				Inicializa comunicación I2C con el address indicado.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-address: Address del Slave al que se desea comunicar.
 *
 */

void Wire::beginTransmission ( uint8_t address )
{
    iam = address;
}

/*
 * Función:			char Wire::endTransmission ( void )
 *
 * Uso:				Finaliza comunicación I2C.
 *
 * Return:			Devuelve un 0 luego de finalizar comunicación.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

char Wire::endTransmission ( void )
{
	Chip_I2C_DeInit(I2C0);
	return 0;
}

/*
 * Función:			char Wire::write ( uint8_t value )
 *
 * Uso:				Envía un byte de información sin signo con el Slave con el que se inició comunicación.
 *
 * Return:			Devuelve un 1 luego de hacerlo.
 *
 * Parámetros:		-value: Información a enviar.
 *
 */

char Wire::write ( uint8_t value )
{
	uint8_t buffer[1];
	buffer[0] = value;
	manager(iam, buffer, 1, (uint8_t*)NULL, 0);
	return 1;
}

/*
 * Función:			char Wire::write ( uint8_t data[], int length )
 *
 * Uso:				Envía la cantidad de bytes deseada al Slave con el que se inició comunicación.
 *
 * Return:			Devuelve el tamaño de bytes enviados.
 *
 * Parámetros:		-data: Información a enviar.
 * 					-length: Cantidad de bytes a enviar.
 *
 */

char Wire::write ( uint8_t data[], int length )
{
	manager(iam, data, length, (uint8_t*)NULL, 0);
	return length;
}

/*
 * Función:			char Wire::requestFrom ( uint8_t address, uint8_t quantity )
 *
 * Uso:				Solicita una cantidad de bytes a un Slave.
 *
 * Return:			Devuelve la información solicitada.
 *
 * Parámetros:		-address: Address del Slave del cual se deberá esperar información.
 * 					-quantity: Cantidad de bytes que se esperan.
 *
 */

char Wire::requestFrom ( uint8_t address, uint8_t quantity )
{
	uint8_t auxReceived[quantity];
	int i;
	manager(address, (uint8_t*)NULL, 0, auxReceived, quantity);
	for(i=0; i < quantity; i++)
	{
		globalReceived[i] = auxReceived[i];
	}
	globalQuantity = quantity;
	globalAuxiliar = 0;
	return quantity;
}

/*
 * Función:			uint8_t Wire::available ( void )
 *
 * Uso:				Indica la cantidad de bytes disponibles para leer.
 *
 * Return:			Devuelve el número de bytes disponibles a ser leidos.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

uint8_t Wire::available ( void )
{
	return globalQuantity;
}

/*
 * Función:			uint8_t Wire::read ( void )
 *
 * Uso:				Leerá de a un byte cada vez que la función sea llamada.
 *
 * Return:			Devuelve la información disponible en el Buffer. De no haber nada, devuelve un 0.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

uint8_t Wire::read ( void )
{
	uint8_t toReturn;
	if(globalQuantity > 0)
	{
		toReturn = globalReceived[globalAuxiliar];
		globalAuxiliar++;
		globalQuantity--;
		return toReturn;
	}
	else
	{
		return 0;
	}
}

/*
 * Función:			void Wire::setClock ( long clockFrequency)
 *
 * Uso:				Seteará la frecuencia para el clock del protocolo I2C.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-clockFrequency: Frecuencia en Hz que se desea para el clock del bus I2C.
 *
 */

void Wire::setClock ( long clockFrequency)
{
    Chip_I2C_SetClockRate(I2C0, clockFrequency);
}
