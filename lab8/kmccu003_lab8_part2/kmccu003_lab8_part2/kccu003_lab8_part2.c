/*    Author : Kiana McCullough
 * 
 *    Partner(s) Name & E-mail: Keilani Conner, kconn006@ucr.edu
 *    Lab Section: B21
 *    Assignment: Lab # 8 Exercise # 2
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
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

int main(void)
{
	/* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; //input potentiometer
	DDRB = 0xFF; PORTB = 0x00; //LEDs [7:0]
	DDRD = 0xFF; PORTD = 0x00; //LEDs [1:0]
	
	unsigned char tmpB = 0x00;
	unsigned char tmpD = 0x00;
	
	ADC_init();
	
	while (1)
	{
		
		unsigned short adc = ADC; // Value of ADC register now stored in variable adc

		tmpB = (char)adc;
		tmpD = (char)(adc >> 8);
		
		PORTB = tmpB;
		PORTD = tmpD;
	}
}


/*Revise Part 1 by replacing the potentiometer with a photoresistor and 330
ohm resistor. Take note of the highest ADC value displayed when the photoresistor is exposed to light,
and the lowest ADC value displayed when the photoresistor is deprived of all light. These values will be used for the remaining lab exercises.*/
