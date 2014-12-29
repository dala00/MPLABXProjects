#include <xc.h>
#include <plib.h>

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ 			(80000000L)

void __delay_us(WORD us)
{
	DWORD start = _CP0_GET_COUNT();
	DWORD end = start + SYS_FREQ / 10000000 / 2 * us;
//	DWORD end = start + SYS_FREQ / 1000000 / 2 * us;
	if (end > start) while (_CP0_GET_COUNT() < end);
	else while (_CP0_GET_COUNT() > start || _CP0_GET_COUNT() < end);
}

void __delay_ms(unsigned int msec)        //1msec遅延関数
{
	unsigned int i;

	for(i=0; i<msec; i++) {
		__delay_us(1000);
	}
}


int main(void)
{
	char s[17];
	int i;

	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

	RPB15R = 0b0101;

	OpenTimer2(T2_ON, 0x550);
	/* Enable OC | 32 bit Mode  | Timer2 is selected | Continuous O/P   | OC Pin High , S Compare value, Compare value*/
	OpenOC1( OC_ON | OC_TIMER_MODE32 | OC_TIMER2_SRC | OC_CONTINUE_PULSE | OC_LOW_HIGH , 0x550, 0x500 );


    while(1)
    {
		for (i = 0; i < 4; i++) {
			__delay_ms(1000);
			SetDCOC1PWM((0x550 - 1) * i / 4);
		}
	}
}

