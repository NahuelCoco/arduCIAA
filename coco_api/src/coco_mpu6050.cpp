#include "coco_mpu6050.h"
#include "coco_wire.h"
#include "math.h"

Wire WireLib;

void MPU6050::WriteReg ( uint8_t reg, uint8_t value )
{
	uint8_t auxiliar[2];

	auxiliar[0] = reg;
	auxiliar[1] = value;

	WireLib.write(auxiliar,2);
}

uint8_t MPU6050::ReadReg ( uint8_t address, uint8_t reg )
{
	WireLib.write(reg);
	WireLib.requestFrom(address, 1);
	uint8_t l = WireLib.read();

	return l;
}

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
	//delay(2);
	WriteReg(CONFIGURACION_ACELEROMETRO, EscalaAcc);
	//delay(2);
	WriteReg(PWR_MGMT_1, ConfigPwr);
	//delay(2);
	WriteReg(0x1A, 0x03);
	//delay(2);
	WriteReg(0x37, 0x02);
	//delay(2);
}

float MPU6050::ComputeGyro ( void )
{
	int16_t xvalue, yvalue, zvalue;
	uint8_t xmsb, xlsb, ymsb, ylsb, zmsb, zlsb;

	WireLib.beginTransmission(ADDRESS_MPU6050);

	xmsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_X_MSB);
	xlsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_X_LSB);
	xvalue = xmsb <<8 | xlsb;

	ymsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Y_MSB);
	ylsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Y_LSB);
	yvalue = ymsb <<8 | ylsb;

	zmsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Z_MSB);
	zlsb = ReadReg(ADDRESS_MPU6050, GIROSCOPO_Z_LSB);
	zvalue = zmsb <<8 | zlsb;

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
	return GyroXL;
}

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

}


int16_t MPU6050::ComputeTemp ( void )
{
	WireLib.beginTransmission(ADDRESS_MPU6050);

	uint8_t temp_msb = ReadReg(ADDRESS_MPU6050, TEMPERATURA_MSB);
	uint8_t temp_lsb = ReadReg(ADDRESS_MPU6050, TEMPERATURA_LSB);

	int16_t temp_read = temp_msb << 8 | temp_lsb;

	int16_t temp = ( temp_read / 340 ) + 36.53;

	return temp;
}

void MPU6050::setFilterComp ( float *AngX, float *AngY, float *AngZ )
{
	fcAngX = AngX;
	fcAngY = AngY;
	fcAngZ = AngZ;

	*fcAngX = 0;
	*fcAngY = 0;
	*fcAngZ = 0;
}

void MPU6050::ComputeFilterComp ( uint32_t tMedido )
{
	float accDenX = 0.00;
	float accDenY = 0.00;
	//float accDenZ = 0;

	float accAngX = 0.00;
	float accAngY = 0.00;
	//float accAngZ = 0;

	previousTime = currentTime;
	currentTime = tMedido;

	elapsedTime = 0.0006;//( currentTime - previousTime ) / 1000.00; //Para tenerlo en segundos

	*GyroX = (*GyroX * 0.7) + (( GyroXL )*0.3);
	*GyroY = (*GyroY * 0.7) + (( GyroYL )*0.3);

	angGyroX = angGyroX + (*GyroX) * elapsedTime;
	angGyroY = angGyroY + (*GyroY) * elapsedTime;

	accDenX= sqrt( pow(*AccX,2) + pow(*AccZ,2) );
	accDenY= sqrt( pow(*AccY,2) + pow(*AccZ,2) );

	accAngY = atan( *AccX / accDenY ) * (180 / 3.1415926);
	accAngX = atan( *AccY / accDenX ) * (180 / 3.1415926);

	*fcAngX = 0.98 * (*fcAngX + angGyroX) + 0.02 * accAngX;
	*fcAngY = 0.98 * (*fcAngY + angGyroY) + 0.02 * accAngY;
}
