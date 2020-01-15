#define _XTAL_FREQ 4000000
#define D1      GPIO5
#define D2      GPIO3
#define relay   GPIO1
#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config OSC = IntRC      // Oscillator Selection bits (internal RC oscillator)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled)
#pragma config CP = OFF         // Code Protection bit (Code protection off)
#pragma config MCLRE = OFF      // GP3/MCLR Pin Function Select bit (GP3/MCLR pin function is digital input, MCLR internally tied to VDD)





void main(void) {
   
    TRISGPIO = 0X02;
    OPTION = 0x80;
    
    
    if((!D1)||(!D1))
    {
        __delay_ms(1000);
        if((!D1)||(!D1))
        {
            relay = 1;
        
            else
            {
                relay = 0;
            }
                
            }
    }
    else
    {
        
        __delay_ms(1000);
        
        if((!D1)||(!D1))
        {
            relay = 1;
        }
        relay = 0;
    }
    return;
}
