#include <stdint.h>

#include <stdbool.h>



#include "efm32gg.h"



void setupDAC()

{

//Enable DAC clock

	*CMU_HFPERCLKEN0 |= 1 << 17;

// Prescaling of frequency

	*DAC0_CTRL = 0x50010;

// Enable left and rilght channel

	*DAC0_CH0CTRL = 0x1;

	*DAC0_CH1CTRL = 0x1;





  /*

    TODO enable and set up the Digital-Analog Converter

    

    1. Enable the DAC clock by setting bit 17 in CMU_HFPERCLKEN0

    2. Prescale DAC clock by writing 0x50010 to DAC0_CTRL

    3. Enable left and right audio channels by writing 1 to DAC0_CH0CTRL and DAC0_CH1CTRL

    4. Write a continuous stream of samples to the DAC data registers, DAC0_CH0DATA and DAC0_CH1DATA, for example from a timer interrupt

  */

}



//Disables DAC to save power. 

void dacSleep(){

  *DAC0_CH0CTRL = 0x0;

  *DAC0_CH1CTRL = 0x0;

}



void dacWake(){

  *DAC0_CH0CTRL = 0x1;

  *DAC0_CH1CTRL = 0x1;

}
