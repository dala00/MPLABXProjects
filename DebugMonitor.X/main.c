#include <xc.h>
#include <stdlib.h>
#include "skUARTlib.h"

#if defined _18F14K50

#pragma config FOSC   = IRC,  PLLEN  = ON,  FCMEN  = OFF
#pragma config IESO   = OFF, USBDIV = OFF, CPUDIV = NOCLKDIV
#pragma config PWRTEN = ON, BOREN  = ON, WDTEN  = OFF
#pragma config HFOFST = OFF, MCLRE  = OFF
#pragma config STVREN = ON,  BBSIZ  = OFF, LVP    = OFF
#pragma config XINST  = OFF
#pragma config CP0    = OFF, CP1    = OFF, CPB    = OFF
#pragma config WRT0   = OFF, WRT1   = OFF, WRTB   = OFF, WRTC   = OFF
#pragma config EBTR0  = OFF, EBTR1  = OFF, EBTRB  = OFF

#define _XTAL_FREQ 8000000         // CLK 12MHz (use from __delay_ms)

#define LCD_RS    PORTCbits.RC0
#define LCD_EN    PORTCbits.RC1
#define LCD_D4    PORTCbits.RC3
#define LCD_D5    PORTCbits.RC4
#define LCD_D6    PORTCbits.RC5
#define LCD_D7    PORTCbits.RC6

#endif

#define	LCD_STROBE()	((LCD_EN=1),(LCD_EN=0))

void Wait(unsigned int num)
{
     int i ;

     // numで指定した回数だけ繰り返す
     for (i=0 ; i<num ; i++) {
          __delay_ms(10) ;     // 10msプログラムの一時停止
     }
}

void lcd_write(unsigned char c)
{
     // 送信データのバイト列上位４ビットを処理
     LCD_D4 = ( ( c >> 4 ) & 0x01 ) ;
     LCD_D5 = ( ( c >> 5 ) & 0x01 ) ;
     LCD_D6 = ( ( c >> 6 ) & 0x01 ) ;
     LCD_D7 = ( ( c >> 7 ) & 0x01 ) ;
     LCD_STROBE() ;
     // 送信データのバイト列下位４ビットを処理
     LCD_D4 = ( ( c ) & 0x01 ) ;
     LCD_D5 = ( ( c >> 1 ) & 0x01 ) ;
     LCD_D6 = ( ( c >> 2 ) & 0x01 ) ;
     LCD_D7 = ( ( c >> 3 ) & 0x01 ) ;
     LCD_STROBE() ;
}

void command(unsigned char c)
{
     LCD_RS = 0 ;
     LCD_D4 = ( ( c ) & 0x01 ) ;
     LCD_D5 = ( ( c >> 1 ) & 0x01 ) ;
     LCD_D6 = ( ( c >> 2 ) & 0x01 ) ;
     LCD_D7 = ( ( c >> 3 ) & 0x01 ) ;
     LCD_STROBE() ;
}

/*******************************************************************************
*    lcd_clear - ＬＣＤモジュールの画面を消す処理                              *
*******************************************************************************/
void lcd_clear(void)
{
     LCD_RS = 0 ;
     lcd_write(0x01) ; // Clear Display : 画面全体に20Hのｽﾍﾟｰｽで表示、ｶｰｿﾙはcol=0,row=0に移動
     __delay_ms(2) ;   // LCDが処理(1.53ms)するのを待ちます
}
/*******************************************************************************
*    lcd_setCursor - ＬＣＤモジュール画面内のカーソル位置を移動する処理        *
*      col : 横(列)方向のカーソル位置(0-15)                                    *
*      row : 縦(行)方向のカーソル位置(0-1)                                     *
********************************************************************************/
void lcd_setCursor(int col, int row)
{
     int row_offsets[] = { 0x00, 0x40 } ;

     LCD_RS = 0 ;
     lcd_write(0x80 | (col + row_offsets[row])) ; // Set DDRAM Adddress : 00H-0FH,40H-4FH
}
/*******************************************************************************
*  lcd_putc - ＬＣＤにデータを１バイト出力する処理                             *
*    c : 出力する文字データ                                                    *
*******************************************************************************/
void lcd_putc(char c)
{
     LCD_RS = 1 ;        // RSの制御信号線をセットします
     lcd_write( c ) ;    // ＬＣＤにデータの送信
}
/*******************************************************************************
*  lcd_puts - LCDに文字列データを出力する処理(文字列をNULL(0x00)まで繰返し出力)*
*    s : 出力する文字列のデータ                                                *
*******************************************************************************/
void lcd_puts(const char * s)
{
	LCD_RS = 1 ;        // RSの制御信号線をセットします
	while(*s) lcd_write(*s++) ;
}
/*******************************************************************************
*  lcd_init - ＬＣＤの初期化処理                                               *
*******************************************************************************/
void lcd_init()
{
     LCD_RS = 0 ;
     LCD_EN = 0 ;

     __delay_ms(15) ;    // 電源ＯＮ後15msまで待ってから初期化

     // LCDの立上げ時のチェックデータ(イニシャライズ処理用)を設定
     command(0x03) ;
     __delay_ms(5) ;
     command(0x02) ;
     // ＬＣＤにコマンドを発行します
     lcd_write(0x28) ;   // function set   : データ線は4本・表示は２行・フォントは5x8ドット
     lcd_write(0x0c) ;   // display control: 画面表示はON・カーソル表示はOFF・カーソル点滅はOFF
     lcd_clear() ;       // Clear Display  : 画面をクリアし、カーソル位置はcol=0,row=0
     lcd_write(0x06) ;   // entry mode set : 文字を表示した次にカーソルを移動するを指示
}

void interrupt inter(void) {
    InterUART();
}

void main(void)
{
    int i = 0;
    char s[17];
//    char debug[2] = "a";

    OSCCON = 0b01100010;
    TRISC  = 0b00000000;
    ANSEL  = 0b00000000;
    ANSELH = 0b00000000;
    PORTC = 0;
    InitUART(12, 10, 51);

    lcd_init();
    lcd_setCursor(0, 0);
    lcd_puts(" Monitor  Start ");

    while(1) {
        if (UART_Available() != 0) {
            char c = UART_Read();
            if (c == '\n' || c == '\r') {
                c = 0;
            }
//            debug[0] = c;
//            UART_Send(debug, 2);
            if (i < 16 || c == 0) {
                s[i++] = c;
                if (c == 0) {
                    lcd_setCursor(0, 1);
                    lcd_puts("                ");
                    lcd_setCursor(0, 1);
                    lcd_puts(s);
                    i = 0;
                }
            }
        }
    }

    return;
}


