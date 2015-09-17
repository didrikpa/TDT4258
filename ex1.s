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

// Set high drive strength for LED
	mov r3, #0x2
	str r3, [r0, GPIO_CTRL]

// Set LED's to light
	ldr r5, =0x55555555
	str r5, [r0, #GPIO_MODEH]

// Set up buttons for pushing
	ldr r4, =0x33333333
	str r4, [r1, #GPIO_MODEL]
	
//
	ldr r6, =0xff
	str r6, [r1, #GPIO_DOUT]

// enable GPIO Interrupts
	ldr r9, =GPIO_BASE
	ldr r8, =0x22222222
	str r8, [r9, #GPIO_EXTIPSELL]

	ldr r10, =0xff
	str r10, [r9, #GPIO_EXTIFALL]

	str r10, [r9, #GPIO_EXTIRISE]

	str r10, [r9, #GPIO_IEN]

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
	ldr r9, =GPIO_BASE
	ldr r11, [r9, #GPIO_IF]
	str r11, [r9, #GPIO_IFC]

	ldr r7, [r1, #GPIO_DIN]
	lsl r7, #8
	str r7, [r0, #GPIO_DOUT]

	bx lr
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing
