#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#include "pwm_init.h"

Uint32 DeadBand = 45;


void pwm_init(void)
{
    EALLOW;
      SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;  // Disable the clock input to epwm module
      EDIS;

      EPwm1Regs.TBCTL.all = 0 ;   // reset all bits in TBCTL



      EPwm1Regs.TBPRD = 1500; // Period = 1500 TBCLK counts frequency at 100khz

      EPwm1Regs.TBPHS.half.TBPHS = 0; //Phase register to zero



      EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;

      EPwm1Regs.TBCTL.bit.CLKDIV  = TB_DIV1;

      EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;

      EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;

      EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;

      EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;


      EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;

      EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;

      EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;

      EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


      EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;

      EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;

      EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;

      EPwm1Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;

      EPwm1Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;

      EPwm1Regs.DBCTL.bit.OUT_MODE = 0b11;

      EPwm1Regs.DBFED = 45.0; // FED = 45 TBCLKs 300ns of deadband
      EPwm1Regs.DBRED = 45.0; // RED = 45 TBCLKs


      EPwm1Regs.CMPA.half.CMPA = 750;//1500 divided by 2
        //////////////////////////////////////////////////////////////////
        // EPWM1 Interrupt
        EPwm1Regs.ETSEL.bit.INTEN = 1;        // Enable Interrupt
        EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRD; // Select Interrupt
        EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;   // Generate pulse on 1st event
        //////////////////////////////////////////////////////////////////
        //EPWM1 for ADC
        EPwm1Regs.ETSEL.bit.SOCAEN = 1;     // Enable SOC on A group
        EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTRU_CMPB;    // Select SOC from from CPMA on upcount
        EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;     // Generate pulse on 1st event


    // Enable clocks for PWM modules
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}
// Interrupt routines uses in this example:


