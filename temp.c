/*
 * temp.c
 *
 *  Created on: Oct 5, 2023
 *      Author: Andrea_Castellaneta
 */
#include "temp.h"
#include "adc.h"
#include <math.h>

/* NTC100k Thermistor Parameters for the Steinhart-Hart model */
#define A	0.0007707687291025694f	/* (float) */
#define B	0.0002042714066703426f	/* (float) */
#define C	0.0000001554826973743622f	/* (float) */

/* Initialize variables used for conversion */
uint16_t adc_val;
float R_ntc, adc_conv, I;;
float R1 = 100000.0;

/* Function to read the temperature from ADC1 */
uint16_t readTemperature(void)
{
	adc_val = HAL_ADC_GetValue(&hadc1);	/* Get the value read by the ADC */
	adc_conv = (float)adc_val * 3.3 / 4096.0;	/* Convert the value in a voltage in the range 0-3V3, according to the 12-bit resolution of the ADC */
	I = adc_conv/R1;	/* Calculate the current in the measurement circuit */
	R_ntc = (3.3-adc_conv)/I;	/* Evaluate the resistance value of the thermistor (approx to uint32_t, because always positive and to easily compare values with the sensor's conversion table) */
	/* Return the corresponding temperature value with the Steinhart-Hart model in °C */
	return (uint16_t) (1 / (A + B * log((float) R_ntc) + C * pow(log((float) R_ntc), 3)) - 273.15);
}
