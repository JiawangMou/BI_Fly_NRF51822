#ifndef __UARTLINK_H
#define __UARTLINK_H
#include <stdbool.h>
#include "atkp.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * ��������ͨ�Ŵ���
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

typedef enum
{
	waitForStartByte1,
	waitForStartByte2,
	waitForMsgID,
	waitForDataLength,
	waitForData,
	waitForChksum1,
	state_done
} uartLinkRxSate_e;

bool uartlinkReceiveATKPPacket(atkp_t *packet);
bool uartlinkSendATKPPacket(atkp_t *packet);

#endif

