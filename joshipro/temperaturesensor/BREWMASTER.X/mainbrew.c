#define _XTAL_FREQ 4000000

#define RS          RC4
#define EN          RC5
#define D4          RB0
#define D5          RB1
#define D6          RB2
#define D7          RB3
#define TS          RC3
#define TS_DIR      TRISC3
#define RELAY       RC0
#define RELAYLED    RA1
#define pgm         RA0

#define mry         RB7
#define inc         RB6
#define dec         RB5
#define startkey    RB4

	
#define samled      RB7


#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)













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


void lcddisplay(unsigned int num)
{
   
   //Lcd_Write_Char((num/10000)+0x30);
  num=num%10000;
 
   //Lcd_Write_Char((num/1000)+0x30);
   num=num%1000;
 
   //Lcd_Write_Char((num/100)+0x30);
   num=num%100;
 
   Lcd_Write_Char((num/10)+0x30);
 
   Lcd_Write_Char((num%10)+0x30);
 
 
    
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

void Write_EEPROM(unsigned char a1,unsigned char a2,unsigned int data_value)//Writing data into specified location in EEPROM
{
	eeprom_write(a1,(data_value/10));	//write first digit of data
	eeprom_write(a2,(data_value%10));	//write second digit of data
}

unsigned int Read_EEPROM(unsigned char b1,unsigned char b2)//Reading data from specified location in EEPROM
{
	unsigned int c1=0,c2=0;
	c1=eeprom_read(b1);		//read first digit of data
	c2=eeprom_read(b2);		//read second digit of data
	return ((c1*10)+c2); 	//return the data read
}

int main()
{
    TRISA = 0x01;
    TRISB = 0xF0;
    TRISC = 0x00;
    
    
    OPTION_REG	= 	0X00;		//Configuring timer 0 with prescale as 256
	
	ANSEL		= 	0X00;		//setting all analog pins as digital 
	ANSELH		= 	0X00;		//setting all analog pins as digital 
	WPUB		=	0XF0;		//Enable the internal pull-ups of the pins RB7-RB4
	IOCB		=	0X00;		//Disable interrupt on port change 
	OSCCON		=	0X65;		//Internal oscillator with 4Mhz frequency and clock stability	
    
    int prg = 0;
    int password = 0;
    int switchsel = 0;
    int highval = 30;
    int lowval=25;
    int curs = 0;
    int open = 0;
    int tempval = 0;
    


   
    
    __delay_ms(100);
    Lcd_Init();
    Lcd_Clear();
    Write_EEPROM(0,1,lowval);
    Write_EEPROM(2,3,highval);
    
  while(1)
  {
                Lcd_Clear();
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(" * BREWMASTER * ");
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String(" EXTERNAL BOILER");
                __delay_ms(2000);
               Lcd_Clear();
                    if(!pgm)
                    {
                        start:
                        
                        prg = 1;

                        //while(!pgm);
                    }
                
                    if(prg == 1)
                    {
                        Lcd_Clear();
                        Lcd_Set_Cursor(1,1);
                        Lcd_Write_String(" ENTER PASSWORD ");
                        Lcd_Set_Cursor(2,7);
                        lcddisplay(password);
                         while(1)
                        {
                        Lcd_Set_Cursor(2,7);
                        lcddisplay(password);
                        if(inc == 0)
                        {
                            
                            if(password == 50)
                            {
                                password = 0;
                            }
                            
                            password++;
                            while(!inc);
                        }
                        else if(dec==0)
                        {
                            
                            if(password == 1)
                            {
                                password = 50;
                            }
                            /*else if(password == 8)
                            {
                                //__delay_ms(1500);
                                Lcd_Clear();
                                Lcd_Set_Cursor(1,1);
                                Lcd_Write_String("password ent");
                                prg = 0;
                                open = 1;
                                password = 0;
                                break;
                            }*/
                            password--;
                            while(!dec);
                           
                        }
                        
                        else if(!startkey)
                            {
                            if(password == 2)
                            {
                                prg = 0;
                                open = 1;
                                password = 0;
                                break;

                            }
                            else
                            {
                                Lcd_Clear();
                                Lcd_Set_Cursor(1,1);
                                Lcd_Write_String("wrong password");
                                __delay_ms(2000);
                                Lcd_Clear();
                                Lcd_Set_Cursor(1,1);
                                Lcd_Write_String("Enter password");
                            }
                                
                            }
                        
                     }
                         Lcd_Clear();
                                Lcd_Set_Cursor(1,1);
                                Lcd_Write_String("while exited");
                                __delay_ms(1000);
                        while (1)
                        {
                            Lcd_Clear();
                            Lcd_Set_Cursor(1,1);
                            Lcd_Write_String("SET MIN TEMP:");
                            Lcd_Set_Cursor(1,14);
                            lcddisplay(lowval);
                            Lcd_Set_Cursor(2,1);
                            Lcd_Write_String("SET MAX TEMP:");
                            Lcd_Set_Cursor(2,14);
                            lcddisplay(highval);
                        if(!pgm)
                        {
                            switchsel++;
                             
                            if(switchsel == 3)
                            {
                            
                                switchsel = 1;
                                
                            }
                            while(!pgm);

                        }
                        if((switchsel == 1)&&(switchsel<=2))
                        {
                            Lcd_Clear();
                            Lcd_Set_Cursor(1,1);
                            Lcd_Write_String("SET MIN TEMP:");
                            Lcd_Set_Cursor(1,14);
                            lcddisplay(lowval);
                            Lcd_Set_Cursor(2,1);
                            Lcd_Write_String("SET MAX TEMP:");
                            Lcd_Set_Cursor(2,14);
                            lcddisplay(highval);
                            Lcd_Set_Cursor(1,16);
                            Lcd_Write_String("<");
                            curs = 1;
                            __delay_ms(500);

                            if(!inc)
                            {
                               
                                lowval++;
                                /*if(lowval==highval)
                                {
                                    lowval = 60;
                                }*/
                                //Lcd_Clear();
                                //Lcd_Set_Cursor(1,1);
                                //Lcd_Write_String("SET MAX TEMP:");
                                Lcd_Set_Cursor(1,14);
                                lcddisplay(lowval);
                                //Lcd_Set_Cursor(2,1);
                                //Lcd_Write_String("SET MIN TEMP:");
                                //Lcd_Set_Cursor(2,14);
                                //lcddisplay(highval);
                                while(!inc);

                            }
                            else if(!dec)
                            {
                               
                                /*if(lowval==60)
                                {
                                    lowval = highval;
                                }*/

                                lowval--;
                                //Lcd_Clear();
                               // Lcd_Set_Cursor(1,1);
                               // Lcd_Write_String("SET MAX TEMP:");
                                Lcd_Set_Cursor(1,14);
                                lcddisplay(lowval);
                               // Lcd_Set_Cursor(2,1);
                                //Lcd_Write_String("SET MIN TEMP:");
                               // Lcd_Set_Cursor(2,14);
                                //lcddisplay(highval);
                                while(!dec);
                            }





                        }
                        else if((switchsel == 2) && (switchsel<=2))
                        {
                            Lcd_Clear();
                            Lcd_Set_Cursor(1,1);
                            Lcd_Write_String("SET MIN TEMP:");
                            Lcd_Set_Cursor(1,14);
                            lcddisplay(lowval);
                            Lcd_Set_Cursor(2,1);
                            Lcd_Write_String("SET MAX TEMP:");
                            Lcd_Set_Cursor(2,14);
                            lcddisplay(highval);
                            Lcd_Set_Cursor(2,16);
                            Lcd_Write_String("<");
                            curs = 2;
                            __delay_ms(500);

                            if(!inc)
                            {
                                //int icr = highval;
                                //icr = icr-1;
                                /*if(highval == 98)
                                {
                                    highval = lowval;
                                }*/

                                highval++;
                            //    Lcd_Clear();
                              //  Lcd_Set_Cursor(1,1);
                               // Lcd_Write_String("SET MAX TEMP:");
                                //Lcd_Set_Cursor(1,14);
                               // lcddisplay(lowval);
                                //Lcd_Set_Cursor(2,1);
                               // Lcd_Write_String("SET MIN TEMP:");
                                Lcd_Set_Cursor(2,14);
                                lcddisplay(highval);

                            }
                            else if(!dec)
                            {
                                /*int der = highval;
                                der = der-1;
                                if(der == lowval)
                                    {
                                        highval = 99;
                                    }*/
                                highval--;
                                //Lcd_Clear();
                                //Lcd_Set_Cursor(1,1);
                                //Lcd_Write_String("SET MIN TEMP:");
                                //Lcd_Set_Cursor(1,14);
                                //lcddisplay(lowval);
                                //Lcd_Set_Cursor(2,1);
                                //Lcd_Write_String("SET MAX TEMP:");
                                Lcd_Set_Cursor(2,14);
                                lcddisplay(highval);

                            }
                        }


                        if(((!mry)&&(curs == 1))||(!mry))
                        {
                            while(!mry);
                            Write_EEPROM(0,1,lowval);
                            Write_EEPROM(2,3,highval);
                            break;
                        }
                        if((!mry)&&(curs == 2))
                        {
                            while(!mry);
                            Write_EEPROM(0,1,lowval);
                            Write_EEPROM(2,3,highval);
                            break;
                        }
                            __delay_ms(500);


                    }
                    }
                
                while(1)
                {
                    
                    if(pgm == 0)
                    {
                      
                       
                        goto start;
                        
                    }
                    tempval = checktemp();
                    lowval=Read_EEPROM(0,1);	//read all previously stored values from EEPROM
                    highval=Read_EEPROM(2,3);
                    Lcd_Clear();
                    Lcd_Set_Cursor(1,1);
                    Lcd_Write_String("TEMPERATURE ");
                    Lcd_Write_Char(0xdf);
                    Lcd_Set_Cursor(1,14);
                    Lcd_Write_String("C");
                    Lcd_Set_Cursor(2,1);
                    Lcd_Write_String("SET:");
                    Lcd_Set_Cursor(2,5);
                    lcddisplay(highval);
                    Lcd_Set_Cursor(2,8);
                    Lcd_Write_String("ACTUAL:");
                    Lcd_Set_Cursor(2,15);
                    lcddisplay(tempval);
                    
                    __delay_ms(250);
                    
                    
                    
                    
                    
                    
                    
                    
                    if((tempval>=lowval)&&(tempval<=highval))
                    {
                        RELAY = 1; 
                        RELAYLED = 1;
                    }
                    else
                    {
                        RELAY = 0;
                        RELAYLED = 0;
                    }
     
                }
                
                
 
    }
    /*if(RA0 == 1)
    {
        RA1 = 1;
    }
    else
    {
        RA1 = 0;
    }*/
     
  } 
 
