#define _XTAL_FREQ 4000000
#include <xc.h>


#define AN0  1<<0
#define AN1  1<<1
#define AN2  1<<2
#define AN3  1<<3

#define relay   GP0
#define led     GP2 
#define switch1 GP5
#define switch2 GP3





#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)



unsigned int GetADCValue(unsigned char Channel)
{
	ADCON0 &= 0xf3;      // Clear Channel selection bits

	switch(Channel)
	{
		//case AN0:	ADCON0 |= 0x00; break;      // Select GP0 pin as ADC input
		case AN1:	ADCON0 |= 0x04; break;      // Select GP1 pin as ADC input
		//case AN2:	ADCON0 |= 0x08; break;      // Select GP2 pin as ADC input
		case AN3:	ADCON0 |= 0x0c; break;      // Select GP4 pin as ADC input

		default:	return 0; 					//Return error, wrong channel selected
	}
    
    __delay_ms(10);      // Time for Acqusition capacitor to charge up and show correct value

	GO_nDONE  = 1;		 // Enable Go/Done

	while(GO_nDONE);     //wait for conversion completion

	return ((ADRESH<<8)+ADRESL);   // Return 10 bit ADC value
}


/*void delays(int a)
{
    int i;
    for(i=0;i<a;i++)
    {
        __delay_ms(1000);
    }

}
void delaymin(int a)
{
    int i; 
    a = a * 60;
    for(i=0;i<a;i++)
    {
        __delay_ms(1000);
    }

}*/

void main()
{	
    
    TRISIO0   	 = 	0;
	TRISIO4		 =	1;
	TRISIO3   	 = 	1;
	TRISIO5		 =	1;
	
	TRISIO1		 =	1;
    TRISIO2   	 = 	0;
	//GPIO  		 = 	0X00;
	ANSEL 		 = 	0X09;
	CMCON  		 = 	0X07;
	ADCON0 		 = 	0X83;
	VRCON        = 	0X00;
    OPTION_REG = 0x07;
    TMR0 = 0x00;
  
    
    
    
    
    
    
    
    
    int count = 0;
    led = 0;
	unsigned int ADC_value = 0;
    unsigned int ADC_value1 = 0;
    
	unsigned int digit1, digit2, digit3, digit4;

	//InitADC(AN3);		// Initialize GP4 as ADC input pin
	//InitADC(AN1);
//    timer();
	while(1)
	{
		
		//ADC_value = GetADCValue(AN3);		// Read ADC value from GP4 pin
        //ADC_value1 = GetADCValue(AN1);
        
        /*
        if(T0IF)
        {
            T0IF = 0;
            count++;
            if(count>=76)             //912 for 60 sec;1min;
            {
               led = 1; 
               if(count>=152)
               {
                   led = 0;
                   
                   if(count>1824)
                   {
                       led = 1; 
                       if(count>13500)
                       {
                           led = 0;
                           count=0;
                       }
                   }
               }
               
            }
            else
            {
                led = 0;  
            
            }
        }*/
        led = 1;
        __delay_ms(500);
        led = 0;
        __delay_ms(500);
		/*if(switch1 == 1)
        {
           led = 1; 
         }
         else
          {
             led = 0;
          }*/ 
        
       /* if((ADC_value>300)&&(ADC_value1>600))
        {
            led= 1;
        }
        else
        {
            led = 0;
        }*/
        
        
        
    }
}