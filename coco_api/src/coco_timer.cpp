#include "coco_timer.h"
#include "coco_gpio.h"

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


	bool val = false;
	//Los siguientes registros se pueden leer en el datasheet o desde la libreria chip.h
	//Leemos el valor de clock seteado en SystemCoreClock
	SystemCoreClockUpdate();
	//Reseteamos el Time Control Register del Timer 0
    LPC_TIMER0->TCR = 0x02;
    //Seteamos el preescaler en 0
    LPC_TIMER0->PR  = 0x00;
    //Seteamos el tiempo a esperar en función a los ciclos por segundos guardados en SystemCoreClock
    //Se divide por mil porque será en micro segundos
    //Este Match Value lo ponemos en la posición cero
    LPC_TIMER0->MR[0] = timeMS * (SystemCoreClock / 1000000-1);
    //Reseteamos todas las interrupciones de este timer
    LPC_TIMER0->IR  = 0xff;
    //Indicamos con que MR tiene que matchear para que el TCR cambie de valor
    LPC_TIMER0->MCR = 0x04;
    //Inicializamos el timer
    LPC_TIMER0->TCR = 0x01;

    //Esperamos a que TCR deje de ser 0x01 lo que significara que termina de contar
    //Esto pasara cuando el registro MR[0] sea igual a TC el cual se incrementa cada ciclo SystemCoreClock/1000-1
    while (LPC_TIMER0->TCR & 0x01);

    val = true;
    return val;

	/*
	double taux = millis();
	bool val = false;
	while(millis()-taux < timeMS);
	val = true;
	return val;*/
}


bool delayMicroseconds ( unsigned int timeUS )
{
	if(timeUS>0)
	{
	long time = (SystemCoreClock / (4*(10000000-1)));
	timeUS=(timeUS-1)*3;
	time = (time)*timeUS;

	long i,j;

	for(i=0;i<=4;i++);

	for(j=0;j<=timeUS;j++)
		for(i=0; i<=4;i++); //Tres bucles equivalen a 1uS
	}

    bool val = true;
    return val;
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

