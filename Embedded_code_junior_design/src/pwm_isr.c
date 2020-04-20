#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"


#include "main.h"
#include "pwm_isr.h"
#include "pwm_init.h"
#include "adc_isr.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

const double PI = 3.14159265358979323846;
// PWM ISR Variable
double d_t = 0;
double m_t = 0.25;
double theta = 0;
double delta_m_t = 0.001;
// MPPT Variables
double Voltage[2] = {0,0};
double Current = 0;
double Power[2] = {0,0};

int state = 0;

Uint32 pwm_counter = 0;
Uint32 xint_counter = 0;

void StateReset()
{
    //pwm_counter = 0;
    state = 0;
}


/*void MPPT()
{
    // Read Voltage and Current, and Calculate the Power
    Current = getCurrent();
    if(Power[0] == 0)
    {
        Voltage[0] = getVoltage();
        Power[0] = Voltage[0]*Current;
    }
    else
    {
        Voltage[1] = getVoltage();
        Power[1] = Voltage[1]*Current;
    }
    // if the power is greater than the old power
    if(Power[1] > Power[0]){
        // if the voltage is greater than old voltage increase m_t
        if(Voltage[1] > Voltage[0]){ m_t = m_t + delta_m_t;}
        // else decrease the m_t
        else {m_t = m_t - delta_m_t;}
    }
    // else
    else if(Power[1] < Power[0]){
        // if voltage greater than previous voltage, lower the m_t
        if(Voltage[1] > Voltage[0]){ m_t = m_t - delta_m_t; }
        // else, increase m_t
        else { m_t = m_t + delta_m_t; }
     }
    // set the voltage and power as the previous
    Voltage[0] = Voltage[1];
    Power[0] = Power[1];
}*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void vary_duty(void)
{


}
__interrupt void pwm_isr(void)
{
     float b[101] = {0};

    unsigned int i = 0,m=0,k=0;
    EALLOW;
    // save XINT interrupt and Disable all Interrupt except XINT
    Uint16 LastXINT = PieCtrlRegs.PIEIER12.all;
    IER &= M_INT12;
    PieCtrlRegs.PIEIER12.all &= M_INT1;
    // Re-enable Interrupts
    EINT;
   float a =sin(3.14159265358979323846*0.02*i);
    if (a>=0){
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR; // Set PWM2A on event A, up
    EPwm1Regs.AQCTLA.bit.CAD =AQ_SET; // Clear PWM2A on event B, down
    EPwm1Regs.CMPA.half.CMPA =b[i];
    }
    else {
    EPwm1Regs.AQCTLA.bit.CAU =AQ_SET; // Set PWM2A on event A, up
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR; // Clear PWM2A on event B, down
    EPwm1Regs.CMPA.half.CMPA=b[i];
    }
    i++;
    if (i==101){
    i=0;
    }
    EPwm1Regs.ETCLR.bit.INT = 1;
    EPwm1Regs.ETCLR.bit.INT = 1; // clearing interrupts
    DINT; // disable interrupts
    PieCtrlRegs.PIEIER12.all = LastXINT; // Restore XINT
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3; // Acknowledge this interrupt to get more from group 3
    EDIS;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
__interrupt void xint_isr(void)
{
    EALLOW;
    EPwm1Regs.TBCTL.bit.SWFSYNC = 1; // Software Force Sync Pulse
    // State Machine
        // INITIAL = 0, VALIDATE = -1 , RUN = 1
     if(state == 0)
     {
         // reset the xint_counter and validate
          xint_counter = 0;
          state = -1;
     }
     else if(state == -1)
     {
         xint_counter++;
         // check if greater than 20, meaning activate the PWM ISR
         if(xint_counter >= 20)
         {
            // change state to RUN
             state = 1;
             xint_counter = 0;
         }
     }
     else if(state == 1)
     {
         //pwm_counter = 0;
        // get the voltage avg
        getAvg();
        // reset the angle of sine wave
        theta = 0;
        // adjust the m_t with MPPT
        // MPPT();
     }
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12; // Acknowledge this interrupt to get more from group 12
    EDIS;
}


