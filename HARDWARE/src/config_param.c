#include <stdbool.h>
#include "nrf.h"
#include "config_param.h"

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

/*�������ò�����ַΪFLASH���һҳ*/
#define CONFIG_PARAM_ADDR  (NRF_FICR->CODEPAGESIZE * (NRF_FICR->CODESIZE-1))

configParam_t configParam;/*���ò���ȫ�ֱ���*/

static bool isConfigParamOK;
static configParam_t configParamDefault=
{
	.version = VERSION,
	.radio.channel = RADIO_CHANNEL,
	.radio.dataRate = RADIO_DATARATE,
	.radio.addressHigh = ((uint64_t)RADIO_ADDRESS >> 32),
	.radio.addressLow = (RADIO_ADDRESS & 0xFFFFFFFFULL),
};

void flash_write(uint32_t pageAddress, uint32_t* data, uint32_t writeNum)
{
	uint32_t* writeAddress = (uint32_t*)pageAddress;
	
	/*����ָ��pageAddress����*/
	NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Een << NVMC_CONFIG_WEN_Pos);/*ʹ�ܲ���FLASH*/
	while (NRF_NVMC->READY == NVMC_READY_READY_Busy);/*�ȴ�NVMC����*/
	NRF_NVMC->ERASEPAGE = (uint32_t)pageAddress;// Erase page:
	while (NRF_NVMC->READY == NVMC_READY_READY_Busy);/*�ȴ�NVMC����*/
	
	/*дFLASH*/
	NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Wen << NVMC_CONFIG_WEN_Pos);/*ʹ��дFLASH*/
	while (NRF_NVMC->READY == NVMC_READY_READY_Busy);/*�ȴ�NVMC����*/
	for(int i=0; i<writeNum; i++)
	{
		*writeAddress = data[i];
		writeAddress += 4;
	}
	while (NRF_NVMC->READY == NVMC_READY_READY_Busy);/*�ȴ�NVMC����*/
	NRF_NVMC->CONFIG = (NVMC_CONFIG_WEN_Ren << NVMC_CONFIG_WEN_Pos);/*����Ϊֻ��*/
	while (NRF_NVMC->READY == NVMC_READY_READY_Busy);/*�ȴ�NVMC����*/
}

void flash_read(uint32_t pageAddress, uint32_t* buffer, uint32_t readNum)
{
	uint32_t* readAddress = (uint32_t*)pageAddress;
	for(int i=0; i<readNum; i++)
	{
		buffer[i] = *readAddress;
		readAddress += 4;
	}
}

/* ����У��ֵ */
static uint8_t calculate_cksum(configParam_t* data)
{ 
	uint8_t cksum=0;	
	uint8_t* c = (uint8_t*)data;
	
	for (int i=0; i<sizeof(configParam_t); i++)
		cksum += *(c++);
	cksum -= data->cksum;
	return cksum;
}

/*���ò�����ʼ��*/
void configParamInit(void)
{
	uint32_t size = sizeof(configParam);
	uint32_t readNum = size/4 + (size%4 ? 1:0);
	
	/* ��ȡ���ò��� */
	flash_read(CONFIG_PARAM_ADDR, (uint32_t*)&configParam, readNum);
	
	if(configParam.version == VERSION)/*�汾��ȷ*/
	{
		uint8_t cksum = calculate_cksum(&configParam);
		if(cksum == configParam.cksum)/*У����ȷ*/
			isConfigParamOK = true;
		 else
			isConfigParamOK = false;
	}
	else/*�汾����*/
	{
		isConfigParamOK = false;
	}
	
	/* ���ò�������д��Ĭ�ϲ��� */
	if(isConfigParamOK == false)	
	{
		configParam = configParamDefault;
		writeConfigParamToFlash();
		isConfigParamOK=true;
	}
}

/*д���ò�����Flash*/
void writeConfigParamToFlash(void)
{
	uint32_t size = sizeof(configParam);
	uint32_t writeNum = size/4 + (size%4 ? 1:0);
	
	uint8_t cksum = calculate_cksum(&configParam);
	configParam.cksum = cksum;
	flash_write(CONFIG_PARAM_ADDR, (uint32_t*)&configParam, writeNum);
}

/*��λ���ò���ΪĬ�ϲ���*/
void resetConigParam(void)
{
	configParam = configParamDefault;
	writeConfigParamToFlash();	
}

