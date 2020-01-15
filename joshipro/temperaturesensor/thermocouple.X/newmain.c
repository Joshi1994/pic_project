#define RS RC0
#define EN RC1
#define D4 RC2
#define D5 RC3
#define D6 RC4
#define D7 RC5

#define sel RB0
#define inc RB1
#define dec RB2
#define ent RB3
#define RELAY RB7

#include <xc.h>
#include <string.h>
#include <stdio.h>
#include<math.h>
#define _XTAL_FREQ 4000000

int start;

void Lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
    __delay_ms(4);
    EN  = 0;             // => E = 0
}

void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	    temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init()
{
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);     //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

void Lcd_Write_String(const char *a)
{
    
    
    int i;
	for(i=0;a[i]!='\0';i++)
	 Lcd_Write_Char(a[i]);
   /*while(*s)
   Lcd_Write_Char(*s++);*/
}

void lcddisplay(unsigned int num)
{
   
   Lcd_Write_Char((num/10000)+0x30);
  num=num%10000;
 
   Lcd_Write_Char((num/1000)+0x30);
   num=num%1000;
 
   Lcd_Write_Char((num/100)+0x30);
   num=num%100;
 
   Lcd_Write_Char((num/10)+0x30);
 
   Lcd_Write_Char((num%10)+0x30);
 
 
    
}


void ADC_Init()
{
    ANSEL = 0X01;
  ADCON0 = 0x01;               //Turn ON ADC and Clock Selection
  ADCON1 = 0x00;               //All pins as Analog Input and setting Reference Voltages
  

   
}

unsigned int ADC_Read(unsigned char channel)
{
  if(channel > 7)              //Channel range is 0 ~ 7
    return 0;

  ADCON0 &= 0xC5;              //Clearing channel selection bits
  ADCON0 |= channel<<3;        //Setting channel selection bits
  __delay_ms(2);               //Acquisition time to charge hold capacitor
  GO_nDONE = 1;                //Initializes A/D conversion
  while(GO_nDONE);             //Waiting for conversion to complete
  return ((ADRESH<<8)+ADRESL); //Return result
}

int getTemp()
{

    int Temp;
    unsigned int Analog_Input;
    
        Analog_Input = ADC_Read(0);	/* store the analog data on a variable */
		
       	
 		Temp = ((Analog_Input * 4.88)-0.0027)/10.0;
  return Temp;
}












































void delay60(int a,int b,int c)
{   
    int i,tempval;
    for(i=0;i<=29;i++)
    {
        tempval = getTemp();
        if((a<10)&&(a>70))
        {
            break;
        }
        if(ent == 0)
        {
            start = 1;
            
            break;
            
        }
        else
        {
            start = 0;
        }
        if((tempval>=70)||(tempval<=10))
        {
            //check = 1;
            break;
            
        }
        
         Lcd_Clear();
        Lcd_Set_Cursor(1,1);
          Lcd_Write_String("On_temp:");
          
           Lcd_Set_Cursor(1,14);
          lcddisplay(b);
          Lcd_Set_Cursor(2,1);
          Lcd_Write_String("Off_temp:");
           Lcd_Set_Cursor(2,14);
          lcddisplay(c);
         __delay_ms(1000);
           Lcd_Clear();
           Lcd_Set_Cursor(1,1);
            Lcd_Write_String("current temp:");
            Lcd_Set_Cursor(1,14);
            lcddisplay(tempval);
            Lcd_Set_Cursor(2,1);
            lcddisplay(i);
        __delay_ms(1000);
    }
}


