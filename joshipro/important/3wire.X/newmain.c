
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


//------------------------------------------------------------------------------------------------
/* 3-wire Serial LCD using 74HC595
    Jayanth Devarayanadurga
    08/05/2013
*/

#define _LCD_FIRST_ROW                         0x80         //Move cursor to the 1st row
#define _LCD_SECOND_ROW                 0xC0         //Move cursor to the 2nd row

#define _LCD_CLEAR                         0x01         //Clear display
//#define _LCD_RETURN_HOME                 0x02         //Return cursor to home position, returns a shifted display to
                                                 //its original position. Display data RAM is unaffected.
#define _LCD_CURSOR_OFF                 0x0C         //Turn off cursor
//#define _LCD_UNDERLINE_ON                 0x0E         //Underline cursor on
//#define _LCD_BLINK_CURSOR_ON            0x0F          //Blink cursor on
//#define _LCD_MOVE_CURSOR_LEFT           0x10          //Move cursor left without changing display data RAM
//#define _LCD_MOVE_CURSOR_RIGHT          0x14           //Move cursor right without changing display data RAM
//#define _LCD_TURN_ON                         0x0C         //Turn Lcd display on
//#define _LCD_TURN_OFF                         0x08         //Turn Lcd display off
//#define _LCD_SHIFT_LEFT                 0x18         //Shift display left without changing display data RAM
//#define _LCD_SHIFT_RIGHT                 0x1E         //Shift display right without changing display data RAM

#define Serial_Data_Pin RC0
#define Serial_Clk_Pin  RC2
#define Enable_Pin      RC5

//Function Prototypes
void Serial_LCD_Cmd(unsigned char Command);
void Serial_LCD_Chr(unsigned int row, unsigned int col, char LCDChar);
void Serial_LCD_Init();
//void Serial_LCD_Out(unsigned int row, unsigned int col, unsigned char str[20]);
void Write_Nibble(unsigned short N);

// Always mention this definition statement
unsigned short Low_Nibble, High_Nibble, p, q,  Mask, N, t, RS, Flag, temp;

void Delay_50ms(){
 __delay_ms(50);
}

void Write_Nibble(unsigned short N){
 Enable_Pin = 0;
 // ****** Write RS *********
 Serial_Clk_Pin = 0;
 Serial_Data_Pin = RS;
 Serial_Clk_Pin = 1;
 Serial_Clk_Pin = 0;
 // ****** End RS Write

 // Shift in 4 bits
 Mask = 8;
  for (t=0; t<4; t++){
   Flag = N & Mask;
   if(Flag==0) Serial_Data_Pin = 0;
   else Serial_Data_Pin = 1;
   Serial_Clk_Pin = 1;
   Serial_Clk_Pin = 0;
   Mask = Mask >> 1;
  }
  // One more clock because SC and ST clks are tied
  Serial_Clk_Pin = 1;
  Serial_Clk_Pin = 0;
  Serial_Data_Pin = 0;
  Enable_Pin = 1;
  __delay_us(500);
  Enable_Pin = 0;
}

void Serial_LCD_Cmd(unsigned char Command)
{
      RS = 0; // It is command, not data
      Low_Nibble = Command & 15;
      High_Nibble = Command/16;
      Write_Nibble(High_Nibble);
      Write_Nibble(Low_Nibble);
      if((Command == 0x0C) || (Command == 0x01) || (Command == 0x0E) || (Command == 0x0F) || (Command == 0x10)
                   || (Command == 0x1E) || (Command == 0x18) || (Command == 0x08) || (Command == 0x14) 
                   || (Command == 0x02))
      Delay_50ms();
}


void Serial_LCD_Chr(unsigned int row, unsigned int col, char LCDChar)
{
        switch(row){

                case 1:
                Serial_LCD_Cmd(0x80 + col-1);
                break;
                case 2:
                Serial_LCD_Cmd(0xC0 + col-1);
                break;
                case 3:
                Serial_LCD_Cmd(0x94 + col-1);
                break;
                case 4:
                Serial_LCD_Cmd(0xD4 + col-1);
                break;
        }

        RS = 1; // It is Data, not command
        Low_Nibble = LCDChar & 15;
        High_Nibble = LCDChar/16;
        Write_Nibble(High_Nibble);
        Write_Nibble(Low_Nibble);

}


void Serial_LCD_Init(){
        Delay_50ms();
        Serial_LCD_Cmd(0x03); // Wake-Up Sequence
        Delay_50ms();
        Serial_LCD_Cmd(0x03);
        Delay_50ms();
        Serial_LCD_Cmd(0x03);
        Delay_50ms();
        Serial_LCD_Cmd(0x02);
        Delay_50ms();
        Serial_LCD_Cmd(0x28); // 4-bits, 2 lines, 5x7 font
        Delay_50ms();
        Serial_LCD_Cmd(0x06); // Entry mode- Auto-increment, No Display shifting
        Delay_50ms();
}


void Lcd_Write_String(unsigned int row, unsigned int col,char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
    {
        Serial_LCD_Chr(row,col++,a[i]);
    }	 //Write_LCD_Data(a[i]);
    row = 1;
        col = 1;
}
void lcddisplay(unsigned int row, unsigned int col,unsigned int num)
{
   
 Serial_LCD_Chr(row,col++,(num/10)+0x30);
   Serial_LCD_Chr(row,col++,(num%10)+0x30);
 
   //Lcd_Write_Char((num%10)+0x30);

 
    
}

void main() {
        
        ANSEL = 0x00;                          // No analog i/p
        TRISC = 0x00;
        Serial_LCD_Init();
        Serial_LCD_Cmd(_LCD_CLEAR);
    Serial_LCD_Cmd(_LCD_CURSOR_OFF);

        while(1){
               Lcd_Write_String(1,1,"Hellooo");
               //Lcd_Write_String(2,1,"sadjfh0000asd");
               for(int i=0;i<=10;i++)
               {
                   lcddisplay(2,1,i);
                   __delay_ms(1000);
               }
               
        }
}