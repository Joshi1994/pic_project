
#define TS          RC3
#define TS_DIR      TRISC3
#define RELAY       RC0
#define RELAYLED    RA1
#define pgm         RA0
#define prelay      RA3   //----------
#define wsensor     RC6   //----------
#define buzz        RA7
#define _XTAL_FREQ  4000000

#define RS          RC4
#define EN          RC5
#define D4          RB0
#define D5          RB1
#define D6          RB2
#define D7          RB3

#define mry         RB7
#define inc         RB6
#define dec         RB5
#define startkey    RB4

	
#define samled      RB7


#define POWERLED  	RA2	

#include <xc.h>


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

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



char waterchk = 0;
int sec = 0;
char waterflag = 0;

int highval = 0;
int lowval=0;
int tempval = 0;

int startval;

int strky = 0;
int pgm1 =0;
int sensor;

 int flag1 = 0;
    int flag2 = 0;
    int flag4 = 0;
    int flag5 = 0;
    int flag6 =0;
    int flag7 = 0;

__EEPROM_DATA(0X00,0X00,0X09,0X00,0X00,0X00,0X00,0X00);

char relayon = 0;
char relayoff = 0;
char timerup = 0;

char pumpfail = 0;




    


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
		__delay_us(100);
			
		reset();						
		write(0xcc);				
		write(0xbe);				
			
		tempL = read();					
		tempH = read();					
		temp=((tempH << 8 )| tempL);	
		temp = temp /16 ;	             
    }
     else
     {
         temp = 0;
     }
     return temp;
}


