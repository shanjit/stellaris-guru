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

/* ADC headers for using the ADC driver functions. */
#include "driverlib/adc.h"

/* Defines and Macros for the UART. */
#include "driverlib/uart.h"

/* Prototypes for the NVIC Interrupt Controller Driver. */
#include "driverlib/interrupt.h"

/* Utility driver to provide simple UART console functions. */
#include "utils/uartstdio.h"

void InitConsole(void)
{
    /* Enable GPIO port A which is used for UART0 pins. */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Make the UART pins be peripheral controlled. */
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Initialize the UART for console I/O. */
    UARTStdioInit(0);
}

int main(void)
{
	unsigned long temp[1];
	/*Set the clocking to directly run from the crystal at 8MHz*/
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

	/*Enable ADC Peripheral*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	/* Set the clock for the GPIO Port C */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	/* Set the type of the GPIO Pin */
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
	
	/* GPIO Pins 5, 6, 7 on PORT C initialised to 0 */
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);
	
	
	/* UART config */
	InitConsole();

	/*Configure ADC Peripheral*/
	/*Configure ADC Sequence*/
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);


	/*Enable ADC sequence*/
	ADCSequenceEnable(ADC0_BASE, 3);

	/*Clear ADC Interrupt*/
	ADCIntClear(ADC0_BASE, 3);

	/* Interrupt Master Enable */
	IntMasterEnable();

    while(1)
    {

	ADCProcessorTrigger(ADC0_BASE, 3);
				while(!ADCIntStatus(ADC0_BASE, 3, false))
				{
				}
				ADCIntClear(ADC0_BASE, 3);
				ADCSequenceDataGet(ADC0_BASE, 3, temp);

	temp[0] = (temp[0] * 330)/1024; //change formula
	UARTprintf("Temperature is : %04d\n", temp[0]);
	SysCtlDelay(SysCtlClockGet() / 12);

	if(temp[0] > 800)
	{
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

	}

	else if(temp[0] > 700)
	{
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);
	}

	else if(temp[0] > 500)
	{
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6, 0);
	}

	else {
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, 0);
	}
	}


	}


