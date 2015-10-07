#include <stdint.h>

#include <stdbool.h>



#include "efm32gg.h"



double y = 1000/44100;

int x = 0;

uint16_t z = 0x555;

uint16_t n = 0x111;

/* TIMER1 interrupt handler */

void __attribute__ ((interrupt)) TIMER1_IRQHandler() 

{  

	*TIMER1_IFC = 0x1;

	n= n*n*n;

	x+=1;

	if(x<=22){

		if(x<=11){

			

			z = z+n*n;

		}

		z = z+n;

	}

	else if(x<=44){

		if (x<33)

		{

			//z = z+n*n;

		}

		z = z-n;

	}

	else{

		x=0;

	}

	

	/*

	uint32_t m = *GPIO_PA_DOUT;

	*GPIO_PA_DOUT = m + 1;

	*/



	

	*DAC0_CH0CTRL = z;

	*DAC0_CH1CTRL = z;





  /*

    TODO feed new samples to the DAC

    remember to clear the pending interrupt by writing 1 to TIMER1_IFC

  */  

}



/* GPIO even pin interrupt handler */

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 

{

	*GPIO_IFC = 0x1;

	*GPIO_PA_DOUT = 0x001;

    /* TODO handle button pressed event, remember to clear pending interrupt */

}



/* GPIO odd pin interrupt handler */

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 

{

	*GPIO_IFC = 0x1;

	*GPIO_PA_DOUT = 0x001;

    /* TODO handle button pressed event, remember to clear pending interrupt */

}

