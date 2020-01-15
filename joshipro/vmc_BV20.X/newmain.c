//DATE:19.12.17 TUESDAY 12.44pm
//------------------------------------------------------------------------------------------------------------
#include <xc.h>
#include<stdio.h>

//------------------------------------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------------------------------------------


#define _XTAL_FREQ 4000000

//defining LCD pins//

#define    LCD_E               		RA6
#define    LCD_RS              		RA7
#define    LCD_E_Dir           		TRISA6
#define    LCD_RS_Dir          		TRISA7
#define    LCD_Data_Bus_D4      	RC0
#define    LCD_Data_Bus_D5      	RC1
#define    LCD_Data_Bus_D6       	RC2
#define    LCD_Data_Bus_D7       	RC3
#define    LCD_Data_Bus_Dir_D4      TRISC0
#define    LCD_Data_Bus_Dir_D5      TRISC1
#define    LCD_Data_Bus_Dir_D6      TRISC2
#define    LCD_Data_Bus_Dir_D7      TRISC3


//defining other I/O pins//

#define    MENU           		RE3
#define    DEC               	RE0
#define    INC               	RE1
#define    RST                  RE2
//#define    RUN_LED              RA0
#define    NO_PAD_LED          	RA3

#define    LIMIT_SW1          	RD7
#define    LIMIT_SW2          	RD5
#define    LIMIT_SW3          	RC7
#define    LIMIT_SW4          	RC5
#define    LIMIT_SW5          	RD3

#define    COIN_BOX_VDD         RB1

#define    MOTOR1             	RD6
#define    MOTOR2             	RD4
#define    MOTOR3             	RC6
#define    MOTOR4             	RC4
#define    MOTOR5             	RD2

#define        MOTOR_SEL1       RA1
#define        MOTOR_SEL2       RA2 
#define        MOTOR_SEL3       RA5
#define        MOTOR_SEL4       RB6
#define        MOTOR_SEL5       RB7

#define   CONFIG_MS1         TRISA1
#define   CONFIG_MS2         TRISA0
#define   CONFIG_MS3         TRISA5
#define   CONFIG_MS4         TRISB6
#define   CONFIG_MS5         TRISB7   

#define     ESC_KEY            RA4
#define     BUZZER             RD1

#define      PASSWORD        05
#define      A               1
#define      B               2
#define      C               3
#define      D               4
#define      E               5

#define      OUTPUT           0
#define      INPUT            1

#define      CH_10            RB2
#define      CH_20            RB3
#define      CH_50            RB4
#define      CH_100           RB5

__EEPROM_DATA(0X03,0X00,0X03,0X00,0X03,0X00,0X03,0X00);
__EEPROM_DATA(0X03,0X00,0X03,0X00,0X03,0X00,0X03,0X00);
__EEPROM_DATA(0X03,0X00,0X03,0X00,0X00,0X01,0X00,0X00);
__EEPROM_DATA(0X02,0X00,0X00,0X03,0X00,0X00,0X05,0X00);
__EEPROM_DATA(0X01,0X00,0X00,0X00,0X00,0X00,0X00,0X00);

void ToggleEpinOfLCD(void);
void LCD_CmdWrite(unsigned char Command) ;
void LCD_DataWrite(char LCDChar);
void LCD_Init(void);
void LCD_DisplayString(const char *s);
void LCD_Clear(void) ;

void PORT_Init();
void displaypass(int data);
void display(int data);
void display_price(int data);

void buzzer(unsigned int a);
void LCD_BuildChar(unsigned char loc,unsigned char *p);
void Write_EEPROM(unsigned char a1,unsigned char a2,unsigned int data_value);
void Write_EEPROM_Price(unsigned char a1,unsigned char a2,unsigned char a3,unsigned int data_value);
unsigned int Read_EEPROM(unsigned char b1,unsigned char b2);
unsigned int Read_EEPROM_Price(unsigned char b1,unsigned char b2,unsigned char b3);
void STOCK_EMPTY_ROUTINE();
void Set_Channel(unsigned int data);
void Monitor_Balance(unsigned int amount);

void escprocess();

unsigned int padcount1=0,rupee1=0,padcount2=0,rupee2=0,balance=0,padcount=0,rupee=0;
unsigned int padcount3=0,rupee3=0,padcount4=0,rupee4=0,padcount5=0,rupee5=0;
unsigned int value=0,error_count=0,count=0,num=0;
unsigned int stock=0;
unsigned char stock_data=0;
bit status=0,menu=0,set=0,check=0,pad=0,coin=0,run=0,coin_input=0,motor=0,Qflag=0;
unsigned char RupeeSymbol[8]={0X1F,0X02,0X0F,0X02,0X1C,0X08,0X04,0X02}; 

