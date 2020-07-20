#ifndef __ESB_H
#define __ESB_H
#include <stdbool.h>
#include <stdint.h>

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

#pragma anon_unions

/*�������ݰ���ʽ*/
typedef __packed struct esbPacket_s 
{
	__packed struct 
	{
		uint8_t size;
		__packed union
		{
			uint8_t s1;
			__packed struct 
			{
				uint8_t noack :1;
				uint8_t pid :2;
			};
		};
		uint8_t data[32];
	};
	uint8_t rssi; /*0-100����λdb��*/
	unsigned int crc;
} EsbPacket;

typedef enum esbDatarate_e { 
	esbDatarate250K=0, 
	esbDatarate1M=1, 
	esbDatarate2M=2 
} EsbDatarate;


void esbInit(void);
void esbDeinit(void);
void esbInterruptHandler(void);
bool esbIsRxPacket(void);
EsbPacket * esbGetRxPacket(void);
void esbReleaseRxPacket(void);
bool esbCanTxPacket(void);
EsbPacket * esbGetTxPacket(void);
void esbSendTxPacket(void);
void esbSetDatarate(EsbDatarate datarate);
void esbSetChannel(unsigned int channel);
void esbSetTxPower(int power);
void esbSetAddress(uint64_t address);

#endif //__ESB_H
