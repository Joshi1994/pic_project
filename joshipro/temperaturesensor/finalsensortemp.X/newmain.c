#define _XTAL_FREQ 4000000

#define RS RA1
#define EN RA2
#define D4 RC0
#define D5 RC1
#define D6 RC2
#define D7 RC3
#define TS RA0
#define TS_DIR TRISA0
#define RELAY RA5

#define sel RA4
#define inc RA3
#define dec RC5
#define ent RC4

	



#include <xc.h>
#include "lcd.h"

//__CONFIG(BGHIGH & UNPROTECT & BORDIS & MCLRDIS & PWRTEN & INTOSCCLKO & WDTDIS);
#pragma config FOSC = INTOSCIO  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Detect (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)











unsigned char reset()
{
	TS_DIR = 0; 				
	TS= 0; 		
	__delay_us(480); 		
	TS_DIR = 1; 				
	__delay_us(60);			
	
	if (TS== 0) 	
	{ 
		__delay_us(480);
		return 0; 			
	} 
	else 
	{
		__delay_us(480);
		return 1; 			
	}
}



void write(char WRT)
{
	char i,Cmd;
	Cmd=WRT;
	TS_DIR = 1; 
	
	for(i = 0; i < 8; i++)
	{
		if((Cmd & (1<<i))!= 0) 
		{
			
			TS_DIR = 0; 			
			TS= 0; 	
			__delay_us(1); 		
			TS_DIR = 1; 			
			__delay_us(60);		
		} 
		else 
		{
			
			TS_DIR = 0; 
			TS= 0; 	
			__delay_us(60); 	
			TS_DIR = 1;			
		}
	}
}



unsigned char read()
{
	char i,result = 0;
	TS_DIR = 1;		 			
	for(i = 0; i < 8; i++)
	{
		TS_DIR = 0; 			
		TS= 0; 		
		__delay_us(2);
		TS_DIR = 1; 				
		if(TS!= 0) 
		result |= 1<<i;
		__delay_us(60);			 
	}
	return result;
}


int checktemp()
{
     unsigned short tempL = 0,tempH = 0;
    unsigned  int temp = 0;
  
     if(!reset())		
	{	
		write(0xcc);						
		write(0x44);						
		__delay_us(600);
			
		reset();						
		write(0xcc);				
		write(0xbe);				
			
		tempL = read();					
		tempH = read();					
		temp=((tempH << 8 )| tempL);	
		temp = temp /16 ;	             
    }
     return temp;
}


void lcddisplay(int data)
{
    Lcd_Write_Char((data/10) + (0x30));
    Lcd_Write_Char((data%10) + (0x30));
}

void relaystatus(int status)
{
    if(status == 1)
          {
              Lcd_Clear();
              Lcd_Write_String("status : ON");
             
          }
          else
          {
              Lcd_Clear();
           Lcd_Write_String("status : OFF");   
          }
}


int main()
{
 
    ANSEL = 0x00;
    CMCON = 0xF7;

    unsigned int a;
    unsigned short tempL = 0,tempH = 0;
    unsigned  int temp = 0;
    unsigned short int data;
    int highval=96;
    int lowval=92;

    int b=0;
    int tempval;
    
       
     int curs;
     int switchsel =0;
     
    
    int c;
     
    TRISC = 0x30;
    TRISA1 = 0;
    TRISA2 = 0;
    TRISA3 = 1;
    TRISA4 = 1;
    TRISA5 = 0;
    __delay_ms(100);
    Lcd_Init();
    Lcd_Clear();
    
  while(1)
  {
  /*tempval = checktemp();
      if(!ent)
      { 
          
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("entring mode");
                __delay_ms(1000);
                Lcd_Clear();
                Lcd_Write_String("On temp: ");
                lcddisplay(lowval);
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("OFF temp:");
                lcddisplay(highval);
                int c = 0;  
                 
                   for(c=0;c<=100;)
                    {
                        
                    if(!sel)
                        {
                        
                            switchsel++;
                             Lcd_Clear();
                            
                              if((switchsel == 1)&&(switchsel<=2))
                                {
                                
                                    Lcd_Clear();
                                    Lcd_Write_String("On: ");
                                    lcddisplay(lowval);
                                    Lcd_Set_Cursor(2,1);
                                    Lcd_Write_String("OFF:");
                                    lcddisplay(highval);

                                    Lcd_Set_Cursor(1,15);
                                    Lcd_Write_String("<<");
                                    curs = 1;

                                }
                              else if((switchsel == 2) && (switchsel<=2))
                            
                                {
                                    Lcd_Clear();
                                    Lcd_Write_String("On: ");
                                    lcddisplay(lowval);
                                    Lcd_Set_Cursor(2,1);
                                    Lcd_Write_String("OFF:");
                                    lcddisplay(highval);
                                    Lcd_Set_Cursor(2,15);
                                    Lcd_Write_String("<<");
                                    curs = 2;
                                           
                              
                              
                                    if(switchsel == 2)
                                    {
                                        switchsel=0;
                                    }
                                }
                            
                        }
                         
                    if((!ent)&&(curs == 1))
                    {
                        break;
                    }
                    if((!ent)&&(curs == 2))
                    {
                        break;
                    }
                        __delay_ms(500);
                        c++;
                    }     
                
                 
         }    
      
     /* if(inc == 0)
        {
           lowval++;
           if(lowval==highval)
           {
               lowval = 60;
           }
           Lcd_Clear();
           Lcd_Write_String("On:");
           lcddisplay(lowval);
          Lcd_Set_Cursor(2,1);
          Lcd_Write_String("OFF:");
          lcddisplay(highval);
          
          }
         else if(dec == 0)
          {
          lowval--;
          Lcd_Clear();
           Lcd_Write_String("On:");
          lcddisplay(lowval);
          Lcd_Set_Cursor(2,1);
           Lcd_Write_String("OFF:");
          lcddisplay(highval);
          
           }
                   
      /*else if((inc == 0)&&(curs == 2))
        {
         highval++;
         Lcd_Clear();
           Lcd_Write_String("On:");
          lcddisplay(lowval);
          Lcd_Set_Cursor(2,1);
           Lcd_Write_String("OFF:");
          lcddisplay(highval);
          
        }
         else if((dec == 0)&&(curs == 2))
         {
           highval--;
           Lcd_Clear();
           Lcd_Write_String("On:");
          lcddisplay(lowval);
          Lcd_Set_Cursor(2,1);
           Lcd_Write_String("OFF:");
          lcddisplay(highval);
          
            }
      
  
  
 
        if((tempval>=lowval)&&(tempval<=highval))
              
          {
              //relaystatus(1);
             RELAY = 1; 
             c = 0;
          }
          else
          {
          //    relaystatus(0);
              RELAY = 0;
              c=1;
          }

          //__delay_ms(1000);
          Lcd_Set_Cursor(1,1);
          Lcd_Write_String("On_temp:");
          lcddisplay(lowval);
          Lcd_Set_Cursor(2,1);
          Lcd_Write_String("Off_temp:");
          lcddisplay(highval);
         __delay_ms(1000);
           Lcd_Clear();
           Lcd_Set_Cursor(1,1);
            Lcd_Write_String("current temp:");
            lcddisplay(tempval);
            
            if(c==1)
            {
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("status OFF");
                
            }
            else
            {
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("status ON");
                
            }
            __delay_ms(1000);
            Lcd_Clear();
            
            
            
          
     
  }*/
 
      __delay_ms(1000);
      RELAY = 1;
      __delay_ms(1000);
      RELAY = 0;
}