void interrupt COINCOUNTER()
{
   if(INTF==1)
   {
      INTF=0;
      coin_input=1;
      count=count+10;
      balance=rupee-count;
      Monitor_Balance(balance);
      
      if(count==rupee)
      {
         INTE=0;   
         count=0;
         balance=0;
         coin_input=0;
         COIN_BOX_VDD=1;
         motor=1;
      }
   }
}
int start = 0;
char dispvalue[10];
void main()

{
   __delay_ms(50);
   PORT_Init();
   LCD_Init();
   CH_10=0;CH_20=0;CH_50=0;CH_100=0;

   if((!INC)&&(!DEC))      //checking for factory reset option
   {
      LCD_DisplayString(" FACTORY RESET  ");
      LCD_CmdWrite(0XC0);
      LCD_DisplayString(" INITIALIZING... ");
      buzzer(1000);
      __delay_ms(500);
      LCD_Clear();
      LCD_DisplayString("  FLASHING....  ");
      buzzer(100);
      __delay_ms(2500);
      
      __EEPROM_DATA(0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00);
      __EEPROM_DATA(0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00);
      __EEPROM_DATA(0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00);
      __EEPROM_DATA(0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00);
      
      padcount1=0;padcount2=0;padcount3=0;padcount4=0;padcount5=0;
      rupee1=10;rupee2=10;rupee3=10;rupee4=10;rupee5=10;
      balance=0;padcount=0;rupee=0;
      value=0;error_count=0;count=0;num=0;stock=0;stock_data=0;
      status=0;menu=0;set=0;check=0;pad=0;coin=0;run=0;coin_input=0;motor=0;Qflag=0;
      
       Write_EEPROM_Price(0X14,0X15,0X16,rupee1);
      Write_EEPROM_Price(0X17,0X18,0X19,rupee2);
      Write_EEPROM_Price(0X1A,0X1B,0X1C,rupee3);
      Write_EEPROM_Price(0X1D,0X1E,0X1F,rupee4);
      Write_EEPROM_Price(0X20,0X21,0X22,rupee5);

      LCD_Clear();
      LCD_DisplayString(" MEMORY FLASHED! ");
      buzzer(1000);
      __delay_ms(1000);
      menu=1;run=1;      //setting run flag and menu flag to go to menu loop
      buzzer(70);
      LCD_Clear();
      goto flash;         //go to the menu loop immediately after flashing
   }
      

   COIN_BOX_VDD=1;INTE=0;   

   padcount1=Read_EEPROM(0,1);
   padcount2=Read_EEPROM(4,5);
   padcount3=Read_EEPROM(8,9);
   padcount4=Read_EEPROM(0X0C,0X0D);
   padcount5=Read_EEPROM(0X10,0X11);
   
   rupee1=Read_EEPROM_Price(0x14,0x15,0X16);
   rupee2=Read_EEPROM_Price(0X17,0X18,0X19);
   rupee3=Read_EEPROM_Price(0X1A,0X1B,0X1C);
   rupee4=Read_EEPROM_Price(0X1D,0X1E,0X1F);
   rupee5=Read_EEPROM_Price(0X20,0X21,0X22);



   LCD_DisplayString("   WELCOME TO   ");  //LCD display for the starting part
   LCD_CmdWrite(0XC0);
   LCD_DisplayString("DHARSAN MACHINES");
   buzzer(2000);
   __delay_ms(1000);
   buzzer(50);
   LCD_Clear();
   run=1;

   while(run)
   {   
            CONFIG_MS1 = INPUT;
            CONFIG_MS2 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS4 = INPUT;
            CONFIG_MS5 = INPUT;
      while(!status)
      { 
         LCD_CmdWrite(0X80);
         LCD_DisplayString("   SELECT ROW  ");
         LCD_CmdWrite(0XC0);
          LCD_DisplayString("_1__2__3__4__5__");
          
          if(!MOTOR_SEL1)
          {
            while(!MOTOR_SEL1);
            M1:
            if(padcount1==0)
            {
               STOCK_EMPTY_ROUTINE();
               break;
            }
            INTE=1;status=1;
            stock=A;
            padcount=padcount1;
            rupee=rupee1;
            if(LIMIT_SW1==0)
            COIN_BOX_VDD=0;
            CONFIG_MS1 = OUTPUT; //Make the switch as output pin
            MOTOR_SEL1=0;       // make it low
            CONFIG_MS2 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS4 = INPUT;
            CONFIG_MS5 = INPUT;
            Set_Channel(rupee);
            buzzer(50);count=0;coin_input=0;balance=0;
            LCD_Clear();
            start = 1;
          }
         
         if(!MOTOR_SEL2)
          {	
            while(!MOTOR_SEL2);
            M2:
            if(padcount2==0)
            {
               STOCK_EMPTY_ROUTINE();
               break;
            }
            INTE=1;
            status=1;
            stock=B;
            padcount=padcount2;
            rupee=rupee2;
            if(LIMIT_SW2==0)
            COIN_BOX_VDD=0;
            CONFIG_MS2 = OUTPUT; //Make the switch as output pin
            MOTOR_SEL2=0;
            CONFIG_MS1 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS4 = INPUT;
            CONFIG_MS5 = INPUT;
            Set_Channel(rupee);
            buzzer(50);count=0;coin_input=0;balance=0;
            LCD_Clear();
            start = 2;
         }
          
         if(!MOTOR_SEL3)
         {	
            while(!MOTOR_SEL3);
            M3:
            if(padcount3==0)
            {
               STOCK_EMPTY_ROUTINE();break;
            }
            INTE=1;status=1;
            stock=C;
            padcount=padcount3;
            rupee=rupee3;
            if(LIMIT_SW3==0)
            COIN_BOX_VDD=0;
            CONFIG_MS3 = OUTPUT; //Make the switch as output pin
            MOTOR_SEL3=0;
            CONFIG_MS1 = INPUT;
            CONFIG_MS2 = INPUT;
            CONFIG_MS4 = INPUT;
            CONFIG_MS5 = INPUT;
            Set_Channel(rupee);
            buzzer(50);count=0;coin_input=0;balance=0;
            LCD_Clear();
            start = 3;
         }
         if(!MOTOR_SEL4)
         {	
            while(!MOTOR_SEL4);
            M4:
            if(padcount4==0)
            {
               STOCK_EMPTY_ROUTINE();break;
            }
            INTE=1;status=1;
            stock=D;
            padcount=padcount4;
            rupee=rupee4;
            if(LIMIT_SW4==0)
            COIN_BOX_VDD=0;
            CONFIG_MS4 = OUTPUT; //Make the switch as output pin
            MOTOR_SEL4=0;
            
            CONFIG_MS1 = INPUT;
            CONFIG_MS2 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS5 = INPUT;
            Set_Channel(rupee);
            buzzer(50);count=0;coin_input=0;balance=0;
            LCD_Clear();
            start = 4;
         }
         if(!MOTOR_SEL5)
         {	
            while(!MOTOR_SEL5);
            M5:
            if(padcount5==0)
            {
               STOCK_EMPTY_ROUTINE();break;
            }
            
            
            INTE=1;status=1;
            stock=E;
            padcount=padcount5;
            rupee=rupee5;
            if(LIMIT_SW5==0)
            COIN_BOX_VDD=0;
            CONFIG_MS5 = OUTPUT; //Make the switch as output pin
            MOTOR_SEL5=0;
            CONFIG_MS1 = INPUT;
            CONFIG_MS2 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS4 = INPUT;
            Set_Channel(rupee);
            buzzer(50);count=0;coin_input=0;balance=0;
            LCD_Clear();
            start = 5;
         }
         if(MENU==0)
         {
            while(MENU==0);
            status=0;
            menu=1;
            buzzer(50);
            LCD_Clear();
            start = 0;
            break;
         }
      }

      while(status)
      {
         
         LCD_CmdWrite(0X80);
         LCD_DisplayString(" ROW_");
         switch(stock)
         {
            case A: stock_data='1';break;
            case B: stock_data='2';break;
            case C: stock_data='3';break;
            case D: stock_data='4';break;
            case E: stock_data='5';break;
         }
         LCD_DataWrite(stock_data);
         LCD_DataWrite(':');
         display(padcount);
         LCD_DisplayString(" / ");
         LCD_DataWrite(0x00); 
         display_price(rupee);
         LCD_CmdWrite(0XC0);
         LCD_DisplayString(" INSERT NOTES...");
        
         
         if(!ESC_KEY)
         {
            while(!ESC_KEY);
            padcount=0;rupee=0;stock=0;
            INTE=0;COIN_BOX_VDD=1;
            status=0;
            CONFIG_MS1 = INPUT;    //Make all the switches again back to input. 
            CONFIG_MS2 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS4 = INPUT;
            CONFIG_MS5 = INPUT;
            LCD_Clear();
            buzzer(50);
            start = 0;
         }
         
         
         
         if(start == 1)                             //Reselecting ROWS
         {
             if(MENU==0)
             {
                while(MENU==0);
                status=0;
                menu=1;
                buzzer(50);
                LCD_Clear();
                start = 0;
                break;
             }
             if(!MOTOR_SEL2)
             {
                while(!MOTOR_SEL2);
                escprocess();
                goto M2;
             }
             if(!MOTOR_SEL3)
             {
                while(!MOTOR_SEL3);
                escprocess();
                goto M3;
             }
             if(!MOTOR_SEL4)
             {
                while(!MOTOR_SEL4);
                escprocess();
                goto M4;
             }
             if(!MOTOR_SEL5)
             {
                while(!MOTOR_SEL5);
                escprocess();
                goto M5;
             }
         }
         if(start == 2)
         {
             if(MENU==0)
            {
                while(MENU==0);
                status=0;
                menu=1;
                buzzer(50);
                start = 0;
                LCD_Clear();
                break;
            }     
             
            if(!MOTOR_SEL1)
            {
                while(!MOTOR_SEL1);
                escprocess();
                goto M1;
            }
            if(!MOTOR_SEL3)
            {
                while(!MOTOR_SEL3);
                escprocess();
                goto M3;
            }
            if(!MOTOR_SEL4)
            {
                while(!MOTOR_SEL4);
                escprocess();
                goto M4;
            }
            if(!MOTOR_SEL5)
            {
                while(!MOTOR_SEL5);
                escprocess();
                goto M5;
            }
        
         
         }
         if(start == 3)
         {
             if(MENU==0)
             {
                while(MENU==0);
                status=0;
                menu=1;
                buzzer(50);
                LCD_Clear();
                start = 0;
                break;
             }
             if(!MOTOR_SEL1)
             {
                while(!MOTOR_SEL1);
                escprocess();
                goto M1;
             }
             if(!MOTOR_SEL2)
             {
                while(!MOTOR_SEL2);
                escprocess();
                goto M2;
             }
             if(!MOTOR_SEL4)
             {
                while(!MOTOR_SEL4);
                escprocess();
                goto M4;
             }
             if(!MOTOR_SEL5)
             {
                while(!MOTOR_SEL5);
                escprocess();
                goto M5;
             }
         }
         if(start == 4)
         {
             if(MENU==0)
             {
                while(MENU==0);
                status=0;
                menu=1;
                buzzer(50);
                LCD_Clear();
                start = 0;
                break;
             }
             if(!MOTOR_SEL1)
             {
                while(!MOTOR_SEL1);
                escprocess();
                goto M1;
             }
             if(!MOTOR_SEL2)
             {
                while(!MOTOR_SEL2);
                escprocess();
                goto M2;
             }
             if(!MOTOR_SEL3)
             {  
                while(!MOTOR_SEL3);
                escprocess(); 
                goto M3;
             }
             if(!MOTOR_SEL5)
             {
                while(!MOTOR_SEL5);
                escprocess();
                goto M5;
             }
             
         }
         if(start == 5)
         {
             if(MENU==0)
             {
                while(MENU==0);
                status=0;
                menu=1;
                buzzer(50);
                LCD_Clear();
                start = 0;
                break;
              }
             if(!MOTOR_SEL1)
             {
                while(!MOTOR_SEL1);
                escprocess();
                goto M1;
             }
             if(!MOTOR_SEL2)
             {
                while(!MOTOR_SEL2);
                escprocess();
                goto M2;
             }
             if(!MOTOR_SEL3)
             {
                while(!MOTOR_SEL3);
                escprocess();
                goto M3;
             }
             if(!MOTOR_SEL4)
             {
                while(!MOTOR_SEL4);
                escprocess();
                goto M4;
             }
           
         }
         while(coin_input)
         {
            LCD_CmdWrite(0X80);
            LCD_DisplayString("  BALANCE: ");
            LCD_DataWrite(0x00); 
            display(balance);
            LCD_DisplayString("  ");
            LCD_CmdWrite(0XC0);
            LCD_DisplayString(" INSERT NOTES.. ");
         }
         
         if((motor)&&(!coin_input))
         {
            motor=0;
            switch(stock)
            {
               case A: while(LIMIT_SW1==1);break;
               case B: while(LIMIT_SW2==1);break;
               case C: while(LIMIT_SW3==1);break;
               case D: while(LIMIT_SW4==1);break;
               case E: while(LIMIT_SW5==1);break;
            }

            LCD_Clear();
            LCD_CmdWrite(0X80);
            LCD_DisplayString("  PROCESSING...");  
            LCD_CmdWrite(0XC0);
            LCD_DisplayString(" PLEASE WAIT...");
            buzzer(50);
            
            switch(stock)
            {
               case A: MOTOR1=1;break;
               case B: MOTOR2=1;break;
               case C: MOTOR3=1;break;
               case D: MOTOR4=1;break;
               case E: MOTOR5=1;break;
            }
           // RUN_LED =1;
            __delay_ms(2000);
         
            switch(stock)
            {
               case A: while(LIMIT_SW1==0);break;
               case B: while(LIMIT_SW2==0);break;
               case C: while(LIMIT_SW3==0);break;
               case D: while(LIMIT_SW4==0);break;
               case E: while(LIMIT_SW5==0);break;
            }

            switch(stock)
            {
               case A: MOTOR1=0;break;
               case B: MOTOR2=0;break;
               case C: MOTOR3=0;break;
               case D: MOTOR4=0;break;
               case E: MOTOR5=0;break;
            }
            
            switch(stock)
            {
               case A: while(LIMIT_SW1==1);break;
               case B: while(LIMIT_SW2==1);break;
               case C: while(LIMIT_SW3==1);break;
               case D: while(LIMIT_SW4==1);break;
               case E: while(LIMIT_SW5==1);break;
            }
            
            padcount--;
            
            switch(stock)
            {
               case A: padcount1=padcount;   Write_EEPROM(2,3,padcount1);break;
               case B: padcount2=padcount;   Write_EEPROM(6,7,padcount2);break;
               case C: padcount3=padcount;   Write_EEPROM(0X0A,0X0B,padcount3);break;
               case D: padcount4=padcount;   Write_EEPROM(0X0E,0X0F,padcount4);break;
               case E: padcount5=padcount;   Write_EEPROM(0X12,0X13,padcount5);break;
            
            }
            
            LCD_CmdWrite(0X80);
            LCD_DisplayString(" PLEASE COLLECT ");  
            LCD_CmdWrite(0XC0);
            LCD_DisplayString("   THANK YOU !   ");
          //  RUN_LED =0;

            buzzer(2000);
            __delay_ms(700);
         

            if(padcount1==0 && padcount2==0 && padcount3==0 && padcount4==0 && padcount5==0 )
            {   
               LCD_Clear();
               LCD_CmdWrite(0X80);
               LCD_DisplayString(" STOCK EMPTY !!! ");  
               LCD_CmdWrite(0XC0);
               LCD_DisplayString("  PLEASE LOAD.. ");   
               buzzer(3000);
               while(RST==1)
               {
                  NO_PAD_LED=~NO_PAD_LED;
                  __delay_ms(75);
            
               }
               while(!RST);
               NO_PAD_LED=0;
               
               padcount1=Read_EEPROM(0,1);
               padcount2=Read_EEPROM(4,5);
               padcount3=Read_EEPROM(8,9);
               padcount4=Read_EEPROM(0X0C,0X0D);
               padcount5=Read_EEPROM(0X10,0X11);
               Write_EEPROM(2,3,padcount1);
               Write_EEPROM(6,7,padcount2);
               Write_EEPROM(0X0A,0X0B,padcount3);
               Write_EEPROM(0X0E,0X0F,padcount4);
               Write_EEPROM(0X12,0X13,padcount5);
            
               LCD_Clear();
               LCD_DisplayString("     STOCKS     ");  
               LCD_CmdWrite(0XC0);
               LCD_DisplayString("    RELOADED..  ");   
               buzzer(2000);
               __delay_ms(700);
         
            }
            status=0;
            CONFIG_MS1 = INPUT;    //Make all the switches again back to input. 
            CONFIG_MS2 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS4 = INPUT;
            CONFIG_MS5 = INPUT;
            buzzer(70);     
            LCD_Clear();
         }
   
      }
   
      flash:
      while(menu)
      {
         LCD_CmdWrite(0X02);
         LCD_DisplayString("ENTER PASSWORD ?");
         LCD_CmdWrite(0XC7);
         if(check==0)
         LCD_DisplayString("**");
         
         if(check==1)
         displaypass(value);
      
         if(!INC)
         {
            
            check=1;
            if(value==99)
            {
               value=0;
               goto loop;
            }
            value++;
            loop:
            value=value;
            while(!INC);
         }

         if(!DEC)
          {
            
            check=1;      //to remove ** and display number
            if(value==0)
            {
               value=99;
               goto jump;
            }
            value--;
            jump:
            value=value;
            while(!DEC);
         }

         if(!MENU)
         {
            while(!MENU);
            if(value==PASSWORD)
            {
               LCD_Clear();
               value=0;
               check=0;menu=0;set=1;pad=0;coin=0;
               
               padcount1=Read_EEPROM(0,1);
               padcount2=Read_EEPROM(4,5);
               padcount3=Read_EEPROM(8,9);
               padcount4=Read_EEPROM(0X0C,0X0D);
               padcount5=Read_EEPROM(0X10,0X11);
               
               rupee1=Read_EEPROM_Price(0x14,0x15,0X16);
               rupee2=Read_EEPROM_Price(0X17,0X18,0X19);
               rupee3=Read_EEPROM_Price(0X1A,0X1B,0X1C);
               rupee4=Read_EEPROM_Price(0X1D,0X1E,0X1F);
               rupee5=Read_EEPROM_Price(0X20,0X21,0X22);
               
               buzzer(50);
               break;
            }
            else
            {
               LCD_Clear();
               LCD_CmdWrite(0X02);
               LCD_DisplayString("    WRONG !!    ");
               LCD_CmdWrite(0XC0);
               LCD_DisplayString("  TRY AGAIN....  ");
               buzzer(50);
               __delay_ms(50);
               buzzer(50);
               __delay_ms(750);
               LCD_Clear();
               value=0;check=0;
               error_count++;
               if(error_count==3)
               {
                  LCD_Clear();
                  LCD_CmdWrite(0X02);
                  LCD_DisplayString("Processor RESET");
                  buzzer(1000);
                  __delay_ms(650);
                  value=0;   
                  error_count=0;
                  check=0;status=0;menu=0,run=0;
                  LCD_Clear();
                  break;
            
               }      
            }
         
         }
   
      }

      while(set)
      {

         if((!pad)&&(!coin))
         {
            LCD_CmdWrite(0X02);
            LCD_DisplayString("ENTER PARAMETERS");
            __delay_ms(1500);
            stock=A;pad=1;coin=0;num=padcount1;
            LCD_Clear();
            LCD_CmdWrite(0X02);
            LCD_DisplayString("  ROW_");
         }
   
         switch(stock)
         {
            case A: stock_data='1';break;
            case B: stock_data='2';break;
            case C: stock_data='3';break;
            case D: stock_data='4';break;
            case E: stock_data='5';break;
         }
         LCD_CmdWrite(0X86);
         LCD_DataWrite(stock_data);
         LCD_DisplayString("  STOCK  ");
         if(pad && !coin)
         {
            LCD_CmdWrite(0XC0);
            LCD_DisplayString("   QTY: ");
            switch(stock)
            {
               case A: padcount=padcount1;Qflag=1;break;
               case B: padcount=padcount2;Qflag=1;break;
               case C: padcount=padcount3;Qflag=1;break;
               case D: padcount=padcount4;Qflag=1;break;
               case E: padcount=padcount5;Qflag=1;break;
            }
            display(padcount);
            LCD_DisplayString(" Nos");
         }
         if(coin && !pad)
         {
            LCD_CmdWrite(0XC0);
            LCD_DisplayString("   PRICE: ");
            LCD_DataWrite(0x00);
            switch(stock)
            {
               case A: rupee=rupee1;break;
               case B: rupee=rupee2;break;
               case C: rupee=rupee3;break;
               case D: rupee=rupee4;break;
               case E: rupee=rupee5;break;
            }
            LCD_CmdWrite(0XCB);
            display_price(rupee);   
            LCD_DataWrite(' ');
         }
         
         if(!INC)
         {
            __delay_ms(100);
         
            if(num==99 && pad)
            {
               num=0;
               if(pad)
               goto label1;
            }
            if(num==990 && coin)
            num=0;
               
            if(coin)num=num+10;
            if(pad) num++;
            
            label1:
            if(pad && !coin)
            {
               switch(stock)
               {
                  case A: padcount1=num;break;
                  case B: padcount2=num;break;
                  case C: padcount3=num;break;
                  case D: padcount4=num;break;
                  case E: padcount5=num;break;
               }
            }
            if(!pad && coin)
            {
               switch(stock)
               {
                  case A: rupee1=num;break;
                  case B: rupee2=num;break;
                  case C: rupee3=num;break;
                  case D: rupee4=num;break;
                  case E: rupee5=num;break;
               }
            }
         }

         if(DEC==0)
         {
            __delay_ms(100);
            
            if(num==0 && pad)
            {
               num=99;
               goto label2;
            }
            if(num==10 && coin)
            {
               num=990;
               goto label2;
            }

            if(coin)num=num-10;
            if(pad) num--;
            
            label2:
            if(pad && !coin)
            {
               switch(stock)
               {
                  case A: padcount1=num;break;
                  case B: padcount2=num;break;
                  case C: padcount3=num;break;
                  case D: padcount4=num;break;
                  case E: padcount5=num;break;
               
               }
            }
            if(!pad && coin)
            {
               switch(stock)
               {
                  case A: rupee1=num;break;
                  case B: rupee2=num;break;
                  case C: rupee3=num;break;
                  case D: rupee4=num;break;
                  case E: rupee5=num;break;
               }
            }   
         }

         if(!MENU)
         {
            while(!MENU);
            if(!pad && coin)
            {
               switch(stock)
               {
                  case A:  Write_EEPROM_Price(0X14,0X15,0X16,rupee1);pad=1;coin=0;stock=B;num=padcount2;Qflag=0;buzzer(50);break;
                  case B:  Write_EEPROM_Price(0X17,0X18,0X19,rupee2);pad=1;coin=0;stock=C;num=padcount3;Qflag=0;buzzer(50);break;
                  case C:  Write_EEPROM_Price(0X1A,0X1B,0X1C,rupee3);pad=1;coin=0;stock=D;num=padcount4;Qflag=0;buzzer(50);break;
                  case D:  Write_EEPROM_Price(0X1D,0X1E,0X1F,rupee4);pad=1;coin=0;stock=E;num=padcount5;Qflag=0;buzzer(50);break;
                  case E:  Write_EEPROM_Price(0X20,0X21,0X22,rupee5);pad=0;coin=0;stock=A;num=0;status=0;set=0;Qflag=0;
                         buzzer(50);
                         LCD_Clear();
                         LCD_DisplayString("  DATAS STORED..");
                         buzzer(1000);
                         LCD_Clear();
                         break;
               }
            }
               
               if(pad && !coin && Qflag)
               {
                  switch(stock)
                  {
                     case A: Write_EEPROM(0,1,padcount1);Write_EEPROM(2,3,padcount1);pad=0;coin=1;num=rupee1;buzzer(50);break;
                     case B: Write_EEPROM(4,5,padcount2);Write_EEPROM(6,7,padcount2);pad=0;coin=1;num=rupee2;buzzer(50);break;
                     case C: Write_EEPROM(8,9,padcount3);Write_EEPROM(0X0A,0X0B,padcount3);pad=0;coin=1;num=rupee3;buzzer(50);break;
                     case D: Write_EEPROM(0X0C,0X0D,padcount4);Write_EEPROM(0X0E,0X0F,padcount4);pad=0;coin=1;num=rupee4;buzzer(50);break;
                     case E: Write_EEPROM(0X10,0X11,padcount5);Write_EEPROM(0X12,0X13,padcount5);pad=0;coin=1;num=rupee5;buzzer(50);break;
                  }
               
               }
            }
         }
      }
   }