void lcddisplay(unsigned int num)
{
   
 
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


//Write_EEPROM(2,3,highval);


int main()
{
    TRISA = 0x01;
    TRISB = 0xF0;
    TRISC = 0xC0;
    
    
    OPTION_REG	= 	0X07;		//Configuring timer 0 with prescale as 256
	TMR0        =   0;
    ANSEL		= 	0X00;		//setting all analog pins as digital 
	ANSELH		= 	0X00;		//setting all analog pins as digital 
	WPUB		=	0XF0;		//Enable the internal pull-ups of the pins RB7-RB4
	
    int prg = 0;
    int password = 0;
    int switchsel = 0;
    
    
    int open = 0;
    
    char wrong = 0;
    
    int power = 0;
    
    RELAY = 0;
    RELAYLED = 0;
    
    int j = 0;
   
    
    char current = 0;
    int tcount = 0;
    char pump = 0;
    char wcount = 0;
    
    
    Lcd_Init();
    Lcd_Clear();
    
    lowval = Read_EEPROM(0,1);
    highval = Read_EEPROM(2,3);
    
    
  while(1)
      
  {
      
    POWERLED = 1;
    if(wsensor)   //water is there
    {
        
        sensor = checktemp();
        if(sensor >0)
        {
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String(" * BREWMASTER * ");
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String(" EXTERNAL BOILER");
                __delay_ms(1000);
                buzz = 1;
                __delay_ms(1000);
                buzz = 0;
                Lcd_Clear();
                int powerval;
                powerval=Read_EEPROM(4,5);
                if(!pgm)
                {
                    char p=0;
                    start:
                    
                    
                    if(wrong == 3)
                    {
                        buzz = 1;
                        __delay_ms(1400);
                        buzz = 0;
                        wrong = 0;
                        goto ready;
                    }
                    Lcd_Clear();
                    Lcd_Set_Cursor(1,1);
                    Lcd_Write_String(" ENTER PASSWORD ");
                    Lcd_Set_Cursor(2,7);
                    Lcd_Write_String("**");
                    while(1)
                    {
                        if(!inc)
                        {
                            password++;
                            p=1;
                            while(!inc);
                            break;   
                        }
                        if(!dec)
                        {
                            password--;
                            p=1;
                            while(!dec);
                            break;
                        }
                    }
                    while(p)
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
                            __delay_ms(150);
                            
                        }
                        else if(dec==0)
                        {
                            if(password == 1)
                            {
                                password = 50;
                            }
                            
                            password--;
                            __delay_ms(150);
                            
                        }
                       
                        if(!startkey)
                        {
                            if(password == 2)
                            {
                                prg = 0;
                                open = 1;
                                password = 0;
                                wrong = 0;
                                break;
                            }
                            else 
                            {
                                Lcd_Clear();
                                Lcd_Set_Cursor(1,1);
                                Lcd_Write_String("wrong password");
                                wrong++;
                                if(wrong<3)
                                {
                                    buzz = 1;
                                    __delay_ms(200);
                                    buzz = 0;
                                    __delay_ms(100);
                                    buzz = 1;
                                    __delay_ms(200);
                                    buzz = 0;
                                    __delay_ms(100);
                                    buzz = 1;
                                    __delay_ms(200);
                                    buzz = 0;
                                }
                                password = 0;
                                goto start;
                            }
                        }
                    }
                    Lcd_Clear();
                    Lcd_Set_Cursor(1,4);
                    Lcd_Write_String("LOGINS....");
                    __delay_ms(1000);
                    initialstart:
                    Lcd_Clear();
                    while (1)
                    {
                        reval:
                        Lcd_Set_Cursor(1,7);
                        Lcd_Write_String("SET");
                        Lcd_Set_Cursor(2,4);
                        Lcd_Write_String("MAX TEMP:");
                        Lcd_Set_Cursor(2,13);
                        lcddisplay(highval);
                        if(!inc)
                        {
                            if(highval == 99)
                            {
                                highval = 00;
                            }
                            
                            
                            highval++;
                            __delay_ms(150);
                            
                        }
                        else if(!dec)
                        {
                            if(highval == 0)
                            {
                                highval = 99;
                            }
                            highval--;
                            __delay_ms(150);
                            
                            
                        }
                        
                        else if(!mry)
                        {
                            while(!mry);
                            
                            Lcd_Clear();
                            Lcd_Set_Cursor(1,1);
                            Lcd_Write_String("Checking");
                            Lcd_Set_Cursor(2,1);
                            Lcd_Write_String("Temperature...");
                            
                                tempval = checktemp();
                                __delay_ms(1000);
                            
                            if(highval<=tempval)
                            {
                                Lcd_Clear();
                                Lcd_Set_Cursor(1,1);
                                Lcd_Write_String("Enter high temp");
                                buzz = 1;
                                __delay_ms(200);
                                buzz = 0;
                                __delay_ms(100);
                                buzz = 1;
                                __delay_ms(200);
                                buzz = 0;
                                __delay_ms(100);
                                buzz = 1;
                                __delay_ms(200);
                                buzz = 0;
                                Lcd_Clear();
                                goto reval;
                            }
                            lowval = highval - 5;
                            power = 1;
                            Write_EEPROM(0,1,lowval);
                            Write_EEPROM(2,3,highval);
                            Write_EEPROM(4,5,power);
                            startval = highval;
                            j = 1;
                            break;
                            
                        }
                                    
                    }
                                
                }
                if(j==1)
                {   
                    ready:
                    Lcd_Clear();
                    
                    while(1)
                    {   Lcd_Set_Cursor(1,2);
                        Lcd_Write_String("* BREWMASTER *");
                        Lcd_Set_Cursor(2,6);
                        Lcd_Write_String("READY...");
                        current=Read_EEPROM(6,7);
                        
                        if(!pgm)
                        {
                            while(!pgm);
                            goto start;
                        }
                        if(!startkey)
                        {   
                            while(!startkey);
                            if(pumpfail == 1)
                            {
                                RELAY = 1;
                                RELAYLED = 1;
                            }
                            Lcd_Clear();
                            powerval=Read_EEPROM(4,5);
                            Write_EEPROM(6,7,0);
                            break;
                        }
                        
                            
                    }
                }
                current=Read_EEPROM(6,7);
                if((powerval == 1)&&(current == 1))
                {
                   goto ready;
                }
                if((powerval == 1)&&(current == 0))
                {
                    temp:
                    lowval=Read_EEPROM(0,1);	//read all previously stored values from EEPROM
                    highval=Read_EEPROM(2,3);
                       
  //----------------------------------------------------------------------------------------------------------------------------                  
                       intwater:
                        if(wsensor)   //water is there
                        {
                            while(1)
                            {
                                tempval = checktemp();
                                int tempflag1 = 0;
                                if(tempval>0)
                                {
                                    if(tempval>=highval)
                                    {
                                        flag7 = 1;
                                        int tempflag2 = 0;
                                        int checkte ;
                                        int tempvalhigh;
                                        tempvalhigh = highval+3;
                                        if(flag5 == 0)
                                        {
                                            flag1 = 0;
                                            flag2 = 1;
                                            goto beep;
                                        }
                                        else if(tempvalhigh<=tempval)
                                        {
                                            tempflag2 = 1;
                                        }
                                        if(tempflag2 == 1)
                                        {   
                                            Lcd_Clear();
                                            while(1)
                                            {
                                                tempflag2 = 0;
                                                Lcd_Set_Cursor(1,1);
                                                Lcd_Write_String("High Temperature");
                                                buzz = 1;
                                                __delay_ms(300);
                                                buzz = 0;
                                                checkte = checktemp();
                                                __delay_ms(500); 
                                                if(tempvalhigh>checkte)
                                                {
                                                    Lcd_Clear();
                                                    goto nxt;
                                                }
                                            }
                                        }
                                    }
                                    nxt:
                                    tempflag1 = 0;
                                    Lcd_Set_Cursor(1,16);
                                    Lcd_Write_String(" ");
                                    Lcd_Set_Cursor(1,2);
                                    Lcd_Write_String("TEMPERATURE ");
                                    Lcd_Write_Char(0xdf);
                                    Lcd_Set_Cursor(1,15);
                                    Lcd_Write_String("C");
                                    Lcd_Set_Cursor(2,1);
                                    Lcd_Write_String("SET:");
                                    Lcd_Set_Cursor(2,5);
                                    lcddisplay(highval);
                                    Lcd_Set_Cursor(2,7);
                                    Lcd_Write_String(" ACTUAL:");
                                    Lcd_Set_Cursor(2,15);
                                    lcddisplay(tempval);
                                    if(startval>=tempval)
                                    {
                                        if(flag4 == 0)
                                        {
                                            startval = 0;
                                            flag1 = 1;
                                            goto beep;
                                        }    
                                    }
                    
                                    if(tempval<=lowval)
                                    {   
                                        if(flag4 == 0)
                                        {
                                            flag1 = 1;
                                            goto beep;
                                        } 

                                    }
                                   
                                    for(int l=0;l<=1000;l++)
                                    {            
                                        
                                        if(!wsensor)
                                        {
                                            waterflag = 1;
                                            waterstart1:
                                            for(int i = 0;i<=1000;i++)
                                            {
                                               if(!wsensor)
                                               {
                                                   wcount=1;
                                               }
                                               else
                                               {
                                                   wcount = 0;
                                               }
                                               if(wcount==0)
                                               {
                                                   goto waterstart1;
                                               }
                                             __delay_ms(1);
                                            }
                                           if(wcount == 1)
                                           {

                                                             pump = 1;
                                                             wcount = 0;
                                                             goto pump;

                                           }
                                           else
                                           {
                                               pump = 0;
                                               goto intwater;
                                           }
                                        }
                                        if(!pgm)
                                        {
                                            buzz = 1;
                                            __delay_ms(350);
                                            if((flag1 == 1)||(flag2 == 1)||(flag7 == 1))
                                            {
                                                flag1 = 0;
                                                flag2 = 1;                    
                                                strky = 0;
                                                pgm1 = 1;
                                                flag7 = 0;
                                                goto beep;
                                            }
                                            while(!pgm);
                                        }
                                        if(!startkey)
                                        {
                                            buzz = 1;
                                            __delay_ms(350);
                                            Write_EEPROM(6,7,1);
                                            if((flag1 == 1)||(flag2 == 1)||(flag7 == 1))
                                            {
                                                flag1 = 0;
                                                flag2 = 1;
                                                strky=1;
                                                flag7 = 0;
                                                goto beep;
                                            }
                                            while(!startkey);
                                        }
                                        __delay_ms(1);                   

                                    }
                                }
                                else
                                {
                                    Lcd_Set_Cursor(1,2);
                                    Lcd_Write_String("SENSOR FAULT!!!");
                                    Lcd_Set_Cursor(2,1);
                                    Lcd_Write_String("                ");
                                    buzz = 1;
                                    __delay_ms(200);
                                    buzz = 0;
                                    __delay_ms(200);

                                }
                            }
                        }
                        else
                        {
                            goto pump;
                        }
                   
                    beep:
                    if(flag1 == 1)
                    {
                        RELAY = 1;
                        RELAYLED = 1;
                        buzz = 1;
                        __delay_ms(250);
                        buzz = 0;     
                        flag4 = 1;
                        flag5 = 0;
                        flag6 = 1;
                        if(strky == 1)
                        {
                            flag2 = 0;
                            strky = 0;
                            flag1 = 0;
                            goto ready;
                        }
                        goto temp;
                    }
                    else if(flag2 == 1)
                    {
                        RELAY = 0;
                        RELAYLED = 0;
                        buzz = 1;
                        __delay_ms(250);
                        buzz = 0;     
                        flag4 = 0;
                        flag5 = 1;
                        flag6 = 1;
                        if(pgm1 == 1)
                        {
                            flag2 = 0;
                            pgm1 = 0;
                            goto start;
                        }
                        if(strky == 1)
                        {
                            flag2 = 0;
                            strky = 0;
                            goto ready;
                        }
                        goto temp;

                    }
                           
                     
               }
               else if(powerval == 0)
               {   wcount++;
           if(wcount<=2)
           {
               
               if(wcount==2)
               {
                   wcount = 0;
               }
               if(!wsensor)
                {
                   for(int i =0;i<=1000;i++)
                   {
                        if(!wsensor)
                        {

                           pump = 1;
                           wcount = 0;

                           goto pump;
                        }
                        __delay_ms(1);
                   }
                                                    
                }
               
               
           }
                    goto initialstart;
               }
        }
        else
        {
            Lcd_Set_Cursor(1,2);
            Lcd_Write_String("SENSOR FAULT!!!");
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("                ");
            buzz = 1;
            __delay_ms(200);
            buzz = 0;
            __delay_ms(200);
        }
    }
    else
    {
        Lcd_Set_Cursor(1,2);
        Lcd_Write_String("Checking water");
        __delay_ms(1000);
        waterchk = Read_EEPROM(6,7);
        while(1)
        {
        if(!wsensor)
        {
            waterstart:                       
            for(int i = 0;i<=1000;i++)
            {
               if(!wsensor)
               {
                   wcount=1;
               }
               else
               {
                   wcount = 0;
               }
               if(wcount == 0)
               {
                   goto waterstart;
               }
             __delay_ms(1);
            }
           if(wcount == 1)
           {
                 
                             pump = 1;
                             wcount = 0;
                             goto pump;
                 
           }
           else
           {
               
               pump = 0;
               goto intwater;
           }
        }
        
        }
        
        pump:
        Lcd_Clear();
        Lcd_Set_Cursor(1,2);
        Lcd_Write_String("LOW Water");
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("                ");
         
         
        while(1)
        {   
            Lcd_Set_Cursor(2,2);
        lcddisplay(timerup);
        Lcd_Set_Cursor(2,6);
        lcddisplay(sec);
            if(T0IF)
            {
                T0IF = 0;
                tcount++;
                sec = tcount/15;
                
                if(tcount<915)
                {
                    if(relayoff == 0)
                    {
                        
                        relayon=1;
                    }
                        
                }
                if(tcount>916)
                {
                    if(relayoff == 1)
                    {
                      timerup++;  
                    relayon=0;
                    }
                  
                }
                if(tcount>=993)
                {
                    
                    tcount = 0;
                    //relayoff = 0;
                    
                }
                if(wsensor)
                {
                    __delay_ms(1000);
                    if(wsensor)
                    {
                        timerup = 0;
                    prelay = 0;
                    tcount = 0;
                   
                    if(pump == 1)
                    {
                        if(waterchk == 1)
                        {
                        pump = 0;
                        goto ready;
                        
                        }
                        else
                        {
                            pump = 0;
                        goto intwater;
                        }
                    }
                    } 
                    
                }
                if((timerup==3)&&(waterflag == 1))
                {
                    
                    while(1)
                    {
                        timerup=0;
                        tcount = 0;
                        Lcd_Set_Cursor(1,2);
                        Lcd_Write_String("pump fault");
                        Lcd_Set_Cursor(2,2);
                        Lcd_Write_String("caution!!!!!");
                        buzz = 1;
                        __delay_ms(100);
                        buzz = 0;
                        __delay_ms(50);
                        RELAY = 0;
                        RELAYLED = 0;
                        prelay = 0;
                        if(wsensor)
                        {  
                            
                            pumpfail = 1;
                            goto ready;
                        }
                    }
                }
                if(relayon == 1)
                {
                    prelay = 1;
                    relayoff = 1;
                }
                if(relayon == 0)
                {
                    prelay = 0;
                    relayoff = 0;
                }
            }
        }
                   
    }
                
 }
 
}
