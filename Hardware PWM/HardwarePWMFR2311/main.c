
#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P2DIR |= BIT0;                     // P1.6 and P1.7 output
    P2SEL0 |= BIT0;                    // P1.6 and P1.7 options select


    P1REN |= BIT1;
        P1OUT |= BIT1;
        P1DIR |= 0x01;
        P1IE |= BIT1;
            P1IES |= BIT1;
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TB1CCR0 = 1000-1;                         // PWM Period
    TB1CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB1CCR1 = 500;                            // CCR1 PWM duty cycle

    TB1CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR

    __bis_SR_register(LPM0_bits + GIE);             // Enter LPM0

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
        P2OUT ^= BIT0; //toggle led
        if(TB1CCR1 >= 1000) { //check if timer has passed 1 khz
            TB1CCR1 = 0; //reset to 0
        }
        else {
            TB1CCR1 = TB1CCR1 + 100; //increment by 100 hz
        }
    P1IFG &=~BIT1; //reset
}

