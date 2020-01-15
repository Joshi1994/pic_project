#define _XTAL_FREQ 4000000

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1
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

#define RS          RB0
#define EN          RB1
#define D4          RB2
#define D5          RB3
#define D6          RB4
#define D7          RB5

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

int main() {
    
    
   SSPCON = 0x25;
   SSPSTAT = 0;
    ANSELH = 0x00;
    ANSEL = 0x00;
    ADCON0 = 0x14;
   TRISA5 = 1;
   TRISB = 0;
   
   TRISC5 = 0;
   TRISC3 = 1;
   //TRISC1 = 1;
   
   //PORTD = 0x00;
   PORTB = 0x00;
  Lcd_Init();
    Lcd_Clear();
   
    int a = 0;
    int i=0;
    char var[60];
    char b;
    while(1)
    {
    
        
        if(BF)
        { 
             //
               b = SSPBUF;
                Lcd_Write_Char(b);
                   //__delay_ms(1000);
              
            //Lcd_Write_Char(b);
                       
        }
            
            
            /*a++;
            if(a>7)
            {
                a=0;
                {
                    
                    for(i=0;var[i]!='\0';i++)
                    {
                    Lcd_Write_Char(var[i]);
                  
                    }
                    __delay_ms(3000);
                }
            }*/
            
        }
    }
     
