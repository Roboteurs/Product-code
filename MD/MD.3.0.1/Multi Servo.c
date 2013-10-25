//******************************************************************************
//   MSP430G2553 based Serial Controlled Camera Pan and Tilt
//
//   This firmware can be used to control up to six servos from
//   your PC's serial port.  Uses 2553 hardware UART to interface
//   to PC.  Uses internal DCO (no external xtal required).
//   ACLK = n/a, MCLK = SMCLK = CALxxx_8MHZ = 8MHz
//   UCOS16 = 1
//   8,000,000Hz, 9600Baud, UCBRx=52, UCBRSx=0, UCBRFx=1
//
//   Description: Echo a received character, RX ISR used. Normal mode is LPM0.
//   USCI_A0 RX interrupt triggers TX Echo.
//
//   PIN		Function	P1DIR.x 	P1SEL.x 	P1SEL2.x
//	 P1.5		TA0.0 		1 			1 			0	
//	 P1.6		TA0.1 		1 			1 			0
//   P2.0		TA1.0 		1 			1 			0
//   P2.1		TA1.1		  1 			1 			0
//   P2.4		TA1.2		  1 			1 			0
//   P3.0***	TA0.2		1 			1 			0	
//
// *** Not available in the DIP package :(	
//
//                MSP430G2553
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |     P1.2/UCA0TXD|------------>
//            |                 | 9600 - 8N1
//            |     P1.1/UCA0RXD|<------------
//            |                 |
//            |      	     P1.5 |------------> servo 0
//			      |			       P1.6 |------------> servo 1
//			      |			       P2.0 |------------> servo 2
//			      |			       P2.1 |------------> servo 3
//			      |			      P12.4 |------------> servo 4
//			       -----------------
//
//	Origonal Author: Eric Gregori ( www.buildsmartrobots.com )
//  Modified By: Roboteurs ( modifications )
//
//    Copyright (C) 2011  Eric Gregori
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
//******************************************************************************
#include  "msp430g2553.h"

#define POSITION_CHAR	'P'
#define RATE_CHAR		'R'	
#define SERVOS			6

unsigned char buffer[6];
unsigned char buffer_index;
unsigned char  rate[SERVOS];
unsigned short servo[SERVOS];
unsigned short input[SERVOS];

void InitPWM(void)
{
	P1SEL  |= (BIT5|BIT6);
	P2SEL  = BIT0|BIT1|BIT4;
	P1DIR  |= (BIT5|BIT6);
	P2DIR  = BIT0|BIT1|BIT4;
	 
	TA0CCR0		= 2000;			// PWM pulse width
	TA0CCR1		= 3000;			// PWM pulse width
	TA0CCR2		= 4000;			// PWM pulse width
	TA1CCR0		= 5000;			// PWM pulse width
	TA1CCR1		= 6000;			// PWM pulse width
	TA1CCR2		= 7000;			// PWM pulse width
	 
	TA0CCTL0 	= 0x0004;		// Mode = 0, OUT = 1
	TA0CCTL1 	= 0x0004;		// Mode = 0, OUT = 1
	TA0CCTL2 	= 0x0004;		// Mode = 0, OUT = 1
	TA1CCTL0 	= 0x0004;		// Mode = 0, OUT = 1
	TA1CCTL1 	= 0x0004;		// Mode = 0, OUT = 1
	TA1CCTL2 	= 0x0004;		// Mode = 0, OUT = 1

	TA1R   		= 0x8FFF;  		// Offset Timer1 Overflow interrupt
	TA0CTL 		= 0x0262; 		// Overflow Interrupt Enable, /2, continuous
	TA1CTL 		= 0x0262; 		// Overflow Interrupt Enable, /2, continuous
}

void InitUART(void)
{
  P1SEL |= (BIT1 + BIT2);                   // P1.1 = RXD, P1.2=TXD
  P1SEL2 |= (BIT1 + BIT2);                  // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  
  // UCOS16 = 1
  // 8,000,000Hz, 9600Baud, UCBRx=52, UCBRSx=0, UCBRFx=1
  UCA0BR0 = 52;                             // 8MHz, OSC16, 9600
  UCA0BR1 = 0;                              // 8MHz, OSC16, 9600
  // UCA0MCTL = UCBRFx | UCBRSx | UCOS16
  UCA0MCTL = 0x10|UCOS16;                   // UCBRFx=1,UCBRSx=0, UCOS16=1
  
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;  						// Enable USCI_A0 RX interrupt
}

void TXdata( unsigned char c )
{
  while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = c;            		        // TX -> RXed character
}
 
