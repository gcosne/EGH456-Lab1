//*****************************************************************************
//Modified version of timers.c
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "grlib/grlib.h"
#include "drivers/kentec320x240x16_ssd2119.h"
#include "drivers/pinout.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Timer (timers)</h1>
//!
//! One timer is set up to interrupt once per second toggling the LED ON or OFF.
//
//*****************************************************************************

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator.
//
//*****************************************************************************
uint32_t g_ui32Flags;
volatile uint8_t ledState=0;


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// The interrupt handler for the timer interrupt.
//
//*****************************************************************************

void
Timer0IntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Toggle the flag for the first timer.
    //
    HWREGBITW(&g_ui32Flags, 0) ^= 1;

    //
    // Update the LED State.
    //
    ROM_IntMasterDisable();


    //Here we toggle the LedState reading the value of PIN7
    ledState^=GPIO_PIN_7;
    GPIOPinWrite(GPIO_PORTQ_BASE,GPIO_PIN_7, ledState);

    ROM_IntMasterEnable();
}

int
main(void)
{
    //""""""""""""""Blinky portion of code""""""""""""""
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

    //
    // Check if the peripheral access is enabled.
    //
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOQ))
    {
    }

    //
    // Enable the GPIO pin for the LED (PQ7).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_7);

    //
    //Initialize the led OFF
    //

    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, 0x0);

    //""""""""""""end Blinky portion of code""""""""""""


    uint32_t ui32SysClock;

    //
    // Run from the PLL at 120 MHz.
    //
    ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                           SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                           SYSCTL_CFG_VCO_480), 120000000);

    //
    // Configure the device pins.
    //

    PinoutSet();


    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    //
    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();

    //
    // Configure the 32-bit periodic timer.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ui32SysClock);

    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timer.
    //
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    //
    // Loop forever while the timer run.
    //
    while(1)
    {
        // Do we need to put something here ?
    }
}
