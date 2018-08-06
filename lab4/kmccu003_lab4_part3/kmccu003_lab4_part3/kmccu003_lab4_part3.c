/*
 * kmccu003_lab4_part3.c
 *
 * Created: 8/6/2018 12:16:27 PM
 * Author : Kiana
 */ 

#include <avr/io.h>

enum States{START, INIT, INC, DEC, WAIT} state;

unsigned char button = 0x00;
unsigned char cnt = 0x00; //button presses
unsigned char tmpC;

void Tick() {
	
	button = ~PINA & 0x01; //input PA0

	switch(state) { //transitions

		case START: 
			state = INIT;
			break;
		
		case INIT:
			
			if(button) { //button is pressed
				state = INC;
			}

			else if(!button_0){ //stay
				state = INIT;
			}
			break;
		
		
		case INC:
			
			if(!button) {
				state = WAIT;
			}
			
			break;
			
		case DEC:
			
			if(!button) {
				state = WAIT;
			}
			
			break;
			
			case WAIT:
					
				if(cnt<6 && button) { //inc
					state = INC;
					cnt++;
				}
				
				else if(cnt>=6 && button) { //6 presses
					state = INIT;
				}

			break;
			
			default:
				break;
			
			
	} //end transitions
	
	
	switch(state) { //actions
			
		case INIT:
			cnt = 0; //reset
			break;
			
		case INC:
			break;
		
		default:
			break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
	
	state = INIT;

	while(1) {
		Tick();
		PORTC = tmpC;
	}
	
	return 0;
}

/*(Challenge) Create your own festive lights display with 6 LEDs connected to port PB5..PB0,
lighting in some attractive sequence. Pressing the button on PA0 changes the lights to the next configuration
in the sequence.  Use a state machine (not synchronous) captured in C.*/