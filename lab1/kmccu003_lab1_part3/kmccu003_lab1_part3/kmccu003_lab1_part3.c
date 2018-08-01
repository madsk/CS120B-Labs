/*    Author : Kiana McCullough
 * 
 *    Partner(s) Name & E-mail: Keilani Conner, kconn006@ucr.edu
 *    Lab Section: B21
 *    Assignment: Lab # 1 Exercise # 3
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
		
		PORTC = 4-cntavail;
		
		if (PORTC == 0x00) { //if full
			PORTC = (PORTC & 0xFF) | 0x80; //set bit 7 to 1
		}
	}

return 0;
}
/*Extend the previous program to still write the available spaces number, 
but only to PC3..PC0, and to set PC7 to 1 if the lot is full.*/
