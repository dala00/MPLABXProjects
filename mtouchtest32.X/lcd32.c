#include <xc.h>
#include <plib.h>
#include "lcd32.h"

DWORD sysFreq;

void lcdDelayUs(WORD us) {
	DWORD start = _CP0_GET_COUNT();
	DWORD end = start + sysFreq / 10000000 / 2 * us;
//	DWORD end = start + sysFreq / 1000000 / 2 * us;
	if (end > start) while (_CP0_GET_COUNT() < end);
	else while (_CP0_GET_COUNT() > start || _CP0_GET_COUNT() < end);
}

void lcdDelayMs(unsigned int msec) {
	unsigned int i;

	for(i = 0; i < msec; i++) {
		lcdDelayUs(1000);
	}
}

void lcdStrobe() {
	mPORTASetBits(LCD_EN);
	lcdDelayUs(1);
	mPORTAClearBits(LCD_EN);
}

void lcdSetBit(int bt, char value) {
	if (value == 1) {
		mPORTBSetBits(bt);
	} else {
		mPORTBClearBits(bt);
	}
}

void lcdCommand(unsigned char c) {
	mPORTAClearBits(LCD_RS);
	lcdSetBit(LCD_D4, c & 1);
	lcdSetBit(LCD_D5, (c >> 1) & 1);
	lcdSetBit(LCD_D6, (c >> 2) & 1);
	lcdSetBit(LCD_D7, (c >> 3) & 1);
	lcdStrobe();
	lcdDelayUs(50);
}

void lcdWrite(unsigned char c) {
	lcdSetBit(LCD_D4, (c >> 4) & 1);
	lcdSetBit(LCD_D5, (c >> 5) & 1);
	lcdSetBit(LCD_D6, (c >> 6) & 1);
	lcdSetBit(LCD_D7, (c >> 7) & 1);
	lcdStrobe();
	lcdSetBit(LCD_D4, c & 1);
	lcdSetBit(LCD_D5, (c >> 1) & 1);
	lcdSetBit(LCD_D6, (c >> 2) & 1);
	lcdSetBit(LCD_D7, (c >> 3) & 1);
	lcdStrobe();
	lcdDelayUs(50);
}

/*******************************************************************************
*    lcd_setCursor - ＬＣＤモジュール画面内のカーソル位置を移動する処理        *
*      col : 横(列)方向のカーソル位置(0-15)                                    *
*      row : 縦(行)方向のカーソル位置(0-1)                                     *
********************************************************************************/
void lcdSetCursor(int col, int row) {
     int row_offsets[] = {0x00, 0x40};

     mPORTAClearBits(LCD_RS);
     lcdWrite(0x80 | (col + row_offsets[row])); // Set DDRAM Adddress : 00H-0FH,40H-4FH
}

void lcdClearLine(int row) {
	lcdSetCursor(0, row);
	lcdPuts("                ");
	lcdSetCursor(0, row);
}

void lcdClear(void) {
     mPORTAClearBits(LCD_RS);
     lcdWrite(0x01); // Clear Display : 画面全体に20Hのｽﾍﾟｰｽで表示、ｶｰｿﾙはcol=0,row=0に移動
     lcdDelayMs(2);   // LCDが処理(1.53ms)するのを待ちます
}

void lcdPutc(char c) {
	mPORTASetBits(LCD_RS);
	lcdWrite(c);
}

void lcdPuts(const char *s) {
	mPORTASetBits(LCD_RS);
	while(*s) {
		lcdWrite(*s++);
	}
}

void lcdInit(DWORD freq) {
	sysFreq = freq;
	mPORTASetPinsDigitalOut(LCD_RS | LCD_EN);
	mPORTBSetPinsDigitalOut(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
	mPORTAClearBits(LCD_RS | LCD_EN);

	lcdDelayMs(40);    // 電源ＯＮ後15msまで待ってから初期化

	// LCDの立上げ時のチェックデータ(イニシャライズ処理用)を設定
	lcdCommand(0x03);
	lcdDelayMs(5);
	lcdCommand(0x02);
	lcdDelayMs(5);
	// ＬＣＤにコマンドを発行します
	lcdWrite(0x28);   // function set   : データ線は4本・表示は２行・フォントは5x8ドット
	lcdWrite(0x0c);   // display control: 画面表示はON・カーソル表示はOFF・カーソル点滅はOFF
	lcdClear();       // Clear Display  : 画面をクリアし、カーソル位置はcol=0,row=0
	lcdWrite(0x06);   // entry mode set : 文字を表示した次にカーソルを移動するを指示
}
