#include "coco_pwm.h"
#include "coco_gpio.h"

/*
 * Función:			void pwm_init ( void )
 *
 * Uso:				Con esta función inicializaremos SCT para usar como PWM. Esta función se ejecuta al llamar
 * 					a la función _coco() en el programa principal.
 *
 * Return:			No devuelve ningun parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void pwm_init ( void )
{
	Chip_SCTPWM_Init(SCT_PWM); //Iniciamos SCT para usar como PWM
	Chip_SCTPWM_SetRate(SCT_PWM, frec_init); //Seteamos frecuencia en 50 Hz
	Chip_SCTPWM_Start(SCT_PWM);
}

/*
 * Función:			void pwm_changeFrec ( long frec )
 *
 * Uso:				Con esta función modificaremos la frecuencia del PWM.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-frec: Valor en Hz de la frecuencia deseada para el periodo del PWM.
 *
 */

void pwm_changeFrec ( long frec ) //Esta función no existe en Arduino como genérica por lo que se optó por este prototipo
{
	//freq = frec;
	Chip_SCTPWM_Stop(SCT_PWM);
	Chip_SCTPWM_Init(SCT_PWM);
	Chip_SCTPWM_SetRate(SCT_PWM, frec);
	Chip_SCTPWM_Start(SCT_PWM);
}

/*
 * Función:			void analogWrite ( uint8_t pin, int8_t value )
 *
 * Uso:				Seteará el valor del Dutty para el PWM en el pin de PWM seleccionado.
 *
 * Return:			Devolverá un 1 en caso de ejecutarse correctamente. De lo contrario, un 0.
 *
 * Parámetros:		-pin: Número de pin PWM en la API COCO del PWM a configurar. Utilice el mismo número
 * 					existe en coco_gpio.h pero verifique que al lado del número tenga "~", eso significará
 * 					que es un pin apto para esta función.
 * 					-value: Este parámetro deberá ser un valor de 0 a 255 y será de utlidad para indicar
 * 					el dutty deseado en el PWM.
 *
 */

bool analogWrite ( uint8_t pin, uint8_t value )
{
	bool ok = 0;
	uint8_t indice=0;

	if ( value >= 0 && value <= 255 )
	{
		if ( pin == 4 || pin == 7 || pin == 29 || pin == 31 || pin == 32 || pin == 33 || pin == 34 || pin == 35 || pin == 39 || pin == 40 || pin == 41 )
		{
			if(pin == 4)
			{
				indice = 6;
			}
			if(pin == 7)
			{
				indice = 7;
			}
			if(pin == 29)
			{
				indice = 13;
			}
			if(pin == 31)
			{
				indice = 3;
			}
			if(pin == 32)
			{
				indice = 0;
			}
			if(pin == 33)
			{
				indice = 10;
			}
			if(pin == 34)
			{
				indice = 1;
			}
			if(pin == 35)
			{
				indice = 12;
			}
			if(pin == 39)
			{
				indice = 2;
			}
			if(pin == 40)
			{
				indice = 5;
			}
			if(pin == 41)
			{
				indice = 4;
			}
			const struct digital_t *p = &digitalPins[pin]; //En coco_gpio.h
			uint8_t percent = value * 100 / 255 ;
			Chip_SCU_PinMux(p->numberPort, p->numberPin, SCU_MODE_INACT, FUNC1);
			Chip_SCTPWM_SetOutPin(SCT_PWM, pin, indice);
			Chip_SCTPWM_SetDutyCycle(SCT_PWM, pin, Chip_SCTPWM_PercentageToTicks(SCT_PWM, percent));
			ok = 1;
		}
	}
	return ok;
}


bool analogWriteUS ( uint8_t pin, int value )
{
	bool ok = 0;
	uint8_t indice=0;
	float percent;
	long period;

	period = (1 / 200.00)*1000000;

	//ticksTotal = Chip_SCTPWM_GetTicksPerCycle(SCT_PWM);
	//Ticks1ms = ticksTotal / 100;
	//TicksOut = Ticks1ms / 1000;
	percent = value * 100.00 / period;

		if ( pin == 4 || pin == 7 || pin == 12 || pin == 13 || pin == 14 || pin == 29 || pin == 31 || pin == 32 || pin == 33 || pin == 34 || pin == 35 || pin == 39 || pin == 40 || pin == 41 )
		{
			if(pin == 4)
			{
				indice = 6;
			}
			if(pin == 7)
			{
				indice = 7;
			}
			if(pin == 29)
			{
				indice = 13;
			}
			if(pin == 31)
			{
				indice = 3;
			}
			if(pin == 32)
			{
				indice = 0;
			}
			if(pin == 33)
			{
				indice = 10;
			}
			if(pin == 34)
			{
				indice = 1;
			}
			if(pin == 35)
			{
				indice = 12;
			}
			if(pin == 39 || pin == 12)
			{
				indice = 2;
			}
			if(pin == 40 || pin == 13)
			{
				indice = 5;
			}
			if(pin == 41 || pin == 14)
			{
				indice = 4;
			}
			const struct digital_t *p = &digitalPins[pin]; //En coco_gpio.h
			Chip_SCU_PinMux(p->numberPort, p->numberPin, SCU_MODE_INACT, FUNC1);
			Chip_SCTPWM_SetOutPin(SCT_PWM, indice+1, indice);
			Chip_SCTPWM_SetDutyCycle(SCT_PWM, indice+1, Chip_SCTPWM_PercentageToTicks(SCT_PWM,percent));
			ok = 1;
		}
	return ok;
}
