        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 

//Initialize GPIO clock

	//Load CMU base address.
	ldr r1, =CMU_BASE
	

	//Load current value of HFPERCLKEN0
	ldr r2,[r1, #CMU_HFPERCLKEN0]
	
	//Set bit for GPIO clk
	mov r3, #1
	lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
	orr r2, r2, r3

	//Store new value
	str r2, [r1, #CMU_HFPERCLKEN0]

// Set up port A and port C
	ldr r0, =GPIO_PA_BASE
	ldr r1, =GPIO_PC_BASE
	
	port_a .req r0
	port_c .req r1

// Set high drive strength for LED
	mov r3, #0x2
	str r3, [port_a, GPIO_CTRL]

// Set LEDs to output.
	ldr r5, =0x55555555
	str r5, [port_a, #GPIO_MODEH]

// Set up buttons for input
	ldr r4, =0x33333333
	str r4, [port_c, #GPIO_MODEL]
	
// Enable internal pull-up
	ldr r6, =0xff
	str r6, [port_c, #GPIO_DOUT]

// enable GPIO Interrupts
	ldr r9, =GPIO_BASE
	gpiobase .req r9
	ldr r8, =0x22222222
	str r8, [gpiobase, #GPIO_EXTIPSELL]

	ldr r10, =0xff
	str r10, [gpiobase, #GPIO_EXTIFALL]

	str r10, [gpiobase, #GPIO_EXTIRISE]

	str r10, [gpiobase, #GPIO_IEN]

	ldr r10, =0x802
	ldr r11, =ISER0
	str r10, [r11]	

// Goes to sleep mode

	ldr r10, =SCR
	ldr r8, =6  
	str r8, [r10]
	wfi
	      
	b .   

	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  
	ldr r11, [gpiobase, #GPIO_IF]
	str r11, [gpiobase, #GPIO_IFC]

	ldr r7, [port_c, #GPIO_DIN]
	lsl r7, #8
	str r7, [port_a, #GPIO_DOUT]

	bx lr
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing
