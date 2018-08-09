/*
 * kmccu003_lab6_part1.c
 *
 * Created: 8/9/2018 11:54:57 AM
 * Author : Kiana
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() { //timer/counter controller reg
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

//start
enum States{START, INIT, INC, DEC, RESET, PRESS, RELEASE} state;

unsigned char button_0 = 0x00;
unsigned char button_1 = 0x00;
unsigned char counter;

void Tick() {
	
	button_0 = ~PINA & 0x01; //inc
	button_1 = ~PINA & 0x02; //dec
	
	switch(state) { //transitions

		case START:
		state = INIT;
		break;
		
		case INIT:
		
		if(button_0 && !button_1) { //button 1 pressed
			state = INC;
		}
		
		else if(!button_0 && button_1) { //button 2 pressed
			state = DEC;
		}
		
		else if(button_0 && button_1) { //both pressed
			state = RESET;
		}
		else if(!button_0 && !button_1){ //stay
			state = INIT;
		}
		break;
		
		
		case INC:
		
		if(button_0 && button_1) { //both pressed
			state = RESET;
		}
		
		/*else {
			state = RELEASE;
		}*/
		
		else if(!button_0 && !button_1) {
			state = RELEASE;
		}
		
		else if(button_0 && !button_1) { //if still button_0 keep incrementing
			state = INC;
		}
		
		break;
		
		case DEC:
		
		if(button_0 && button_1) { //both pressed
			state = RESET;
		}
		
		/*else {
			state = RELEASE;
		}*/
		
		else if(!button_0 && !button_1) { //if released
			state = RELEASE;
		}
		
		else if(!button_0 && button_1) { //if still button keep decrementing
			state = DEC;
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
		
		case PRESS:
		if(button_0 && button_1) { //both pressed, stay
			state = RESET;
		}
		
		else if(!button_0 && button_1) { //dec
			state = DEC;
		}
		
		else if(button_0 && !button_1) { //inc
			state = INC;
		}
		else {
			state = PRESS;
		}
		break;
		
		case RELEASE:
		if(!button_0 && !button_1) { //released, wait for press
			state = PRESS;
		}
		
		else if(button_0 && button_1) { //reset
			state = RESET;
		}
		
		else {
			state = RELEASE;
		}
		
		default:
		break;
		
	} //end transitions
	
	switch(state) { //actions
		
		case INIT:
		break;
		LCD_Cursor(1); //positions the cursor on the LCD display
		LCD_WriteData(counter + '0');
		
		case INC:
		if(counter < 9) {
			counter += 1;
		}
		LCD_Cursor(1); //positions the cursor on the LCD display
		LCD_WriteData(counter + '0'); //Writes a char at the position the cursor is currently in
		break;
		
		case DEC:
		if(counter > 0) {
			counter -= 1;
		}
		LCD_Cursor(1); //positions the cursor on the LCD display
		LCD_WriteData(counter + '0');
		break;
		
		case RESET:
		counter = 0x00;
		LCD_Cursor(1); //positions the cursor on the LCD display
		LCD_WriteData(counter + '0');
		break;
		
		default:
		break;
	}
}

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
	
	state = INIT;
	//tmpC = 0x07;
	
	LCD_init(); //initialize LCD
	
	TimerSet(1000); //1 sec
	TimerOn();

	while(1) {
		Tick();
		while(!TimerFlag) {};
			TimerFlag = 0;
	}
	
	return 0;
}

//OLD
/*Buttons are connected to PA0 and PA1. Output for PORTC is initially 7.
Pressing PA0 increments PORTC once (stopping at 9). Pressing PA1 decrements PORTC once (stopping at 0).
If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0.*/

//NEW
/*Buttons are connected to PA0 and PA1. Output PORTC and PORTD drive the LCD display, initially displaying 0.
Pressing PA0 increments the display (stopping at 9). Pressing PA1 decrements the display (stopping at 0).
If both buttons are depressed (even if not initially simultaneously), the display resets to 0.
If a button is held, then the display continues to increment (or decrement) at a rate of once per second.
Use a synchronous state machine captured in C.

Note: The LCD display displays ASCII values so 9 is not the same as ‘9’. For
displaying numbers 0 thru 9, a quick conversion technique is to add the
character ‘0’ to the number:

LCD_WriteData( 9 ); // will display nothing on the LCD
LCD_WriteData( 9 + ‘0’ ); // will display 9 on the LCD*/

