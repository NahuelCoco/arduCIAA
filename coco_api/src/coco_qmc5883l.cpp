#include <coco_qmc5883l.h>
#include "coco_wire.h"
#include "coco_timer.h"
#include "math.h"

Wire WireLibak;

/*
 * Función:			void QMC5883L::WriteReg ( uint8_t reg, uint8_t value )
 *
 * Uso:				Permite escribir 1 byte en un registro mediante I2C
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-reg: Registro que se quiere escribir.
 * 					-value: Valor de 1 byte que se desea escribir en el registro.
 *
 */

void QMC5883L::WriteReg ( uint8_t reg, uint8_t value )
{
	uint8_t auxiliar[2];

	auxiliar[0] = reg;
	auxiliar[1] = value;

	WireLibak.write(auxiliar,2);
}

/*
 * Función:			uint8_t QMC5883L::ReadReg ( uint8_t address, uint8_t reg )
 *
 * Uso:				Permite leer 1 byte en un registro mediante I2C
 *
 * Return:			Devuelve 1 byte correspondiente al registro que se desea leer.
 *
 * Parámetros:		-address: Address del modulo que se desea leer.
 * 					-reg: Registro de 1 byte que se desea leer.
 *
 */

uint8_t QMC5883L::ReadReg ( uint8_t address, uint8_t reg )
{
	WireLibak.write(reg);
	WireLibak.requestFrom(address, 1);
	uint8_t l = WireLibak.read();

	return l;
}

/*
 * Función:			QMC5883L::QMC5883L ( uint8_t setMode, int16_t *setMagX, int16_t *setMagY, int16_t *setMagZ, float *setHeading )
 *
 * Uso:				Constructor que permite inicializar las variables donde se almacenara la informacion y provee de la configuracion requerida.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-setMode: Configuración requerida del magnetometro.
 * 					-*setMagX/Y/Z: Punteros que apuntan a la posición de memoria donde se almacenará la información del magnetómetro escalado.
 * 					-*setHeading: Puntero que apunta a la posición de memoria donde se almacenará la información del heading.
 *
 */

QMC5883L::QMC5883L ( uint8_t setMode, int16_t *setMagX, int16_t *setMagY, int16_t *setMagZ, float *setHeading )
{
	MagX = setMagX;
	MagY = setMagY;
	MagZ = setMagZ;
	Heading = setHeading;

	WireLibak.begin();

	WireLibak.beginTransmission(ADDRESS_MAGNETOMETRO);

	//Hacemos un soft_reset

	WriteReg(MAGNETOMETRO_RESET, 0x01);

	//Configuramos el magnetómetro

	WriteReg(MAGNETOMETRO_SRPERIOD, MAGNETOMETRO_SRPERIOD_RECOMMENDED);
	WriteReg(MAGNETOMETRO_CONTROLREGISTER, setMode);

}

/*
 * Función:			void QMC5883L::CalibrateMag ( void )
 *
 * Uso:				Permite obtener un valor de offset de manera de centrar las mediciones y obtener valores de heading fiables.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void QMC5883L::CalibrateMag ( void )
{

	int16_t xvalue, yvalue, zvalue;
	uint8_t xmsb, xlsb, ymsb, ylsb, zmsb, zlsb;
	int16_t xMinValue=0, xMaxValue=0, yMinValue=0, yMaxValue=0, zMinValue=0, zMaxValue=0;

	double long actualTime = millis();

	//Durante un minuto tomará muestras en los ejes X e Y de manera de obtener los mínimos y máximos.

	while(millis() - actualTime < 60000)
	{
		WireLibak.beginTransmission(ADDRESS_MAGNETOMETRO);

		xmsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_X_MSB);
		xlsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_X_LSB);
		xvalue = xmsb << 8 | xlsb;

		ymsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Y_MSB);
		ylsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Y_LSB);
		yvalue = ymsb << 8 | ylsb;

		zmsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Z_MSB);
		zlsb = ReadReg(ADDRESS_MAGNETOMETRO, MAGNETOMETRO_Z_LSB);
		zvalue = ymsb << 8 | ylsb;


		if (xvalue < xMinValue) xMinValue = xvalue;
		if (xvalue > xMaxValue) xMaxValue = xvalue;
		if (yvalue < yMinValue) yMinValue = yvalue;
		if (yvalue > yMaxValue) yMaxValue = yvalue;
		if (zvalue < zMinValue) zMinValue = zvalue;
		if (zvalue > zMaxValue) zMaxValue = zvalue;
	}

	//Se interpola y se obtiene un promedio que hará de offset para mitigar Hard Iron Effects

	XOffset = (xMaxValue + xMinValue) / 2;
	YOffset = (yMaxValue + yMinValue) / 2;
	ZOffset = (zMaxValue + zMinValue) / 2;

	//Se obtiene un promedio para calcular el escalar que mitigara Soft Iron Effects

	float averageOffset_X = (xMaxValue - xMinValue) / 2.00;
	float averageOffset_Y = (yMaxValue - yMinValue) / 2.00;
	float averageOffset_Z = (zMaxValue - zMinValue) / 2.00;

	float averageOffset = ( averageOffset_X + averageOffset_Y + averageOffset_Z ) / 3.00;

	XScale = averageOffset / averageOffset_X;
	YScale = averageOffset / averageOffset_Y;
	ZScale = averageOffset / averageOffset_Z;
}

/*
 * Función:			void QMC5883L::ReadMag ( void )
 *
 * Uso:				Permite leer los registros del magnetometro y corregir según el offset.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void QMC5883L::ReadMag ( void )
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

	*MagX = (xvalue - XOffset)*XScale;
	*MagY = (yvalue - YOffset)*YScale;
	*MagZ = (zvalue - ZOffset)*ZScale;
}

/*
 * Función:			void QMC5883L::ComputeMag ( float angX, float angY )
 *
 * Uso:				Permite calcular el heading corrigiendo según las inclinaciones de la computadora de vuelo.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-angX/Y: Valores de inclinación obtenidos del MPU6050 para corregir medición. Importante que coincidan los ejes.
 *
 */

void QMC5883L::ComputeMag ( float angX, float angY )
{
	float angX_rad, angY_rad;

	//Grados a radianes
	angX_rad = angX*3.1415926/180.00;
	angY_rad = angY*3.1415926/180.00;

	//Se compensa según el analisis: https://www.cypress.com/file/130456/download
	float cosAngX = cos(angX_rad);
	float cosAngY = cos(angY_rad);
	float sinAngX = sin(angX_rad);
	float sinAngY = sin(angY_rad);

	//Compensación de inclinación.
	float MagY_correction_temporal = (*MagY)*cosAngX - (*MagZ)*sinAngX;
	float MagX_correction_temporal = (*MagX)*cosAngY + (*MagY)*sinAngX*sinAngY + (*MagZ)*sinAngY*cosAngX;

	//Filtro complementario que permite hacer fluido los cambios en la lectura
	MagX_correction = MagX_correction*0.7 + MagX_correction_temporal*0.3;
	MagY_correction = MagY_correction*0.7 + MagY_correction_temporal*0.3;

	//Calculamos el heading y lo convertimos a grados
	*Heading = atan2f( MagY_correction, MagX_correction ) * (180 / 3.1415926);
	*Heading = *Heading + 9.3; //Declinacion Magnetica angular de Quilmes: 9.3°: https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml

	//Limitamos rango de trabajo entre 0° y 360°
	if (*Heading > 360.0) *Heading -= 360.0;
	if (*Heading < 0.0) *Heading += 360.0;

}
