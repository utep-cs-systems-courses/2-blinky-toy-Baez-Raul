//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT |= LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  
  or_sr(0x18);		/* CPU off, GIE on */
}

int idle = 0;

void
__interrupt_vec(WDT_VECTOR) WDT() {
  /* 250 interrupts/sec */
  if (idle >= 5) {		/* Idle for 5 interrupts, then turns on */
    P1OUT |= LED_RED;
    idle = 0;
  }
  else {
    P1OUT &= ~LED_RED;
  }
  idle++;
} 

