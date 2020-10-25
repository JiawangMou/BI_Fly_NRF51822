#include "button.h"
#include "config_param.h"
#include "led.h"
#include "nrf.h"
#include "nrf_gpio.h"
#include "pinout.h"
#include "pm.h"
#include "radiolink.h"
#include "systick.h"
#include "uart.h"
#include "uartlink.h"
#include <string.h>

/********************************************************************************
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * main.c
 * ����ϵͳ��ʼ����ͨ�Ŵ���
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/2
 * �汾��V1.0
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
 ********************************************************************************/
#define GSENABLE 0

static bool powerFlag = true;

static void mainloop(void);
static void handleRadioCmd(atkp_t* packet);
int         main()
{
    /*�ȴ��ⲿ��������*/
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
        ;

    LED_INIT();
    LED_OFF();
    systickInit();
    //	buttonInit(buttonIdle);

    /*��������2S�ɿ�������STM32����bootloader*/
    //	if(BUTTON_READ() == BUTTON_PRESSED)
    //	{
    //		msDelay(50);
    //		uint32_t ledTime= systickGetTick();
    //		uint32_t tickCnt = systickGetTick();
    //		while(BUTTON_READ() == BUTTON_PRESSED)
    //		{
    //			if(systickGetTick() - tickCnt > 2000)
    //			{
    //				nrf_gpio_cfg_output(UART_RTS_PIN);
    //				nrf_gpio_pin_set(UART_RTS_PIN);
    //				if(systickGetTick() - ledTime > 200)
    //				{
    //					ledTime = systickGetTick();
    //					LED_TOGGLE();
    //				}
    //			}
    //		}
    //	}
    /*�����ͨ�ų�ʼ��*/
    LED_ON();
    pmInit();
    uartInit();
    configParamInit();
    radiolinkInit();
    /*ͨ�Ŵ���*/
    mainloop();
    while (1)
        ;
}

static void mainloop(void)
{
    static atkp_t   radiolinkRxPacket;
    static atkp_t   AckTxPacket;
    static atkp_t   uartlinkRxPacket;
    static uint32_t LEDCountTime;
    static int      vbatSendTime;
    static int      radioRSSISendTime;

    while (1) {
        if (radiolinkReceiveATKPPacket(&radiolinkRxPacket) == true) {
            if (radiolinkRxPacket.msgID == DOWN_RADIO) //��������
            {
                handleRadioCmd(&radiolinkRxPacket);
            } else if (radiolinkRxPacket.msgID == DOWN_POWERCMD) {
                // if (powerFlag == true) {
                //     powerFlag           = false;
                //     AckTxPacket.msgID   = DOWN_POWERCMD;
                //     AckTxPacket.dataLen = 1;
                //     AckTxPacket.data[0] = powerFlag;
                //     radiolinkSendATKPPacket(&AckTxPacket);
                //     powerOff();
                //     LEDCountTime = systickGetTick();
                // } else {
                //     powerFlag = true;
                //     powerOn();
                //     AckTxPacket.msgID   = DOWN_POWERCMD;
                //     AckTxPacket.dataLen = 1;
                //     AckTxPacket.data[0] = powerFlag;
                //     radiolinkSendATKPPacket(&AckTxPacket);
                // }
                if (radiolinkRxPacket.data[0] = 1) {
                    powerOn();
                } else {
                    powerOff();
                    LEDCountTime = systickGetTick();
                }
            } else //ת����STM32
            {
                if (powerFlag == true) {
                    uartlinkSendATKPPacket(&radiolinkRxPacket);
                }
            }
        }

        if (uartlinkReceiveATKPPacket(&uartlinkRxPacket) == true) {
            if (uartlinkRxPacket.msgID == DOWN_RADIO) //��������
            {

            } else //ת����ң����
            {
                if (GSENABLE) {
                    radiolinkSendATKPPacket(&uartlinkRxPacket);
                }
            }
        }

        if ((systickGetTick() > 1500) && powerFlag == true) /*��ʱ1500ms��STM32����*/
        {
            /*100ms���͵�ص�����STM32*/
            if ((systickGetTick() >= vbatSendTime + 100)) {
                float   bat;
                uint8_t flags = 0;
                atkp_t  txPacket;

                vbatSendTime     = systickGetTick();
                txPacket.msgID   = DOWN_POWER;
                txPacket.dataLen = 5;
                flags |= (pmIsCharging() == true) ? 0x01 : 0;
                flags |= (pmUSBPower() == true) ? 0x02 : 0;
                txPacket.data[0] = flags;
                bat              = pmGetVBAT();
                memcpy(txPacket.data + 1, &bat, sizeof(float));
                uartlinkSendATKPPacket(&txPacket);
            }
            /*50ms����һ��RSSI��ң����*/
            if (systickGetTick() >= radioRSSISendTime + 50) {
                atkp_t txPacket;

                radioRSSISendTime = systickGetTick();
                txPacket.msgID    = UP_RADIO;
                txPacket.dataLen  = 2;
                txPacket.data[0]  = U_RADIO_RSSI;
                txPacket.data[1]  = getRadioRssi();
                radiolinkSendATKPPacket(&txPacket);
            }
        }
        if ((systickGetTick() - LEDCountTime > 2000) && powerFlag == false) {
            LED_ON();
            LEDCountTime = systickGetTick();
            while (systickGetTick() < LEDCountTime + 2000)
                ;
            LED_OFF();
            LEDCountTime = systickGetTick();
        }

        //		/*�����¼�����*/
        //		buttonEvent_e be = buttonGetState();
        //		if(be == buttonShortPress)
        //		{
        //			pmPowerOff();/*�ػ�*/
        //		}
        //		else if(be == buttonLongPress)
        //		{
        //			nrf_gpio_pin_clear(PM_VCCEN_PIN);
        //			int timeOut = systickGetTick();
        //			while(BUTTON_READ()==BUTTON_PRESSED &&
        //				systickGetTick() < timeOut+3000)/*�����ͷŻ�ʱ3S���˳�*/
        //			{
        //				msDelay(400);
        //				LED_TOGGLE();
        //			}
        //			resetConigParam();/*�ָ�Ĭ�ϲ���*/
        //			__disable_irq();
        //			NVIC_SystemReset();/*�����λ*/
        //		}

        /*��������͵�Դ����*/
        // buttonProcess();
        pmProcess();
    }
}

