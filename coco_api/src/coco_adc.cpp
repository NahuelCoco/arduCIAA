#include "coco_adc.h"

/*
 * A continuación se realizará la declaración de los pines digitales
 * La información del número de puerto, pin y canal ADC se obtuvo del
 * datasheet mientras que el número de pin digital se asigno de forma
 * personalizada en función a las comodidades
 * _______________________________________________________
 * Número de ADC | Número de CANAL | Nombre en API COCO
 * -------------------------------------------------------
 * 0/1             | 0             | A0
 * -------------------------------------------------------
 * -------------------------------------------------------
 * 0/1             | 1             | A1
 * -------------------------------------------------------
 * -------------------------------------------------------
 * 0/1             | 2             | A2
 * -------------------------------------------------------
 * -------------------------------------------------------
 * 0/1             | 3             | A3
 * -------------------------------------------------------
 * -------------------------------------------------------
 * 0/1             | 4             | A4
 * -------------------------------------------------------
 * -------------------------------------------------------
 * 0/1             | 5             | A5
 * -------------------------------------------------------
 * -------------------------------------------------------
 * 0/1             | 6             | A6
 * -------------------------------------------------------
 * -------------------------------------------------------
 * 0/1             | 7             | A7
 * -------------------------------------------------------
 */

//Vector que nos será de utilidad para seleccionar el canal a utilizar

ADC_CHANNEL_T channels[] = {
		ADC_CH0,
		ADC_CH1,
		ADC_CH2,
		ADC_CH3,
		ADC_CH4,
		ADC_CH5,
		ADC_CH6,
		ADC_CH7
};

/*
 * Función:			void adc_init ( bool channel )
 *
 * Uso:				Inicializará el canal deseado con las configuraciones explicitadas en los comentarios.
 * 					La inicialización ya se hace al llamar a la función _coco() en el programa principal.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-channel: Valor del canal que se procederá a habilitar.
 *
 */

void adc_init ( bool channel )
{
	/*
	 * Declararemos la variable config de manera de poder setear la velocidad.
	 * Luego se seteará la resolución en 10 bits y deshabilitaremos el modo BURST.
	 * Se recomienda modificar estos parámetros si así se desea desde "coco_adc.h"
	 */
	ADC_CLOCK_SETUP_T config = {
			ADC_SAMPLE_RATE,
			ADC_RESOLUTION,
			ADC_STATE //Al configurar "DISABLE" se deshabilita el modo BURST y por ende hay que tener en cuenta que se manejará manualmente
					//Si coloco "ENABLE", habilito el modo BURST y la conversion sera automatica.
	};

	//Para el canal 1
	if ( channel )
	{
		Chip_ADC_Init(LPC_ADC1, &config);
		Chip_ADC_SetBurstCmd(LPC_ADC1, ADC_STATE);
		Chip_ADC_SetSampleRate(LPC_ADC1, &config, ADC_SAMPLE_RATE);
		Chip_ADC_SetResolution(LPC_ADC1, &config, ADC_RESOLUTION);

		for (uint8_t aux = 0; aux <8; aux++)
		{
			Chip_ADC_EnableChannel(LPC_ADC1, channels[aux], DISABLE);
			Chip_ADC_Int_SetChannelCmd(LPC_ADC1, channels[aux], DISABLE);
		}
	}
	//Para el canal 0 - Este es el inicializado por default desde la función _coco()
	else
	{
		Chip_ADC_Init(LPC_ADC0, &config);
		Chip_ADC_SetBurstCmd(LPC_ADC0, ADC_STATE);
		Chip_ADC_SetSampleRate(LPC_ADC0, &config, ADC_SAMPLE_RATE);
		Chip_ADC_SetResolution(LPC_ADC0, &config, ADC_RESOLUTION);

		for (uint8_t aux = 0; aux <8; aux++)
		{
			Chip_ADC_EnableChannel(LPC_ADC0, channels[aux], DISABLE);
			Chip_ADC_Int_SetChannelCmd(LPC_ADC0, channels[aux], DISABLE);
		}
	}
}

/*
 * Función:			uint16_t analogRead ( uint8_t numberADC )
 *
 * Uso:				Esta función será utilizada para la lectura del canal deseado.
 *
 * Return:			Esta función devuelve el valor de lectura del ADC dentro de un rango máximo de 0 a 1023 para
 * 					una resolución de 10 bits.
 *
 * Parámetros:		-numberADC: El ADC que se desea leer que será indicado con las variables A0 hasta A7.
 * 								Ingresando a "coco_adc.h" podrá ver el nombre de etiqueta con el que se referencia
 * 								en el Datasheet para poder ubicar su pin en el microcontrolador.
 * 								En caso de querer utilizar el canal 1, sumar 8 al canal deseado, por ejemplo,
 * 								Para usar el A1 en canal 1, colocar A1 + 8.
 *
 */

uint16_t analogRead ( uint8_t numberADC )
{
	uint16_t value = 0;
	if(numberADC > 7)
	{
		Chip_ADC_EnableChannel(LPC_ADC1, channels[numberADC - 8], ENABLE); //Habilitamos un canal
		Chip_ADC_SetStartMode(LPC_ADC1, ADC_START_NOW, ADC_TRIGGERMODE_RISING); //Comienza la conversión y mide en el flanco descendente
		while( Chip_ADC_ReadStatus(LPC_ADC1, channels[numberADC - 8], ADC_DR_DONE_STAT) != SET ); //Esperamos que termine de convertir
		Chip_ADC_ReadValue(LPC_ADC1, channels[numberADC - 8], &value); //Tomamos el valor
		Chip_ADC_EnableChannel(LPC_ADC1, channels[numberADC - 8], DISABLE); //Deshabilitamos un canal

	}
	else
	{
		Chip_ADC_EnableChannel(LPC_ADC0, channels[numberADC], ENABLE); //Habilitamos un canal
		Chip_ADC_SetStartMode(LPC_ADC0, ADC_START_NOW, ADC_TRIGGERMODE_RISING); //Comienza la conversión y mide en el flanco descendente
		while( Chip_ADC_ReadStatus(LPC_ADC0, channels[numberADC], ADC_DR_DONE_STAT) != SET ); //Esperamos que termine de convertir
		Chip_ADC_ReadValue(LPC_ADC0, channels[numberADC], &value); //Tomamos el valor
		Chip_ADC_EnableChannel(LPC_ADC0, channels[numberADC], DISABLE); //Deshabilitamos un canal
	}

	return value;
}
