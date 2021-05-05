#ifndef __COCO_MPU6050_H_
#define __COCO_MPU6050_H_

#include "chip.h"
#include "coco_wire.h"

//REGISTROS DEL MPU6050//
#define ADDRESS_MPU6050 0x68 //ADDRESS DEL MPU6050
#define CONFIGURACION_GIROSCOPO 0x1B //CONFIGURACION DEL GIROSCOPO
#define CONFIGURACION_ACELEROMETRO 0x1C //CONFIGURACION DEL ACELEROMETRO
#define PWR_MGMT_1 0x6B //CONFIGURACION, SLEEP=1 ENTRA EN MODO DE BAJO CONSUMO Y TOMA DATOS EN FUNCION A LA FRECUENCIA DE LP_WAKE_CTRL. TAMBIEN SE CONFIGURA EL CLK
#define CONFIGURACION 0x00 //SLEEP DESACTIVADO Y CLK INTERNO DE 8 MHz

//VALORES DE CONFIGURACION DEL GIROSCOPO//
#define GIROSCOPO_ESCALA_250 0x0 //Escala del giroscopo en +/-250 °/s
#define GIROSCOPO_ESCALA_500 0b00001000 //Escala del giroscopo en +/-500 °/s
#define GIROSCOPO_ESCALA_1000 0b00010000 //Escala del giroscopo en +/-1000°/s
#define GIROSCOPO_ESCALA_2000 0b00011000 //Escala del giroscopo en +/-2000°/s

#define GIROSCOPO_SENSIBILIDAD_1 131.00 // Sensibilidad LSB / °/s
#define GIROSCOPO_SENSIBILIDAD_2 65.50
#define GIROSCOPO_SENSIBILIDAD_3 32.80
#define GIROSCOPO_SENSIBILIDAD_4 16.40

//VALORES DE CONFIGURACION DEL ACELEROMETRO//
#define ACELEROMETRO_ESCALA_1 0x0 //Escala del giroscopo en +/-2 g
#define ACELEROMETRO_ESCALA_2 0b00001000 //Escala del giroscopo en +/-4 g
#define ACELEROMETRO_ESCALA_3 0b00010000 //Escala del giroscopo en +/-8 g
#define ACELEROMETRO_ESCALA_4 0b00011000 //Escala del giroscopo en +/-16 g

#define ACELEROMETRO_SENSIBILIDAD_1 16384.00 // Sensibilidad LSB / g
#define ACELEROMETRO_SENSIBILIDAD_2 8192.00
#define ACELEROMETRO_SENSIBILIDAD_3 4096.00
#define ACELEROMETRO_SENSIBILIDAD_4 2048.00

//REGISTROS DE TEMPERATURA//
#define TEMPERATURA_MSB 0x41
#define TEMPERATURA_LSB 0x42

//REGISTROS DEL ACELEROMETRO//
#define ACELEROMETRO_X_MSB 0x3B
#define ACELEROMETRO_X_LSB 0x3C
#define ACELEROMETRO_Y_MSB 0x3D
#define ACELEROMETRO_Y_LSB 0x3E
#define ACELEROMETRO_Z_MSB 0x3F
#define ACELEROMETRO_Z_LSB 0x40

//REGISTROS DEL GIROSCOPO//
#define GIROSCOPO_X_MSB 0x43
#define GIROSCOPO_X_LSB 0x44
#define GIROSCOPO_Y_MSB 0x45
#define GIROSCOPO_Y_LSB 0x46
#define GIROSCOPO_Z_MSB 0x47
#define GIROSCOPO_Z_LSB 0x48


#ifdef __cplusplus
extern "C" {
#endif

class MPU6050
{
	public:
		MPU6050 ( uint8_t setEscalaGyro, uint8_t setEscalaAcc, uint8_t setConfigPwr, float *fGyroX, float *fGyroY, float *fGyroZ, float *fAccX, float *fAccY, float *fAccZ );
		void WriteReg ( uint8_t reg, uint8_t value );
		uint8_t ReadReg ( uint8_t address, uint8_t reg );
		void ComputeGyro ( void );
		void ComputeAcc ( void );
		int16_t ComputeTemp ( void );
		void setFilterComp ( float *AngX, float *AngY, float *AngZ );
		void ComputeFilterComp ( void );
		void CalibrateActitude ( void );

	private:
		uint8_t EscalaGyro;
		uint8_t EscalaAcc;
		uint8_t ConfigPwr;

		float *GyroX, *GyroY, *GyroZ;
		float GyroXL, GyroYL, GyroZL;
		float *AccX, *AccY, *AccZ;

		float *fcAngX, *fcAngY, *fcAngZ;
		float AccX_offset = 0.00, AccY_offset = 0.00;
		float GyroX_offset = 0.00, GyroY_offset = 0.00;

		float angGyroX=0, angGyroY=0;
};

#ifdef __cplusplus
}
#endif

#endif /* __COCO_MPU6050_H_ */

