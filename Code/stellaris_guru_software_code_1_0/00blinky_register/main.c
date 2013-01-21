 /* Centre for Electronic Design and Technology, NSIT, New Delhi
 * 
 * Dhananjay V Gadre, dhananjay.gadre@gmail.com
 * Shanjit Singh Jajmann, shanjitsingh@gmail.com
 * Rohit Dureja, rohit.dureja@gmail.com
 * 
 */

#include "inc/lm3s608.h"

int main(void)
{
    volatile unsigned long ulLoop;

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOC;

    //
    // Do a dummy read to insert a few cycles after enabling the peripheral.
    //
    ulLoop = SYSCTL_RCGC2_R;// Burn the corresponding value into the clock control

    //
    // Enable the GPIO pin for the LED (PC5).  Set the direction as output, and
    // enable the GPIO pin for digital function.  Care is taken to not disrupt
    // the operation of the JTAG pins on PC0-PC3.
    //
    GPIO_PORTC_DIR_R |= 0xE0; //Set the direction.
    GPIO_PORTC_DEN_R |= 0xE0;
    GPIO_PORTC_AFSEL_R = 0x00;

    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
        //

        GPIO_PORTC_DATA_R |= 0xE0;
    //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
        {
        }

        GPIO_PORTC_DATA_R &= ~(0xE0);

        //
        // Delay for a bit.
        //
        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
        {
        }
    }
}
