/*
 * kmccu003_lab3_part1.c
 *
 * Created: 8/1/2018 2:02:42 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

enum States{Init, s0, s1} state;
	
unsigned char button = 0x00;
unsigned char tmpB = 0x00;

void Tick() {
	
	switch(state) { //transitions
		case Init: //automatically go to s0
			state = s0;
			break;
			
		case s0:
			if (button) { //button pressed
				state = s1;
			}
			else { //button not pressed stay in s1
				state = s0;
			}
			break;
		
		case s1:
			if (button) { //second press
				state = s0;
			}	
			else { //button not pressed stay in s1
				state = s1;
			}
			break;
			
			default:
			break;
	}
	
	switch(state) { //actions
		case Init:
			tmpB = 0x01; //bit 0
			PORTB = tmpB;
			break;
			
		case s0:
			tmpB = 0x01; //bit1
			PORTB = tmpB;
			break;
			
		case s1:
			tmpB = 0x02;
			PORTB = tmpB;
			break;
			
		default:
			break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	
	button = PINA & 0x01;
	tmpB = 0;
	state = Init;

	while(1) {
		void Tick();
	}
	
	return 0;
}
