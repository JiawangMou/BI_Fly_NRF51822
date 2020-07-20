#ifndef __CONFIG_PARAM_H
#define __CONFIG_PARAM_H
#include <stdint.h>
#include "esb.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * ���ò�����������
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

/* Ĭ�����ò��� */
#define  VERSION	11		/*11��ʾV1.1*/

#define  RADIO_CHANNEL 		2
#define  RADIO_DATARATE 	esbDatarate250K
#define  RADIO_ADDRESS 		0x123456789AULL

/*�������ýṹ*/
typedef struct{
	uint8_t channel;		
	EsbDatarate dataRate;
	uint32_t addressHigh;	/*ͨ�ŵ�ַ��4�ֽ�*/
	uint32_t addressLow;	/*ͨ�ŵ�ַ��4�ֽ�*/
}radioConfig_t;

/*��������ṹ*/
typedef struct
{
	uint8_t version;		/*����汾��*/
	radioConfig_t radio;	/*��������*/
	uint8_t cksum;			/*У��*/
} configParam_t;

extern configParam_t configParam;


void configParamInit(void);
void writeConfigParamToFlash(void);
void resetConigParam(void);


#endif /*__CONFIG_PARAM_H*/
