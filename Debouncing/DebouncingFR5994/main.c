#include <msp430.h> 


/**
 * main.c
 */



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5; //disable high impedance mode
    P5REN |= BIT5; //setup button 5.5
    P5OUT |= BIT5;
    P5DIR |= BIT5;
    P5IE |= BIT5;
    P5IES |= BIT5;
    P5IFG &= ~BIT5;

    P5REN |= BIT6; //setup button 5.6
        P5OUT |= BIT6;
        P5DIR |= BIT6;

        P5IE |= BIT6;
        P5IES |= BIT6;
        P5IFG &= ~BIT6;

    P1DIR |= BIT0; //setup led 1.0
    P1DIR |= BIT1; //setup led 1.1
__bis_SR_register(LPM0_bits + GIE); //low power
}


#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void) { //button 5 interrupt service routine
    if(P5IN & ~BIT5) { //if button 5.5 is pressed
    P5IE &= ~BIT5;          //disable interrupt
        WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
        SFRIE1 |= WDTIE;           //enable WDT interrupt
        P5IFG &=~BIT5;           //clear flag
    }
    if(P5IN & ~BIT6) { //if button 5.6 is pressed
        P5IE &= ~BIT6;          //disable interrupt
        WDTCTL = WDT_MDLY_32;   //Watchdog 32ms delay
        SFRIE1 |= WDTIE;           //enable WDT interrupt
        P5IFG &=~BIT6;           //clear flag
    }
}


#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    SFRIE1 &= ~WDTIE;      //disable WDT interrupt
    SFRIFG1 &= ~WDTIFG;    //clear flag
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    if (!(P5IN & BIT5))//if button is pressed, switch LED
    {
        P1OUT ^= BIT1; //toggle led 1.1
    }
    P5IE |= BIT5;   //enable button interrupt

    if (!(P5IN & BIT6))//if button is pressed, switch LED
        {
            P1OUT ^= BIT0; //toggle led 1.0
        }
    P5IE |= BIT6; //reset enable
}
