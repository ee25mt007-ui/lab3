#include <stdint.h>
#include "tm4c123gh6pm.h"

void delay(int n){
    int i,j;
    for(i=1;i<n;i++){
       for(j=1;j<3180;j++){
       }
    }
}

int main(void) {
SYSCTL_RCGC2_R = 0x00000020;;
GPIO_PORTF_LOCK_R = 0x4C4F434B;
GPIO_PORTF_CR_R = 0x1F;
GPIO_PORTF_DIR_R = 0x0E;
GPIO_PORTF_DEN_R = 0x1F;
GPIO_PORTF_PUR_R = 0x11;

int count=0;

while(1){
if((GPIO_PORTF_DATA_R & 0x10)==0){
    if(count==0){
        GPIO_PORTF_DATA_R =0x02;
        delay(50);
        GPIO_PORTF_DATA_R &= ~0x04;
        GPIO_PORTF_DATA_R &= ~0x08;
        count++;
    }
    else if (count==1){
        GPIO_PORTF_DATA_R =0x08;
        delay(50);
        GPIO_PORTF_DATA_R &= ~0x02;
        GPIO_PORTF_DATA_R &= ~0x04;
        count++;
    }
    else if(count==2){
        GPIO_PORTF_DATA_R =0x04;
        delay(50);
        GPIO_PORTF_DATA_R &= ~0x02;
        GPIO_PORTF_DATA_R &= ~0x08;
        count++;
    }
    else {
        delay(50);
        GPIO_PORTF_DATA_R &= ~0x0E;
        count=0;
    }
}
}
}
