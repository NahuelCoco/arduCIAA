#include "coco_timer.h"
#include "coco_gpio.h"

bool flagTimer = false;

/*
 * Libreria en proceso. Solo utilizar función delay
 */

/*
 * Función:			void initTimer ( void )
 *
 * Uso:				Con esta función inicializaremos el timer 0.
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
}

extern "C"
{
	void TIMER0_IRQHandler(void)
	{

		if(Chip_TIMER_MatchPending(LPC_TIMER0, 0))
		{

			Chip_TIMER_ClearMatch(LPC_TIMER0, 0);
			flagTimer = true;

		}

	}
}

/*
 * Libreria en proceso. Solo utilizar función delay
 */

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
	uint32_t aux = SystemCoreClock/1000; //Se divide por 1000 para que sea milisegundos
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


bool delayMicroseconds ( long timeUS )
{
	bool rtrn = false;
	uint32_t aux = SystemCoreClock/1000000; //Se divide por 1000 para que sea milisegundos
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

uint32_t millis ( void )
{
	/*
	 * TC se incrementa en 1 por cada 1/SystemCoreClock.
	 * Entonces si en una vuelta paso 1/SystemCoreClock, en TC vueltas, cuantas pasaron?
	 * El multiplicador x1000 esta para que el tiempo se devuelva en milisegundos
	 * Se resta 1 como compensación del registro TC + 1
	 */
	uint32_t h = LPC_TIMER0->TCR;
	if (h != 1)
	{
		//En nuestro caso usare el timer 0 para realizar el delay y ese sera el que procedere a resetear
		LPC_TIMER0->TCR = 0x02;
		//Seteamos el preescaler en 0
		LPC_TIMER0->PR  = 0x00;
		LPC_TIMER0->TCR = 0x01;
	}
	uint32_t var = (LPC_TIMER0->TC)/(SystemCoreClock / (1000-1));
	return var;

}

double millisMicroseconds ( void )
{
	/*
	 * TC se incrementa en 1 por cada 1/SystemCoreClock.
	 * Entonces si en una vuelta paso 1/SystemCoreClock, en TC vueltas, cuantas pasaron?
	 * El multiplicador x1000 esta para que el tiempo se devuelva en milisegundos
	 * Se resta 1 como compensación del registro TC + 1
	 */
	uint32_t h = LPC_TIMER0->TCR;
	if (h != 1)
	{
		//En nuestro caso usare el timer 0 para realizar el delay y ese sera el que procedere a resetear
		LPC_TIMER0->TCR = 0x02;
		//Seteamos el preescaler en 0
		LPC_TIMER0->PR  = 0x00;
		LPC_TIMER0->TCR = 0x01;
	}
	return ((LPC_TIMER0->TC)/(SystemCoreClock / 1000000-1));
}

