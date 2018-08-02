/*
 * kmccu003_lab3_part1.c
 *
 * Created: 8/1/2018 2:02:42 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

enum States{INIT, s0, s1} state;
	
unsigned char button = 0x00;
unsigned char tmpB = 0x00;

void Tick() {
	
	switch(state) { //transitions
		case INIT: //automatically go to s0
			state = s0;
			break;
			
		case s0:
			state = (PINA & 0x01) ? s1 :state;
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
		case INIT:
			tmpB = 0x01; //bit 0
			//PORTB = tmpB;
			break;
			
		case s0:
			tmpB = 0x01; //bit1
			//PORTB = tmpB;
			break;
			
		case s1:
			tmpB = 0x02;
			//PORTB = tmpB;
			break;
			
		default:
			break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x01; // Configure port B's 8 pins as outputs
	
	
	tmpB = 0;
	state = Init;

	while(1) {
		button = PINA & 0x01;
		void Tick();
		PORTB = tmpB;
	}
	
	return 0;
}

/*PB0 and PB1 each connect to an LED, and PB0's LED is initially on.
Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED,
staying that way after button release. Pressing the button again turns off PB1's LED and
turns on PB0's LED.*/