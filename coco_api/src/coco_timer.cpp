#include "coco_timer.h"
#include "coco_gpio.h"

bool flagTimer = false;
double long globalClock = 0;

/*
 * Función:			void initTimer ( void )
 *
 * Uso:				Con esta función inicializaremos el timer 0 y el 1.
 *
 * Return:			No hay retorno.
 *
 * Parámetros:		No necesita parámetros.
 *
 */

void initTimer ( void )
{
	Chip_TIMER_Init(LPC_TIMER0);
	Chip_TIMER_Reset(LPC_TIMER0);

	Chip_TIMER_Init(LPC_TIMER1);
	Chip_TIMER_Reset(LPC_TIMER1);

	Chip_TIMER_SetMatch(LPC_TIMER1, 0, (SystemCoreClock/TICKRATE_HZ_MS));
	Chip_TIMER_MatchEnableInt(LPC_TIMER1, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 0);
	Chip_TIMER_Enable(LPC_TIMER1);
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER1_IRQn);
}

/*
 * Función:			bool delay ( long timeMS )
 *
 * Uso:				Con esta función realizaremos un retardo de un tiempo determinado.
 *
 * Return:			Devolverá un 1 cuando finalice el tiempo
 *
 * Parámetros:		-timeMS: Tiempo en milisegundos que se desea esperar.
 *
 */

bool delay ( long timeMS )
{
	bool rtrn = false;
	uint32_t aux = SystemCoreClock/TICKRATE_HZ_MS;
	timeMS = timeMS*aux;

	Chip_TIMER_SetMatch(LPC_TIMER0, 0, (timeMS));
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 0);
	Chip_TIMER_Enable(LPC_TIMER0);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);

	while(!flagTimer);

	flagTimer = false;

	Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
	Chip_TIMER_Disable(LPC_TIMER0);
	Chip_TIMER_Reset(LPC_TIMER0);

	rtrn = true;
	return rtrn;
}

/*
 * Función:			bool delay ( long timeUS )
 *
 * Uso:				Con esta función realizaremos un retardo de un tiempo determinado.
 *
 * Return:			Devolverá un 1 cuando finalice el tiempo
 *
 * Parámetros:		-timeUS: Tiempo en microsegundos que se desea esperar.
 *
 */

bool delayMicroseconds ( long timeUS )
{
	bool rtrn = false;
	uint32_t aux = SystemCoreClock/TICKRATE_HZ_US;
	timeUS = timeUS*aux;

	Chip_TIMER_SetMatch(LPC_TIMER0, 0, (timeUS));
	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 0);
	Chip_TIMER_Enable(LPC_TIMER0);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);

	while(!flagTimer);

	flagTimer = false;

	Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
	Chip_TIMER_Disable(LPC_TIMER0);
	Chip_TIMER_Reset(LPC_TIMER0);

	rtrn = true;
	return rtrn;
}

/*
 * Función:			double long millis ( void )
 *
 * Uso:				Esta función devolverá el tiempo que pasó desde que se inicio el uC.
 *
 * Return:			Devolverá un double long con el tiempo.
 *
 * Parámetros:		No requiere parámetros.
 *
 */

double long millis ( void )
{
	return globalClock;
}

extern "C"
{

/*
 * Función:			void TIMER0_IRQHandler(void)
 *
 * Uso:				Interrupción que cambiará el estado del flag cuando se cumpla el tiempo.
 *
 * Return:			No retorna parámetro.
 *
 * Parámetros:		No necesita parámetro.
 *
 */

	void TIMER0_IRQHandler(void)
	{

		if(Chip_TIMER_MatchPending(LPC_TIMER0, 0))
		{

			Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
			flagTimer = true;

		}

	}

/*
* Función:			void TIMER1_IRQHandler(void)
*
* Uso:				Se utilizará para aumentar en 1 cada 1 mS a globalClock.
*
* Return:			No retorna parámetro.
*
* Parámetros:		No necesita parámetro.
*
*/

	void TIMER1_IRQHandler(void)
	{

		if(Chip_TIMER_MatchPending(LPC_TIMER1, 0))
		{

			Chip_TIMER_ClearMatch(LPC_TIMER1, 0);
			globalClock++;
		}

	}


}
