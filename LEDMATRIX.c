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
//giving name “enable”  to 5th pin of PORTD, since it Is connected to LCD enable pin
#define RS  6
//giving name “registerselection” to 6th pin of PORTD, since is connected to LCD RS pin

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
	unsigned short dspot;	unsigned short bspot;
int R = 0;int C = 0;unsigned char btn0 = 0x00; //right
unsigned char btn1 = 0x00; //left
unsigned char btn2 = 0x00; //up
unsigned char btn3 = 0x00; //downunsigned char btn4 = 0x00; //downunsigned char btn5 = 0x00;unsigned char btn6 = 0x00;enum States{Initial, Wait, Reset, WaitReset} state;

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
}void A2D_init() 
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void Set_A2D_Pin(unsigned char pinNum)
{
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for ( i=0; i<15; i++ ) { asm("nop"); }
}void readjoystick(){	switch(ADMUX)	{		//case 0x00: LRValue = ADC; break;		//case 0x01: UDValue = ADC; break;		case 0x40://When ADMUX==0x40

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
		}	}	switch(ADMUX)		{			case 0x00: ADMUX = 1; break;			case 0x01: ADMUX = 0; break;			default: break;		}}unsigned char R0 = 0x7F;unsigned char R1 = 0xBF;unsigned char R2 = 0xDF;unsigned char R3 = 0xEF;unsigned char R4 = 0xF7;unsigned char R5 = 0xFB;unsigned char R6 = 0xFD;unsigned char R7 = 0xFE;unsigned char C0 = 0x01;unsigned char C1 = 0x02;unsigned char C2 = 0x04;unsigned char C3 = 0x08;unsigned char C4 = 0x10;unsigned char C5 = 0x20;unsigned char C6 = 0x40;unsigned char C7 = 0x80;unsigned char Row[8];unsigned char Col[8];void set1dArrays(){	Row[0] = R0;	Row[1] = R1;	Row[2] = R2;	Row[3] = R3;	Row[4] = R4;	Row[5] = R5;	Row[6] = R6;	Row[7] = R7;		Col[0] = C0;	Col[1] = C1;	Col[2] = C2;	Col[3] = C3;	Col[4] = C4;	Col[5] = C5;	Col[6] = C6;	Col[7] = C7;	}unsigned char arr[8][8];void set2dArray(){	for(int i = 0; i < 8; i++)	{		for (int j = 0; j < 8; j++)		{			arr[i][j] = 0;		}	}}int main(void){	DDRA = 0x00; PORTA= 0xFF; //input for analogs
	DDRB = 0xFF; PORTB = 0x00;	//output for led matrix	DDRC = 0xFF; PORTC = 0x00;	//input for buttons	DDRD = 0xFF; PORTD = 0x00;	// output for led matrix	ADC_init();	A2D_init();	TimerSet(1);
	TimerOn();	state = Initial;	ADMUX = 0;	 _delay_ms(50);//giving delay of 50ms
		 ADMUX |=(1<<REFS0);//setting the reference of ADC
		 ADCSRA |=(1<<ADEN) |(1<ADPS2)|(1<ADPS1) |(1<<ADPS0);

		 //enabling the ADC,, setting prescalar 128	set1dArrays();	set2dArray();	R = 0;	C = 0;	int Rp = 7;	int Cp = 7;	//PORTB = Row[R] & Row[R + 1];	//PORTD = Col[C] + Col[C + 1];	//PORTB = Row[R];	//PORTD = Col[C];	srand(time(NULL));	int r = rand() % 7;	int c = rand() % 7;	int score = 0x0F;	PORTC = score;	arr[r][c] =  1;		while(1)	{				PORTB = Row[r];		PORTD = Col[c];		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;		//==============================================		//IncLED();		PORTB = Row[R] & Row[R + 1];		PORTD = Col[C];		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;				PORTB = Row[R];		PORTD = Col[C] + Col[C + 1];		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;				//=============================================		PORTB = Row[Rp] & Row[Rp - 1];				//player two		PORTD = Col[Cp];		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;				PORTB = Row[Rp];		PORTD = Col[Cp] + Col[Cp - 1];		while (!TimerFlag);	// Wait 1 sec
		TimerFlag = 0;				readjoystick();		//readjoystick2();				if(VERTICAlMOV > 600 && R > 0)		{			R = R - 1;			PORTB = Row[R];		}		else if(VERTICAlMOV < 400 && R < 6)		{			R = R + 1;			PORTB = Row[R];		}		readjoystick();		//readjoystick2();		if(HORIZONTALMOV > 600 && C > 0)		{			C = C - 1;			PORTD = Col[C];		}		else if(HORIZONTALMOV < 400 && C < 6)		{			C = C + 1;			PORTD = Col[C];		}						if(GetBit(~PINA,6))			//reset		{			r = rand() % 7;			c = rand() % 7;			arr[R][C] = 0;			arr[r][c] = 1;			R = 0;			C = 0;						Rp = 7;			Cp = 7;			score = 0x0F;			PORTC = score;		}		btn2 = GetBit(~PINA, 2);//left		btn3 = GetBit(~PINA, 3);//right		btn4 = GetBit(~PINA, 4);//down		btn5 = GetBit(~PINA, 5);//up		btn6 = GetBit(~PINA, 6);//up				if(btn2 && Rp < 7)		//left		{			Rp = Rp + 1;			PORTB = Row[Rp];		}		else if(btn3 && Rp > 1)//right		{			Rp = Rp - 1;			PORTB = Row[Rp];		}		if(btn5 && Cp < 7)//down		{			Cp = Cp + 1;			PORTB = Col[Cp];		}		else if(btn4 && Cp > 1)//up		{			Cp = Cp - 1;			PORTB = Col[Cp];		}				if((R == r && C == c) || ((R + 1 == r) && C + 1 == c))		{
			r = rand() % 7;			c = rand() % 7;			arr[R][C] = 0;			arr[r][c] = 1;			score = score + 1;			PORTC = score;		}		if((Rp == r && Cp == c) || ((Rp - 1 == r) && Cp - 1 == c))		{
			r = rand() % 7;			c = rand() % 7;			arr[Rp][Cp] = 0;			arr[r][c] = 1;			if(score > 0)			{				score = score - 1;			PORTC = score;}		}			}}
