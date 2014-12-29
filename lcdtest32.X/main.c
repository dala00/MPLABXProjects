#include <xc.h>
#include <plib.h>
#include "lcd32.h"

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ 			(80000000L)

int main(void)
{
	char s[17];

    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

	lcdInit(SYS_FREQ);
	lcdPuts("Monitor");

	int i = 0;
    while(1)
    {
		lcdDelayMs(1000);
		itoa(s, i++, 10);
		lcdClearLine(1);
		lcdPuts(s);
	}
}