/*���������*/
static uint8_t generateRandomNum(void)
{
    NRF_RNG->TASKS_START   = 1;
    NRF_RNG->EVENTS_VALRDY = 0;
    while (NRF_RNG->EVENTS_VALRDY == 0)
        ;
    NRF_RNG->TASKS_STOP = 1;
    return NRF_RNG->VALUE;
}

/*�������ô���*/
static void handleRadioCmd(atkp_t* packet)
{
    if (packet->data[0] == D_RADIO_HEARTBEAT) {
        uartlinkSendATKPPacket(packet);
    } else if (packet->data[0] == D_RADIO_GET_CONFIG) {
        atkp_t        txPacket;
        radioConfig_t radio;

        radio.addressLow  = NRF_FICR->DEVICEID[0]; /*оƬUID�����ֽ�*/
        radio.addressHigh = NRF_FICR->DEVICEID[1]; /*оƬUID�����ֽ�*/
        radio.dataRate    = RADIO_DATARATE;
        uint8_t ch        = generateRandomNum();
        ch                = ch & 0x7F;
        if (ch < 2)
            ch = 2;
        else if (ch > 125)
            ch = 125;
        radio.channel = ch; /*ͨ����2-125*/

        txPacket.msgID   = UP_RADIO;
        txPacket.dataLen = sizeof(radio) + 1;
        txPacket.data[0] = U_RADIO_CONFIG;
        memcpy(txPacket.data + 1, &radio, sizeof(radio));
        radiolinkSendATKPPacket(&txPacket);
    } else if (packet->data[0] == D_RADIO_SET_CONFIG) {
        radioConfig_t radio;
        memcpy(&radio, packet->data + 1, sizeof(radio));

        radiolinkSetChannel(radio.channel);
        radiolinkSetDatarate(radio.dataRate);
        uint64_t addr = (uint64_t)radio.addressHigh << 32 | radio.addressLow;
        radiolinkSetAddress(addr);
        configParam.radio = radio;
        writeConfigParamToFlash();
    }
}
