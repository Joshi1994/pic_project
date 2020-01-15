#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//--------------------------------------------------------------------------------------------------------------

#include <xc.h>
#define _XTAL_FREQ 4000000

//---------------------------------------------------------------------------------------------------------------

#define dsensor1        RB4
#define dsensor2        RB5
#define buzzer          RA3
#define stprelay1       RC0
#define stprelay2       RC1
#define blwrelay        RA6
#define lrelay          RA7
#define dswitch1        RB6
#define dswitch2        RB7
//----------------------------------------------------------------------------------------------------------------
#define ON              1
#define OFF             0
//----------------------------------------------------------------------------------------------------------------
int count = 0;
int i =0;

void main(void) 
{
    TRISA       =   0x00;
    TRISB       =   0xFF;
    TRISC       =   0x00;
    
    ANSEL       =   0x00;
    ANSELH		= 	0X00;
    WPUB		=	0XFF;
    IOCB		=	0X00;		
	OSCCON		=	0X65;	
    OPTION_REG  =   0x07;
    //INTCON      =   0x24;
    TMR0        =   0;	
    PORTA = 0X00;
    PORTC = 0X00;
    while(1)
    {      
        stprelay2 = OFF; 
        stprelay1 = OFF;
        blwrelay = 0;
        lrelay = 0;
        
        if((!dsensor1)&&(!dsensor2))
        {
            
            buzzer = OFF;
            RC6 = 1;
            RC7 = 0;
            if(!dswitch1)
            {
                while(!dswitch1);
                
                stprelay1 = ON;
                stprelay2 = OFF;
                while(!dsensor1)
                  {
                      if(T0IF)
                      {
                          T0IF = 0;
                          count++;
                          if(count>=900)
                          {
                              count = 0;
                              buzzer = ON;
                              __delay_ms(1000);
                              buzzer = OFF;
                             stprelay2 = OFF; 
                             stprelay1 = OFF;
                             break;
                          }
                          if(!dswitch2)
                          {
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                          }
                      }
                  }
                
                  blwrelay = ON;
                  lrelay = ON;
                  RC7 = 0;
                  count = 0;
                  while(1)
                  {
                    if(T0IF)
                       {
                          T0IF = 0;
                          count++;
                          if(!dswitch2)
                          {
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              
                          }
                          if(!dsensor1)
                          {
                                    count=0;
                                    buzzer   = 0;
                                    
                                    blwrelay = OFF;
                                    lrelay   = OFF;
                                    __delay_ms(2000);
                                    
                                    stprelay1 = OFF;
                                    __delay_ms(100);
                                    
                                    stprelay1 = OFF;
                                    stprelay2 = OFF;
                                    break;
                          }
                            
                          if(count>=915)
                            {
                               RC7 = 1;
                               if(dsensor1 == 1)
                               {
                                
                                    buzzer=1;
                                    __delay_ms(550);
                                    buzzer = 0;
                                    __delay_ms(100);
                                    
                               }    
                                
                            }
                          else if((count>=915)&&(!dsensor1))
                             {
                                    RC7 = 0;
                                    count=0;
                                    buzzer   = 0;
                                    blwrelay = OFF;
                                    lrelay   = OFF;
                                    __delay_ms(2000);
                                    stprelay1 = OFF;
                                    stprelay2 = OFF;
                                    break;
                              }
                                
                        }
                   }
                
               
            }
             else
            {
                stprelay1 = OFF;
                buzzer   = 0;
                blwrelay = OFF;
                lrelay   = OFF;
            }
            if(!dswitch2)
            {
                while(!dswitch2);
                
                
                  stprelay2 = ON;
                  stprelay1 = OFF;
                  while(!dsensor2)
                  {
                      if(T0IF)
                      {
                          T0IF = 0;
                          count++;
                          if(count>=900)
                          {
                              count = 0;
                              buzzer = ON;
                              __delay_ms(1000);
                              buzzer = OFF;
                             stprelay2 = OFF; 
                             stprelay1 = OFF;
                             break;
                          }
                          if(!dswitch1)
                          {
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              
                          }
                      }
                  }
                  blwrelay = ON;
                  lrelay = ON;
                  count = 0;
                 while(1)
                  {
                    if(T0IF)
                       {
                          T0IF = 0;
                          count++;
                          if(!dswitch1)
                          {
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              buzzer = 1;
                              __delay_ms(90);
                              buzzer = 0;
                              __delay_ms(50);
                              
                          }
                          if(!dsensor2)
                          {
                                    count=0;
                                    buzzer   = 0;
                                    blwrelay = OFF;
                                    lrelay   = OFF;
                                    __delay_ms(2000);
                                    stprelay2 = OFF;
                                    stprelay1 = OFF;
                                    break;
                          }
                            
                          if(count>=915)
                            {
                               RC7 = 1;
                               if(dsensor2 == 1)
                               {
                                
                                    buzzer=1;
                                    __delay_ms(550);
                                    buzzer = 0;
                                    __delay_ms(100);
                                    
                               }    
                                
                            }
                          else if((count>=915)&&(!dsensor2))
                             {
                                    RC7 = 0;
                                    count=0;
                                    buzzer   = 0;
                                    blwrelay = OFF;
                                    lrelay   = OFF;
                                    __delay_ms(2000);
                                    stprelay2 = OFF;
                                    stprelay1 = OFF;
                                    break;
                              }
                                
                        }
                   }
                  
            }
            else
            {
                stprelay2 = OFF;
                buzzer   = OFF;
                blwrelay = OFF;
                lrelay   = OFF;
                
            }
            
        }
        else
        {
             stprelay2 = OFF;
                stprelay1 = OFF;
                blwrelay = OFF;
                lrelay   = OFF;
            buzzer = OFF;
            __delay_ms(500);
            buzzer = ON;
            __delay_ms(500);
            RC7 = 1;
            RC6 = 0;
        }
       
    }
    
    
   
}
