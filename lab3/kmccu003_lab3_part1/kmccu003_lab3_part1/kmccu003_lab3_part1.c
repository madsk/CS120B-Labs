/*
 * kmccu003_lab3_part1.c
 *
 * Created: 8/1/2018 2:02:42 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

enum States{Init, s1, s2} state;
	
unsigned char button = 0;
unsigned char tmpB = 0;

void Tick {
	switch(state) { //transitions
		case Init:
			state = Init;
			break;
		case Init:
			state
		
		
		
		
		
	}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs

	while(1) {
		void Tick();
	}
}
