
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P1DIR |= BIT0;                     // P1.6 and P1.7 output
    P1SEL0 |= BIT0;                    // P1.6 and P1.7 options select
    P1SEL1 = 0;                    // P1.6 and P1.7 options select


    P5REN |= BIT6;
        P5OUT |= BIT6;
        P5DIR |= BIT6;
        P5IE |= BIT6;
            P5IES |= BIT6;
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TA0CCR0 = 1000-1;                         // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = 500;                            // CCR1 PWM duty cycle

    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TBR

    __bis_SR_register(LPM0_bits + GIE);             // Enter LPM0

}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void) {
        P1OUT ^= BIT0; //toggle led 1.0
        if(TA0CCR1 >= 1000) { //check if timer has passed 1 khz
            TA0CCR1 = 0; //reset to 0
        }
        else {
            TA0CCR1 = TA0CCR1 + 100; //increment by 100 hz
        }
    P5IFG &=~BIT6; //reset
}

