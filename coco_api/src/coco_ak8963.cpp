#include "coco_ak8963.h"
#include "coco_wire.h"
#include "coco_timer.h"
#include "math.h"

Wire WireLibak;

void AK8963::WriteReg ( uint8_t reg, uint8_t value )
{
	uint8_t auxiliar[2];

	auxiliar[0] = reg;
	auxiliar[1] = value;

	WireLibak.write(auxiliar,2);
}

uint8_t AK8963::ReadReg ( uint8_t address, uint8_t reg )
{
	WireLibak.write(reg);
	WireLibak.requestFrom(address, 1);
	uint8_t l = WireLibak.read();

	return l;
}

AK8963::AK8963 ( uint8_t setMode, int16_t *setMagX, int16_t *setMagY, int16_t *setMagZ, float *setHeading )
{
	MagX = setMagX;
	MagY = setMagY;
	MagZ = setMagZ;
	Heading = setHeading;

	WireLibak.begin();

	WireLibak.beginTransmission(ADDRESS_MAGNETOMETRO);

	WriteReg(MAGNETOMETRO_SRPERIOD, MAGNETOMETRO_SRPERIOD_RECOMMENDED);
	WriteReg(MAGNETOMETRO_CONTROLREGISTER, setMode);

}

void AK8963::ReadMag ( void )
{
	int16_t xvalue, yvalue, zvalue;
	uint8_t xmsb, xlsb, ymsb, ylsb, zmsb, zlsb;

	WireLibak.beginTransmission(ADDRESS_MAGNETOMETRO);

	xmsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_X_MSB);
	xlsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_X_LSB);
	xvalue = xmsb << 8 | xlsb;

	ymsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Y_MSB);
	ylsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Y_LSB);
	yvalue = ymsb << 8 | ylsb;

	zmsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Z_MSB);
	zlsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Z_LSB);
	zvalue = zmsb << 8 | zlsb;

	*MagX = xvalue;
	*MagY = yvalue;
	*MagZ = zvalue;
}

void AK8963::ComputeMag ( float angX, float angY )
{
	float magxx, magyy;
	magxx = (*MagZ)*sin(angY*3.1415926/180)-(*MagY)*cos(angY*3.1415926/180);
	magyy = (*MagX)*cos(angX*3.1415926/180)+(*MagY)*sin(angX*3.1415926/180)*sin(angY*3.1415926/180)+(*MagZ)*sin(angX*3.1415926/180)*cos(angY*3.1415926/180);

	*Heading = atan2f( magxx, magyy ) * (180 / 3.1415926) - 9.7;

	//if ( *Heading < 0 ) *Heading += 360;
	//if ( *Heading >= 360 ) *Heading -= 360;
}
