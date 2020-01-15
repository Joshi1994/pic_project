#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
//---------------------------------------------------------------------------------------------------------
#include <xc.h>
#define _XTAL_FREQ 4000000
//---------------------------------------------------------------------------------------------------------
#define led      GP2
#define relay    GP0
#define switch1  GP5
#define switch2  GP3
//---------------------------------------------------------------------------------------------------------

int anread1()                                       //reading AN3 pin
{
    ADCON0 = 0x1F;
    __delay_ms(10);
    GO_nDONE  = 1;                                  // Enable Go/Done
    while(GO_nDONE);                                //wait for conversion completion
    return ((ADRESH<<8)+ADRESL);
}
int anread2()                                       ////reading AN1 pin
{
    ADCON0 = 0x17;
    __delay_ms(10);
    GO_nDONE  = 1;                                  // Enable Go/Done
    while(GO_nDONE);                                //wait for conversion completion
    return ((ADRESH<<8)+ADRESL);
}
void main(void) 
{
    TRISIO0   	 = 	0;
    TRISIO1		 =	1;
    TRISIO2   	 = 	0;
	TRISIO4		 =	1;
	TRISIO3   	 = 	1;
	TRISIO5		 =	1;
  
    ANSEL 		 = 	0X09;
	CMCON  		 = 	0X07;
	ADCON0 		 = 	0X83;
	VRCON        = 	0X00;
    OPTION_REG   =  0x07;
    TMR0         =  0x00;
  
    
    int count    = 0;
    
    unsigned int aread1 = 0;
    unsigned int aread2 = 0;
    
    led = 0;
    relay = 0;
    while(1)
    {
        //aread1 = anread1();                                 //AN3 pin
        //aread2 = anread2();                                 //AN1 pin
        
        if(!T0IF)
        {
            T0IF = 0;
            count++;
            if(count < 30)
            {
                led = 1;
            }
            else
            {
                led = 0;
                
            }
        }
    }
    
    
    
    
}
