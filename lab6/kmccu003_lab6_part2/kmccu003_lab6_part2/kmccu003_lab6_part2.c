/*
 * kmccu003_lab6_part2.c
 *
 * Created: 8/9/2018 12:51:13 PM
 * Author : Kiana
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"

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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum States {START, INIT, led_0, led_1, led_2, press_0, press_1, press_2, release_0, release_1, release_2, reset} state;

unsigned char tmpA = 0x00;
unsigned char score = 5;

void Tick() { //begin fct
	
	tmpA = ~PINA & 0x01; //use ~ for hardware testing
	
	switch(state) {
		
		case START:
			state = INIT;
			break;
		
		case INIT:
		if(tmpA == 0x00) {
			LCD_ClearScreen();
			//score = 5;
			LCD_Cursor(1); //positions the cursor on the LCD display
			LCD_WriteData(score + '0'); //display 5 on screen
			state = led_0;
		}
			break;
		
		case led_0:
			if(tmpA == 0x01) { //if button is pressed, have to pause game
				if(score > 0) {
					score -= 1;
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
				}
				state = press_0;
			}
			else {
				state = led_1; //move on to next
			}
			break;
		
		case led_1:
			if(tmpA == 0x01) { //if button is pressed, have to pause game
				if(score < 9) {
					score += 1;
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
				}
				state = press_1;
			}
			else{
				state = led_2;
			}
			break;
		
		case led_2:
			if(tmpA == 0x01) { //if button is pressed, have to pause game
				if(score > 0) {
					score -= 1;
					LCD_Cursor(1);
					LCD_WriteData(score + '0');
				}
				state = press_2;
			}
			else{
				state = led_0;
			}
			break;
		
		case press_0:
			if(tmpA == 0x01) { //wrong!
				state = press_0;
			}
			else {
				state = release_0;
			}
			break;
		
		case press_1:
			if(tmpA == 0x01) { //right!
				state = press_1;
			}
			else {
				state = release_1;
			}
			break;
		
		case press_2:
			if(tmpA == 0x01) { //wrong!
				state = press_2;
			}
			else {
				state = release_2;
			}
			break;
		
		case release_0:
			if(tmpA == 0x00) {
				state = release_0;
			}
			else {
				state = reset;
			}
			break;
		
		case release_1:
			if(tmpA == 0x00) {
				state = release_1;
			}
			else {
				state = reset;
			}
			break;
		
		case release_2:
			if(tmpA == 0x00) {
				state = release_2;
			}
			else {
				state = reset;
			}
			break;
		
		case reset:
			if(tmpA == 0x00) {
				state = INIT;
			}
			break;
	} //end transitions
	
	switch(state) { //actions
		
		case INIT:
			break;
		
		case led_0:
			PORTB = 0x01;
			break;
		
		case led_1:
			PORTB = 0x02;
			break;
			
		case led_2:
			PORTB = 0x04;
			break;
		
		case press_0: //decrement

			/*else if(score < 0) {
				LCD_DisplayString(1, "YOU LOSE!")
			}*/
			break;
		
		case press_1: //increment

			if(score == 9) {
				LCD_DisplayString(1, "YOU WON!");
			}
			break;
		
		case press_2: //decrement

			/*else if(score < 0) {
				LCD_DisplayString(1, "YOU LOSE!")
			}*/
			break;
		
		case release_0:
		PORTB = 0x01;
			break;
		
		case release_1:
		PORTB = 0x02;
			break;
		
		case release_2:
		PORTB = 0x04;
			break;
		
		case reset:
		//PORTC = 0x00;
		if(score == 9) {
			LCD_ClearScreen();
			score = 5;
		}
			break;

		default:
			break;
		
	}//end actions
}//end fct

int main(void) {
	
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // LED
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
	
	state = INIT;
	
	LCD_init(); //initialize LCD
	
	TimerSet(260); //1 sec
	TimerOn();

	while(1) {
		Tick();
		while(!TimerFlag) {};
			TimerFlag = 0;
	}
	
	return 0;
}

/*(Challenge) Extend the earlier light game to maintain a score on the LCD display.
The initial score is 5. Each time the user presses the button at the right time, the score increments.
Each time the user fails, the score decrements. When reaching 9, show victory somehow.*/