#include <msp430.h>

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    P1DIR |= BIT0;                     // P1.6 and P1.7 output
    P1DIR |= BIT2;
    P1SEL |= BIT2;                    // P1.6 and P1.7 options select

    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1IE |= BIT1;
    P1IES |= BIT1;

    TA0CCR0 = 1000-1;                         // PWM Period
    TA0CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR1 = 500;                            // CCR1 PWM duty cycle

    TA0CTL = TASSEL_2 + MC_1;  // SMCLK, up mode, clear TBR

    __bis_SR_register(LPM0_bits + GIE);             // Enter LPM0

}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
        if(TA0CCR1 >= 1000) { // check if timer has passed 1 khz
            TA0CCR1 = 0; //reset
        }
        else {
            TA0CCR1 = TA0CCR1 + 100; //increment by 100 hz
        }
    P1IFG &=~BIT1; //reset flag
}

