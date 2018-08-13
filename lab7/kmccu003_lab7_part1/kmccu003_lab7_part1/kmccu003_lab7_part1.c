/*
 * kmccu003_lab7_part1.c
 *
 * Created: 8/13/2018 11:52:51 AM
 * Author : Kiana
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

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
unsigned char tmpB = 0;

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
			T_LED = led_0;
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
enum blink{init_1, off, on} B_LED;
unsigned char tmpBlink = 0;

void blinkingLED() { //begin
	
	switch(B_LED) { //transitions
		
	case init_1:
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
		
		case init_1:
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
enum combine{leds} c_state;
void combineLEDs() { //begin
	
	switch(c_state) { //transitions
		
		case leds: //repeat
			c_state = leds;
			break;
	}
	
	switch(c_state) { //actions
			
		case leds:
			tmpB = tmpT | tmpBlink; //which to write to port B
			break;
			
			default:
			break;
	}
	
} //end combine
	
	
	
int main(void) {

	DDRB = 0xFF; PORTB = 0x00;
	
	//unsigned char t_count = 0;
	//unsigned char b_count = 0;
	
	T_LED = init;
	B_LED = init;
	c_state = leds;
	
	TimerSet(1000); //1 sec
	TimerOn();

	while(1) {

			threeLEDs();
			blinkingLED();
			combineLEDs();
			PORTB = tmpB;
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
