#include<xc.h>

#define RS RC2
#define EN RC3
#define D0 RA0
#define D1 RA1
#define D2 RA2
#define D3 RA3
#define D4 RA4
#define D5 RA5
#define D6 RC0
#define D7 RC1


#define _XTAL_FREQ 8000000
#include "lcd.h"



void main()
{
 int i;
  TRISA = 0x00;
  TRISC = 0x00;
  ANSEL = 0x00;
  CMCON = 0x07;
  
  Lcd8_Init();
  while(1)
  {
    Lcd8_Set_Cursor(1,1);
    Lcd8_Write_String("electroSome LCD Hello World");
    for(i=0;i<15;i++)
    {
      __delay_ms(100);
      Lcd8_Shift_Left();
    }
    for(i=0;i<15;i++)
    {
      __delay_ms(100);
      Lcd8_Shift_Right();
    }
    Lcd8_Clear();
    Lcd8_Set_Cursor(2,1);
    Lcd8_Write_Char('e');
    Lcd8_Write_Char('S');
    __delay_ms(2000);
  }
}