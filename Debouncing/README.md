# Software Debouncing
Sean Hastings:  Software debouncing uses the watchdog timer with a 32ms delay when a button is pressed, which halts the program. This means the code will only run for the button if the button is still held after 32ms. Since the WDT should normally be disabled, it is only enabled when the button is pressed. This debouncing eliminates the issue of picking up false posedges and/or negedges when pressing a mechanical button. All boards have successfully implemented debouncing.

## Extra Work
### Low Power Modes
LPM0 is used for this implementation; since timers are needed LPM4 cannot be used.

### Double the fun
I used the FR5994 to debounce two switches by using the watch dog timer. I check which button is pressed in the button interrupt and watch dog interrupt in order to properly debounce.
