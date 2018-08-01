/*    Author : Kiana McCullough
 * 
 *    Partner(s) Name & E-mail: Keilani Conner, kconn006@ucr.edu
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
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	//DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	// initialize to 0s
	unsigned char tmpA = 0x00; // intermediate variable used for port updates
	unsigned char tmpC = 0x00;
	//unsigned char count = 0x00;
	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		tmpA = PINA & 0x0F; // Mask PINA to only get the bit you are interested in, first 4 bits
		tmpC = 0x00;
		
		// 2) Perform Computation

		
		if (tmpA <= 4) { //low fuel PC6
			tmpC = SetBit(tmpC, 6, 1);
		}
		
		else {
			tmpC = SetBit(tmpC, 6, 0); //turn off low fuel light if not less than 4
		}
		
		if (tmpA == 1 || tmpA == 2) {
			tmpC = SetBit(tmpC, 0, 1); //clear lsb, then set to 1
			//tmpC = 0x20;
		}
		
		if (tmpA == 3 || tmpA == 4) {
			//tmpC = SetBit(tmpC, 1, 1);
			tmpC = 0x30;
		}
		
		if (tmpA == 5 || tmpA == 6) {
			tmpC = 0x38;
		}
				
		if (tmpA == 7 || tmpA == 8 || tmpA == 9) {
			tmpC = 0x3C;
		}
						
		if (tmpA == 10 || tmpA == 11 || tmpA == 12) {
			tmpC = 0x3E;
		}
								
		if (tmpA == 13 || tmpA == 14 || tmpA == 15) {
			tmpC = 0x3F;
		}
		
		if (GetBit(PINA, 4) && GetBit(PINA, 5) && !GetBit(PINA, 6)) {
			tmpC = SetBit(tmpC, 7, 1); //seatbelt light
		}
		
		else { //turn off seatbelt light
			tmpC = SetBit(tmpC, 7, 0);
		}
		
		PORTC = tmpC;

		// 3) write results to port
		//PORTC = count; //result of 1's in A & B
	}
}

/*In addition to the above, PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated,
and PA6 is 1 if the driver's seatbelt is fastened. PC7 should light a "Fasten seatbelt" icon if a key
is in the ignition, the driver is seated, but the belt is not fastened.*/