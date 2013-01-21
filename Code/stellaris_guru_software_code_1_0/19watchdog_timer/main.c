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

#include "inc/hw_ints.h"

/* Defines and Macros for GPIO API */
#include "driverlib/gpio.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

#include "driverlib/adc.h"

#include "driverlib/uart.h"

#include "driverlib/interrupt.h"

#include "utils/uartstdio.h"

#include "driverlib/watchdog.h"


unsigned long temp;

void UartInit(void)
{
    //
    // Enable GPIO port A which is used for UART0 pins.

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);


    /* Make the UART pins be peripheral controlled. */
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O.

    UARTStdioInit(0);
}

//*****************************************************************************
//
// The interrupt handler for the watchdog.  This feeds the dog (so that the
// processor does not get reset) and winks the LED connected to GPIO B0.
//
//*****************************************************************************
void WatchdogIntHandler(void)
{
    //
    // Clear the watchdog interrupt.
    //
    WatchdogIntClear(WATCHDOG0_BASE);

    /* Toggle the GPIO PC5 value. */
    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5) ^ GPIO_PIN_5);

}

void blinky(void)
{
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
	SysCtlDelay(SysCtlClockGet()/36);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6);
	SysCtlDelay(SysCtlClockGet()/36);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7);
	SysCtlDelay(SysCtlClockGet()/36);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0);
	SysCtlDelay(SysCtlClockGet()/36);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

}

void Pin_Int(void)
	{
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);
		// Enable the watchdog interrupt.
		IntDisable(INT_WATCHDOG);
	}

int main(void)
{
	/*Set the clocking to directly run from the crystal at 8MHz*/
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	/* Set the clock for the GPIO Port C */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	/* Set the clock for the Watchdog */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

	/* Set the type of the GPIO Pin */
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
	
	/* GPIO Pins 5, 6, 7 on PORT C initialised to 0 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);

	/*Register Interrupt to call Interrupt Handler*/
	GPIOPortIntRegister(GPIO_PORTE_BASE, Pin_Int);

	/*Clear interrupt register*/
	GPIOPinIntClear(GPIO_PORTE_BASE, GPIO_PIN_0);

	/*Configure GPIO pad with internal pull-up enabled*/
	GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

	/*Set interrupt triggering sequence*/
	GPIOIntTypeSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_LOW_LEVEL);

	/*Enable interrupts on selected pin*/
	GPIOPinIntEnable(GPIO_PORTE_BASE, GPIO_PIN_0);

	/*Enable interrupts on selected port*/
	IntEnable(INT_GPIOE);

	/* Blinky */
    blinky();

	/* UART config */
	UartInit();

	UARTprintf("I just got RESET ! ");
	
	// Set the period of the watchdog timer.
    //
    WatchdogReloadSet(WATCHDOG0_BASE, SysCtlClockGet());

    // Enable reset generation from the watchdog timer.
    //
    WatchdogResetEnable(WATCHDOG0_BASE);

    // Enable the watchdog timer.
    //
    WatchdogEnable(WATCHDOG0_BASE);

    // Enable the watchdog interrupt.
    //
    IntEnable(INT_WATCHDOG);

    // Enable processor interrupts.
    //
    IntMasterEnable();

    while(1)
    {

	}

}
