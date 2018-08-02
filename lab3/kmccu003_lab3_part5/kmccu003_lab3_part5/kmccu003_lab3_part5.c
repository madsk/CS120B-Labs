/*
 * kmccu003_lab3_part5.c
 *
 * Created: 8/2/2018 1:50:57 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>


// Bit-access function

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {

	return (b ? x | (0x01 << k) : x & ~(0x01 << k));

}

unsigned char GetBit(unsigned char x, unsigned char k) {

	return ((x & (0x01 << k)) != 0);

}

enum States{INIT, LOCKED, PRESSED, RELEASED, UNLOCK} state;

unsigned char button_X = 0x00; //X
unsigned char button_Y = 0x00; //Y
unsigned char button_H = 0x00; //#
unsigned char button_lock = 0x00; //bit 7

unsigned char tmpB;
unsigned char tmpC;

void Tick() {
	
	button_X = PINA & 0x01; //bit 0
	button_Y = PINA & 0x02; //bit 1
	button_H = PINA & 0x04; //bit 2
	button_lock = PINA & 0x80; //bit 7
	tmpC = 0x00;
	
	switch(state) { //transitions
		
		case INIT: //goes straight to locked
			state = LOCKED;
		break;
		
		case LOCKED: //state 1
		if(button_H && !button_X && !button_Y && !button_lock) { //if # is pressed
			state = PRESSED;
		}
		
		else if (button_lock) { //if bit 7 is on
			state = LOCKED;
		}
		
		else { //stay
			state = LOCKED;
		}
		break;
		
		case PRESSED: //state 2
			if(button_H && !button_X && !button_Y && !button_lock) { //if # is pressed again, stay here
				state = PRESSED;
			}
		
			else if(!button_X && !button_Y && !button_H && !button_lock) { //go to release
				state = RELEASED;
			}
			
			else { //lock right away for wrong input
				state = LOCKED;
			}
		break;
		
		case RELEASED: //state 3
			if(!button_X && !button_Y && !button_H && !button_lock) { //stay if no button is pressed
				state = RELEASED;
			}
		
			else if(!(GetBit(tmpB, 0))){
				if(!button_X && button_Y && !button_H && !button_lock) { //if y is pressed and button is locked, open it!
					state = UNLOCK;
				}
			}
			
			else if(GetBit(tmpB, 0)) { //if it is unlocked, press y to unlock it
				if(!button_X && button_Y && !button_H && !button_lock) { //if y is pressed and button is unlocked
					state = LOCKED;
				}
			}
			
			else {
				state = LOCKED;
			}
		break;
		
		case UNLOCK: //state 4
			if(!button_X && !button_Y && !button_H && button_lock) { //if button lock is pressed from inside
				state = LOCKED;
			}
			else if(!button_X && !button_Y && button_H && !button_lock) {
				state = PRESSED;
			} //lock by using # and 7 again
			else {
				state = UNLOCK; //keep open if not locked from inside
			}
		break;
		
	} //end transitions
	
	switch(state) { //actions
		case INIT:
			tmpC = 0x00;
			PORTC = tmpC;
			break;
		
		case LOCKED: //wrong input automatically locks to 0
			
			tmpB = 0x00;
			tmpC = 0x01;
			PORTC = tmpC;
			break;
		
		case PRESSED:
			tmpC = 0x02;
			PORTC = tmpC;
			break;
		
		case RELEASED:
			tmpC = 0x03;
			PORTC = tmpC;
			break;
			
		case UNLOCK: //PB0 is 1 if unlocked
			tmpB = 0x01;
			tmpC = 0x04;
			PORTC = tmpC;
			break;
		
		default:
			break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	
	state = INIT; //initially locked

	while(1) {
		Tick();
		PORTB = tmpB;
	}
	
	return 0;
}

//4. (Challenge) Extend the above door so that it can also be locked by entering the earlier code.