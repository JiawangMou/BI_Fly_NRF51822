#ifndef __BUTTON_H
#define __BUTTON_H
#include "pinout.h"
#include "nrf_gpio.h"

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

#define BUTTON_READ()	(nrf_gpio_pin_read(BUTTON_PIN) & 1UL)

#define BUTTON_PRESSED		0UL
#define BUTTON_RELEASED 	1UL

typedef enum {
	buttonIdle=0, 
	buttonShortPress, 
	buttonLongPress
} buttonEvent_e;


void buttonInit(buttonEvent_e init);
void buttonProcess(void);
buttonEvent_e buttonGetState(void);


#endif /*__BUTTON_H*/
