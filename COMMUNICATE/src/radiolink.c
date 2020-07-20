#include <string.h>
#include "radiolink.h"
#include "esb.h"
#include "config_param.h"

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

static uint8_t rssi;

/*���߳�ʼ��*/
void radiolinkInit(void)
{
	uint64_t addr = (uint64_t)configParam.radio.addressHigh<<32 | configParam.radio.addressLow;
	esbInit();
	esbSetDatarate((EsbDatarate)configParam.radio.dataRate);
	esbSetChannel(configParam.radio.channel);
	esbSetAddress(addr);
}

/*���߷���ATKPPacket*/
bool radiolinkSendATKPPacket(const atkp_t* send)
{
	if(esbCanTxPacket() && (send->dataLen <= ATKP_MAX_DATA_SIZE))
	{
		EsbPacket* packet = esbGetTxPacket();
		if (packet) 
		{
			memcpy(packet->data, send, send->dataLen+2);/*2��msgID��dataLen���ֽ�*/
			packet->size = send->dataLen+2;
			esbSendTxPacket();
			return true;
		}
	}
	return false;
}

/*���߽���ATKPPacket*/
bool radiolinkReceiveATKPPacket(atkp_t* rxPacket)
{	
	if(esbIsRxPacket())
	{
		EsbPacket* packet = esbGetRxPacket();
		if(packet)
		{
			rssi = packet->rssi;/*��RSSI�ȱ���������������ظ�ң����*/
			memcpy(rxPacket, packet->data, packet->size);
			esbReleaseRxPacket();
			return true;
		}
	}
	return false;
}

/*��ȡ�����ź�*/
uint8_t getRadioRssi(void)
{
	return rssi;
}

/*�������ߵ�ַ*/
void radiolinkSetAddress(uint64_t address)
{
	esbSetAddress(address);
}

/*��������ͨ��*/
void radiolinkSetChannel(uint8_t channel)
{
	esbSetChannel(channel);
}

/*������������*/
void radiolinkSetDatarate(EsbDatarate dataRate)
{
	esbSetDatarate(dataRate);
}
