/*    Author : Kiana McCullough
 *    Lab Section: B21
 *    Assignment: Lab #2 Exercise #5
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFE; PORTB = 0x01; // Configure port B's (B0 is input, B1-B7 are outputs)
	DDRD = 0x00; PORTD = 0xFF; // Configure port D's 8 pins as inputs
	
	unsigned char tmpB = 0x00;
	unsigned char tmpD = 0x00;
	unsigned char weight = 0x00;
	
    while (1) 
    {
		tmpB = PINB & 0x01; //0x01 is bit 0
		tmpD = PIND;
		weight = (PINB & 0x01) + PIND;
		
		if((tmpD >= 5 && tmpB == 0) || (tmpD > 79 && tmpB == 1)) {
			SetBit(tmpB, 2, 0);
			SetBit(tmpB, 1, 1); //enable airbag PB1 = 1
		}
		
		//if((weight >= 70) || (tmpD > 70 && tmpB == 1)) {
		if(weight >= 70) {
			SetBit(tmpB, 1, 0); //disable airbag
			SetBit(tmpB, 2, 1); //airbag disabled light on PB2 = 1
		}
		
		if ((tmpD <= 5 && tmpB == 0) || (tmpD < 5 && tmpB == 1)) {
			SetBit(tmpB, 1, 0); //both off
			SetBit(tmpB, 2, 0); 
		}
		
		PORTB = tmpB;
		//PORTD = tmpD;
    }
}
/*(Challenge): A car's passenger-seat weight sensor outputs a 9-bit value (ranging from 0 to 511)
and connects to input PD7..PD0PB0 on the microcontroller. If the weight is equal to or above 70 pounds,
the airbag should be enabled by setting PB1 to 1. If the weight is above 5 but below 70,
the airbag should be disabled and an "Airbag disabled" icon should light by setting PB2 to 1.
(Neither B1 nor B2 should be set if the weight is 5 or less, as there is no passenger).*/

/*Note: A port can be set to be input for some pins, and output for other pins by setting the
DDR appropriately, e.g. DDRA = 0xF0; will set port A to output on the high nibble,
and input on the low nibble.*/