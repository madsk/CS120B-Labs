/*
 * kmccu003_lab3_part2.c
 *
 * Created: 8/1/2018 6:06:38 PM
 * Author : Kiana
 */ 

#include <avr/io.h>

enum States{START, INIT, INC, DEC, RESET} state;

unsigned char button_0 = 0x00;
unsigned char button_1 = 0x00;
unsigned char tmpC;

void Tick() {
	
	button_0 = PINA & 0x01; //inc
	button_1 = PINA & 0x02; //dec
	
	switch(state) { //transitions
		
		case START: //automatically go INIT
		state = INIT;
	break;
		
		case INIT:
			if(!button_0 && !button_1) { //stay
				state = INIT;
			}
			
			else if(button_0 && !button_1) { //button 1 pressed
				state = INC;
			}
			
			else if(!button_0 && button_1) { //button 2 pressed
				state = DEC;
			}
			
			else if(button_0 && button_1) { //both pressed
				state = RESET;
			}
		break;
		
		case INC:
			if(button_0 && button_1) { //both pressed
				state = RESET;
			}
			
			else if(!button_0 && !button_1) { //neither
				state = INIT;
			}
		
			else if(button_0 && !button_1) { //stay
				state = INC;
			}
			
			else if(!button_0 && button_1) { //go to dec
				state = DEC;
			}
		break;
			
		case DEC:
			if(button_0 && button_1) { //both pressed
				state = RESET;
			}
			
			else if(!button_0 && !button_1) { //neither
				state = INIT;
			}
			
			else if(!button_0 && button_1) { //stay
				state = DEC; 
			}
			
			else if(button_0 && !button_1) {
				state = INC;
			}
		break;
			
			case RESET:
				if(button_0 && button_1) { //both pressed, stay
					state = RESET;
				}
					
				else if(!button_0 && !button_1) { //neither
					state = INIT;
				}
					
				else if(!button_0 && button_1) { //dec
					state = DEC;
				}
				
				else if(button_0 && !button_1) { //inc
					state = INC;
				}
			break;
			
			
	} //end transitions
	
	
		
	
	switch(state) { //actions
		case START:
			break;
			
		case INIT:
			break;
		
		case INC:
			if(tmpC < 9) {
				tmpC += 1;
			}
			break;
		
		case DEC:
			if(tmpC > 0) {
				tmpC -= 1;
			}
			break;
			
		case RESET:
			tmpC = 0;
			break;
		
		default:
			break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
	
	state = INIT;
	tmpC = 0x07;

	while(1) {
		Tick();
		PORTC = tmpC;
	}
	
	return 0;
}

/*Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. 
Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0).
If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.*/