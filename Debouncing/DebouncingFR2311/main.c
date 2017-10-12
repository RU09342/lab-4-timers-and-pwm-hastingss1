#include <msp430.h> 


/**
 * main.c
 */



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    P1REN |= BIT1;
    P1OUT |= BIT1;
    P1DIR |= 0x01;

    P1IE |= BIT1;
    P1IES |= BIT1;
    P1IFG &= ~BIT1;

__bis_SR_register(LPM0_bits + GIE);
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    P1IE &= ~BIT1;          //disable interrupt
        WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
        SFRIE1 |= WDTIE;           //enable WDT interrupt
        P1IFG &=~BIT1;           //clear flag
    }


#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    SFRIE1 &= ~WDTIE;      //disable WDT interrupt
    SFRIFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P1IN & BIT1))//if button is pressed, switch LED
    {
        P1OUT ^= BIT0;
    }
    P1IE |= BIT1;   //enable button interrupt

}
