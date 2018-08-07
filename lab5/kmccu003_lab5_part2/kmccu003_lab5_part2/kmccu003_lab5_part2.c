/*
 * kmccu003_lab5_part2.c
 *
 * Created: 8/7/2018 12:02:57 PM
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

enum States {INIT, led_0, led_1, led_2, press_0, press_1, press_2, release_0, release_1, release_2, reset} state;
	
unsigned char tmpA = 0x00;

void Tick() { //begin fct
	
	tmpA = ~PINA & 0x01; //use ~ for hardware testing
	
	switch(state) {
		
		case INIT:
			state = led_0;
			break;
		
		case led_0:
			if(tmpA == 0x01) { //if button is pressed, have to pause game
				state = press_0;
			}
			else {
			state = led_1;
			}
			break;
		
		case led_1:
			if(tmpA == 0x01) { //if button is pressed, have to pause game
				state = press_1;
			}
			else{
		state = led_2;
			}
			
		break;
		
		case led_2:
			if(tmpA == 0x01) { //if button is pressed, have to pause game
				state = press_2;
			}
			else{
			state = led_0;
			}
			break;
	
	case press_0:
		if(tmpA == 0x01) {
			state = press_0;
		}
		else {
			state = release_0;
		}
		break;
		
	case press_1:
	if(tmpA == 0x01) {
		state = press_1;
	}
	else {
		state = release_1;
	}
	break;
	
	case press_2:
	if(tmpA == 0x01) {
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
			PORTC = 0x01;
			break;
		
		case led_1:
			PORTC = 0x02;
			break;
		
		case led_2:
			PORTC = 0x04;
			break;
		
		case press_0:
			break;
		
		case press_1:
			break;
		
		case press_2:
			break;
		
		case release_0:
			PORTC = 0x01;
			break;
		
		case release_1:
			PORTC = 0x02;
			break;
		
		case release_2:
			PORTC = 0x04;
			break;
		
		case reset:
			//PORTC = 0x00;
			break;

		default:
		state = INIT;
			break;
			
	}//end actions
}//end fct

int main(void)
{
	/* Replace with your application code */
	DDRA = 0x00; //set port C to output
	PORTA = 0xFF; //init port C to 0's
	DDRC = 0xFF; //set port C to output
	PORTC = 0x00; //init port C to 0's
	
	TimerSet(260);
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

/*(Challenge) Create a simple light game that requires pressing a button on PA0 while the middle of three LEDs
on PB0, PB1, and PB2 is lit. The LEDs light for 300 ms each in sequence. When the button is pressed,
the currently lit LED stays lit. Pressing the button again restarts the game.*/