void main()
{
    
  TRISA = 0xFF;                 //Analog pins as Input
  TRISB = 0x0F;                 
  TRISC = 0x00;                 //Port C as Output
  
  ANSELH = 0X80;
  ADCON0 = 0x00;
  ADCON1 = 0X00;
  INTE = 0;
  ADC_Init();                   //Initialize ADC
  Lcd_Init();

  
  

  
    unsigned short int data;
    int highval=70;
    int lowval=10;
     int b=0;
    int tempval;
    
       
     int curs;
     int switchsel =0;
     
     int c = 0;
     
     //__delay_ms(100);
    Lcd_Init();
    Lcd_Clear();
   int  start = 0;
   
   char Temperature_buffer[10];
     while(1)
    {
      Lcd_Clear();
         tempval = getTemp();
      
         
         /*
           if(ent == 0)
            { 
               start: 
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Entering setup");
                __delay_ms(1000);
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("On temp: ");
                lcddisplay(lowval);
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("OFF temp:");
                lcddisplay(highval);
                 
                 
                   while(1)
                    {
                        
                    if(!sel)
                        {
                        
                            switchsel++;
                            if(switchsel == 3)
                            {
                                switchsel = 1;
                            }
                             while(sel == 0);
                        }
                              if((switchsel == 1)&&(switchsel<=2))
                                {
                                
                                  
                                    Lcd_Clear();
                                    Lcd_Set_Cursor(1,1);
                                    Lcd_Write_String("On: ");
                                    Lcd_Set_Cursor(1,12);
                                    lcddisplay(lowval);
                                    Lcd_Set_Cursor(2,1);
                                    Lcd_Write_String("OFF:");
                                    Lcd_Set_Cursor(2,12);
                                    lcddisplay(highval);

                                    Lcd_Set_Cursor(1,15);
                                    Lcd_Write_String("<<");
                                    curs = 1;
                                    
                                   
                                    if(!inc)
                                            {
                                        
                                               lowval++;
                                               if(lowval==highval)
                                               {
                                                   lowval = 60;
                                               }
                                               //Lcd_Clear();
                                               Lcd_Set_Cursor(1,1);
                                               Lcd_Write_String("On:");
                                               Lcd_Set_Cursor(1,12);
                                               lcddisplay(lowval);
                                              //Lcd_Set_Cursor(2,1);
                                              //Lcd_Write_String("OFF:");
                                              //Lcd_Set_Cursor(2,12);
                                              //lcddisplay(highval);
                                              while(!inc);
                                              }
                                    else if(!dec)
                                              {
                                        
                                        if(lowval==60)
                                               {
                                                   lowval = highval;
                                               }
                                        
                                              lowval--;
                                              Lcd_Clear();
                                              Lcd_Set_Cursor(1,1);
                                              Lcd_Write_String("On:");
                                              Lcd_Set_Cursor(1,12);
                                              lcddisplay(lowval);
                                              //Lcd_Set_Cursor(2,1);
                                              //Lcd_Write_String("OFF:");
                                              //Lcd_Set_Cursor(2,12);
                                              //lcddisplay(highval);
                                              while(!dec);
                                               }

                                          
                                    
                                    

                                }
                              else if((switchsel == 2) && (switchsel<=2))
                            
                                {
                                  
                                    Lcd_Clear();
                                    Lcd_Set_Cursor(1,1);
                                    Lcd_Write_String("On: ");
                                    Lcd_Set_Cursor(1,12);
                                    lcddisplay(lowval);
                                    Lcd_Set_Cursor(2,1);
                                    Lcd_Write_String("OFF:");
                                    Lcd_Set_Cursor(2,12);
                                    lcddisplay(highval);
                                    Lcd_Set_Cursor(2,15);
                                    Lcd_Write_String("<<");
                                    curs = 2;
                                           
                                  
                                    
                                         if(!inc)
                                            {
                                             
                                             //int icr = highval;
                                               //icr = icr-1;
                                               if(highval == 98)
                                               {
                                                   highval = lowval;
                                               }
                                             
                                             highval++;
                                               
                                             
                                             
                                            
                                             Lcd_Clear();
                                             //Lcd_Set_Cursor(1,1);
                                             //Lcd_Write_String("On:");
                                             //Lcd_Set_Cursor(1,12);
                                             //lcddisplay(lowval);
                                             Lcd_Set_Cursor(2,1);
                                             Lcd_Write_String("OFF:");
                                             Lcd_Set_Cursor(2,12);
                                             lcddisplay(highval);
                                             while(!inc);
                                            }
                                           else if(!dec)
                                             {
                                               
                                               int der = highval;
                                               der = der-1;
                                               if(der == lowval)
                                               {
                                                   highval = 99;
                                               }
                                              highval--;
                                             // Lcd_Clear();
                                              //Lcd_Set_Cursor(1,1);
                                              //Lcd_Write_String("On:");
                                              //Lcd_Set_Cursor(1,12);
                                              //lcddisplay(lowval);
                                              Lcd_Set_Cursor(2,1);
                                              Lcd_Write_String("OFF:");
                                              Lcd_Set_Cursor(2,12);
                                              lcddisplay(highval);
                                              while(!dec);
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
                        __delay_ms(100);
                        //c++;
                    
                         
                   }
                while(ent == 0);
           }
           else if(start == 1)
           {
               goto start;
           }
       Lcd_Clear();
   
      
       if((tempval>=10)&&(tempval<=70))
       {
           
        if(((tempval>=lowval)&&(tempval<=highval))&&((lowval>=10)&&(highval<=70)))
              
          {
              //relaystatus(1);
             RELAY = 1; 
             
             
             delay60(tempval,lowval,highval);
             RELAY = 0;
             delay60(tempval,lowval,highval);
             c = 0;
             
             
             
                
             
          }
          else
          {
             Lcd_Clear();
            Lcd_Set_Cursor(1,1);
          
            Lcd_Write_String("Set TEMP 10-70");
          
              RELAY = 0;
              c=1;
              __delay_ms(1000);
          }
       }
       else
       {
           if(tempval<10)
            {
                 Lcd_Clear();
                RELAY = 0;
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("LOW TEMP");
                __delay_ms(1000);
            }
        else if(tempval>70)
            {
                Lcd_Clear();
                RELAY = 0;
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("HIGH TEMP");
                __delay_ms(1000);
            }
            
       }  
       
      
      */
         int Temperature = 99;
      
   //sprintf(Temperature_buffer,"%d",(int)tempval,0xdf);
//sprintf(Temperature_buffer,"%cC  ",0xdf);  
    
  //              Lcd_Write_String(Temperature_buffer);
                Lcd_Write_Char(0xdf);
  __delay_ms(100);
    Lcd_Clear();
  
  
  
     }
  
  }
