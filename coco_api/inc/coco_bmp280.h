#ifndef __COCO_BMP280_H_
#define __COCO_BMP280_H_

#include "chip.h"
#include "coco_wire.h"

//REGISTROS DEL BMP280
#define ADDRESS_BMP280 0x76 //READ-ONLY
#define TEMPERATURA_xLSB_BMP280 0xFC //READ-ONLY
#define TEMPERATURA_LSB_BMP280 0xFB //READ-ONLY
#define TEMPERATURA_MSB_BMP280 0xFA //READ-ONLY
#define PRESION_xLSB_BMP280 0xF9 //READ-ONLY
#define PRESION_LSB_BMP280 0xF8 //READ-ONLY
#define PRESION_MSB_BMP280 0xF7 //READ-ONLY
#define CONFIG_BMP280 0xF5 //READ-WRITE
#define CTRLMEAS_BMP280 0xF4 //READ-WRITE
#define STATUS_BMP280 0xF3 //DO-NOT-WRITE
#define RESET_BMP280 0xE0 //WRITE-ONLY
#define ID_BMP280 0xD0 //READ-ONLY
#define CAL25_BMP280 0xA1 //READ-ONLY
#define CAL0_BMP280 0x88 //0x88 + 25decimal = 0xA1. Contamos con 26 registros de calibracion. READ-ONLY

//VALORES DE CONFIGURACION
#define GORESET_BMP280 0xB6 //Configuración necesaria para hacer un Soft_Reset

#define OSRSP_OFF_BMP280 0b000 //Lectura de presión apagada
#define OSRSP_OSX1_BMP280 0b001 //Resolución de presión: 16bits/2.62Pa
#define OSRSP_OSX2_BMP280 0b010 //Resolución de presión: 17bits/1.31Pa
#define OSRSP_OSX4_BMP280 0b011 //Resolución de presión: 18bits/0.66Pa
#define OSRSP_OSX8_BMP280 0b100 //Resolución de presión: 19bits/0.33Pa
#define OSRSP_OSX16_BMP280 0b101 //Resolución de presión: 20bits/0.16Pa

#define OSRST_OFF_BMP280 0b000 //Lectura de temperatura apagada
#define OSRST_OSX1_BMP280 0b001 //Resolución de temperatura: 16bits/0.005°C
#define OSRST_OSX2_BMP280 0b010 //Resolución de temperatura: 17bits/0.0025°C
#define OSRST_OSX4_BMP280 0b011 //Resolución de temperatura: 18bits/0.00012°C
#define OSRST_OSX8_BMP280 0b100 //Resolución de temperatura: 19bits/0.0006°C
#define OSRST_OSX16_BMP280 0b101 //Resolución de temperatura: 20bits/0.0003°C

#define T_STDBY_05MS_BMP280 0b000
#define T_STDBY_625MS_BMP280 0b000
#define T_STDBY_125MS_BMP280 0b000
#define T_STDBY_250MS_BMP280 0b000
#define T_STDBY_500MS_BMP280 0b000
#define T_STDBY_101MS_BMP280 0b000
#define T_STDBY_110MS_BMP280 0b000
#define T_STDBY_111MS_BMP280 0b000

#define PWRMODE_SLEEP_BMP280 0b00
#define PWRMODE_FORCED_BMP280 0b01
#define PWRMODE_NORMAL_BMP280 0b11

#define IIRFILTER_OFF_BMP280 0b000 //Filtro apagado
#define IIRFILTER_X2_BMP280 0b001 //Coeficiente de filtrado x2
#define IIRFILTER_X4_BMP280 0b010 //Coeficiente de filtrado x4
#define IIRFILTER_X8_BMP280 0b011 //Coeficiente de filtrado x8
#define IIRFILTER_X16_BMP280 0b100 //Coeficiente de filtrado x16

//REGISTROS DE COMPENSACION
#define DIGT1_LSB_BMP280 0x88
#define DIGT1_MSB_BMP280 0x89
#define DIGT2_LSB_BMP280 0x8A
#define DIGT2_MSB_BMP280 0x8B
#define DIGT3_LSB_BMP280 0x8C
#define DIGT3_MSB_BMP280 0x8D
#define DIGP1_LSB_BMP280 0x8E
#define DIGP1_MSB_BMP280 0x8F
#define DIGP2_LSB_BMP280 0x90
#define DIGP2_MSB_BMP280 0x91
#define DIGP3_LSB_BMP280 0x92
#define DIGP3_MSB_BMP280 0x93
#define DIGP4_LSB_BMP280 0x94
#define DIGP4_MSB_BMP280 0x95
#define DIGP5_LSB_BMP280 0x96
#define DIGP5_MSB_BMP280 0x97
#define DIGP6_LSB_BMP280 0x98
#define DIGP6_MSB_BMP280 0x99
#define DIGP7_LSB_BMP280 0x9A
#define DIGP7_MSB_BMP280 0x9B
#define DIGP8_LSB_BMP280 0x9C
#define DIGP8_MSB_BMP280 0x9D
#define DIGP9_LSB_BMP280 0x9E
#define DIGP9_MSB_BMP280 0x9F

#define UP_MSB_BMP280 0xF7 //Registro MSB con los 8 bits de mayor peso para temperatura
#define UP_LSB_BMP280 0xF8 //Registro LSB con los 8 bits de medio peso para temperatura
#define UP_xLSB_BMP280 0xF9 //Registro xLSB con los 4 bits de menor peso para temperatura

#define UT_MSB_BMP280 0xFA //Registro MSB con los 8 bits de mayor peso para presion
#define UT_LSB_BMP280 0xFB //Registro LSB con los 8 bits de medio peso para presion
#define UT_xLSB_BMP280 0xFC //Registro xLSB con los 4 bits de menor peso para presion

#ifdef __cplusplus
extern "C" {
#endif

class BMP280
{
	public:
		void WriteReg ( uint8_t reg, uint8_t value );
		uint8_t ReadReg ( uint8_t address, uint8_t reg );
		void begin (void);
		void getMeasurements ( float *temperature, float *pressure, float *altitude );

		int16_t  digt2, digt3, digp2, digp3, digp4, digp5, digp6, digp7, digp8, digp9;
		uint16_t digt1, digp1;
		int32_t ut, up;
		float referencePressureBMP280 = 0.00;
		float altitudeBMP280;
};

#ifdef __cplusplus
}
#endif

#endif /* __COCO_BMP280_H_ */

