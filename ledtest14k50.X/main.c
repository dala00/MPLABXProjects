#include <xc.h>

// PIC18F14K50
#pragma config FOSC   = IRC,  PLLEN  = ON,  FCMEN  = OFF
#pragma config IESO   = OFF, USBDIV = OFF, CPUDIV = NOCLKDIV
#pragma config PWRTEN = OFF, BOREN  = OFF, WDTEN  = OFF
#pragma config HFOFST = OFF, MCLRE  = OFF
#pragma config STVREN = ON,  BBSIZ  = OFF, LVP    = OFF
#pragma config XINST  = OFF
#pragma config CP0    = OFF, CP1    = OFF, CPB    = OFF
#pragma config WRT0   = OFF, WRT1   = OFF, WRTB   = OFF, WRTC   = OFF
#pragma config EBTR0  = OFF, EBTR1  = OFF, EBTRB  = OFF

#define _XTAL_FREQ 12000000         // CLK 12MHz (use from __delay_ms)
#define LED RB5

void init(void)
{
    TRISA  = 0b00000000;            // direction: all out
    TRISB  = 0b00000000;            // direction: all out
    TRISC  = 0b00000000;            // direction: all out
    ANSEL  = 0b00000000;            // analog/digital: all digital
    ANSELH = 0b00000000;            // analog/digital: all digital
}

void main(void)
{
    init();
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;                 // output to port c
    PORTA = 0xFF;
    PORTB = 0xFF;
    PORTC = 0xFF;

    while(1) {

        LED = 1;
        __delay_ms(65);             // < 66 ms at 12MHz
        LED = 0;
        __delay_ms(65);             // < 66 ms at 12MHz
    }

    return;
}
