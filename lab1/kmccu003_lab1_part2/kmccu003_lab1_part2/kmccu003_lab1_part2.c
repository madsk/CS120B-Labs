/*	  Author : Kiana McCullough
 * 
 *    Partner(s) Name & E-mail: Keilani Conner, kconn006@ucr.edu
 *    Lab Section: B21
 *    Assignment: Lab # 1 Exercise # 2
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; 
	DDRC = 0xFF; PORTC = 0x00; 
	unsigned char cntavail = 0;
	
    while (1) 
    {
		cntavail = 0;
		
		if(PINA & 0x01) { //bit 0
			cntavail += 1;
		}
		
		if(PINA & 0x02) { //bit 1
			cntavail += 1;
		}
		
		if(PINA & 0x04) { //bit 2
			cntavail += 1;
		}
		
		if(PINA & 0x08) { //bit 3
			cntavail += 1;
		}
		
		//cntavail -= 4;
		PORTC = 4-cntavail;
    }
}

/*Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is parked in the space,
of a four-space parking lot. Write a program that outputs in binary on port C the number of available
spaces (Hint: declare a variable "unsigned char cntavail"; you can assign a number to a port as follows:
PORTC = cntavail;).*/