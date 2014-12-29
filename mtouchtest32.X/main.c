#include <xc.h>
#include <plib.h>
#include "lcd32.h"

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8

#define SYS_FREQ 			(80000000L)
#define CMT_COUNT 100

DWORD CTM_ReadRobe() {
	DWORD temp, cap , i;

	temp = 0 ;
	for (i=0 ; i < CMT_COUNT ; i++) {
		//sampling開始
		AcquireADC10();
		// ゼロ電位から開始させる為に電荷を放電させる
		CTMUCONbits.IDISSEN = 1 ;
		lcdDelayUs(125) ;
		CTMUCONbits.IDISSEN = 0 ;
		// 電極に充電を行う
		CTMUCONbits.EDG2STAT = 0 ;
		CTMUCONbits.EDG1STAT = 1 ;
		lcdDelayUs(125) ;
		CTMUCONbits.EDG1STAT = 0 ;
		//sampling終了
		mAD1ClearIntFlag();
		ConvertADC10();
		// 電極の電圧(AN0)を読み取る
		while (!mAD1GetIntFlag()) ;
		cap = ReadADC10(0);
		temp = temp + cap;
		mAD1ClearIntFlag();
	}
	return temp / CMT_COUNT;              // 10回の平均値を返す
}

int main(void)
{
	char s[17];

	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	mJTAGPortEnable(DEBUG_JTAGPORT_OFF);

	CloseADC10();	// ensure the ADC is off before setting the configuration

	// 				Turn module on | ouput in integer | trigger mode auto | enable autosample
	#define PARAM1  ADC_MODULE_ON | ADC_IDLE_CONTINUE | ADC_FORMAT_INTG | ADC_CLK_MANUAL | ADC_AUTO_SAMPLING_OFF | ADC_SAMP_OFF
	// 				ADC ref external    | disable offset test    | disable scan mode | perform 2 samples | use dual buffers | use alternate mode
	#define PARAM2  ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | ADC_SCAN_OFF | ADC_SAMPLES_PER_INT_1 | ADC_ALT_BUF_OFF | ADC_ALT_INPUT_OFF
	//				  use ADC internal clock | set sample time
	#define PARAM3  ADC_CONV_CLK_SYSTEM | ADC_SAMPLE_TIME_0
	//               set AN2 as analog inputs
	#define PARAM4	ENABLE_AN2_ANA
	// do not assign channels to scan
	#define PARAM5	0
	// use ground as neg ref for A | use AN2 for input A
	// configure to sample AN4 & AN5
	SetChanADC10( ADC_CH0_NEG_SAMPLEA_NVREF | ADC_CH0_POS_SAMPLEA_AN2); // configure to sample AN2
	OpenADC10( PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 ); // configure ADC using the parameters defined above
	EnableADC10(); // Enable the ADC

	//IRNG = 0b10, ON = 1
	CTMUCON = 0x40408000;

	lcdInit(SYS_FREQ);
	lcdPuts("Monitor Debug");

	DWORD i = 0, temp;
    while(1)
    {
		lcdDelayMs(200);
		temp = CTM_ReadRobe();
		itoa(s, temp, 10);
		lcdClearLine(1);
		lcdPuts(s);
	}
}

