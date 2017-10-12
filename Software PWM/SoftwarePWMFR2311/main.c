#include <msp430.h> 


/**
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;           // give power to channel

    TB0CTL |= TBIE;             //enable TB0 interrupt
    TB0CCTL0 |= CCIE;              //enable CCR0 interrupt
    TB0CCTL0 &= ~TBIFG;            //clear flag
    TB0CCTL1 |= CCIE;              //enable CCR1 interrupt
    TB0CCTL1 &= ~TBIFG;            //clear flag

    TB0CTL = TBSSEL_2 + MC_1 + ID_0;           // SMCLK, upmode, clk/1
    TB0CCR1 = 500;                               //50% duty to start
    TB0CCR0 = 1000;                             //1 kHz signal

    P1IE |=  BIT1;                            // P1.1 interrupt enabled
    P1IES |= BIT1;                            //falling edge
    P1REN |= BIT1;                            // Enable resistor on SW2 (P1.1)
    P1OUT |= BIT1;                             //Pull up resistor on P1.1
    P1IFG &= ~BIT1;                           // P1.1 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P1OUT |= BIT0;      //turn on LED
    P2DIR |= BIT0;       // P2.0 pin output
    P2OUT |= BIT0; //turn on led

    __bis_SR_register(GIE);  //not low power mode
    while(1){
    } //needed since not in LPM
    // Enter LPM0 w/ interrupt
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void) { //timer b0 interrupt
    P1OUT |= BIT0; //turn led 1.0
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR(void) { //timer b1 interrupt
    switch(TB0IV){
    case 2://CCR1
    P1OUT &= ~BIT0; //turn off led 1.0
    break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) { //button interrupt
        P2OUT ^= BIT0; //toggle led 2.0
        if(TB0CCR1 >= 1000) { //check if timer has passed 1 khz
            TB0CCR1 = 0; //reset to 0
        }
        else {
            TB0CCR1 = TB0CCR1 + 100; //increment by 100 hz
        }
    P1IFG &=~BIT1; //reset flag
}
