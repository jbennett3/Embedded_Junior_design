#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#include "gpio_init.h"
#include "main.h"

void gpio_init(void)
{
	EALLOW;
	// PWM GPIO Config
	   GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;  // GPIO0 = PWM1A
	   GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;  // GPIO1 = PWM1B

	// SHUTDOWN Pin
	   GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;   // Enable pullup on GPIO4
	   GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  // GPIO4 = GPIO4
	   GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;
	   GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
	// GPIO XINT
	   GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 6; // Sets the Sampling window
	   GpioCtrlRegs.GPBPUD.bit.GPIO32 = 1; // Pullup resistor
	   GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1; // Mux
	   GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0; // Sets the direction on GPIO32 as input
	   GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 2; //XINT3 Qual using 6 samples
	   GpioIntRegs.GPIOXINT3SEL.bit.GPIOSEL = 0; // Sets GPIO32 = XINT3
	// XINT Setup
	   XIntruptRegs.XINT3CR.bit.POLARITY = 1; // Rising Edge
	   XIntruptRegs.XINT3CR.bit.ENABLE = 1;  // Enable XINT3
	EDIS;
}
