#include <stdint.h>

#include <stdbool.h>



#include "efm32gg.h"



/* function to setup the timer */

void setupTimer(uint16_t period)

{



//Setting bit 6

*CMU_HFPERCLKEN0 |= 1 << 6;

//Writing the period (14 000 000/44100 = ca 317(HEX = 13D))

*TIMER1_TOP = period;

// Enable timer interrupt generation

*TIMER1_IEN = 0x1;

//Enable timer interrupts

*ISER0 |= 1 <<12;

//Start timer

*TIMER1_CMD = 0x1;

  /*

    TODO enable and set up the timer

    

    1. Enable clock to timer by setting bit 6 in CMU_HFPERCLKEN0

    2. Write the period to register TIMER1_TOP

    3. Enable timer interrupt generation by writing 1 to TIMER1_IEN

    4. Start the timer by writing 1 to TIMER1_CMD

    

    This will cause a timer interrupt to be generated every (period) cycles. Remember to configure the NVIC as well, otherwise the interrupt handler will not be invoked.

  */  

}





