#define _XTAL_FREQ 4000000
#include <xc.h>


__CONFIG(BGHIGH & UNPROTECT & BORDIS & MCLRDIS & PWRTEN & INTOSCCLKO & WDTDIS);
int main()
{
  unsigned int a;
  TRISC = 0x0f;
  //TRISA = 0x19;

    while(1)
    {


        RC0=1;
                __delay_ms(500);

            RC0=0;
                __delay_ms(500);

}

}

