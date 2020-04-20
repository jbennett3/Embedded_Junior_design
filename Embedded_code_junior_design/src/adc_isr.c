#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "main.h"
#include "adc_isr.h"

Uint16 DetectedADCVoltage = 0;
Uint16 DetectedADCCurrent = 0;

Uint32  RunningVoltageSum = 0;
Uint32  RunningCurrentSum = 0;
Uint32  adc_counter = 0;

float VAvg = 0;
float CAvg = 0;
double c_b = -0.5965;
double c_m = 3.354;
double v_b = -0.025605;
double v_m = 14.224751066857;
float outputVoltage = 0;
float outputCurrent = 0;

void adc_init(void)
{
    InitAdc();
    EALLOW;
    // Enable ADC SEQ1
    AdcRegs.ADCMAXCONV.all = 0x0001;       // Setup 2 conv's on SEQ1
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0; // Setup ADCINA0 as 1st SEQ1 conv - Voltage Sensor
    AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1; // Setup ADCINA1 as 2nd SEQ1 conv - Current Sensor
    // Enable SOCA from ePWM to start SEQ1
    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
    EDIS;

}

void getAvg()
{
    VAvg = (RunningVoltageSum / adc_counter);
    CAvg = (RunningCurrentSum / adc_counter);
    // Voltage Sensor
    if(DetectedADCVoltage < 8)
    {
        outputVoltage = 0;
    }
    else
    {
        // Digital Value = 4096* (Input Analog Voltage-ADCLO)/3
        outputVoltage = v_m*(VAvg*(3)/4096)+v_b;
    }
    // Current Sensor
    if(DetectedADCCurrent < 6)
    {
        outputCurrent = 0;
    }
    else
    {
        // Digital Value = 4096* (Input Analog Voltage-ADCLO)/3
        outputCurrent = c_m*(CAvg*3/4096)+c_b;
    }

}

double getVoltage()
{
    return outputVoltage;
}

double getCurrent()
{
    return outputCurrent;
}

void ResetADC()
{
    // Reset all the Major ADC Variables
    DetectedADCVoltage = 0;
    DetectedADCCurrent = 0;
    VAvg = 0;
    CAvg = 0;
    RunningVoltageSum = 0;
    RunningCurrentSum = 0;
    adc_counter = 0;
}

__interrupt void
adc_isr(void)
{
    EALLOW;
    // Get the Data from AdcRegs
    DetectedADCVoltage = AdcRegs.ADCRESULT0 >> 4;
    DetectedADCCurrent = AdcRegs.ADCRESULT1 >> 4;
    // For Averaging
    RunningVoltageSum += DetectedADCVoltage;
    RunningCurrentSum += DetectedADCCurrent;
    // ADC Counter
    adc_counter++;
    // Reinitialize for next ADC sequence
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
    EDIS;
    return;
}
