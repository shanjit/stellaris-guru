 /* Centre for Electronic Design and Technology, NSIT, New Delhi
 * 
 * Dhananjay V Gadre, dhananjay.gadre@gmail.com
 * Shanjit Singh Jajmann, shanjitsingh@gmail.com
 * Rohit Dureja, rohit.dureja@gmail.com
 * 
 */

/* Defines the base address of the memories and peripherals */
#include "inc/hw_memmap.h"

/* Defines the common types and macros */
#include "inc/hw_types.h"

/* Defines and Macros for GPIO API */
#include "driverlib/gpio.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

/* Prototypes for the NVIC Interrupt Controller Driver. */
#include "driverlib/interrupt.h"

/*  Macros that define the interrupt assignment on Stellaris. */
#include "inc/hw_ints.h"

/* Driver for the SysTick timer in NVIC. */
#include "driverlib/systick.h"

volatile unsigned long g_ulcount = 0;

void Pin_Int(void)
	{
		GPIOPinIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);

		if(g_ulcount==5)
		{
		g_ulcount=0;
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
		SysCtlDelay(SysCtlClockGet() / 12);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
		SysCtlDelay(SysCtlClockGet() / 12);
		}
		g_ulcount++;
	}

int main(void)
{
	/* Set the clocking to directly run from the crystal at 8MHz */
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	/* Set the clock for the GPIO Port C and E */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	/* Set the type of the GPIO Pin */
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
	
	/*Configure GPIO pad with internal pull-up enabled*/
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

	/* GPIO Pins 5, 6, 7 on PORT C initialized to 1
	 * All LEDs off.
	 * */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	
	/*Register Interrupt to call Interrupt Handler*/
	GPIOPortIntRegister(GPIO_PORTE_BASE, Pin_Int);

	/*Clear interrupt register*/
	GPIOPinIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);

	/*Set interrupt triggering sequence*/
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);

	/*Enable interrupts on selected pin*/
	GPIOPinIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0);

	/*Enable interrupts on selected port*/
	IntEnable(INT_GPIOE);

	/*Enable global interrupts*/
	IntMasterEnable();
	
	while(1)
    {
		
    }

}
