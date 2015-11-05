#include <stdint.h>

#include <stdbool.h>



#include "efm32gg.h"



uint8_t button_mapper();

void button_handler();

uint16_t sound1();

uint16_t sound2();

uint16_t sound3();

uint16_t sound4();

void resetCounter();





uint16_t sample;



int play = 0;











/* TIMER1 interrupt handler 



Plays sound according to buttons pushed*/

void __attribute__ ((interrupt)) TIMER1_IRQHandler(int sound) 

{  

	*TIMER1_IFC = 0x1;

	//(*GPIO_PA_DOUT)++;



	if(play==1 || play == 5){

		sample = sound1();

	}

	 else if(play==2 || play == 6){

		sample = sound2();

	}

	else if(play==3 || play == 7){

		sample = sound3();

	}

	else if(play==4 || play == 8){

		sample = sound4();

	}





		*DAC0_CH0DATA = sample;

		*DAC0_CH1DATA = sample;



	

}



	



	

/* GPIO even pin interrupt handler */

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 

{

	*GPIO_IFC = 0xff;

	button_handler();

    /* TODO handle button pressed event, remember to clear pending interrupt */

}



/* GPIO odd pin interrupt handler */

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 

{



	*GPIO_IFC = 0xff;

	button_handler();

	

    /* TODO handle button pressed event, remember to clear pending interrupt */

}





void button_handler(){



	int input = button_mapper();//What button is being pushed?



	if(input!=0){

		

		timerWake();

		dacWake();

		*GPIO_PA_DOUT = *GPIO_PC_DIN <<8 ;

		resetCounter();

		play = input;

	}

	else{

		timerSleep();

		dacSleep();

	}



}



//finds out witch button is being pressed.

uint8_t button_mapper(){

	uint8_t input = ~(*GPIO_PC_DIN);

	switch(input){

		case 0x1:

			return 1;



		case 0x2:

			return 2;



		case 0x4:

			return 3;



		case 0x8:

			return 4;



		case 0x10:

			return 5;



		case 0x20:

			return 6;



		case 0x40:		

			return 7;

		

		case 0x80:

			return 8;



		default:

			return 0;

 

	}

	return 0;

}















