
// PIC16F676 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#define _XTAL_FREQ 4000000

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA3/MCLR pin function select (RA3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
char D[10];
#define Data_Pin    RC0
#define Clk_Pin     RC1
#define RS RC4
#define EN RC5
/*#define D0 RA0
#define D1 RA1
#define D2 RA2
#define D3 RC2
#define D4 RA4
#define D5 RA5
#define D6 RC0
#define D7 RC1
*/






void Lcd8_Port(char a)
{
	if(a & 1)
		D[0] = 1;
	else 
		D[0] = 0;
	
	if(a & 2)
		D[1] = 1;
	else
		D[1] = 0;
	
	if(a & 4)
		D[2] = 1;
	else
		D[2] = 0;
	
	if(a & 8)
		D[3] = 1;
	else
		D[3] = 0;
	
	if(a & 16)
		D[4] = 1;
	else
		D[4] = 0;

	if(a & 32)
		D[5] = 1;
	else
		D[5] = 0;
	
	if(a & 64)
		D[6] = 1;
	else 
		D[6] = 0;
	
	if(a & 128)
		D[7] = 1;
	else
		D[7] = 0;
    
    for(int i = 0;i<=7;i++)
    {
        Clk_Pin = 1;
        __delay_us(10);
        Data_Pin = D[i];
        //__delay_us(10);
        Clk_Pin = 0;
        //Clk_Pin = 1;
        //Clk_Pin = 0;
        
    }
    //Clk_Pin = 1;
}
void Lcd8_Cmd(char a)
{ 
  RS = 0;             // => RS = 0
  Lcd8_Port(a);             //Data transfer
  EN  = 1;             // => E = 1
  __delay_ms(5);
  EN  = 0;             // => E = 0
}

Lcd8_Clear()
{
	  Lcd8_Cmd(1);
}

void Lcd8_Set_Cursor(char a, char b)
{
	if(a == 1)
	  Lcd8_Cmd(0x80 + b);
	else if(a == 2)
		Lcd8_Cmd(0xC0 + b);
}

void Lcd8_Init()
{
	Lcd8_Port(0x00);
	RS = 0;
	__delay_ms(25);
	///////////// Reset process from datasheet /////////
  Lcd8_Cmd(0x30);
__delay_ms(5);
  Lcd8_Cmd(0x30);
	__delay_ms(15);
  Lcd8_Cmd(0x30);
  /////////////////////////////////////////////////////
  Lcd8_Cmd(0x38);    //function set
  Lcd8_Cmd(0x0C);    //display on,cursor off,blink off
  Lcd8_Cmd(0x01);    //clear display
  Lcd8_Cmd(0x06);    //entry mode, set increment
}

void Lcd8_Write_Char(char a)
{
   RS = 1;             // => RS = 1
   Lcd8_Port(a);             //Data transfer
   EN  = 1;             // => E = 1
  __delay_ms(4);
   EN  = 0;             // => E = 04
}

void Lcd8_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	 Lcd8_Write_Char(a[i]);
}

void Lcd8_Shift_Right()
{
	Lcd8_Cmd(0x1C);
}

void Lcd8_Shift_Left()
{
	Lcd8_Cmd(0x18);
}

void main()
{
 
 ANSEL = 0x00;
 CMCON = 0x07;
 
  TRISA = 0x00;
  
  TRISC = 0x00;
  Lcd8_Init();
  Lcd8_Set_Cursor(1,1);
    //Lcd8_Write_String("SDSAD");
  while(1)
  {
      
      Lcd8_Write_String("SDSAD");
    
    
  }
}