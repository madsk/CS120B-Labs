/*
 * kmccu003_lab7_part1.c
 *
 * Created: 8/13/2018 11:52:51 AM
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

//threeLEDs
//enum blink{} B_LED;
enum three{init, led_0, led_1, led_2} T_LED;
unsigned char tmpT = 0;

void threeLEDs() { //begin
	
	switch(T_LED) { //transitions
		case init:
			T_LED = led_0;
			break;
			
		case led_0:
			T_LED = led_1;
			break;
			
		case led_1:
			T_LED = led_2;
			break;
			
		case led_2:
			T_LED = init;
			break;
	}
	
	switch(T_LED) { //actions
		case init:
		break;
		
		case led_0:
		tmpT = 0x01;
		break;
		
		case led_1:
		tmpT = 0x02;
		break;
		
		case led_2:
		tmpT = 0x04;
		break;
		
		default:
		break;
	}
	
	
}//end three LEDs

//blinkingLED
enum blink{init, off, on} B_LED;
unsigned char tmpBlink = 0;

void blinkingLED() { //begin
	
	switch(B_LED) { //transitions
		
	case init:
		B_LED = off;
		break;
	
	case off:
		B_LED = on;
		break;
		
	case on:
		B_LED = off;
		break;
	
	default:
		B_LED = init;
		break;
	}
	
	switch(B_LED) { //actions
		
		case init:
			break;
		
		case off:
			tmpBlink = 0x00;
			break;
		
		case on:
			tmpBlink = 0x08;
			break;
		
		default:
			break;
	}	
} //end blinking LED

//Combine
enum combine{init, leds} c_state;
void combineLEDs() { //begin
	
	switch(combine) { //transitions
		
		case init:
			break;
		
		case leds:
			c_state = leds;
			break;
	}
	
	switch(combine) { //actions
		
		case init:
			break;
			
		case leds:
			PORTD = tmpT | tmpBlink; //which to write to port D
			break;
	}
	
} //end combine
	
	
	
int main(void) {
	
	//DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	//DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs
	
	unsigned char t_count = 0;
	unsigned char b_count = 0;
	
	T_LED = init;
	B_LED = init;
	c_state = init;
	
	LCD_init(); //initialize LCD
	
	TimerSet(100); //1 sec
	TimerOn();

	while(1) {
		
		t_count++;
		b_count++;
		
		if(t_count == 10) {
			threeLEDs();
			t_count = 0;
		}
		
		if(b_count == 10) {
			blinkingLED();
			b_count = 0;
		}
		
		combineLEDs();

		while(!TimerFlag) {};
		TimerFlag = 0;
	}
	
	return 0;
}

/*Connect LEDs to PB0, PB1, PB2, and PB3.
In one state machine (Three LEDs), output to a shared variable (threeLEDs) the following behavior:
set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 1 second each.
In a second state machine (Blinking LED), output to a shared variable (bilnkingLED) the following
behavior: set bit 3 to 1 for 1 second, then 0 for 1 second. In a third state machine (Combine LEDs),
combine both shared variables and output to the PORTB. Note: only one SM is writing to outputs.
Do this for the rest of the quarter.*/
