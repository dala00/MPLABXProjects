#include <xc.h>

#pragma config FOSC = 4, WDTE = OFF, PWRTE = ON, O, MCLRE = OFF

static unsigned char count;
static unsigned char flag;
static bit uping;
static unsigned char maxcount;
static unsigned char wait;

static void interrupt inter() {
    T0IF = 0;

    count++;
    if (count == 4) {
        flag = 1;
    } else if (count == maxcount) {
        flag = 0;
        count = 0;
        if (++wait == 10) {
            if (uping) {
                maxcount++;
                if (maxcount == 32) {
                    uping = 0;
                }
            } else {
                maxcount--;
                if (maxcount == 8) {
                    uping = 1;
                }
            }
            wait = 0;
        }
    }
}

main()	{
    GPIO = 0;
    CMCON = 0x07;		// コンパレータ未使用
    ANSEL = 0x00;		// AN未使用
    TRISIO = 0x3B;		// GP2:out
    OPTION_REG = 0x80;          //プリスケーラ2
    TMR0 = 0x00;                //カウント0
    T0IE = 1;                   //タイマー０使う
    GIE = 1;                    //タイマー使う

    count = 0;
    flag = 0;
    uping = 1;
    maxcount = 8;
    wait = 0;

    while(1) {
        if (flag == 1 && GPIO2) {
            GPIO2 = 0;
        } else if (flag == 0 && !GPIO2) {
            GPIO2 = 1;
        }
    }
}
