#include "coco_gpio.h"


#define digitalPinsNumber ( sizeof(digitalPins) / sizeof(struct digital_t) )

/*
 * Función:			bool pinMode ( uint8_t digitalNumber, bool setting )
 *
 * Uso:				Con esta función indicaremos el estado salida/entrada de un GPIO.
 *
 * Return:			Devolverá un 1 en caso de ejecutarse correctamente. De lo contrario, un 0.
 *
 * Parámetros:		-digitalNumber: Número de pin digital en la API COCO del GPIO a configurar.
 * 					-setting: OUTPUT para que sea salida. INPUT para que sea entrada.
 *
 */

bool pinMode ( uint8_t digitalNumber, bool setting )
{
	const struct digital_t *gpio = &digitalPins[digitalNumber];
	bool value = 0;
	if ( digitalNumber <= digitalPinsNumber)
	{
		if( setting )
		{
			Chip_SCU_PinMuxSet(gpio->numberPort, gpio->numberPin, (SCU_MODE_INACT | SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN));
			Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpio->gpioPort, gpio->gpioPin);
		}

		else
		{
			Chip_SCU_PinMuxSet(gpio->numberPort, gpio->numberPin, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS)); //SCU_MODE_PULLDOWN/PULLUP
			Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, gpio->gpioPort, gpio->gpioPin);
		}
		value = 1;
	}
	else
	{
		value = 0;
	}
	return value;
}

/*
 * Función:			bool digitalWrite ( uint8_t digitalNumber, bool state )
 *
 * Uso:				Con esta función configuraremos un GPIO en un 1 o en un 0.
 *
 * Return:			Devolverá un 1 en caso de ejecutarse correctamente. De lo contrario, un 0.
 *
 * Parámetros:		-digitalNumber: Número de pin digital en la API COCO del GPIO a configurar.
 * 					-state: HIGH para establecerlo en 1. LOW para establecerlo en 0.
 *
 */

bool digitalWrite ( uint8_t digitalNumber, bool state )
{
	const struct digital_t *gpio = &digitalPins[digitalNumber];
	bool value = 0;
	if ( digitalNumber <= digitalPinsNumber)
	{
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpio->gpioPort, gpio->gpioPin, state);
		value = 1;
	}
	else
	{
		value = 0;
	}
	return value;
}

/*
 * Función:			bool digitalRead ( uint8_t digitalNumber )
 *
 * Uso:				Con esta función podremos conocer el estado de un GPIO.
 *
 * Return:			Devolverá el estado del GPIO indicado como parámetro.
 *
 * Parámetros:		-digitalNumber: Número de pin digital en la API COCO del GPIO a configurar.
 *
 */

bool digitalRead ( uint8_t digitalNumber )
{
	const struct digital_t *gpio = &digitalPins[digitalNumber];
	bool value = 0;
	if ( digitalNumber <= digitalPinsNumber)
	{
		value = (bool) Chip_GPIO_GetPinState( LPC_GPIO_PORT, gpio->gpioPort, gpio->gpioPin );
	}
	else
	{
		value = 0;
	}
	return value;
}

/*
 * Función:			bool digitalToggle ( uint8_t digitalNumber )
 *
 * Uso:				Con esta función invertiremos el estado de un GPIO.
 *
 * Return:			Devolverá un 1 en caso de ejecutarse correctamente. De lo contrario, un 0.
 *
 * Parámetros:		-digitalNumber: Número de pin digital en la API COCO del GPIO a configurar.
 *
 */

bool digitalToggle ( uint8_t digitalNumber )
{
	bool value = 0;
	if ( digitalNumber <= digitalPinsNumber)
	{
		digitalWrite(digitalNumber, !digitalRead(digitalNumber));
		value = 1;
	}
	else
	{
		value = 0;
	}
	return value;
}
