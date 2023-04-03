#include <msp430.h>
#include <stdlib.h>
#include "libTimer.h"
#include "buzzer.h"
#include <stdio.h>

#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3

#define SWITCHES (SW1 | SW2 | SW3 | SW4)

int main() {
    configureClocks();
 
    buzzer_init();
    buzzer_set_period(0);	/* start buzzing!!! 2MHz/1000 = 2kHz*/
    P2REN |= SWITCHES;
    P2IE |= SWITCHES;
    P2OUT |= SWITCHES;
    P2DIR &= ~SWITCHES;

    or_sr(0x18);          // CPU off, GIE on
}

int note = 4500;
int majorScale[7] = {440,440,220,440,440,440,220};
    
void
switch_interrupt_handler()
{
  char p1val = P2IN;
  char changed = 0;
  int interval = 0;
  
  P2IES |= (p1val & SWITCHES);	/*  up  */
  P2IES &= (p1val | ~SWITCHES);	/* down */
  
  if (!(p1val & SW1)) {
    buzzer_set_period(note);
  }
  else if (!(p1val & SW2) && !changed) {
      changed = 1;
      if (interval == 0)
	interval = 6;
      else
	interval--;
      note += majorScale[interval];
    }
  else if (!(p1val & SW3) && !changed) {
      changed = 1;
      note -= majorScale[interval];
      if (interval == 6)
	interval = 0;
      else
	interval++;
    } 
  if ((p1val & SW2) && (p1val & SW3)) {
    changed = 0;
  }
  if (p1val & SW1) {
    buzzer_set_period(0);
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
