#define _XTAL_FREQ 4000000

#define RS RA1
#define EN RA2
#define D4 RC0
#define D5 RC1
#define D6 RC2
#define D7 RC3
#define TS RA0
#define TS_DIR TRISA0
#include <xc.h>
#include "lcd.h";

__CONFIG(BGHIGH & UNPROTECT & BORDIS & MCLRDIS & PWRTEN & INTOSCCLKO & WDTDIS);

float Temp=0; 

int sensor_init();
void write(unsigned char);
unsigned char Read();


int main()
{
 
    ANSEL = 0x00;
    CMCON = 0x0f;

    unsigned int a,templow,temphigh;
    float temp;
    TRISC = 0x00;
    TRISA1 = 0;
    TRISA2 = 0;
    __delay_ms(100);
    Lcd_Init();
  while(1)
  {
      Lcd_Clear();
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("temperature:");
      
      if(!reset())
                {
                    write(0xcc);

                    write(0x44);
                 __delay_ms(750);
                    reset();
                    
                     write(0xcc);

                     write(0xbe);

                    templow = read();
                    temphigh = read();
                    temp = (temphigh<<4)+templow;
                    temp = temp * 6.25;

                }
                
    
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String("tem");
  } 
 
}

int sensor_init()

{

            TS_DIR = 0;

            TS = 0;

            __delay_ms(1);

            __delay_ms(0.25);

             TS_DIR = 1;

              __delay_ms(0.10);
                if(TS==1)
                {
                __delay_ms(0.480);
                return 0;
                }
                else
                {
                    __delay_ms(0.480);
                return 1;
                }

               

}

void write(unsigned char cmd)

{
    char i;

for(i=0;i<8;i++)

{
    if((cmd & (1<<i))!=0)
    {
        TS_DIR = 0;
        TS = 0;
        __delay_ms(0.01);
        TS_DIR = 1;
        __delay_ms(0.060);
    }
    else
    {
        TS_DIR = 0;
        TS = 0;
        __delay_ms(0.60);
        TS_DIR = 1;
    }


}

}

unsigned char Read()

{

unsigned char i,result = 0;

TS_DIR = 0;

for(i=0;i<8;i++)

{

TS_DIR = 0;

TS = 0;
__delay_ms(0.02);

if(TS!=0)

result |=1<<i;

__delay_ms(0.070);


}

return result;

}