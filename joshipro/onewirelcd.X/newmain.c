
// PIC16F676 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
__EEPROM_DATA(0X00,0X00,0X09,0X00,0X00,0X00,0X00,0X00);
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
#define _LCD_CURSOR_OFF                 0x0C         //Turn off cursor


#define Serial_Data_Pin RC0
#define Serial_Clk_Pin  RC2
#define Enable_Pin      RC5
#define TS          RA1
#define TS_DIR      TRISA1
#define inc         RA2
#define dec         RA3
#define prg         RA4

//Function Prototypes
void Serial_LCD_Cmd(unsigned char Command);
void Serial_LCD_Chr(unsigned int row, unsigned int col, char LCDChar);
void Serial_LCD_Init();

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
		__delay_us(600);
			
		reset();						
		write(0xcc);				
		write(0xbe);				
			
		tempL = read();					
		tempH = read();					
		temp=((tempH << 8 )| tempL);	
		temp = temp /16 ;	             
    }
     return temp;
}
/*void Write_EEPROM(unsigned char a1,unsigned char a2,unsigned int data_value)//Writing data into specified location in EEPROM
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
}*/


void main() {
        
        CMCON = 0x07;
        ANSEL = 0x00;                          // No analog i/p
        TRISC = 0x00;
        //TRISA = 0xFF;
        TRISA1 = 1;
        TRISA2 = 1;
        TRISA3 = 1;
        Serial_LCD_Init();
        Serial_LCD_Cmd(_LCD_CLEAR);
    Serial_LCD_Cmd(_LCD_CURSOR_OFF);
    int tempval = 0;
    char storedval = 0;
    char b1=0,b2=0;
     //storedval = Read_EEPROM(0,1);
    b1=eeprom_read(0);		//read first digit of data
	b2=eeprom_read(1);		//read second digit of data
	storedval = ((b1*10)+b2); 	//return the data read
    while(1)
        {
               tempval = checktemp();
               Lcd_Write_String(1,1,"Current temp");
               lcddisplay(1,14,tempval);
               
                  
            }
}