void main(void)
{
  unsigned short i;	
	
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  BCSCTL1 = CALBC1_8MHZ;                    // Set DCO to 8Mhz
  DCOCTL = CALDCO_8MHZ;						// Set DCO to 8Mhz
  P1DIR = BIT0;								//
  buffer_index = 0;							//  
  for( i=0; i<SERVOS; ++i )
  { 
  	servo[i] = 4500; 
  	input[i] = 4500; 
  	rate[i]  = 0;
  }                       
  InitPWM();
  InitUART();
  TXdata('>');

  __bis_SR_register(GIE);       // interrupts enabled
  
  // 012345   $=START_CHAR
  // $sxxxx - 0000 -> 9999
  while(1)
  {
  	if( buffer_index > 6 )
  	{
  		if( buffer[1] < SERVOS )
  		{
  			i = buffer[2]*1000;
  			i += buffer[3]*100;
  			i += (buffer[4]*10);
  			i += buffer[5];
  			if( buffer[0] == POSITION_CHAR )
  			{
  				if( i < 2000 ) i = 2000;
  				input[buffer[1]] = i;
  			}
  			if( buffer[0] == RATE_CHAR )
  			{
  				if( i > 255 ) i = 255;
  				rate[buffer[1]] = i;
  			}
  		}
  		buffer_index = 0;
  		TXdata('>');
  	}
  }// while(1) 
}

//  Echo back RXed character, confirm TX buffer is ready first
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	unsigned char c;
	c = UCA0RXBUF;	
	TXdata(c);
	switch( buffer_index )
	{
		case 0: // POSITION_CHAR
			if( c != POSITION_CHAR && c != RATE_CHAR ) break;
			buffer[0] = c;
			buffer_index++;
			break;
		case 1: // servo
		case 2: // digit 1
		case 3: // digit 2
		case 4: // digit 3
		case 5: // digit 4
			c -= '0';
			if( c > 9 ) break;
			buffer[buffer_index] = c;
			buffer_index++;
			break;
		case 6: // 0x0d
			if( c == 0x0d ) buffer_index++;
			break;	
		default:
			break;
	}
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1 (void)
{
	volatile unsigned short i;
	
	if( TA0IV == TA0IV_TAIFG )
	{		
		P1OUT |= BIT0;
		// Set to mode 0
		TA0CCTL0 	= 0x0004;
		TA0CCTL1 	= 0x0004;
		TA0CCTL2 	= 0x0004;		
		i = TA0R;
		while( TA0R == i );
		TA0CCR0		= servo[0];
		TA0CCR1		= servo[1];
		TA0CCR2		= servo[2];		
		TA0CCTL0 	= 0x00A4;
		TA0CCTL1 	= 0x00A4;
		TA0CCTL2 	= 0x00A4;
		P1OUT &= ~BIT0;	
		
		for( i=0; i<SERVOS/2; ++i )
  		{ 
  			if( rate[i] > 0 )
  			{
  				if( input[i] > servo[i] )
  					servo[i] = servo[i] + rate[i];
  				if( input[i] < servo[i] )
  					servo[i] = servo[i] - rate[i];
  				if( servo[i] > 10000 ) servo[i] = 10000;
  				if( servo[i] < 2000 ) servo[i] = 2000;
  			}
  			else
  			{
  				servo[i] = input[i];
  			}
  		}	
	}	
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1 (void)
{
	volatile unsigned short i;
	
	if( TA1IV == TA1IV_TAIFG )
	{		
		// Set to mode 0
		TA1CCTL0 	= 0x0004;
		TA1CCTL1 	= 0x0004;
		TA1CCTL2 	= 0x0004;		
		i = TA1R;
		while( TA1R == i );
		TA1CCR0		= servo[3];
		TA1CCR1		= servo[4];
		TA1CCR2		= servo[5];
		TA1CCTL0 	= 0x00A4;
		TA1CCTL1 	= 0x00A4;
		TA1CCTL2 	= 0x00A4;		

		for( i=SERVOS/2; i<SERVOS; ++i )
  		{ 
  			if( rate[i] > 0 )
  			{
  				if( input[i] > servo[i] )
  					servo[i] = servo[i] + rate[i];
  				if( input[i] < servo[i] )
  					servo[i] = servo[i] - rate[i];
  				if( servo[i] > 10000 ) servo[i] = 10000;
  				if( servo[i] < 2000 ) servo[i] = 2000;
  			}
  			else
  			{
  				servo[i] = input[i];
  			}
  		}
  	}		
}

