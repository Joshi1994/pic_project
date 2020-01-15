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

//--------------------------------------------------------------------------------------------------------------

#include <xc.h>
#define _XTAL_FREQ 4000000

//---------------------------------------------------------------------------------------------------------------

#define dsensor1        RB4
#define dsensor2        RB5
#define buzzer          RA3
#define stprelay1       RC0
#define stprelay2       RC1
#define blwrelay        RA6
#define lrelay          RA7
#define dswitch1        RB6
#define dswitch2        RB7
//----------------------------------------------------------------------------------------------------------------
#define ON              1
#define OFF             0
//----------------------------------------------------------------------------------------------------------------
int count = 0;

int main()
{
    TRISA       =   0x00;
    TRISB       =   0xFF;
    TRISC       =   0x00;
    PORTA = 0X00;
    PORTC = 0X00;
    WPUB		=	0XFF;
    OPTION_REG  =   0x07;
    while(1)
    {
        stprelay1 = ON;
        /*if(!)
            {
                //while(!dswitch1);
                //stprelay1 = ON;
                blwrelay = ON;
             }
        else
        {
            blwrelay = OFF;
        }*/
    
    }
}
