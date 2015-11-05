#include <stdint.h>

#include <stdbool.h>



#include "efm32gg.h"



/* function to set up GPIO mode and interrupts*/

void setupGPIO()

{

	

  /* TODO set input and output pins for the joystick */

    

  /* Example of HW access from C code: turn on joystick LEDs D4-D8

     check efm32gg.h for other useful register definitions

  */

  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock*/

  *GPIO_PA_CTRL = 2;  /* set high drive strength */

  *GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */

  *GPIO_PC_MODEL = 0x33333333; //Set pins 0-7 ar input

  *GPIO_PC_DOUT = 0xff; //Enables internal pull-upp

  *GPIO_EXTIPSELL = 0x22222222;//Enables GPIO interrupts

  *GPIO_EXTIFALL = 0xff;

  *GPIO_EXTIRISE = 0xff;

  *GPIO_IEN = 0xff;



  *GPIO_PA_DOUT = *GPIO_PC_DIN<<8; /* Sets the initial LEDs */

}







