/*
 * kmccu003_lab8_part4.c
 *
 * Created: 8/15/2018 2:30:43 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion comple
}

	//unsigned char tmpB = 0x00;
	unsigned char tmpMAX = 0x00;
	unsigned long MAX = 0x327;
	
int main(void)
{
	/* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; //input potentiometer
	DDRB = 0xFF; PORTB = 0x00; //LEDs [7:0]
	DDRD = 0xFF; PORTD = 0x00; //LEDs [1:0]
	
	tmpMAX = MAX/8;
	
	ADC_init();
	
	while (1)
	{
		
		unsigned short adc = ADC; // Value of ADC register now stored in variable adc

		
		if(adc <= tmpMAX) {
			PORTB = 0x01;
		}
		else if (adc < tmpMAX) {
			PORTB = 0x02;
		}
	}
}

/*Design a system, using a bank of eight LEDs, where the number of LEDs illuminated is a representation of
how much light is detected. For example, when more light is detected, more LEDs are illuminated.

Criteria:
The LEDs should illuminated in sequence from 0 to 7, based on the amount of light detected by the photo resistor.

Hints:
Use the maximum ADC value observed from part 2 as the highest amount of light detectable by the photo resistor.
Divide that number by eight to determine the thresholds for each LED.*/
