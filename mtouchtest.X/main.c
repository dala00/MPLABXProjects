// 12F1822によるＬＣＤモニターのサンプルプログラム
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include "skUARTlib.h"

#define _XTAL_FREQ 8000000

// コンフィギュレーション１の設定
// CLKOUTﾋﾟﾝをRA4ﾋﾟﾝで使用する(CLKOUTEN_OFF)：内部ｸﾛｯｸ使用する(FOSC_INTOSC)
// 外部ｸﾛｯｸ監視しない(FCMEN_OFF)：外部・内部ｸﾛｯｸの切替えでの起動はなし(IESO_OFF)
// 電源電圧降下常時監視機能ON(BOREN_ON)：電源ONから64ms後にﾌﾟﾛｸﾞﾗﾑを開始する(PWRTEN_ON)
// ｳｵｯﾁﾄﾞｯｸﾞﾀｲﾏｰ無し(WDTE_OFF)：
// 外部ﾘｾｯﾄ信号は使用せずにﾃﾞｼﾞﾀﾙ入力(RA3)ﾋﾟﾝとする(MCLRE_OFF)
// ﾌﾟﾛｸﾞﾗﾑﾒﾓﾘｰを保護しない(CP_OFF)：ﾃﾞｰﾀﾒﾓﾘｰを保護しない(CPD_OFF)
__CONFIG(CLKOUTEN_OFF & FOSC_INTOSC & FCMEN_OFF & IESO_OFF & BOREN_ON &
         PWRTE_ON & WDTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF) ;
// コンフィギュレーション２の設定
// 動作クロックを32MHzでは動作させない(PLLEN_OFF)
// スタックがオーバフローやアンダーフローしたらリセットをする(STVREN_ON)
// 低電圧プログラミング機能使用しない(LVP_OFF)
// Flashﾒﾓﾘｰを保護しない(WRT_OFF)：電源電圧降下常時監視電圧(2.5V)設定(BORV_25)
__CONFIG(PLLEN_OFF & STVREN_ON & WRT_OFF & BORV_HI & LVP_OFF);

/*******************************************************************************
*  メインの処理                                                                *
*******************************************************************************/
void main()
{
	char s[8] ;
	unsigned int cap ;

	OSCCON  = 0b01110010 ; // 内部クロックは8ＭＨｚとする

	ANSELA  = 0b00000100 ; // アナログはAN2を使用し、残りをすべてデジタルI/Oに割当
	TRISA   = 0b00000100 ; // AN2(RA2/CPS2)だけ入力その他のピンは出力に割当てる(RA3は入力専用)
	PORTA   = 0b00000000 ; // 出力ピンの初期化(全てLOWにする)
	// 容量検知モジュール(ＣＰＳＭ)の設定
	CPSCON0 = 0b00001000 ; // オシレータは中範囲(中速の発信周波数)で利用する
	CPSCON1 = 0b00000010 ; // CPS2から入力する
	// タイマー１の設定
	T1CON   = 0b11000001 ; // 容量検知オシレータでTIMER1をｶｳﾝﾄする、ﾌﾟﾘｽｹｰﾗｶｳﾝﾄ値 1:1
	TMR1H   = 0 ;          // タイマー1の初期化
	TMR1L   = 0 ;
	PEIE    = 1 ;          // 周辺装置割り込みを許可する
	GIE     = 1 ;          // 全割り込み処理を許可する

	CPSON   = 1 ;          // 容量検知モジュール開始
	InitUART(0, 3, 51);

	while(1) {
		// 容量検知モジュールの値を読み込む
		CPSON = 0 ;                 // 容量検知モジュール停止
		cap = (TMR1H*256) + TMR1L ; // カウント値を読み込む
		TMR1H = 0 ;                 // タイマー1の初期化
		TMR1L = 0 ;
		CPSON = 1 ;                 // 容量検知モジュール開始
		// 読んだ値をLCDモニターに表示する
		utoa(s,cap,10) ;            // カウント値を文字列に変換する
		UART_Send(s, strlen(s) + 1);
		// 400ms後に繰り返す
		__delay_ms(400) ;
	}
}

