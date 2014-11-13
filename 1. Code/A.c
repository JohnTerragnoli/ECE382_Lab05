//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab5.c
// Date:	Fall 2014
// Purp:	Demo the decoding of an IR packet
//-----------------------------------------------------------------
#include <msp430g2553.h>
#include "A.h"

extern void init();
extern void initNokia();
extern void clearDisplay();
//add an int to the end.  This ensures that the value 0x00 or 0xFF can be passed into the .asm file.
extern void drawBlock(unsigned char row, unsigned char col, signed int color);



int32	packet;
int16	packetData[48];
int8	newIRPacket = FALSE;
int8	packetIndex = 0;



// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void main(void) {

	//nothing in this section was touched.
				// Setup MSP to process IR and buttons



	//----------------------------------------------------------------------
	//DRAWING CODE
	unsigned char	x, y;

	//to pass in the color as a direct parameter.
	signed int	color;

	//IFG1=0; /* clear interrupt flag1 */
	//WDTCTL=WDTPW+WDTHOLD; /* stop WD */
	initMSP430();

	init();
	initNokia();



	clearDisplay();

	x=4;		y=4;

	//initializes the code
	color = 0xFF;

	//sets the color to be black initialy :)  to omoimasu
	drawBlock(y,x,color);
	//touched nothing here.

	//----------------------------------------------------------------------





	while(1)  {


		//only go if we have a new packet coming in.
		if(newIRPacket){


			_disable_interrupt();
			//toggles the leds on/off.
			if(packet==ZERO){
				color ^= 0xFF;

			}

			//move up
			if (packet==CH_UP) {
				if (y>=1) y=y-1;

			}

			if (packet==CH_DW) {
				if (y<=6) y=y+1;

			}

			if (packet==VOL_DW) {
				if (x>=1) x=x-1;

			}

			if (packet==VOL_UP) {
				if (x<=10) x=x+1;

			}

			init();
			initNokia();

			newIRPacket = FALSE;
			drawBlock(y,x,color);

			initMSP430();

		}//end newIRPacket
		//newIRPacket = FALSE;
		drawBlock(y,x,color);

		_enable_interrupt();

	} // end infinite loop
} // end main

// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}

// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global packet variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits


	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
		case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;
			if((pulseDuration < maxLogic0Pulse) && (pulseDuration > minLogic0Pulse)){
				packet = (packet << 1) | 0;
			}

			if((pulseDuration < maxLogic1Pulse) && (pulseDuration > minLogic1Pulse)){
				packet = (packet << 1) | 1;
			}

			packetData[packetIndex++] = pulseDuration;
			TACTL = 0;				//turn off timer A e.w.
			LOW_2_HIGH; 				// Setup pin interrupr on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time measurements are based at time 0
			TA0CCR0 = 0xFFFF;
			TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;
			HIGH_2_LOW; 						// Setup pin interrupr on positive edge
			break;
	} // end switch

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {

	//this was taken from Eric Wardners.  I wasn't quite sure what to do with this method.
	TACTL = 0;
	TACTL ^= TAIE;
	packetIndex = 0;
	newIRPacket = TRUE;
	TACTL &= ~TAIFG;
}
