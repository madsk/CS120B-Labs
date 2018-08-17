/*    Author : Kiana McCullough
 * 
 *    Partner(s) Name & E-mail: Keilani Conner, kconn006@ucr.edu
 *    Lab Section: B21
 *    Assignment: Lab # 8 Exercise # 1
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

/*Make sure your breadboard is wired according to the prelab. The potentiometer is used to adjust the voltage supplied to the microcontroller for ADC.
Design a system that reads the 10-bit ADC result from the ADC register, and displays the result on a bank of 10 LEDs.

Hints:
Assuming the breadboard has been wired according to the prelab photo, display the lower 8 bits on port B, and the upper 2 bits on port D.
Use a “short” variable to hold the ADC result.
Use bit shifting and casting to align the proper bits to the proper ports. For example:*/

           /*unsigned short my_short = 0xABCD;
            unsigned char my_char = (char)my_short; // my_char = 0xCD
            my_char = (char)(my_short >> 4); // my_char = 0xBC*/