void PORT_Init()
{
   TRISA      =   0X37;      
   TRISB      =   0XC1;      
   TRISC      =   0XA0;
   TRISD      =   0XA8;
   TRISE      =   0X0F;      
   PORTB       =   0X00;      //clearing port initially
   PORTC       =   0X00;      //clearing port initially
   PORTA      =   0X00;      //clearing port initially
   PORTD      =   0X00;      //clearing port initially
   PORTE      =   0X00;      //clearing port initially
   OPTION_REG   =    0X87;      //Configuring timer 0 with prescale as 256
   INTCON      =    0X90;      //Enabling the global interrupts
   ANSEL      =    0X00;      //setting all analog pins as digital 
   ANSELH      =    0X00;      //setting all analog pins as digital 
   WPUB      =   0XC1;      //Enable the internal pull-ups of the pins RB7-RB4
   IOCB         =   0X00;      //Disable interrupt on port change 
   OSCCON      =   0X65;      //Internal oscillator with 4Mhz frequency and clock stability      
}   

void ToggleEpinOfLCD(void)
{
   LCD_E = 1;                   // Give a pulse on E pin
   __delay_ms(1);            // so that LCD can latch the data from data bus
   LCD_E = 0;                 
   __delay_ms(1);    
}


void LCD_CmdWrite(unsigned char Command)  
{
   LCD_RS = 0;                 // It is a command

      
   PORTC = ((Command>>4)&0x0F);// Write upper nibble of data
   ToggleEpinOfLCD();          // Give pulse on E pin
   
         
   PORTC = (Command&0x0F);   // Write lower nibble of data
   ToggleEpinOfLCD();        // Give pulse on E pin   
   
   
}

