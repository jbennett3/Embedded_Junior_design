#ifndef INCLUDE_PWM_ISR_H_
#define INCLUDE_PWM_ISR_H_
//#define pi = 3.14159265358979323846
__interrupt void xint_isr(void);
__interrupt void pwm_isr(void);
void initepwm2example(void);
//_interrupt void epwm2_isr(void);

#endif /* INCLUDE_PWM_ISR_H_ */
