#include "coco_mpu6050.h"
#include "coco_wire.h"
#include "math.h"

Wire WireLib;

/*
 * Función:			void MPU6050::WriteReg ( uint8_t reg, uint8_t value )
 *
 * Uso:				Permite escribir 1 byte en un registro mediante I2C
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-reg: Registro que se quiere escribir.
 * 					-value: Valor de 1 byte que se desea escribir en el registro.
 *
 */

void MPU6050::WriteReg ( uint8_t reg, uint8_t value )
{
	uint8_t auxiliar[2];

	auxiliar[0] = reg;
	auxiliar[1] = value;

	WireLib.write(auxiliar,2);
}

/*
 * Función:			uint8_t MPU6050::ReadReg ( uint8_t address, uint8_t reg )
 *
 * Uso:				Permite leer 1 byte en un registro mediante I2C
 *
 * Return:			Devuelve 1 byte correspondiente al registro que se desea leer.
 *
 * Parámetros:		-address: Address del modulo que se desea leer.
 * 					-reg: Registro de 1 byte que se desea leer.
 *
 */

uint8_t MPU6050::ReadReg ( uint8_t address, uint8_t reg )
{
	WireLib.write(reg);
	WireLib.requestFrom(address, 1);
	uint8_t l = WireLib.read();

	return l;
}

/*
 * Función:			MPU6050::MPU6050 ( uint8_t setEscalaGyro, uint8_t setEscalaAcc, uint8_t setConfigPwr, float *fGyroX, float *fGyroY, float *fGyroZ, float *fAccX, float *fAccY, float *fAccZ )
 *
 * Uso:				Constructor que permite inicializar las variables donde se almacenara la informacion y provee de la configuracion requerida.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-setEscalaGyro: Escala con la que trabajará el giroscopo.
 * 					-setEscalaAcc: Escala con la que trabajará el acelerometro.
 * 					-setConfigPwr: Configuración del modo de trabajo.
 * 					-*fGyroX/Y/Z: Punteros que apuntan a la posición de memoria donde se almacenará la información del giróscopo escalado.
 * 					-*fAccX/Y/Z: Punteros que apuntan a la posición de memoria donde se almacenará la información del acelerometro escalado.
 *
 */

MPU6050::MPU6050 ( uint8_t setEscalaGyro, uint8_t setEscalaAcc, uint8_t setConfigPwr, float *fGyroX, float *fGyroY, float *fGyroZ, float *fAccX, float *fAccY, float *fAccZ )
{
	EscalaGyro = setEscalaGyro;
	EscalaAcc = setEscalaAcc;
	ConfigPwr = setConfigPwr;

	GyroX = fGyroX;
	GyroY = fGyroY;
	GyroZ = fGyroZ;

	AccX = fAccX;
	AccY = fAccY;
	AccZ = fAccZ;

	WireLib.begin();

	WireLib.beginTransmission(ADDRESS_MPU6050);

	WriteReg(CONFIGURACION_GIROSCOPO, EscalaGyro);
	WriteReg(CONFIGURACION_ACELEROMETRO, EscalaAcc);
	WriteReg(PWR_MGMT_1, ConfigPwr);
	WriteReg(0x1A, 0x03);
	WriteReg(0x37, 0x02);
}