void LCD_DataWrite(char LCDChar)  
{
   LCD_RS = 1;            

   PORTC = ((LCDChar>>4)&0x0F); // Write upper nibble of data
   ToggleEpinOfLCD();            // Give pulse on E pin
   
            
   PORTC = (LCDChar&0x0F);     // Write lower nibble of data
   ToggleEpinOfLCD();           // Give pulse on E pin   
   

}


void LCD_Init(void)
{ 
   // Firstly make all pins output

   LCD_E                = 0;   // E  = 0
   LCD_RS                = 0;   // RS = 0
   LCD_Data_Bus_D4       = 0;     // Data bus = 0
   LCD_Data_Bus_D5       = 0;     // Data bus = 0
   LCD_Data_Bus_D6       = 0;     // Data bus = 0
   LCD_Data_Bus_D7       = 0;     // Data bus = 0
   LCD_E_Dir              = 0;   // Make Output
   LCD_RS_Dir              = 0;   // Make Output
   LCD_Data_Bus_Dir_D4     = 0;   // Make Output
   LCD_Data_Bus_Dir_D5       = 0;   // Make Output
   LCD_Data_Bus_Dir_D6     = 0;   // Make Output
   LCD_Data_Bus_Dir_D7     = 0;   // Make Output

       LCD_CmdWrite(0x02);
   LCD_CmdWrite(0x28);    //function set
   LCD_CmdWrite(0x0c);    //display on,cursor off,blink off
   LCD_CmdWrite(0x01);    //clear display
   LCD_CmdWrite(0x06);    //entry mode, set increment
   
   LCD_BuildChar(0,RupeeSymbol);

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

void displaypass(int data)
{
    LCD_DataWrite((data/10)+0x30);   
    LCD_DataWrite((data%10)+0x30);
}
void display(int data)   //Function to display the given data
{   
        sprintf(dispvalue,"%d",data);  /* convert integer to ASCII string */
        LCD_DisplayString(dispvalue);
    
    
}

void display_price(int data)   //Function to display the given data
{   
       sprintf(dispvalue,"%d",data);  /* convert integer to ASCII string */
       LCD_DisplayString(dispvalue);
    
}

void LCD_BuildChar(unsigned char loc,unsigned char *p)
{
   unsigned char i;
   if(loc<8)
   {
      LCD_CmdWrite(0X40+(loc*8));
      for(i=0;i<=8;i++)
      LCD_DataWrite(p[i]);
   }
   LCD_CmdWrite(0x80);
}   
void buzzer(unsigned int a)//Function to use buzzer for a desired time
{
   BUZZER=1;
   for(int i=0;i<=a;i++) //Beeps buzzer upto the given delay
   __delay_ms(1);
   BUZZER=0;
}


void Write_EEPROM(unsigned char a1,unsigned char a2,unsigned int data_value)//Writing data into specified location in EEPROM
{
   eeprom_write(a1,(data_value/10));   //write first digit of data
   eeprom_write(a2,(data_value%10));   //write second digit of data
}

unsigned int Read_EEPROM(unsigned char b1,unsigned char b2)//Reading data from specified location in EEPROM
{
   unsigned int c1=0,c2=0;
   c1=eeprom_read(b1);      //read first digit of data
   c2=eeprom_read(b2);      //read second digit of data
   return ((c1*10)+c2);    //return the data read
}
void Write_EEPROM_Price(unsigned char a1,unsigned char a2,unsigned char a3,unsigned int data_value)//Writing data into specified location in EEPROM
{
   eeprom_write(a1,(data_value/100));      //write first digit of data
   eeprom_write(a2,((data_value/10)%10));   //write second digit of data
   eeprom_write(a3,(data_value%10));      //write third digit of data
}

unsigned int Read_EEPROM_Price(unsigned char b1,unsigned char b2,unsigned char b3)//Reading data from specified location in EEPROM
{
   unsigned int c1=0,c2=0,c3=0;
   c1=eeprom_read(b1);      //read first digit of data
   c2=eeprom_read(b2);      //read second digit of data
   c3=eeprom_read(b3);      //read third digit of data
   return ((c1*100)+(c2*10)+c3);    //return the data read
}


void STOCK_EMPTY_ROUTINE()
{
   LCD_Clear();
   LCD_DisplayString("  STOCK EMPTY!!  ");
   LCD_CmdWrite(0XC0);
   LCD_DisplayString(" CHOOSE AGAIN...  ");
   buzzer(70);
   __delay_ms(60);
   buzzer(70);
   __delay_ms(1500);
   buzzer(50);
   LCD_Clear();
}

void Set_Channel(unsigned int data)
{
   if(data==10)
   {
       CH_10=0;CH_20=1;CH_50=1;CH_100=1;
   }
   else if(data>=20 && data<=40)
   {
      CH_10=0;CH_20=0;CH_50=1;CH_100=1;
   }
   else if(data>=50 && data<=90)
   {
      CH_10=0;CH_20=0;CH_50=0;CH_100=1;
   }
   else if(data>=100)
   {
      CH_10=0;CH_20=0;CH_50=0;CH_100=0;
   }
   else
   {
       CH_10=1;CH_20=1;CH_50=1;CH_100=1;
   }
}

void Monitor_Balance(unsigned int amount)
{
   if(amount<= 90)   { CH_100 = 1;}
   if(amount<= 40)   { CH_100 = 1; CH_50=1; }
   if(amount<= 10)   { CH_100 = 1; CH_50=1; CH_20=1; }
   if(amount==0)   { CH_100 = 1; CH_50=1; CH_20=1;  CH_10=1; }
}

void escprocess()                                                               //Resetting function
{   
            
			padcount=0;rupee=0;stock=0;
            INTE=0;COIN_BOX_VDD=1;
            status=0;
            CONFIG_MS1 = INPUT;    //Make all the switches again back to input. 
            CONFIG_MS2 = INPUT;
            CONFIG_MS3 = INPUT;
            CONFIG_MS4 = INPUT;
            CONFIG_MS5 = INPUT;
            LCD_Clear();
            buzzer(50);
	
}