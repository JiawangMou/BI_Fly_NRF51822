#ifndef __PM_H
#define __PM_H
#include <stdbool.h>

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * ��Դ������������
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

void pmPowerOff(void);
void pmInit(void);
bool pmUSBPower(void);
bool pmIsCharging(void);
float pmGetVBAT(void);
void pmProcess(void);


#endif //__PM_H
