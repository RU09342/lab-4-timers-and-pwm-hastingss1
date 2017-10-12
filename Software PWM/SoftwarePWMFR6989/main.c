#include <msp430.h> 


/**
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // give power to channel

    TA0CTL |= TAIE;             //enable TA0 interrupt
    TA0CCTL0 |= CCIE;              //enable CCR0 interrupt
    TA0CCTL0 &= ~TAIFG;            //clear flag
    TA0CCTL1 |= CCIE;              //enable CCR1 interrupt
    TA0CCTL1 &= ~TAIFG;            //clear flag

    TA0CTL = TASSEL_2 + MC_1 + ID_0;           // SMCLK, upmode, clk/1
    TA0CCR1 = 500;                               //50% duty to start
    TA0CCR0 = 1000;                             //1 kHz signal

    P1IE |=  BIT1;                            // P1.1 interrupt enabled
    P1IES |= BIT1;                            //falling edge
    P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
    P1OUT |= BIT1;                             //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P1OUT |= BIT0;      //turn on LED
    P9DIR |= BIT7;       // P1.0 pin output
    P9OUT |= BIT7;

    __bis_SR_register(GIE);  //not low power mode
    while(1){
    } //needed since not in LPM
    // Enter LPM0 w/ interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void) {
    P1OUT |= BIT0;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void) {
    switch(TA0IV){
    case 2://CCR1
    P1OUT &= ~BIT0;
    break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    //P1IE &= ~BIT1;
        P9OUT ^= BIT7;
        if(TA0CCR1 >= 1000) {
            TA0CCR1 = 0;
        }
        else {
            TA0CCR1 = TA0CCR1 + 100;
        }
    P1IFG &=~BIT1;
}
