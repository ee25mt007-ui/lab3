#include <stdint.h>
#include "tm4c123gh6pm.h"

volatile int pwmLevel = 160;  

void PWM_Timer0A_Init(void) {
    SYSCTL_RCGCTIMER_R |= 0x01;     
    SYSCTL_RCGCGPIO_R  |= 0x02;     
    GPIO_PORTB_AFSEL_R |= 0x40;     
    GPIO_PORTB_PCTL_R  = (GPIO_PORTB_PCTL_R & ~0x0F000000) | 0x07000000; 
    GPIO_PORTB_DEN_R   |= 0x40;    

    TIMER0_CTL_R &= ~0x01;          
    TIMER0_CFG_R = 0x04;           
    TIMER0_TAMR_R = 0x0A;          
    TIMER0_TAILR_R = 319;           
    TIMER0_TAMATCHR_R = pwmLevel;  
    TIMER0_CTL_R |= 0x01;          
}

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;       
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   
    GPIO_PORTF_CR_R   = 0x1F;        
    GPIO_PORTF_DIR_R &= ~0x11;        
    GPIO_PORTF_DEN_R |= 0x11;        
    GPIO_PORTF_PUR_R |= 0x11;         
}

int main(void) {
    PortF_Init();
    PWM_Timer0A_Init();

    while (1) {
       
        if ((GPIO_PORTF_DATA_R & 0x10) == 0) {
            if (pwmLevel < 319) pwmLevel += 16;  
            TIMER0_TAMATCHR_R = pwmLevel;
            while ((GPIO_PORTF_DATA_R & 0x10) == 0);  
        }

        
        if ((GPIO_PORTF_DATA_R & 0x01) == 0) {
            if (pwmLevel > 0) pwmLevel -= 16;  
            TIMER0_TAMATCHR_R = pwmLevel;
            while ((GPIO_PORTF_DATA_R & 0x01) == 0);  
    }
}