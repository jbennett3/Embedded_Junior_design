#ifndef INCLUDE_ADC_ISR_H_
#define INCLUDE_ADC_ISR_H_

interrupt void adc_isr(void);
void adc_init(void);
void getAvg();

void ResetADC();
double getVoltage();
double getCurrent();

#endif /* INCLUDE_ADC_ISR_H_ */
