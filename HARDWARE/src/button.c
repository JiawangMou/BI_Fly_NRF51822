#include <stdbool.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "pinout.h"
#include "button.h"
#include "systick.h"

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

#define BUTTON_LONGPRESS_TICK 1000 /*�����ж�Ϊ����ʱ��*/

static buttonEvent_e state;

/*������ʼ��*/
void buttonInit(buttonEvent_e init)
{
	nrf_gpio_cfg_input(BUTTON_PIN, NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_sense_input(BUTTON_PIN, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
	state = init;
}

/*����ɨ�账��*/
void buttonProcess()
{
	static unsigned int lastTick;
	static unsigned int pressedTick;
	static bool pressed = false;

	if (lastTick != systickGetTick())
	{
		lastTick = systickGetTick();

		if(pressed==false && BUTTON_READ()==BUTTON_PRESSED)
		{
			pressed = true;
			pressedTick = systickGetTick();
		} 
		else if(pressed==true) 
		{
			if(BUTTON_READ()==BUTTON_RELEASED)
				pressed = false;
			if ((systickGetTick()-pressedTick) > BUTTON_LONGPRESS_TICK) 
				state = buttonLongPress;
			else if(BUTTON_READ()==BUTTON_RELEASED)
				state = buttonShortPress;
		}
	}
}

/*��ȡ����״̬*/
buttonEvent_e buttonGetState()
{
	buttonEvent_e currentState = state;
	state = buttonIdle;
	return currentState;
}



