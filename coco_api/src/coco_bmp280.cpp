#include "coco_bmp280.h"
#include "coco_wire.h"
#include "math.h"

Wire WireLibBMP;

/*
 * Función:			void BMP280::WriteReg ( uint8_t reg, uint8_t value )
 *
 * Uso:				Esta función será utilizada por las demás para el envio de 1 byte de datos por protocolo I2C.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-reg: Registro que se desea escribir.
 * 					-value: 1 byte de información a escribir en el registro "reg".
 *
 */

void BMP280::WriteReg ( uint8_t reg, uint8_t value )
{
	uint8_t auxiliar[2]; //Vector que almacenara el registro y el valor a escribir.

	auxiliar[0] = reg;
	auxiliar[1] = value;

	WireLibBMP.write(auxiliar,2); //Enviamos primero el registro a escribir y luego el valor
}

/*
 * Función:			uint8_t BMP280::ReadReg ( uint8_t address, uint8_t reg )
 *
 * Uso:				Esta función será utilizada por las demás para recibir 1 byte de datos por protocolo I2C.
 *
 * Return:			Devuelve 1 byte de información recibida por I2C.
 *
 * Parámetros:		-reg: Address del sensor a leer.
 * 					-reg: Registro que se desea leer.
 *
 */

uint8_t BMP280::ReadReg ( uint8_t address, uint8_t reg )
{
	WireLibBMP.write(reg); //Indicamos el registro a leer
	WireLibBMP.requestFrom(address, 1); //Esperamos 1 byte del sensor "Address"
	uint8_t l = WireLibBMP.read(); //El valor leido se almacena en "l"

	return l;
}

/*
 * Función:			void BMP280::begin ( void )
 *
 * Uso:				Esta función será utilizada por las demás para recibir 1 byte de datos por protocolo I2C.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		No requiere ningún parámetro.
 *
 */

