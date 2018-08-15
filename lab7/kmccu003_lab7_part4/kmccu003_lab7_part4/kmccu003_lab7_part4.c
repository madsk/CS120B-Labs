/*
 * kmccu003_lab7_part4.c
 *
 * Created: 8/14/2018 1:35:54 PM
 * Author : ucrcse
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

enum Sound{s_on, s_off} sound;
unsigned char s = 0x00;
unsigned char button = 0x00;

void speaker() {
	button = ~PINA & 0x04; //PA2
	
	switch(sound) { //transitions
		
		case s_off:
		if(button == 0x04) { //on if button is pressed
			sound = s_on;
		}
		else {
			sound = s_off;
		}
		break;
		
		case s_on:
		if(button == 0x04) {
			sound = s_off;
		}
		break;
	} //end transitions
	
	switch(sound) { //actions
		
		case s_on:
		s = 0x10;
		break;
		
		case s_off:
		s = 0x00;
		break;
		
		default:
		break;
	}
	
}//end speaker

unsigned char button_0 = 0x00;
unsigned char button_1 = 0x00;
unsigned char freq_cnt = 0;

enum Freq{start, wait, inc, dec, wait2} freq;
void change_freq() { //begin
	
	button_0 = ~PINA & 0x01; //PA0
	button_1 = ~PINA & 0x02; //PA1
	
	switch(freq) { //transitions
		
		case start:
			freq = wait;
			break;
			
		case wait:
			if(button_0 && !button_1) { //increment
				freq = inc;
			}
			
			else if(!button_0 && button_1) { //decrement
				freq = dec;
			}
			
			else {
				freq = wait; //stay
			}
			break;
			
		case inc:
			freq = wait2;
			break;
			
		case dec:
			freq = wait2;
			break;
			
		case wait2:
			if(button_0) {
				freq = wait2;
			}
			else if(button_1) {
				freq = wait2;
			}
			else {
				freq = wait;
			}
			break;
				
	}
	
	switch(freq) {
			
		case inc:
			freq_cnt += 1;
			break;
			
		case dec:
			freq_cnt -= 1;
			break;
			
		default:
		break;
		
		
	}
	
	
	}; //end freq change

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
		tmpB = tmpT | tmpBlink | s; //which to write to port B
		break;
		
		default:
		break;
	}
	
} //end combine

int main(void) {

	DDRB = 0xFF; PORTB = 0x00; //output
	DDRA = 0x00; PORTA = 0xFF; //input
	
	unsigned long tot_three = 300;
	unsigned long tot_blink = 1000;
	unsigned long s_time = freq_cnt;
	unsigned long s_period = 1; //speaker period
	
	T_LED = init;
	B_LED = init;
	c_state = leds;
	sound = s_off;
	
	TimerSet(s_period); //set period to 2ms
	TimerOn();

	while(1) {
		if(tot_three >= 300) { //when 300
		threeLEDs();
		tot_three = 0; //reset
		}
		
		if(tot_blink >= 1000) { //when 1000
			blinkingLED();
			tot_blink = 0;
		}
		
		if(s_time >= freq_cnt) {
			speaker(); //execute every period (1ms)
			s_time = 0;
		}
		
		change_freq();
		combineLEDs();
		
		PORTB = tmpB;
		
		while(!TimerFlag) {};
		TimerFlag = 0;
		tot_three += s_period; //add 1 every time
		tot_blink += s_period;
		s_time += s_period;
	}
	
	return 0;
}


/*Extend the previous exercise to allow a user to adjust the sound frequency up or down using buttons connected to PA0 (up) and PA1 (down).
Using our 1 ms timer abstraction, the fastest you'll be able to pulse is 1 ms on and 1 ms off, meaning 500 Hz.
You'll probably want to introduce another synchSM that polls the buttons and sets a global variable storing the current frequency that in turn is read by the frequency generator task.*/