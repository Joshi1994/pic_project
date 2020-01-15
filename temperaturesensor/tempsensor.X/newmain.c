#include <xc.h>

#include "lcd_hd44780_pic16.h"




void main (void)
{
  //Initialize the LCD Module
  LCDInit(LS_NONE);

  //Clear the display
  LCDClear();

  //Write a string
  LCDWriteString("Hello World !");

  while(1)
  {
     //Do nothing, just loop indefinitely
  }

}