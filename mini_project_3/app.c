#include <avr/io.h>
#include "lm35_sensor.h"
#include "dc_motor.h"
#include "lcd.h"
#include "ADC.h"

static ADC_ConfigType Config_1;
static ADC_ConfigType* Ptr_Config_1;

void LCD_sendNumber(uint8 num){
	uint8 arr[20];
	uint8 num_digit = 0;

	while (num>0)
	{
		arr[num_digit] =num%10;
		num /= 10;
		num_digit++;
	}

	for (uint8 i = num_digit-1; i<num_digit; i--)
	{
		LCD_displayCharacter('0' + arr[i]);
	}
}


void fanOnDisplay (uint8 Temprature)
{
	LCD_moveCursor(0, 3);
	LCD_displayString ("FAN IS ON ");
	LCD_moveCursor(1, 3);
	LCD_displayString ("TEMP = ");
//	LCD_intgerToString (Temprature);
	LCD_sendNumber(Temprature);
	LCD_displayString (" ");
}
void fanOffDisplay (uint8 Temprature)
{
	LCD_moveCursor(0, 3);
	LCD_displayString ("FAN IS OFF");
	LCD_moveCursor(1, 3);
	LCD_displayString ("TEMP = ");
//	LCD_intgerToString (Temprature);
	LCD_sendNumber(Temprature);
}



int main (void)
{

	/*Setup ADC configuration*/
	Ptr_Config_1 = &Config_1;
	Ptr_Config_1 -> prescaler = 8;
	Ptr_Config_1 -> ref_volt = V_INTERNAL;


	/*driver initialization*/
	ADC_init(Ptr_Config_1);
	DcMotor_Init();
	LCD_init();


	uint8 Temprature;

	while (TRUE)
	{
		Temprature =LM35_getTemperature();

		if (Temprature < 30)
		{
			fanOffDisplay(Temprature);
			DcMotor_Rotate(STOP, HALT);
		}
		else if ((Temprature >= 30) && (Temprature < 60))
		{
			fanOnDisplay(Temprature);
			DcMotor_Rotate(CW, QUARTER_SPEED);
		}
		else if ((Temprature >= 60) && (Temprature < 90))
		{
			fanOnDisplay(Temprature);
			DcMotor_Rotate(CW, HALF_SPEED);
		}
		else if ((Temprature >= 90) && (Temprature < 120))
		{
			fanOnDisplay(Temprature);
			DcMotor_Rotate(CW, THREE_QUARTER_SPEED);
		}
		else
		{
			fanOnDisplay(Temprature);
			DcMotor_Rotate(CW, FULL_SPEED);
		}
	}

}





