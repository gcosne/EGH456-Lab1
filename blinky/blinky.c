//*****************************************************************************
//
// timers.c - Timers example.
//
// Copyright (c) 2013-2016 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 2.1.3.156 of the DK-TM4C129X Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "utils/sine.h"
#include "inc/tm4c129xnczad.h" // WARNING should not be imported
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "grlib/grlib.h"

volatile int state = 0;

//*****************************************************************************
//
//! addtogroup example_list
//! <h1>Timer (timers)</h1>
//!
//! This example application demonstrates the use of the timers to generate
//! periodic interrupts.  One timer is set up to interrupt once per second and
//! the other to interrupt twice per second; each interrupt handler will toggle
//! its own indicator on the display.
//
//*****************************************************************************

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator as displayed
// on the display.
//
//*****************************************************************************
uint32_t g_ui32Flags;

//*****************************************************************************
//
// Graphics context used to show text on the display.
//
//*****************************************************************************
tContext g_sContext;

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
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void
Timer0IntHandler(void)
{
    //
    // Turn on the LED.
    //
    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, GPIO_PIN_7);

    /*if(state == 0)
    {
        GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, GPIO_PIN_7);
        state = 1;
    }
    else if(state == 1)
    {
        GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, 0x0);
        state = 0;
    }*/

    //
    // Clear the timer interrupt.
    //
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Toggle the flag for the first timer.
    //
    HWREGBITW(&g_ui32Flags, 0) ^= 1;

    //
    // Update the interrupt status on the display.
    //
    IntMasterDisable();

    IntMasterEnable();
}

//*****************************************************************************
//
// This example application demonstrates the use of the timers to generate
// periodic interrupts.
//
//*****************************************************************************

int
main(void)
{
    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION))
    {
    }

    //
    // Enable the GPIO pin for the LED (PQ7).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_7);

    //
    // Turn off the LED.
    //
    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, 0x0);

    uint32_t ui32SysClock;

    //
    // Run from the PLL at 120 MHz.
    //
    ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                           SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                           SYSCTL_CFG_VCO_480), 120000000);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Configure the two 32-bit periodic timers.
    //
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32SysClock);
    TimerLoadSet(TIMER1_BASE, TIMER_A, ui32SysClock / 2);

    //
    // Setup the interrupts for the timer timeouts.
    //
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    //
    // Enable the timers.
    //
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_A);

    //
    // Loop forever while the timers run.
    //
    while(1)
    {
    }
}



/*
//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2013-2016 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.3.156 of the DK-TM4C129X Firmware Package.
//
//*****************************************************************************

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/tm4c129xnczad.h"
#include "driverlib/interrupt.h"

#include "driverlib/rom.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "grlib/grlib.h"

uint32_t g_ui32Flags;
tContext g_sContext;

// The interrupt handler for the first timer interrupt.
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

    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, GPIO_PIN_7);
    //
    //GPIOPinRead(GPIO_PORTQ_BASE, GPIO_PIN_7);
    //GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, !GPIOPinRead(GPIO_PORTQ_BASE, GPIO_PIN_7));
    //GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, ^GPIO_PIN_7);

    ROM_IntMasterEnable();
}


//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED.
//
//*****************************************************************************

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************
int
main(void)
{
    //
    // Enable the GPIO pin for the LED (PQ7).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_7);
    //GPIOPinTypeGPIOInput(GPIO_PORTQ_BASE, GPIO_PIN_7);

    uint32_t ui32SysClock;

    //
    // Run from the PLL at 120 MHz.
    //
    ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                               SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                               SYSCTL_CFG_VCO_480), 120000000);



    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    // Enable processor interrupts.
    //
    ROM_IntMasterEnable();


    // Configure the 32-bit periodic timer.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ui32SysClock);

    // Setup the interrupts for the timer timeouts.
    //
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_7);
    while(1)
                {
                GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, GPIO_PIN_7);
                }

    //ROM_TimerEnable(TIMER0_BASE, TIMER_A);


    volatile uint32_t ui32Loop;

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




       ROM_TimerEnable(TIMER0_BASE, TIMER_A);

    while(1){}

    //
    // Loop forever.
    //
    while(1)
    {
        //
        // Turn on the LED.
        //
        GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, GPIO_PIN_7);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }

        //
        // Turn off the LED.
        //
        GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, 0x0);

        //
        // Delay for a bit.
        //
        for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
        {
        }
    }
}
*/
