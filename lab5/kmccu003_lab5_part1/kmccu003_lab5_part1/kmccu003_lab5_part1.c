/*
 * kmccu003_lab5_part1.c
 *
 * Created: 8/6/2018 6:08:15 PM
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

enum States {INIT, led_0, led_1, led_2} state;

void Tick() {
	switch(state) { //transitions
		case INIT:
			state = led_0;
			break;
			
		case led_0:
			state = led_1;
			break;
		
		case led_1:
			state = led_2;
			break;
		
		case led_2:
			state = led_0;
			break;
	}
	
	switch(state) { //actions
		
		case INIT:
			break;
		
		case led_0:
			PORTC = 0x01;
			break;
		
		case led_1:
			PORTC = 0x02;
			break;
		
		case led_2:
			PORTC = 0x04;
			break;
			
		default:
			break;
		
	}
}

int main(void)
{
    /* Replace with your application code */
	DDRC = 0xFF; //set port C to output
	PORTC = 0x00; //init port C to 0's
	
	TimerSet(1000);
	TimerOn();
	//unsigned char tmpB = 0x00;
	
	state = INIT;
	
    while (1) 
    {
		//tmpB = ~tmpB;
		//PORTB = tmpB;
		
		Tick();
		
		while(!TimerFlag); //wait 1 sec
		TimerFlag = 0;
    }
}

/*Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each.
Implement that synchSM in C using the method defined in class. In addition to demoing your program,
you will need to show that your code adheres entirely to the method with no variations. */