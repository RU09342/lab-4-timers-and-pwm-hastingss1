#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P1DIR |= BIT0;                            // P1.6 output
     P1SEL0 |= BIT0;                            // P1.6 for TA0 CCR1 Output Capture
     P1SEL1 = 0;                               // Select default function for P1.6 (see table 19 in general datasheet)

    P1REN |= BIT1;
        P1OUT |= BIT1;
        P1IE |= BIT1;
            P1IES |= BIT1;
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TA0CCR0 = 1000-1;                         // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = 500;                            // CCR1 PWM duty cycle

    TA1CTL = TASSEL_2 + MC_1;  // SMCLK, up mode, clear TBR

    __bis_SR_register(LPM0_bits + GIE);             // Enter LPM0

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
        P1OUT ^= BIT0; //toggle led 1.0
        if(TA0CCR1 >= 1000) { //check if timer has passed 1000 khz
            TA0CCR1 = 0; //reset to 0
        }
        else {
            TA0CCR1 = TA0CCR1 + 100; //increment by 100
        }
    P1IFG &=~BIT1; //reset
}

