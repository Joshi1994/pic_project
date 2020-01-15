//----------------------------------------------------------------------------------------------------
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA3/MCLR pin function select (RA3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
//------------------------------------------------------------------------------------------------------------


#include <xc.h>
#define _XTAL_FREQ 4000000

//-------------------------------------------------------------------------------------------------------------------

#define Data_Pin    RA1
#define Clk_Pin     RA2
#define Enable_Pin  RA4

// Always mention this definition statement
unsigned short Low_Nibble, High_Nibble, p, q,  Mask, N,t, RS, Flag, temp;

void Delay_50ms(){
 __delay_ms(50);
}

void Write_LCD_Nibble(unsigned short N){
 Enable_Pin = 0;
 // ****** Write RS *********
 Clk_Pin = 0;
 Data_Pin = RS;
 Clk_Pin = 1;
 Clk_Pin = 0;
 // ****** End RS Write

 // Shift in 4 bits
 Mask = 8;
  for (t=0; t<4; t++){
   Flag = N & Mask;
   if(Flag==0) Data_Pin = 0;
   else Data_Pin = 1;
   Clk_Pin = 1;
   Clk_Pin = 0;
   Mask = Mask >> 1;
  }
  // One more clock because SC and ST clks are tied
  Clk_Pin = 1;
  Clk_Pin = 0;
  Data_Pin = 0;
  Enable_Pin = 1;
  Enable_Pin = 0;
}
// ******* Write Nibble Ends

 void Write_LCD_Data(unsigned short D){
 RS = 1; // It is Data, not command
 Low_Nibble = D & 15;
 High_Nibble = D/16;
 Write_LCD_Nibble(High_Nibble);
 Write_LCD_Nibble(Low_Nibble);
 }

void Write_LCD_Cmd(unsigned short C){
 RS = 0; // It is command, not data
 Low_Nibble = C & 15;
 High_Nibble = C/16;
 Write_LCD_Nibble(High_Nibble);
 Write_LCD_Nibble(Low_Nibble);
}

void Initialize_LCD(){
 Delay_50ms();
 Write_LCD_Cmd(0x20); // Wake-Up Sequence
 Delay_50ms();
 Write_LCD_Cmd(0x20);
 Delay_50ms();
 Write_LCD_Cmd(0x20);
 Delay_50ms();
 Write_LCD_Cmd(0x28); // 4-bits, 2 lines, 5x7 font
 Delay_50ms();
 Write_LCD_Cmd(0x0C); // Display ON, No cursors
 Delay_50ms();
 Write_LCD_Cmd(0x06); // Entry mode- Auto-increment, No Display shifting
 Delay_50ms();
 Write_LCD_Cmd(0x01);
 Delay_50ms();
}

void Position_LCD(unsigned short x, unsigned short y){
 temp = 127 + y;
 if (x == 2) temp = temp + 64;
 Write_LCD_Cmd(temp);
}

 
 void Lcd_Write_String(const char *a)
{
    
    
    int i;
	for(i=0;a[i]!='\0';i++)
	 Write_LCD_Data(a[i]);
   /*while(*s)
   Lcd_Write_Char(*s++);*/
}
 
 
 



void main() {
CMCON = 0x07;  // Disable Comparators
TRISA = 0b00000000;  // All Outputs except GP3
ANSEL = 0x00; // No analog i/p

Initialize_LCD();

while(1) 
{
        /*Position_LCD(1,4);
        Write_LCD_Text(Message1);
        Position_LCD(2,2);
        Write_LCD_Text(Message2);
        __delay_ms(1500);
        Write_LCD_Cmd(0x01);  // Clear LCD
        __delay_ms(1000);*/
    //RA2 = 1;
   // __delay_ms(1000);
    //RA2 = 0;
    //__delay_ms(1000);
    Lcd_Write_String("akjdf");
    
} 

}