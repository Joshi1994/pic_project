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
#define ON      1
#define OFF     0



#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
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
  
    
   int initial = 0;
    int repeat = 0;
    float ADC1 = 0;
    float ADC2 = 0;
    int TIMERM = 0;
    int TIMERS = 0;
    int count = 0;
    int counts = 0;
    
    
    
    
    
    
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
                        ADC1 = GetADCValue(AN3);		// min
                        ADC1 = ADC1 * 0.005;
                        ADC2 = GetADCValue(AN1);        //sec
                        ADC2 = ADC2 * 0.005;
                        
                        if(ADC1 == 1)
                        {
                            led = ON;
                            __delay_ms(1000);
                            led = OFF;
                        }
                        if(ADC1 == 2)
                        {
                            led = ON;
                            __delay_ms(2000);
                            led = OFF;
                        }
                        if(ADC1 == 3)
                        {
                            led = ON;
                            __delay_ms(3000);
                            led = OFF;
                        }
                        if(ADC1 == 4)
                        {
                            led = ON;
                            __delay_ms(4000);
                            led = OFF;
                        }
                        if(ADC1 == 5)
                        {
                            led = ON;
                            __delay_ms(5000);
                            led = OFF;
                        }
                }
}
		
		/*
        
        if(initial == 0)
        
        {
            initial = 1;
            if(repeat == 1)
            {
                
                while(1)
                {       start:
                        ADC1 = GetADCValue(AN3);		// min
                ADC1 = ADC1 * 0.005;
                        ADC2 = GetADCValue(AN1);        //sec
                        ADC2 = ADC2 * 0.005;
                        if(ADC1 <=200)
                        {
                            __delay_ms(250);
                            if(ADC1 <=200)
                            TIMERM = 915;
                            //break;
                        }
                        else if(ADC1 <= 400)
                        {
                            __delay_ms(250);
                            if(ADC1 <= 400)
                            TIMERM = 1830;
                            //break;
                        }
                        else if(ADC1 <= 600)
                        {
                            __delay_ms(250);
                            if(ADC1 <= 600)
                            TIMERM = 2745;
                            //break;
                        }
                        else if(ADC1 <= 800)
                        {
                            __delay_ms(250);
                            if(ADC1 <= 800)
                            TIMERM = 3660;
                            //break;
                        }
                        else if(ADC1 <= 1024)
                        {
                            __delay_ms(250);
                            if(ADC1 >= 1000)
                            TIMERM = 4575;
                            //break;
                        }



                        if(ADC2 <100)
                        {
                            __delay_ms(250);
                            if(ADC2 <=200)
                            TIMERS = 915;
                            //break;
                        }
                        else if(ADC2 <= 200)
                        {
                            __delay_ms(250);
                            if(ADC2 <= 400)
                            TIMERS = 1830;
                            //break;
                        }

                        else if(ADC2 <= 300)
                        {
                            __delay_ms(250);
                            if(ADC2 <= 600)
                            TIMERS = 2745;
                            //break;
                        }
                        else if(ADC2 <= 400)
                        {
                            __delay_ms(250);
                            if(ADC2 <= 800)
                            TIMERS = 3660;
                            //break;
                        }
                        else if(ADC2 <= 5000)
                        {
                            __delay_ms(250);
                            if(ADC2 >= 1000)
                            TIMERS = 4575;
                            //break;
                        }
                        else if(ADC2<=600)
                        {
                            __delay_ms(250);
                            if(ADC2 <=200)
                            TIMERS = 915;
                            //break;
                        }

                        else if(ADC2 <= 700)
                        {
                            __delay_ms(250);
                            if(ADC2 <= 400)
                            TIMERS = 1830;
                            //break;
                        }
                        else if(ADC2 <= 800)
                        {
                            __delay_ms(250);
                            if(ADC2 <= 600)
                            TIMERS = 2745;
                            //break;
                        }
                        else if(ADC2 <= 900)
                        {
                            __delay_ms(250);
                            if(ADC1 <= 800)
                            TIMERS = 3660;
                            //break;
                        }
                        else if(ADC2 <= 1024)
                        {
                            __delay_ms(250);
                            if(ADC2 >= 1000)
                            TIMERS = 4575;
                            //break;
                        }
                }
            }
            else 
            {
                goto start;
            }
        }
        
        /*switch (ADC1)
            case 200:   TIMERM = 915;  break;
            case 400:   TIMERM = 1830; break;
            case 600:   TIMERM = 2745; break;
            case 800:   TIMERM = 3660; break;
            case 1000:  TIMERM = 4575; break;
           
        switch (ADC2)
            case 100:   TIMERS = 15;  break;
            case 200:   TIMERS = 30;  break;
            case 300:   TIMERS = 45;  break;
            case 400:   TIMERS = 60;  break;
            case 500:   TIMERS = 75;  break;
            case 600:   TIMERS = 90;  break;
            case 700:   TIMERS = 105; break;
            case 800:   TIMERS = 120; break;
            case 900:   TIMERS = 135; break;
            case 1000:  TIMERS = 150; break;
            
            
            
        
        
        
           if(initial == 1)
           {
                    if(T0IF)
                    {
                        T0IF = 0;
                        if(switch1 == 1)
                        {
                            initial = 0;
                            count = 0;
                            counts = 0;
                            repeat = 1;
                        }

                        count++;
                        if(count<TIMERM)
                        {
                            led = ON;

                        }
                        if(count>=TIMERM)
                        {
                            led = OFF;
                            counts++;
                        }
                        if(counts>=TIMERS)
                        {
                            count=0;
                            counts = 0;

                        }
                    }
                
                }
            
               
            }
            
        }
        
		*/
    