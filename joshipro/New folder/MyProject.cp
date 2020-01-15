#line 1 "C:/Users/admin/Desktop/joshipro/mikro/MyProject.c"



sfr sbit Mmc_Chip_Select at RB2_bit;
sfr sbit Mmc_Chip_Select_Direction at TRISB2_bit;





sbit LCD_RS at RD0_bit;
sbit LCD_RW at RD1_bit;
sbit LCD_EN at RD2_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_RS_Direction at TRISD0_bit;
sbit LCD_EN_Direction at TRISD2_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D7_Direction at TRISD7_bit;


unsigned char filename[] = "TEMP.TXT";
unsigned char error;
unsigned char i;
unsigned int adc_value;
unsigned char Temperature_Log[ 27 ];

char seconds, minutes, hours, day, month, year;
char prev_minutes=0;

void main()
{
 unsigned char ones,tens,hundreds,fractional;
 unsigned int temp;

 TRISD = 0x00;

 Lcd_Init();
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Cmd(_LCD_CLEAR);
 Delay_ms(100);

 SPI1_Init();
 SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV64, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);
 Delay_us(10);

 error = MMC_Init();
 while(error == 1)
 {
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1," CARD NOT FOUND");
 error = MMC_Init();
 }

 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1," CARD DETECTED!");
 Lcd_Out(2,1,"CARD INITIALIZED");
 Delay_ms(1000);

 MMC_Fat_Init();
 SPI1_Init_Advanced(_SPI_MASTER_OSC_DIV4, _SPI_DATA_SAMPLE_MIDDLE, _SPI_CLK_IDLE_LOW, _SPI_LOW_2_HIGH);

 Mmc_Fat_Assign(&filename,0xA0);

 while(1)
 {
 Mmc_Fat_Append();
 Mmc_Fat_Write(Temperature_Log,27);
 prev_minutes = minutes;
 }
 Delay_ms(1000);
 }
}