/*
 * Función:			void MPU6050::ComputeGyro ( void )
 *
 * Uso:				Permite leer el giroscopo en sus tres ejes y escalar la lectura. Ademas, corrige el offset.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void MPU6050::ComputeGyro ( void )
{
	int16_t xvalue, yvalue, zvalue;
	uint8_t xmsb, xlsb, ymsb, ylsb, zmsb, zlsb;

	WireLib.beginTransmission(ADDRESS_MPU6050);

	//Leemos los tres ejes. El MSB se corre 8 bits hacia la izquierda y se hace una OR con el LSB

	xmsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_X_MSB);
	xlsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_X_LSB);
	xvalue = xmsb <<8 | xlsb;

	ymsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Y_MSB);
	ylsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Y_LSB);
	yvalue = ymsb <<8 | ylsb;

	zmsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Z_MSB);
	zlsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Z_LSB);
	zvalue = zmsb <<8 | zlsb;

	//Escalamos segun lo configurado.

	if ( EscalaGyro == GIROSCOPO_ESCALA_250 )
	{
		GyroXL = xvalue / GIROSCOPO_SENSIBILIDAD_1;
		GyroYL = yvalue / GIROSCOPO_SENSIBILIDAD_1;
		GyroZL = zvalue / GIROSCOPO_SENSIBILIDAD_1;
	}

	if ( EscalaGyro == GIROSCOPO_ESCALA_500 )
	{
		GyroXL = xvalue / GIROSCOPO_SENSIBILIDAD_2;
		GyroYL = yvalue / GIROSCOPO_SENSIBILIDAD_2;
		GyroZL = zvalue / GIROSCOPO_SENSIBILIDAD_2;
	}

	if ( EscalaGyro == GIROSCOPO_ESCALA_1000 )
	{
		GyroXL = xvalue / GIROSCOPO_SENSIBILIDAD_3;
		GyroYL = yvalue / GIROSCOPO_SENSIBILIDAD_3;
		GyroZL = zvalue / GIROSCOPO_SENSIBILIDAD_3;
	}

	if ( EscalaGyro == GIROSCOPO_ESCALA_2000 )
	{
		GyroXL = xvalue / GIROSCOPO_SENSIBILIDAD_4;
		GyroYL = yvalue / GIROSCOPO_SENSIBILIDAD_4;
		GyroZL = zvalue / GIROSCOPO_SENSIBILIDAD_4;
	}

	//Correjimos offset

	GyroXL = GyroXL - GyroX_offset;
	GyroYL = GyroYL - GyroY_offset;
}

/*
 * Función:			void MPU6050::ComputeAcc ( void )
 *
 * Uso:				Permite leer el acelerometro en sus tres ejes y escalar la lectura. Ademas, corrige el offset.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void MPU6050::ComputeAcc ( void )
{
	int16_t xvalue, yvalue, zvalue;
	uint8_t xmsb, xlsb, ymsb, ylsb, zmsb, zlsb;

	WireLib.beginTransmission(ADDRESS_MPU6050);

	xmsb = ReadReg(ADDRESS_MPU6050, ACELEROMETRO_X_MSB);
	xlsb = ReadReg(ADDRESS_MPU6050, ACELEROMETRO_X_MSB);
	xvalue = xmsb <<8 | xlsb;

	ymsb = ReadReg(ADDRESS_MPU6050, ACELEROMETRO_Y_MSB);
	ylsb = ReadReg(ADDRESS_MPU6050, ACELEROMETRO_Y_MSB);
	yvalue = ymsb <<8 | ylsb;

	zmsb = ReadReg(ADDRESS_MPU6050, ACELEROMETRO_Z_MSB);
	zlsb = ReadReg(ADDRESS_MPU6050, ACELEROMETRO_Z_MSB);
	zvalue = zmsb <<8 | zlsb;

	if (EscalaAcc == ACELEROMETRO_ESCALA_1)
	{
		*AccX = xvalue / ACELEROMETRO_SENSIBILIDAD_1;
		*AccY = yvalue / ACELEROMETRO_SENSIBILIDAD_1;
		*AccZ = zvalue / ACELEROMETRO_SENSIBILIDAD_1;
	}

	if (EscalaAcc == ACELEROMETRO_ESCALA_2)
	{
		*AccX = xvalue / ACELEROMETRO_SENSIBILIDAD_2;
		*AccY = yvalue / ACELEROMETRO_SENSIBILIDAD_2;
		*AccZ = zvalue / ACELEROMETRO_SENSIBILIDAD_2;
	}

	if (EscalaAcc == ACELEROMETRO_ESCALA_3)
	{
		*AccX = xvalue / ACELEROMETRO_SENSIBILIDAD_3;
		*AccY = yvalue / ACELEROMETRO_SENSIBILIDAD_3;
		*AccZ = zvalue / ACELEROMETRO_SENSIBILIDAD_3;
	}

	if (EscalaAcc == ACELEROMETRO_ESCALA_4)
	{
		*AccX = xvalue / ACELEROMETRO_SENSIBILIDAD_4;
		*AccY = yvalue / ACELEROMETRO_SENSIBILIDAD_4;
		*AccZ = zvalue / ACELEROMETRO_SENSIBILIDAD_4;
	}
	*AccX = *AccX - AccX_offset;
	*AccY = *AccY - AccY_offset;
}

/*
 * Función:			void MPU6050::CalibrateActitude ( void )
 *
 * Uso:				Permite obtener valores de offset cuando la aeronave se encuentre en su posicion de despegue.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void MPU6050::CalibrateActitude ( void )
{
	float AccX_offset_temporal = 0.00, AccY_offset_temporal = 0.00;
	float GyroX_offset_temporal = 0.00, GyroY_offset_temporal = 0.00;

	float numberSamples = 1000.00;

	for ( int m = 0; m < numberSamples; m++ )
	{
		ComputeGyro(); //Leemos giroscopo
		ComputeAcc(); //Leemos acelerometro

		//Acumulamos las lecturas realizadas en X e Y
		AccX_offset_temporal = *AccX + AccX_offset_temporal;
		AccY_offset_temporal = *AccY + AccY_offset_temporal;
		GyroX_offset_temporal = *GyroX + GyroX_offset_temporal;
		GyroY_offset_temporal = *GyroY + GyroY_offset_temporal;
	}

	//Calculamos el promedio de esas lecturas.

	AccX_offset = AccX_offset_temporal / numberSamples;
	AccY_offset = AccY_offset_temporal / numberSamples;
	GyroX_offset = GyroX_offset_temporal / numberSamples;
	GyroY_offset = GyroY_offset_temporal / numberSamples;
}

/*
 * Función:			int16_t MPU6050::ComputeTemp ( void )
 *
 * Uso:				Permite obtener la temperatura ambiente.
 *
 * Return:			Devuelve 16 bits correspondiente a la temperatura leida y calculada segun Datasheet.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

int16_t MPU6050::ComputeTemp ( void )
{
	WireLib.beginTransmission(ADDRESS_MPU6050);

	//Leemos los 16 bits correspondientes a la información de temperatura.

	uint8_t temp_msb = ReadReg(ADDRESS_MPU6050, TEMPERATURA_MSB);
	uint8_t temp_lsb = ReadReg(ADDRESS_MPU6050, TEMPERATURA_LSB);

	int16_t temp_read = temp_msb << 8 | temp_lsb;

	//Calculamos temperatura según indica datasheet.

	int16_t temp = ( temp_read / 340 ) + 36.53;

	return temp;
}

/*
 * Función:			void MPU6050::setFilterComp ( float *AngX, float *AngY, float *AngZ )
 *
 * Uso:				Permite apuntar variables de la libreria a las indicadas por el usuario para almacenar el angulo de Roll y Pitch.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-*AngX/Y/Z: Punteros que apuntan a variables provistas por el usuario.
 *
 */

