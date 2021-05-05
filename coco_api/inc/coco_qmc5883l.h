#ifndef __COCO_QMC5883L_H_
#define __COCO_QMC5883L_H_

#include "chip.h"
#include "coco_wire.h"


#define ADDRESS_MAGNETOMETRO  		    	0x0D //QMC5883L
//REGISTROS DEL MAGNETOMETRO//
#define MAGNETOMETRO_SRPERIOD				0x0B
#define MAGNETOMETRO_CONTROLREGISTER		0x09
#define MAGNETOMETRO_RESET					11


//REGISTROS DE CONFIGURACION A UTILIZAR
#define MAGNETOMETRO_SRPERIOD_RECOMMENDED	0x01
#define MAGNETOMETRO_STANDBY			    0b00000000
#define MAGNETOMETRO_CONTINUOS				0b00000001

#define MAGNETOMETRO_ODR_10Hz        		0b00000000
#define MAGNETOMETRO_ODR_50Hz 		        0b00000100
#define MAGNETOMETRO_ODR_100Hz    		    0b00001000
#define MAGNETOMETRO_ODR_200Hz     		    0b00001100

#define MAGNETOMETRO_RNG_2G      		    0b00000000
#define MAGNETOMETRO_RNG_8G     		    0b00010000

#define MAGNETOMETRO_OSR_512      		    0b00000000
#define MAGNETOMETRO_OSR_256         		0b01000000
#define MAGNETOMETRO_OSR_128         		0b10000000
#define MAGNETOMETRO_OSR_64          		0b11000000

//REGISTROS DE LECTURA DEL MAGNETOMETRO//
#define MAGNETOMETRO_X_LSB 					0x00
#define MAGNETOMETRO_X_MSB 					0x01
#define MAGNETOMETRO_Y_LSB 					0x02
#define MAGNETOMETRO_Y_MSB 					0x03
#define MAGNETOMETRO_Z_LSB 					0x04
#define MAGNETOMETRO_Z_MSB 					0x05



#ifdef __cplusplus
extern "C" {
#endif

class QMC5883L
{
	public:
		QMC5883L ( uint8_t setMode, int16_t *setMagX, int16_t *setMagY, int16_t *setMagZ, float *setHeading );
		void WriteReg ( uint8_t reg, uint8_t value );
		uint8_t ReadReg ( uint8_t address, uint8_t reg );
		void CalibrateMag ( void );
		void ReadMag ( void );
		void ComputeMag ( float gyroX, float gyroY );
		int16_t XOffset=0, YOffset=0, ZOffset=0;
		float XScale=1.00, YScale=1.00, ZScale=1.00;

	private:
		int16_t *MagX, *MagY, *MagZ;
		float *Heading=0;
		float MagX_correction=0.00, MagY_correction=0.00;
};

#ifdef __cplusplus
}
#endif

#endif /* __COCO_QMC5883L_H_ */

