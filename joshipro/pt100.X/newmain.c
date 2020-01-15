#define _XTAL_FREQ 4000000

#define RS          RC4
#define EN          RC5
#define D4          RB0
#define D5          RB1
#define D6          RB2
#define D7          RB3
#define RELAY       RC0
#define RELAYLED    RA3
#include <xc.h>


#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA3/MCLR pin function select (RA3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = ON       // Brown-out Detect Enable bit (BOD enabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)






void main()
{
    
    TRISA = 0x00;
    TRISB = 0xF0;
    TRISC = 0x00;
   
 
     while(1)
    {
         RELAYLED  =  1;
         __delay_ms(1000);
         RELAYLED = 0;
      __delay_ms(1000);
         
     }
  
  }