void BMP280::begin ( void )
{
	float numberSamples = 200.00; //Se tomaran 200 muestras iniciales
	WireLibBMP.begin(); //Inicializamos I2C
	WireLibBMP.beginTransmission(ADDRESS_BMP280); //Comenzamos la transmision con el BMP280

	WriteReg(RESET_BMP280, GORESET_BMP280); //Hacemos un Soft_Reset del BMP280

	for(int i=0; i<100000;i++); //Espera

	//Configuramos el registro CONTROL MEASUREMENT
	/*
	 * Bit 7,6,5: osrs_t[2:0] Controla el sobre muestreo de datos de temperatura. Si colocamos 0b000 estaremos apagando medicion de temperatura.
	 * Bit 4,3,2: osrs_p[2:0] Controla el sobre muestreo de datos de presión. Si colocamos 0b000 estaremos apagando medicion de presion.
	 * Bit 1,0: mode[1:0] Controla el modo
	 */
	WriteReg(CTRLMEAS_BMP280, OSRST_OSX1_BMP280<<5 | OSRSP_OSX4_BMP280<<2 | PWRMODE_NORMAL_BMP280); //0b00101111

	for(int i=0; i<100000;i++); //Espera

	//Configuramos el registro CONFIG
	/*
	 * Bit 7,6,5: t_sb[2:0] - 0b000 Controla el tiempo de Standby.
	 * Bit 4,3,2: filter[2:0] - 0b100 Controla el filtro IIR.
	 * Bit 1: RESERVADO.
	 * Bit 0: spi3w_en[0] - 0b0 Habilita 3-wire SPI interface.
	 */
	WriteReg(CONFIG_BMP280, T_STDBY_05MS_BMP280<<5 | IIRFILTER_X16_BMP280<<2 | 0b0); //0b00010000

	for(int i=0; i<100000;i++); //Espera

	for (int j = 0; j < numberSamples; j++) //Tomamos las muestras
	{

	//Leemos los registros de calibracion

	uint8_t digt1_lsb = ReadReg(ADDRESS_BMP280, DIGT1_LSB_BMP280);
	uint8_t digt1_msb = ReadReg(ADDRESS_BMP280, DIGT1_MSB_BMP280);
	digt1 = digt1_msb << 8 | digt1_lsb;

	uint8_t digt2_lsb = ReadReg(ADDRESS_BMP280, DIGT2_LSB_BMP280);
	uint8_t digt2_msb = ReadReg(ADDRESS_BMP280, DIGT2_MSB_BMP280);
	digt2 = digt2_msb << 8 | digt2_lsb;

	uint8_t digt3_lsb = ReadReg(ADDRESS_BMP280, DIGT3_LSB_BMP280);
	uint8_t digt3_msb = ReadReg(ADDRESS_BMP280, DIGT3_MSB_BMP280);
	digt3 = digt3_msb << 8 | digt3_lsb;

	uint8_t digp1_lsb = ReadReg(ADDRESS_BMP280, DIGP1_LSB_BMP280);
	uint8_t digp1_msb = ReadReg(ADDRESS_BMP280, DIGP1_MSB_BMP280);
	digp1 = digp1_msb << 8 | digp1_lsb;

	uint8_t digp2_lsb = ReadReg(ADDRESS_BMP280, DIGP2_LSB_BMP280);
	uint8_t digp2_msb = ReadReg(ADDRESS_BMP280, DIGP2_MSB_BMP280);
	digp2 = digp2_msb << 8 | digp2_lsb;

	uint8_t digp3_lsb = ReadReg(ADDRESS_BMP280, DIGP3_LSB_BMP280);
	uint8_t digp3_msb = ReadReg(ADDRESS_BMP280, DIGP3_MSB_BMP280);
	digp3 = digp3_msb << 8 | digp3_lsb;

	uint8_t digp4_lsb = ReadReg(ADDRESS_BMP280, DIGP4_LSB_BMP280);
	uint8_t digp4_msb = ReadReg(ADDRESS_BMP280, DIGP4_MSB_BMP280);
	digp4 = digp4_msb << 8 | digp4_lsb;

	uint8_t digp5_lsb = ReadReg(ADDRESS_BMP280, DIGP5_LSB_BMP280);
	uint8_t digp5_msb = ReadReg(ADDRESS_BMP280, DIGP5_MSB_BMP280);
	digp5 = digp5_msb << 8 | digp5_lsb;

	uint8_t digp6_lsb = ReadReg(ADDRESS_BMP280, DIGP6_LSB_BMP280);
	uint8_t digp6_msb = ReadReg(ADDRESS_BMP280, DIGP6_MSB_BMP280);
	digp6 = digp6_msb << 8 | digp6_lsb;

	uint8_t digp7_lsb = ReadReg(ADDRESS_BMP280, DIGP7_LSB_BMP280);
	uint8_t digp7_msb = ReadReg(ADDRESS_BMP280, DIGP7_MSB_BMP280);
	digp7 = digp7_msb << 8 | digp7_lsb;

	uint8_t digp8_lsb = ReadReg(ADDRESS_BMP280, DIGP8_LSB_BMP280);
	uint8_t digp8_msb = ReadReg(ADDRESS_BMP280, DIGP8_MSB_BMP280);
	digp8 = digp8_msb << 8 | digp8_lsb;

	uint8_t digp9_lsb = ReadReg(ADDRESS_BMP280, DIGP9_LSB_BMP280);
	uint8_t digp9_msb = ReadReg(ADDRESS_BMP280, DIGP9_MSB_BMP280);
	digp9 = digp9_msb << 8 | digp9_lsb;

	for(int i=0; i<100;i++); //Espera

	//Leemos los registros de ADC_Temperatura

	uint8_t ut_xlsb = ReadReg(ADDRESS_BMP280, UT_xLSB_BMP280);
	uint8_t ut_lsb = ReadReg(ADDRESS_BMP280, UT_LSB_BMP280);
	uint8_t ut_msb = ReadReg(ADDRESS_BMP280, UT_MSB_BMP280);
	ut = ut_msb << 12 | ut_lsb << 4 | ut_xlsb >> 4;

	for(int i=0; i<100;i++); //Espera

	//Leemos los registros de ADC_Presion

	uint8_t up_xlsb = ReadReg(ADDRESS_BMP280, UP_xLSB_BMP280);
	uint8_t up_lsb = ReadReg(ADDRESS_BMP280, UP_LSB_BMP280);
	uint8_t up_msb = ReadReg(ADDRESS_BMP280, UP_MSB_BMP280);
	up = up_msb << 12 | up_lsb << 4 | up_xlsb >> 4;

	//Procedemos al calculo de la temperatura segun indica el Datasheet

	float var1 = ( (ut/16384.00) - (digt1/1024.00) ) * digt2;
	float var2 = ( (ut/131072.00) - (digt1/8192.00) ) * ( (ut/131072.00) - (digt1/8132.00) ) * digt3;
	float t_fine = var1 + var2;
	float temperatureBMP280 = t_fine/5120.00; //°C

	//Procedemos al calculo de la presion segun indica el Datasheet

	var1 = (t_fine/2.00) - 64000.00;
	var2 = var1 * var1 * digp6/32768.00;
	var2 = var2 + ((var1) * (digp5) * 2.00);
	var2 = (var2/4.00)+(digp4*65536.00);
	var1 = ((digp3 * var1 * var1/524288.00) + (digp2*var1))/524288.00;
	var1 = (1.00 + (var1/32768.00))*digp1;
	float p = 1048576.00 - up;
	p = (p-(var2/4096.00))*(6250.00/var1);
	var1 = digp9*p*p/2147483648.00;
	var2 = p*digp8/32768.00;
	float pressureBMP280_Pa = p + ((var1+var2+digp7)/16.00); //Pa

	referencePressureBMP280 = pressureBMP280_Pa/100.00 + referencePressureBMP280;
	}

	//Obtenemos la referencia de presion que permitira obtener la altura desde un punto relativo
	//Tambien se podria calcular desde el nivel del mar en un dia especifico
	referencePressureBMP280 = referencePressureBMP280 / numberSamples;
}

