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

    P5IE |=  BIT6;                            // P5.6 interrupt enabled
    P5IES |= BIT6;                            //falling edge
    P5REN |= BIT6;                            // Enable resistor on SW2 (P5.6)
    P5OUT |= BIT6;                             //Pull up resistor on P5.6
    P5IFG &= ~BIT6;                           // P5.6 Interrupt Flag cleared

    P1DIR |= BIT0;       // P1.0 pin output
    P1OUT |= BIT0;      //turn on LED
    P1DIR |= BIT1;       // P1.0 pin output

    __bis_SR_register(GIE);  //not low power mode
    while(1){
    } //needed since not in LPM
    // Enter LPM0 w/ interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void) { //timer a0 interrupt
    P1OUT |= BIT0; //turn on led1.0
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void) { //timer a1 interrupt
    switch(TA0IV){
    case 2://CCR1
    P1OUT &= ~BIT0; //turn off led 1.0
    break;
    }
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void) { //button interrupt
        P1OUT ^= BIT1; //toggle led 1.1
        if(TA0CCR1 >= 1000) { //check if timer is greater than 1 khz
            TA0CCR1 = 0; //reset
        }
        else {
            TA0CCR1 = TA0CCR1 + 100; //increment by 100 hz
        }
    P5IFG &=~BIT6; //reset flag
}
