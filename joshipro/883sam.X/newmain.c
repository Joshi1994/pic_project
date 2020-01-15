
/////////////////////////////////////////////////////////////////////////////////////////////////


/*	Name		:	ORBITO  
 *	Purpose		:	ELASTIC TAPE CUTTING MACHINE 
 *  Author		:	
 *  Date		:	24/12/2017
 *  Revision	:	NONE
 */

////////////////////////////////////////////////////////////////////////////////////////////////

// PIC16F883 Configuration Bit Settings

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

#include<xc.h>
#define _XTAL_FREQ 4000000	//4MHZ FREQUENCY 

//defining other I/O pins//

//10 keys switches used///

#define     MENU_KEY      	 		RA5			
#define 	MODE_KEY 				RB7				
#define 	SHIFT_KEY				RA7			
#define 	ENTER_KEY				RA4	
#define 	INC_KEY					RA3	
#define 	DEC_KEY					RA6	
#define 	FWD_KEY					RE3	
#define 	REV_KEY					RB4	
#define		START_KEY				RB6
#define		CUTTER_KEY				RB5

//LED's used//		
#define 	AUTO_LED  				RA2			
#define 	MAN_LED 				RA1	
#define 	PROCESS_LED 			RA0	

//for stepper drive//		
#define 	CLOCK  					RC5			
#define 	DIR  					RC4		
#define 	ENABLE 					RC3	

//miscellaneous//
#define		BUZZER					RC1
#define 	POWER_FAIL   			RC0
#define		RELAY					RC2	


//defining LCD pins//
#define	 	LCD_E   				RC5			//LCD Enable pin
#define 	LCD_RS  				RC4		//LCD Register Select pin
#define 	LCD_E_Dir  				TRISC7		//LCD Enable directory
#define 	LCD_RS_Dir 				TRISC6		//LCD Register Select directory
#define 	LCD_Data_Bus_D4			RB0			//LCD Data pins D4-D7
#define 	LCD_Data_Bus_D5 		RB1
#define 	LCD_Data_Bus_D6 		RB2
#define 	LCD_Data_Bus_D7 		RB3
#define 	LCD_Data_Bus_Dir_D4 	TRISB0		//LCD Data pins directory
#define 	LCD_Data_Bus_Dir_D5 	TRISB1
#define 	LCD_Data_Bus_Dir_D6 	TRISB2
#define 	LCD_Data_Bus_Dir_D7 	TRISB3



void PORT_Config()
{
	TRISA		=	0XF8;		//RA0-input;RA1-RA5-output
	TRISB		=	0XF0;		//RB0-RB3-output;RB4-RB7-input
	TRISC		=	0X01;		//RC3-input;rest of the pins-output
	TRISE		=	0X08;
	PORTB 		=	0X00;		//clearing port initially
	PORTC 		=	0X00;		//clearing port initially
	PORTA		=	0X00;		//clearing port initially
	PORTE		=	0X00;
	OPTION_REG	= 	0X07;		//Configuring timer 0 with prescale as 256
	INTCON		= 	0X80;		//Enabling the global interrupts
	ANSEL		= 	0X00;		//setting all analog pins as digital 
	ANSELH		= 	0X00;		//setting all analog pins as digital 
	WPUB		=	0XF0;		//Enable the internal pull-ups of the pins RB7-RB4
	IOCB		=	0X00;		//Disable interrupt on port change 
	OSCCON		=	0X65;		//Internal oscillator with 4Mhz frequency and clock stability		
}

void ToggleEpinOfLCD(void)
{
	LCD_E = 1;               // Give a pulse on E pin
	__delay_ms(1);      	// so that LCD can latch the data from data bus
	LCD_E = 0;                 
	__delay_ms(1); 	
}


void LCD_CmdWrite(unsigned char Command)  
{
	LCD_RS = 0;				  		// It is a command

		
	PORTB = ((Command>>4)&0x0F); 	// Write upper nibble of data
	ToggleEpinOfLCD();		  		// Give pulse on E pin
	
      	
	PORTB = (Command&0x0F);  		// Write lower nibble of data
	ToggleEpinOfLCD();			  	// Give pulse on E pin	
	
	
}

void LCD_DataWrite(char LCDChar)  
{
	LCD_RS = 1;				

	PORTB = ((LCDChar>>4)&0x0F); 	// Write upper nibble of data
	ToggleEpinOfLCD();		  		// Give pulse on E pin
	
     		 
	PORTB = (LCDChar&0x0F);  		// Write lower nibble of data
	ToggleEpinOfLCD();		  		// Give pulse on E pin	
	

}


void LCD_Init(void)
{ 
	// Firstly make all pins output
	LCD_E  		 		 = 0;   // E  = 0
	LCD_RS  	 		 = 0;   // RS = 0
	LCD_Data_Bus_D4		 = 0;  	// Data bus = 0
	LCD_Data_Bus_D5		 = 0;  	// Data bus = 0
	LCD_Data_Bus_D6		 = 0;  	// Data bus = 0
	LCD_Data_Bus_D7		 = 0;  	// Data bus = 0
	LCD_E_Dir    		 = 0;   // Make Output
	LCD_RS_Dir    	 	 = 0;   // Make Output
	LCD_Data_Bus_Dir_D4  = 0;   // Make Output
	LCD_Data_Bus_Dir_D5  = 0;   // Make Output
	LCD_Data_Bus_Dir_D6  = 0;   // Make Output
	LCD_Data_Bus_Dir_D7  = 0;   // Make Output

    LCD_CmdWrite(0x02);
	LCD_CmdWrite(0x28);    //function set
	LCD_CmdWrite(0x0c);    //display on,cursor off,blink off
	LCD_CmdWrite(0x01);    //clear display
	LCD_CmdWrite(0x06);    //entry mode, set increment
    
}


void LCD_DisplayString(const char *s)
{
	while(*s)
		LCD_DataWrite(*s++);   // print first character on LCD 
}


void LCD_Clear(void)       // Clear the Screen and return cursor to zero position
{
	LCD_CmdWrite(0x01);    // Clear the screen
	__delay_ms(2);         // Delay for cursor to return at zero position
}
	
	
void main()
{
PORT_Config();
LCD_Init();
__delay_ms(100);
while(1)
{
LCD_Clear();
LCD_CmdWrite(0x80);
LCD_DisplayString("BALAJI.P");
PORTC=0XFF;
__delay_ms(1000);
LCD_Clear();
LCD_CmdWrite(0x80);
LCD_DisplayString("WELCOME......");
PORTC=0X00;
__delay_ms(1000);
}
}