void MPU6050::setFilterComp ( float *AngX, float *AngY, float *AngZ )
{
	fcAngX = AngX;
	fcAngY = AngY;
	fcAngZ = AngZ;

	*fcAngX = 0;
	*fcAngY = 0;
	*fcAngZ = 0;
}

/*
 * Función:			void MPU6050::ComputeFilterComp ( void )
 *
 * Uso:				Ejecuta el calculo del filtro complementario. Posee dos filtros, uno para el giróscopo y otro para el ángulo. Esto reducirá ruido.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void MPU6050::ComputeFilterComp ( void )
{
	float accDenX = 0.00;
	float accDenY = 0.00;

	float accAngX = 0.00;
	float accAngY = 0.00;

	uint32_t elapsedTime = 0.0006; //En segundos

	//Filtro complementario para giroscopo. Complemento 70/30.

	*GyroX = (*GyroX * 0.7) + (( GyroXL )*0.3);
	*GyroY = (*GyroY * 0.7) + (( GyroYL )*0.3);

	//Calculo angular segun el giroscopo

	angGyroX = angGyroX + (*GyroX) * elapsedTime;
	angGyroY = angGyroY + (*GyroY) * elapsedTime;

	//Calculo angular segun acelerometro

	accDenX= sqrt( pow(*AccX,2) + pow(*AccZ,2) );
	accDenY= sqrt( pow(*AccY,2) + pow(*AccZ,2) );

	accAngY = atan( *AccX / accDenY ) * (180 / 3.1415926);
	accAngX = atan( *AccY / accDenX ) * (180 / 3.1415926);

	//Filtro complementario para el angulo. 98% del giroscopo y 2% del acelerometro.

	*fcAngX = 0.98 * (*fcAngX + angGyroX ) + 0.02 * accAngX;
	*fcAngY = 0.98 * (*fcAngY + angGyroY ) + 0.02 * accAngY;

}
