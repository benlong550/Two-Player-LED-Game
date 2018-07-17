#include <avr/io.h>
#include <time.h>
#include <avr/interrupt.h>
//#include "lcd.h"
//#include "adc.h"
//header to enable data flow control over pins
#define F_CPU 1000000
//telling controller crystal frequency attached
#include <util/delay.h>
//header to enable delay function in program
#define    E   5
//giving name �enable�  to 5th pin of PORTD, since it Is connected to LCD enable pin
#define RS  6
//giving name �registerselection� to 6th pin of PORTD, since is connected to LCD RS pin

unsigned char GetBit(unsigned char x, unsigned char k){
return ((x &(0x01 << k))!=0);}

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
// AVR timer/counter controller register TCCR1
TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
// bit2bit1bit0=011: pre-scaler /64
// 00001011: 0x0B
// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
// Thus, TCNT1 register will count at 125,000 ticks/s

// AVR output compare register OCR1A.
OCR1A = 125;	// Timer interrupt will be generated when TCNT1==OCR1A
// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
// So when TCNT1 register equals 125,
// 1 ms has passed. Thus, we compare to 125.
// AVR timer interrupt mask register
TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

//Initialize avr counter
TCNT1=0;

_avr_timer_cntcurr = _avr_timer_M;
// TimerISR will be called every _avr_timer_cntcurr milliseconds

//Enable global interrupts
SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
TimerISR(); // Call the ISR that the user uses
_avr_timer_cntcurr = _avr_timer_M;
}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
_avr_timer_M = M;
_avr_timer_cntcurr = _avr_timer_M;
}


void ADC_init()
{
ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
ADMUX = 1;
}
int HORIZONTALMOV =0;
int VERTICAlMOV =0;
int V = 0;
int H = 0;
	unsigned short dspot;
int R = 0;
unsigned char btn1 = 0x00; //left
unsigned char btn2 = 0x00; //up
unsigned char btn3 = 0x00; //down

static unsigned char pos2 = 0x7F; // sets the pattern displayed on columns
static unsigned char pos1 = 0x80; // grounds column to display pattern
//static unsigned char pos2 = 0xEF; // sets the pattern displayed on columns
//static unsigned char pos1 = 0x10; // grounds column to display pattern

void Catch()
{
	switch(state)
	{
		case Initial:
			state = Wait;
			break;

	case Wait:
		readjoystick();
		
		
		break;

	case Reset:
		if (btn4)
		{
			state = Reset;
		}
		else
		{
			state = WaitReset;
		}
		break;
		
	case WaitReset:
			state = Wait;
			break;
	}

	switch(state)
	{
		case Initial:
				readjoystick();
				btn4 = GetBit(~PINA,4);
				//pos1 = 0x80;
				//pos2 = 0x7F;

				pos1 = 0x10;
				pos2 = 0xEF;

				break;

		case Reset:		

		case WaitReset:
				
				break;

		case Wait:
				readjoystick();
				btn3 = GetBit(~PINA,4);
				break;

		

		}
		PORTD = pos1;
		PORTB = pos2;
		return state;
}
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void Set_A2D_Pin(unsigned char pinNum)
{
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for ( i=0; i<15; i++ ) { asm("nop"); }
}

		{
			ADCSRA |=(1<<ADSC);//start ADC conversion
			while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
			HORIZONTALMOV = ADC;//moving value
			ADC=0;//reset ADC register
			ADMUX=0x41;//changing channel
			break;
		}

		case 0x41:
		{
			ADCSRA |=(1<<ADSC);// start ADC conversion
			while ( !(ADCSRA & (1<<ADIF)));// wait till ADC conversion
			VERTICAlMOV = ADC;// moving value
			ADC=0;// reset ADC register
			ADMUX=0x40;// changing channel
			break;
		}
	DDRB = 0xFF; PORTB = 0x00;	//output for led matrix
	TimerOn();

		 ADCSRA |=(1<<ADEN) |(1<ADPS2)|(1<ADPS1) |(1<<ADPS0);

		 //enabling the ADC,, setting prescalar 128
		TimerFlag = 0;
		TimerFlag = 0;
		TimerFlag = 0;
		TimerFlag = 0;
		TimerFlag = 0;
			r = rand() % 7;
			r = rand() % 7;