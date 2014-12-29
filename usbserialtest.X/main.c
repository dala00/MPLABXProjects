#include <xc.h>
#include "skUARTlib.h"

// PIC18F14K50
#pragma config FCMEN  = OFF
#pragma config IESO   = OFF, USBDIV = OFF, CPUDIV = NOCLKDIV
#pragma config WDTEN  = OFF
#pragma config BOREN=NOSLP,BORV=30,PWRTEN=ON
#pragma config HFOFST = OFF, MCLRE  = OFF
#pragma config STVREN = ON,  BBSIZ  = OFF, LVP    = OFF
#pragma config XINST  = OFF
#pragma config CP0    = OFF, CP1    = OFF, CPB    = OFF
#pragma config WRT0   = OFF, WRT1   = OFF, WRTB   = OFF, WRTC   = OFF
#pragma config EBTR0  = OFF, EBTR1  = OFF, EBTRB  = OFF
//内部クロック
#pragma config FOSC   = IRC,  PLLEN  = OFF

#define _XTAL_FREQ 8000000         // CLK 12MHz (use from __delay_ms)

void interrupt inter(void) {
    InterUART();
}

void init(void)
{
    OSCCON = 0b01100010;
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB  = 0b00100000; //RX in
    TRISC = 0;
    USBEN = 0;
    IOCA0 = 1;
    IOCA1 = 1;
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
}

void main(void)
{
    char data[3];

    init();
    InitUART(12, 10, 51);
    data[1] = 0x0d;
    data[2] = 0x0a;

    while(1) {
        if (UART_Available() != 0) {
            data[0] = UART_Read();
            data[0] += 2;
            UART_Send(data, 3);
        }
    }

    return;
}
