// skUARTlib.h
// ＵＳＡＲＴ関数ライブラリ用インクルードファイル
#ifndef _SKUARTLIB_H_
#define _SKUARTLIB_H_

#define UART_BUFFER_SIZE   32     // ＵＳＡＲＴの受信バッファサイズ

void InterUART( void ) ;
void InitUART(int rx, int tx, char brg) ;
void UART_Send(const char *dt,int num) ;
int  UART_Available() ;
unsigned int UART_Read() ;
void UART_Flush() ;

#endif
