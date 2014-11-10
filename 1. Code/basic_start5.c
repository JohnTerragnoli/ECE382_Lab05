//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab5.c
// Date:	Fall 2014
// Purp:	Demo the decoding of an IR packet
//-----------------------------------------------------------------
#include <msp430g2553.h>
#include "basic_start5.h"



int8	newIrPacket = FALSE;
int16	packetData[48];
int8	packetIndex = 0;

//place to store the packet not in an array.
int32	IRPacket = 0x00000000;


unsigned char execute=TRUE;
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void main(void) {

	//nothing was changed about this method.
	initMSP430();				// Setup MSP to process IR and buttons

	//just counters used later to waste time.
	unsigned counter = 0;
	unsigned counter1 = 0;

	while(1)  {

		if(execute==TRUE) {

			//prevents the program from reading another button press while we
			//are analyzing this one!!
			_disable_interrupt();


			//ensures that the index is set to 0 just in case.
			packetIndex = 0;

			//takes care of data that is not logic 0 or 1.  We don't want this in
			//the packet!!!
			while(packetData[packetIndex]==otherData) {
				packetIndex++;
			}


			//gets all of the useful informaiton out of the packet.
			while(packetIndex<packetLength) {
				IRPacket+=packetData[packetIndex];
				IRPacket<<=1;
				packetIndex++;
			}


			//gets the last part of the data packet.
			IRPacket+=packetData[packetIndex];



			//these if statements just toggle the LEDs on or off.
			if(IRPacket == PWR) {
				P1OUT ^= BIT0;
			}

			if(IRPacket == ZERO) {
				P1OUT ^= BIT6;
			}


			//delays to prevent the bouncing affect and from counting each button press
			//multiple times. Prevents it from being "sticky."
			for(counter=0; counter<0xFF;counter++){
				for(counter1=0; counter1<0xFF; counter1++){
				}
			}



			//resets the packet so that we can accept the next one.
			packetIndex=0;
			IRPacket=0x00000000;


			//now we can accept more packets of information.
			_enable_interrupt();

			//don't want to be executing when the button has not been pressed yet.
			execute =FALSE;
		} else {
			IRPacket = 0x00000000;
		}




	} // end if new IR packet arrived
} // end infinite loop
// end main

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
// and store the bit in the global irPacket variable. Going forward there
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


			//determines if a 0 or a 1 occured.  You can tell by the length of the pulse!!!
			//the binary values are temporarily stored in the variable "pulseDuration"
			if (pulseDuration >= minLogic1Pulse && pulseDuration <= maxLogic1Pulse) {
				pulseDuration = 1;
			} else if (pulseDuration >= minLogic0Pulse && pulseDuration <= maxLogic0Pulse ) {
				pulseDuration = 0;
			}else {
				pulseDuration = otherData;
			}


			packetData[packetIndex++] = pulseDuration;

			LOW_2_HIGH; // Setup pin interrupr on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;						// time measurements are based at time 0
			HIGH_2_LOW; 						// Setup pin interrupr on falling edge
			break;
	} // end switch


	//This means we have recorded a packet length of data and we are ready to start executing it.
	if(packetIndex>packetLength){
		execute=TRUE;
	}

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry


} // end pinChange ISR













//originally there was an interrupt method at the bottom.  However, I started coding in main and forgot to go back and utilize this method.
//It was probably here for a reason but I made-do without it.


//looking back on it, though, this method could have been used as the delay created in the main loop.
