/* 
 * File:   lcd32.h
 * Author: me
 *
 * Created on 2014/06/21, 23:54
 */

#ifndef LCD32_H
#define	LCD32_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LCD_RS    BIT_0
#define LCD_EN    BIT_1
#define LCD_D4    BIT_11
#define LCD_D5    BIT_13
#define LCD_D6    BIT_14
#define LCD_D7    BIT_15

void lcdDelayUs(WORD us);
void lcdDelayMs(unsigned int msec);
void lcdStrobe();
void lcdSetBit(int bt, char value);
void lcdCommand(unsigned char c);
void lcdWrite(unsigned char c);
void lcdSetCursor(int col, int row);
void lcdClearLine(int row);
void lcdClear(void);
void lcdPutc(char c);
void lcdPuts(const char *s);
void lcdInit(DWORD freq);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD32_H */

