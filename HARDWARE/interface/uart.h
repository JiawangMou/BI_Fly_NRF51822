#ifndef __UART_H
#define __UART_H
#include <stdbool.h>

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * ������������
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

void uartInit(void);
void uartDeinit(void);
void uartPuts(char* string);
void uartSend(char* data, int len);
void uartPutc(char c);
bool uartIsDataReceived(void);
char uartGetc(void);

#endif /*__UART_H*/
