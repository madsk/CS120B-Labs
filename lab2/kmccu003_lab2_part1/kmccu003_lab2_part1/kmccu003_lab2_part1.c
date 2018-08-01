/*    Author : Kiana McCullough
 * 
 *    Partner(s) Name & E-mail: Keilani Conner, kconn006@ucr.edu
 *    Lab Section: B21
 *    Assignment: Lab # 2 Exercise # 1
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
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	// initialize to 0s
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
	unsigned char tmpA = 0x00;
	unsigned char count = 0x00;
	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpA = PINA; // Mask PINA to only get the bit you are interested in
		tmpB = PINB;
		
		count = 0; //reset
		// 2) Perform Computation
		
		for(unsigned char i = 0; i < 8; i++) {
			if (GetBit(tmpA, i)) { // add up 1's in A, get each bit up to i
				count++;
			}
		}
		
		for(unsigned char i = 0; i < 8; i++) {
			if (GetBit(tmpB, i)) { // add up 1's in B
				count++;
			}
		}
		
		// 3) write results to port
		PORTC = count; //result of 1's in A & B
	}
}

//Count the number of 1s on ports A and B and output that number on port C.