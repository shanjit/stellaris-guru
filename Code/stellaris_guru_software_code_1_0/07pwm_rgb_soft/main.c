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

void pin_pwm(unsigned long ulPort1, unsigned char ucPin1, unsigned long ulPort2, unsigned char ucPin2)
{
	volatile unsigned long count;

	volatile unsigned long on_time, off_time;

	for(on_time=10000, off_time =0 ;on_time>0 ;on_time=on_time-20,off_time=off_time+20)
		{
        GPIOPinWrite(ulPort1, ucPin1, ucPin1);
        GPIOPinWrite(ulPort2, ucPin2, 0);
        for(count=0;count<on_time;count++)
        {
		}
		GPIOPinWrite(ulPort1, ucPin1, 0);
		GPIOPinWrite(ulPort2, ucPin2, ucPin2);
		for(count=0;count<off_time;count++)
        {
		}
		}

		for(on_time=10000, off_time =0 ;on_time>0 ;on_time=on_time-20,off_time=off_time+20)
		{
        GPIOPinWrite(ulPort1, ucPin1, 0);
        GPIOPinWrite(ulPort2, ucPin2, ucPin2);
        for(count=0;count<on_time;count++)
        {
		}
		GPIOPinWrite(ulPort1, ucPin1, ucPin1);
		GPIOPinWrite(ulPort1, ucPin2, 0);
		for(count=0;count<off_time;count++)
        {
		}
		}
}

int main(void)
{
	/* Set the clocking to directly run from the crystal at 8MHz */
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	/* Set the clock for the GPIO Port C */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	/* Set the type of the GPIO Pin */
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	
	/* GPIO Pins 5, 6, 7 on PORT C initialized to 1
	 * All LEDs are off.
	 * */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	while(1)
    {
		/* Any possible combinations can be selected */
		pin_pwm(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PORTC_BASE, GPIO_PIN_6);
		pin_pwm(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PORTC_BASE, GPIO_PIN_7);
		pin_pwm(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PORTC_BASE, GPIO_PIN_5);
		pin_pwm(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PORTC_BASE, GPIO_PIN_7);
		pin_pwm(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PORTC_BASE, GPIO_PIN_5);
		pin_pwm(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PORTC_BASE, GPIO_PIN_6);
	}
		
}
