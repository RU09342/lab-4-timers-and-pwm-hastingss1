# Hardware PWM
Sean Hastings: Note: In order to directly choose CCR1, a different select Pin must be used for each family. The F uses P1SEL, the FR uses PxSEL0 and PxSEL1, and the G2 uses PxSEL and PxSEL2.

Timer outputs can be wired directly to an output pin without the need of a timer interrupt by using the CCRX Capture Mode. Using the datasheets of each board, it can be determined what PSEL has to be used to drive the output pin. OUTMOD_7 is used so that the pin is set high on CCR0 reset, and set low on the CCR1 capture, which is no different than in a software PWM. All of the boards successfully implement hardware PWM.