/*
 * Función:			void BMP280::getMeasurements ( float *temperature, float *pressure, float *altitude )
 *
 * Uso:				Esta función será utilizada por las demás para el envio de 1 byte de datos por protocolo I2C.
 *
 * Return:			No devuelve ningún parámetro.
 *
 * Parámetros:		-*temperature: Puntero que apunta a la variable donde se almacenara la temperatura.
 * 					-*pressure: Puntero que apunta a la variable donde se almacenara la presion.
 * 					-*altitude: Puntero que apunta a la variable donde se almacenara la altitud.
 *
 */

void BMP280::getMeasurements ( float *temperature, float *pressure, float *altitude )
{
	float pressureBMP280_Pa = 0;
	float temperatureBMP280 = 0;

	//Leemos los registros de calibracion

	uint8_t digt1_lsb = ReadReg(ADDRESS_BMP280, DIGT1_LSB_BMP280);
	uint8_t digt1_msb = ReadReg(ADDRESS_BMP280, DIGT1_MSB_BMP280);
	digt1 = digt1_msb << 8 | digt1_lsb;

	uint8_t digt2_lsb = ReadReg(ADDRESS_BMP280, DIGT2_LSB_BMP280);
	uint8_t digt2_msb = ReadReg(ADDRESS_BMP280, DIGT2_MSB_BMP280);
	digt2 = digt2_msb << 8 | digt2_lsb;

	uint8_t digt3_lsb = ReadReg(ADDRESS_BMP280, DIGT3_LSB_BMP280);
	uint8_t digt3_msb = ReadReg(ADDRESS_BMP280, DIGT3_MSB_BMP280);
	digt3 = digt3_msb << 8 | digt3_lsb;

	uint8_t digp1_lsb = ReadReg(ADDRESS_BMP280, DIGP1_LSB_BMP280);
	uint8_t digp1_msb = ReadReg(ADDRESS_BMP280, DIGP1_MSB_BMP280);
	digp1 = digp1_msb << 8 | digp1_lsb;

	uint8_t digp2_lsb = ReadReg(ADDRESS_BMP280, DIGP2_LSB_BMP280);
	uint8_t digp2_msb = ReadReg(ADDRESS_BMP280, DIGP2_MSB_BMP280);
	digp2 = digp2_msb << 8 | digp2_lsb;

	uint8_t digp3_lsb = ReadReg(ADDRESS_BMP280, DIGP3_LSB_BMP280);
	uint8_t digp3_msb = ReadReg(ADDRESS_BMP280, DIGP3_MSB_BMP280);
	digp3 = digp3_msb << 8 | digp3_lsb;

	uint8_t digp4_lsb = ReadReg(ADDRESS_BMP280, DIGP4_LSB_BMP280);
	uint8_t digp4_msb = ReadReg(ADDRESS_BMP280, DIGP4_MSB_BMP280);
	digp4 = digp4_msb << 8 | digp4_lsb;

	uint8_t digp5_lsb = ReadReg(ADDRESS_BMP280, DIGP5_LSB_BMP280);
	uint8_t digp5_msb = ReadReg(ADDRESS_BMP280, DIGP5_MSB_BMP280);
	digp5 = digp5_msb << 8 | digp5_lsb;

	uint8_t digp6_lsb = ReadReg(ADDRESS_BMP280, DIGP6_LSB_BMP280);
	uint8_t digp6_msb = ReadReg(ADDRESS_BMP280, DIGP6_MSB_BMP280);
	digp6 = digp6_msb << 8 | digp6_lsb;

	uint8_t digp7_lsb = ReadReg(ADDRESS_BMP280, DIGP7_LSB_BMP280);
	uint8_t digp7_msb = ReadReg(ADDRESS_BMP280, DIGP7_MSB_BMP280);
	digp7 = digp7_msb << 8 | digp7_lsb;

	uint8_t digp8_lsb = ReadReg(ADDRESS_BMP280, DIGP8_LSB_BMP280);
	uint8_t digp8_msb = ReadReg(ADDRESS_BMP280, DIGP8_MSB_BMP280);
	digp8 = digp8_msb << 8 | digp8_lsb;

	uint8_t digp9_lsb = ReadReg(ADDRESS_BMP280, DIGP9_LSB_BMP280);
	uint8_t digp9_msb = ReadReg(ADDRESS_BMP280, DIGP9_MSB_BMP280);
	digp9 = digp9_msb << 8 | digp9_lsb;


	for(int i=0; i<100;i++); //Espera

	//Leemos los registros de ADC_Temperatura

	uint8_t ut_xlsb = ReadReg(ADDRESS_BMP280, UT_xLSB_BMP280);
	uint8_t ut_lsb = ReadReg(ADDRESS_BMP280, UT_LSB_BMP280);
	uint8_t ut_msb = ReadReg(ADDRESS_BMP280, UT_MSB_BMP280);
	ut = ut_msb << 12 | ut_lsb << 4 | ut_xlsb >> 4;

	for(int i=0; i<100;i++); //Espera

	//Leemos los registros de ADC_Presion

	uint8_t up_xlsb = ReadReg(ADDRESS_BMP280, UP_xLSB_BMP280);
	uint8_t up_lsb = ReadReg(ADDRESS_BMP280, UP_LSB_BMP280);
	uint8_t up_msb = ReadReg(ADDRESS_BMP280, UP_MSB_BMP280);
	up = up_msb << 12 | up_lsb << 4 | up_xlsb >> 4;

	//Calculamos temperatura segun indica el datasheet

	float var1 = ( (ut/16384.00) - (digt1/1024.00) ) * digt2;
	float var2 = ( (ut/131072.00) - (digt1/8192.00) ) * ( (ut/131072.00) - (digt1/8132.00) ) * digt3;
	float t_fine = var1 + var2;
	temperatureBMP280 = t_fine/5120.00; //°C
	temperatureBMP280 = temperatureBMP280 - 4;

	//Calculamos presion segun indica el datasheet

	var1 = (t_fine/2.00) - 64000.00;
	var2 = var1 * var1 * digp6/32768.00;
	var2 = var2 + ((var1) * (digp5) * 2.00);
	var2 = (var2/4.00)+(digp4*65536.00);
	var1 = ((digp3 * var1 * var1/524288.00) + (digp2*var1))/524288.00;
	var1 = (1.00 + (var1/32768.00))*digp1;
	float p = 1048576.00 - up;
	p = (p-(var2/4096.00))*(6250.00/var1);
	var1 = digp9*p*p/2147483648.00;
	var2 = p*digp8/32768.00;

	pressureBMP280_Pa = (p + ((var1+var2+digp7)/16.00)); //Pa

	float pressureBMP280_hPa = pressureBMP280_Pa/100.00; //hPa

	//float altitudeBMP280_temporal = 44330 * (1 - powf(pressureBMP280_hPa/referencePressureBMP280,1/5.255));
	float altitudeBMP280_temporal = ( 1 - powf(pressureBMP280_hPa/referencePressureBMP280 , 1/5.257) )*(temperatureBMP280+273.15) / 0.0065;

	altitudeBMP280 = altitudeBMP280_temporal;

	if (altitudeBMP280 < 0)
		altitudeBMP280 = 0;

	*temperature = temperatureBMP280;
	*pressure = pressureBMP280_hPa;
	*altitude = altitudeBMP280;
}
