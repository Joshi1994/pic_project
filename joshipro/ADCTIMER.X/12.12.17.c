#include <xc.h>
#define _XTAL_FREQ 4000000


#define relay   GP0
#define led     GP2 
#define switch1 GP5
#define switch2 GP3



// CONFIG
#pragma config FOSC = INTRCIO   //  (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)


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
  
    
  
    int ADC1 = 0;
    int ADC2 = 0;
    int TIMERM = 0;
    int TIMERS = 0;
    int count = 0;
    int counts = 0;
    int off =0;
    int ledf = 0;
    
    
    
    
    
    int count = 0;
    led = 0;
	unsigned int ADC_value = 0;
    unsigned int ADC_value1 = 0;
    
	unsigned int digit1, digit2, digit3, digit4;
    
    
    while(1)
    {
        
                        ADC1 = GetADCValue(AN3);		// min
                       // ADC1 = ADC1 * 0.005;
                        ADC2 = GetADCValue(AN1);        //sec
                       // ADC2 = ADC2 * 0.005;
                        
                        if((ADC1>0)&&(ADC1<200))
                        {
                            
                            TIMERM = 1035;                   //1min
                            
                        }
                        
                        if((ADC1>200)&&(ADC1<400))
                        {
                            
                            TIMERM = 1950;                  //2min
                        }
                        if((ADC1>400)&&(ADC1<600))
                        {
                            TIMERM = 2865;                  //3min
                        }
                        if((ADC1>600)&&(ADC1<800))
                        {
                            TIMERM = 3780;                  //4min
                        }
                        if((ADC1>800)&&(ADC1<=1024))
                        {
                            TIMERM = 4695;                    //5min
                        }
                        
                        
                        
                        if((ADC2>0)&&(ADC2<100))
                        {
                            
                            

                            TIMERS = 15;                        //1sec
                        }
                        if((ADC2>100)&&(ADC2<200))
                        {
                            
                            TIMERS = 30;                        //2sec
                        }
                        if((ADC2>200)&&(ADC2<300))
                        {
                            
                            
                            TIMERS = 45;                        //3sec
                        }
                        if((ADC2>300)&&(ADC2<400))
                        {
                            TIMERS = 60;                        //4sec
                        }
                        if((ADC2>400)&&(ADC2<500))
                        {
                            TIMERS = 75;                        //5sec
                        }
                        if((ADC2>500)&&(ADC2<600))
                        {
                            TIMERS = 90;                        //6sec
                        }
                        if((ADC2>600)&&(ADC2<700))
                        {
                            TIMERS = 105;                      //7sec
                        }
                        if((ADC2>700)&&(ADC2<800))
                        {
                            TIMERS = 120;                      //8sec
                        }
                        if((ADC2>800)&&(ADC2<900))
                        {
                            TIMERS = 135;                       //9sec
                        }
                        if((ADC2>900)&&(ADC2<=1024))
                        {
                            TIMERS = 150;                       //10sec
                        }
                    
                        
                        if(T0IF)
                        {
                            T0IF = 0;
                            count++;
                            if(count<TIMERM)
                            {
                                if(ledf == 0)
                                {
                                led = 1;
                                relay = 1;
                                ledf = 1;
                                }
                               
                            
                            }
                            else
                            //if(count == TIMERM)
                            {
                                led = 0;
                                relay = 0;
                                //ledf = 0;
                                //off = 1;
                                ledf = 0;
                                counts++;
                            }
                            //if(off == 1)
                            {
                              //  counts++;
                            }
                            if(counts >=TIMERS)
                            {
                                count = 0;
                                counts = 0;
                                off = 0;
                            }
                            
                                    
                        }
                        
                       
                        
                        /*    
                       
                    if(T0IF)
                    {
                        T0IF = 0;
                        

                        counts++;
                        if(counts<TIMERS)   //ON TIME
                        {
                            led = 1;

                        }
                        if(counts == TIMERS)
                        {
                            led = 0;
                            off = 1;
                        }
                        if(off == 1)
                        {
                            count++;
                        }
                        if(count >= TIMERM)
                        {
                            off = 0;
                            count = 0;
                            counts=0;
                            
                        }
                        
                    }*/
                
        	} 
 
    
}