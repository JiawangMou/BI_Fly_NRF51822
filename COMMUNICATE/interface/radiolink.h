#ifndef __RADIOLINK_H
#define __RADIOLINK_H
#include <stdint.h>
#include <stdbool.h>
#include "atkp.h"
#include "esb.h"

/*����ָ��*/
#define  U_RADIO_RSSI		0x01
#define  U_RADIO_CONFIG		0x02	

/*����ָ��*/
#define  D_RADIO_HEARTBEAT	0xFF
#define  D_RADIO_GET_CONFIG	0x01
#define  D_RADIO_SET_CONFIG	0x02

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

void radiolinkInit(void);
bool radiolinkSendATKPPacket(const atkp_t* send);
bool radiolinkReceiveATKPPacket(atkp_t* rxPacket);
uint8_t getRadioRssi(void);
void radiolinkSetAddress(uint64_t address);
void radiolinkSetChannel(uint8_t channel);
void radiolinkSetDatarate(EsbDatarate dataRate);


#endif /*__RADIOLINK_H*/
