#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include<math.h>
#include <stdio.h>
#include "main.h"
#include "gpio_init.h"
#include "pwm_init.h"
#include "pwm_isr.h"
#include "adc_isr.h"

/******************************* Local #defines *******************************/


/******************************* Local typedefs *******************************/


/************************* Local function prototypes **************************/

/********************* Code sections to be copied to RAM **********************/


/****************************** Local structures ******************************/


/************************ Global variable definitions *************************/

void main(void)
{


    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
     InitSysCtrl();

#ifdef FLASH_LINK
    // Copy time critical code and Flash setup code to RAM
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash();
#endif
    // Set the High-Speed Peripheral Clock Prescaler (HISPCP) such that HSPCLK = 25 MHz
    // HSPCLK = SYSCLKOUT/(HISPCP x 2)
    EALLOW;
#if CPU_FRQ_150MHZ
    SysCtrlRegs.HISPCP.all = 3;
#else
    SysCtrlRegs.HISPCP.all = 2;
#endif
    EDIS;
    // Step 2. Initalize GPIO:
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    DINT;

    gpio_init();

    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    InitPieCtrl();
    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    InitPieVectTable();
    // Step 4. Initialize all the Device Peripherals:
    //////////////////////////////////////////////////////////////////////////////////
    EALLOW;
    // the 3 necessary ISR
    PieVectTable.XINT3 = &xint_isr;
    PieVectTable.EPWM1_INT = &pwm_isr;
    PieVectTable.ADCINT = &adc_isr;
    EDIS;
    //////////////////////////////////////////////////////////////////////////////////
    // PWM Initilization
    getAvg();
    pwm_init();
    adc_init();
    //////////////////////////////////////////////////////////////////////////////////
    // Enabling the CPU INTERRUPTS
    IER|=M_INT12; // XINT CPU Group
    IER|=M_INT3; // PWM_ISR CPU Group
    IER|=M_INT1; // ADC_ISR CPU GROUP
    //////////////////////////////////////////////////////////////////////////////////
    // Enabling all the Specific Interrupts
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;  // Enable the PIE block
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1; // XINT
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;  // PWM
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;  // ADC
    //////////////////////////////////////////////////////////////////////////////////
    // Step 5. User specific code, enable interrupts:
    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
    //////////////////////////////////////////////////////////////////////////////////
    while (1)
    {
        asm(" NOP");

    }
}

