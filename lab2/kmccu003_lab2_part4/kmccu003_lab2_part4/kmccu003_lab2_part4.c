/*    Author : Kiana McCullough
 *    Lab Section: B21
 *    Assignment: Lab # 2 Exercise # 3
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
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	
	unsigned char tmpB = 0x00;
	unsigned char tmpC = 0x00;
	
    while (1) 
    {
		tmpB = 0x00;
		tmpC = 0x00;
		
		tmpB = (PINA & 0xF0) >> 4; //check only last 4 bits of A (7, 6, 5, 4)
		tmpC = (PINA & 0x0F) << 4; //check only first 4 bits of A (3, 2, 1, 0)
		
		PORTB = tmpB; //upper nibble of PINA
		PORTC = tmpC; //lower nibble of PINA
    }
}

/*(Challenge): Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4. 
That is to say,  take the upper nibble of PINA and map it to the lower nibble of PORTB,
likewise take the lower nibble of PINA and map it to the upper nibble of
PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).*/