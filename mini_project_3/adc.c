 /******************************************************************************
 *
 * Module: ADC
 *
 * File Name: adc.c
 *
 * Description: Source file for the ATmega16 ADC driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#include "avr/io.h" /* To use the ADC Registers */
#include "adc.h"
#include "common_macros.h" /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
ADC_ConfigType Config_1;
void ADC_init(const ADC_ConfigType * Config_Ptr)
{
	Config_Ptr = &Config_1;
	/* ADMUX Register Bits Description:
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 */

	switch(Config_Ptr -> ref_volt)
	{
		case (VCC):
				CLEAR_BIT(ADMUX,REFS0);
				CLEAR_BIT(ADMUX,REFS1);
				break;
		case(V_INTERNAL):
				SET_BIT(ADMUX,REFS0);
				SET_BIT(ADMUX,REFS1);
				break;
		case(AVCC):
				SET_BIT(ADMUX,REFS0);
				CLEAR_BIT(ADMUX,REFS1);
				break;
		default:
		CLEAR_BIT(ADMUX,REFS0);
		CLEAR_BIT(ADMUX,REFS1);
	}

	/* ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADATE   = 0 Disable Auto Trigger
	 * ADPS2:0 = 011 to choose ADC_Clock = F_CPU/8 = 1Mhz/8 = 125Khz --> ADC must operate in range 50-200Khz
	 */
	ADCSRA = 0;

	ADCSRA = (1<<ADEN);
	switch (Config_Ptr -> prescaler)
	{
	case (2):
			SET_BIT(ADCSRA, ADPS0);
		break;
	case (4):
			SET_BIT (ADCSRA, ADPS1);
		break;
	case (8):
			SET_BIT (ADCSRA, ADPS1);
			SET_BIT (ADCSRA, ADPS0);
		break;
	case (16):
			SET_BIT (ADCSRA, ADPS2);
		break;
	case (32):
			SET_BIT (ADCSRA, ADPS0);
			SET_BIT (ADCSRA, ADPS2);
		break;
	case (64):
			SET_BIT (ADCSRA, ADPS1);
			SET_BIT (ADCSRA, ADPS2);
		break;
	case (128):
			SET_BIT (ADCSRA, ADPS1);
			SET_BIT (ADCSRA, ADPS2);
			SET_BIT (ADCSRA, ADPS0);
		break;
	default:
		SET_BIT(ADCSRA, ADPS0);
	}

}

uint16 ADC_readChannel(uint8 channel_num)
{
	channel_num &= 0x07; /* Input channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* Clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* Choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* Start conversion write '1' to ADSC */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* Wait for conversion to complete, ADIF becomes '1' */
	SET_BIT(ADCSRA,ADIF); /* Clear ADIF by write '1' to it :) */
	return ADC; /* Read the digital value from the data register */
}
