#include "nrf.h"
#include "systick.h"

/********************************************************************************	 
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * �δ�ʱ����������
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

static uint32_t tick = 0;



void TIMER2_IRQHandler()
{
	tick++;
	NRF_TIMER2->EVENTS_COMPARE[0] = 0;	//����ȽϼĴ����¼���־
}

/*�δ�ʱ����ʼ��1ms�ж�һ��*/
void systickInit()
{
	NRF_TIMER2->TASKS_CLEAR = 1;	//�����ʱ��

	NRF_TIMER2->PRESCALER = 4;		//2^4=16��Ƶ,16M/16=1Mhz
	NRF_TIMER2->CC[0] = 1000;		//���ò���Ƚ�ֵ	
	NRF_TIMER2->SHORTS = 1UL<<TIMER_SHORTS_COMPARE0_CLEAR_Pos;		//����Ƚϴ���0����ֵ
	NRF_TIMER2->INTENSET = (1UL << TIMER_INTENSET_COMPARE0_Pos);	//���ñȽϴ���0�Ƚ��ж�
	NVIC_SetPriority(TIMER2_IRQn, 2);
	NVIC_EnableIRQ(TIMER2_IRQn);	

	NRF_TIMER2->TASKS_START = 1;
}

uint32_t systickGetTick()
{
	return tick;
}

void msDelay(int len)
{
	uint32_t start = systickGetTick();
	while (systickGetTick() < start+len);
}

