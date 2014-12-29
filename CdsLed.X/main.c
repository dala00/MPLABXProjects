#include <xc.h>

#pragma config FOSC = 4, WDTE = OFF, PWRTE = ON, MCLRE = OFF

static bit lighting = 0;
static bit offing = 0;
static bit doOff = 0;
static int count = 0;
static int cds = 0;

static void interrupt inter() {
    T0IF = 0;

    if (offing) {
        if (++count >= 1440) {
            doOff = 1;
            offing = 0;
        }
    }
}

main()	{
    GPIO = 0;
    CMCON = 0x07;		// コンパレータ未使用
    TRISIO = 0b00010000;		// GP4:in
    OPTION_REG = 0b10000111;          //プリスケーラ256
    TMR0 = 0x00;                //カウント0
    T0IE = 1;                   //タイマー０使う
    GIE = 1;                    //タイマー使う

    ADCS1 = 1;
    ADCS0 = 1;
    //アナログ3を使用
    ANS0 = 0;
    ANS1 = 0;
    ANS2 = 0;
    ANS3 = 1;
    //左からアナログデータ右づめ、アナログ３使用(2つのbit)、アナログ有効化
    ADCON0 = 0b10001101;

    while(1) {
        GO_DONE = 1;
        while (GO_DONE);
        cds = ADRESH * 256 + ADRESL;
        
        if (cds < 768) {
            if (GPIO2) {
                GPIO2 = 0;
                offing = 0;
                doOff = 0;
            }
        } else {
            if (!GPIO2) {
                GPIO2 = 1;
                offing = 1;
                count = 0;
                doOff = 0;
            }
        }
        
        if (doOff) {
            doOff = 0;
            GPIO2 = 0;
        }
    }
}

