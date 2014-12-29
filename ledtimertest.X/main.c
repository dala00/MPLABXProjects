#include <xc.h>

#if defined _12F675

#pragma config FOSC = 4, WDTE = OFF, PWRTE = ON, O, MCLRE = OFF
#define LED GPIO2
#define TIMER_FLAG T0IF

#elif defined __PIC24FJ64GB002__

_CONFIG1(JTAGEN_OFF & GCP_OFF & GWRP_OFF & ICS_PGx1 & FWDTEN_OFF )
_CONFIG2(IESO_OFF & FNOSC_FRCPLL & FCKSM_CSDCMD & OSCIOFNC_OFF & IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_NONE)
#define LED PORTBbits.RB7
#define TIMER_FLAG IFS0bits.T1IF

#endif

unsigned char count;
char flag = 0;

#if defined __PIC24F__
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt() {
#else
static void interrupt inter() {
#endif
    TIMER_FLAG = 0;

    if (++count == 2) {
        count = 0;
        flag = 1;
    }
}

int main()	{
#if defined _12F675

    GPIO = 0;
    CMCON = 0x07;		// コンパレータ未使用
    ANSEL = 0x00;		// AN未使用
    TRISIO = 0x3B;		// GP2:out
    OPTION_REG = 0x84;          //プリスケーラ32
    TMR0 = 0x00;                //カウント0
    T0IE = 1;                   //タイマー０使う
    GIE = 1;                    //タイマー使う

#elif defined __PIC24FJ64GB002__

    CLKDIV = 0;
    AD1PCFG = 0xFFFF;
    TRISB = 0;
    PORTB = 0;
    TMR1 = 0;
    T1CON = 0b1000000000110000;
    PR1 = 31249;                     // 500msec
    IPC0bits.T1IP = 5;
    IEC0bits.T1IE = 1;

#endif

    count = 0;
    flag = 0;
    LED = 1;

    while(1) {
        if (flag) {
            LED = 1 - LED;
            flag = 0;
        }
    }
}
