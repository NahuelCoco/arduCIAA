#ifndef __COCO_H_
#define __COCO_H_

#include "chip.h"
#include "coco_adc.h"
#include "coco_aux.h"
#include "coco_gpio.h"
#include "coco_pwm.h"
#include "coco_timer.h"
#include "coco_uart.h"
#include "coco_wire.h"
#include "coco_spi.h"
#include "coco_pid.h"
#include "coco_mpu6050.h"
#include "coco_ak8963.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"

//UART
uart0 Serial;
uart1 Serial1;
uart2 Serial2;

//I2C
Wire Wire;

//SPI
SPI SPI;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Función:			void _coco ( void )
 *
 * Uso:				Inicializa parámetros y constructores al llamarla.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void _coco ( void )
{
	SystemCoreClockUpdate();

	initTimer();
    adc_init(0); // Inicializo el ADC con el uso del ADC0
    pwm_init(); //Inicializo el SCT para PWM.

}

#ifdef __cplusplus
}
#endif

#endif /* __COCO_H_ */

