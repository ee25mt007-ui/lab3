#include <stdint.h>
#include "tm4c123gh6pm.h"

void SysTick_Handler(void);
void GPIOF_Handler(void);
void SysTick_Init(void);
void PortF_Init(void);

int main(void) {
    PortF_Init();
    SysTick_Init();

    __asm("CPSIE I");  // Enable global interrupts

    while (1) {
        // Infinite loop
    }
}

void SysTick_Init(void) {
    NVIC_ST_CTRL_R = 0;                  // Disable SysTick during setup
    NVIC_ST_RELOAD_R = 16000000 - 1;     // 1-second reload (for 16 MHz clock)
    NVIC_ST_CURRENT_R = 0;               // Clear current value
    NVIC_ST_CTRL_R = 0x07;               // Enable SysTick with core clock and interrupts
}

void PortF_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;           // Enable clock for Port F
    while ((SYSCTL_PRGPIO_R & 0x20) == 0) {}; // Wait until Port F is ready

    GPIO_PORTF_LOCK_R = 0x4C4F434B;      // Unlock Port F
    GPIO_PORTF_CR_R = 0x1F;              // Allow changes to PF4-0

    GPIO_PORTF_DIR_R = 0x0E;             // PF1, PF2, PF3 as output; PF0, PF4 as input
    GPIO_PORTF_DEN_R = 0x1F;             // Enable digital I/O for PF4-0
    GPIO_PORTF_PUR_R = 0x11;             // Enable pull-up resistors on PF0 and PF4

    GPIO_PORTF_IS_R &= ~0x11;            // PF0, PF4 edge-sensitive
    GPIO_PORTF_IBE_R &= ~0x11;           // Not both edges
    GPIO_PORTF_IEV_R &= ~0x11;           // Falling edge trigger
    GPIO_PORTF_ICR_R = 0x11;             // Clear any prior interrupt
    GPIO_PORTF_IM_R |= 0x11;             // Unmask interrupts on PF0 and PF4

    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF3FFFFF) | 0x00A00000; // Set interrupt priority
    NVIC_EN0_R |= 0x40000000;            // Enable interrupt 30 (Port F)
}

void SysTick_Handler(void) {
    GPIO_PORTF_DATA_R ^= 0x02;           // Toggle PF1 (Red LED)
}

void GPIOF_Handler(void) {
    if (GPIO_PORTF_RIS_R & 0x01) {       // Check if PF0 caused interrupt
        GPIO_PORTF_ICR_R = 0x01;         // Clear interrupt flag
        GPIO_PORTF_DATA_R ^= 0x04;       // Toggle PF2 (Blue LED)
    }
    if (GPIO_PORTF_RIS_R & 0x10) {       // Check if PF4 caused interrupt
        GPIO_PORTF_ICR_R = 0x10;         // Clear interrupt flag
        GPIO_PORTF_DATA_R ^= 0x08;       // Toggle PF3 (Green LED)
    }
}
