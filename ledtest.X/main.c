#include <xc.h>

#if defined _12F675

#pragma config FOSC = INTRCIO, WDTE = OFF, PWRTE = ON, O, MCLRE = OFF
#define LED GPIO4
#define _XTAL_FREQ 4000000         // CLK 12MHz (use from __delay_ms)

#elif defined _18F14K50

#pragma config FOSC   = HS,  PLLEN  = ON,  FCMEN  = OFF
#pragma config IESO   = OFF, USBDIV = OFF, CPUDIV = NOCLKDIV
#pragma config PWRTEN = OFF, BOREN  = OFF, WDTEN  = OFF
#pragma config HFOFST = OFF, MCLRE  = OFF
#pragma config STVREN = ON,  BBSIZ  = OFF, LVP    = OFF
#pragma config XINST  = OFF
#pragma config CP0    = OFF, CP1    = OFF, CPB    = OFF
#pragma config WRT0   = OFF, WRT1   = OFF, WRTB   = OFF, WRTC   = OFF
#pragma config EBTR0  = OFF, EBTR1  = OFF, EBTRB  = OFF
#define _XTAL_FREQ 12000000         // CLK 12MHz (use from __delay_ms)
#define LED RB4

#elif defined __PIC24FJ64GB002__

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx1 & FWDTEN_OFF )
_CONFIG2(IESO_OFF & FNOSC_FRCPLL & FCKSM_CSDCMD & OSCIOFNC_OFF & IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_NONE)
#define LED PORTBbits.RB15

#endif


void init(void)
{
#if defined _12F675

    GPIO = 0;
    CMCON = 0x07;
    TRISIO = 0;
    ANSEL = 0b00110000;
    OPTION_REG = 0b10000010;

#elif defined _18F14K50

    TRISB  = 0b00000000;            // direction: all out
    TRISC  = 0b00000000;            // direction: all out
    ANSEL  = 0b00000000;            // analog/digital: all digital
    ANSELH = 0b00000000;            // analog/digital: all digital

#elif defined __PIC24FJ64GB002__

    TRISB  = 0b00000000;            // direction: all out

#endif
}

#if defined __PIC24FJ64GB002__
void __delay_ms(unsigned long ms)
{
    int i;
    for (i = 0; i < 10000; i++) {
        
    }
}
#endif

int main(void)
{
    init();
#ifndef _12F675
    PORTB = 0;
#endif

    while(1) {

        LED = 1;
        __delay_ms(65);             // < 66 ms at 12MHz
        LED = 0;
        __delay_ms(65);             // < 66 ms at 12MHz
    }

    return 0;
}
