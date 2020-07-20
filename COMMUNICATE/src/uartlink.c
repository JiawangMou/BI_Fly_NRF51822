#include "uartlink.h"
#include "uart.h"

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

/*�������ӽ���ATKPPacket*/
bool uartlinkReceiveATKPPacket(atkp_t *p)
{
	static uartLinkRxSate_e rxState = waitForStartByte1;
	static uint8_t dataIndex=0;
	static uint8_t cksum=0;
	static uint8_t length=0;
	uint8_t c;

	p->dataLen = 0;

	if (rxState == state_done)
	{
		rxState = waitForStartByte1;
		dataIndex = 0;
	}

	while (uartIsDataReceived() && (rxState != state_done))
	{
		c = uartGetc();
		switch(rxState)
		{
			case waitForStartByte1:
				rxState = (c == UP_BYTE1) ? waitForStartByte2 : waitForStartByte1;
				cksum = c;
				break;
			case waitForStartByte2:
				rxState = (c == UP_BYTE2) ? waitForMsgID : waitForStartByte1;
				cksum += c;
				break;
			case waitForMsgID:
				p->msgID = c;
				rxState = waitForDataLength;
				cksum += c;
				break;
			case waitForDataLength:
				length = c;
				cksum += c;
				dataIndex = 0;
				if (length > 0 && length <= ATKP_MAX_DATA_SIZE)
					rxState = waitForData;
				else if (length > ATKP_MAX_DATA_SIZE)
					rxState = waitForStartByte1;
				else
					rxState = waitForChksum1;
				break;
			case waitForData:
				if(dataIndex < ATKP_MAX_DATA_SIZE)
				{
					p->data[dataIndex] = c;
					cksum += c;
				}
				dataIndex++;
				if (dataIndex >= length)
				{
					rxState = waitForChksum1;
				}
				break;
			case waitForChksum1:
				if (cksum == c)	/*����У����ȷ*/
				{
					p->dataLen = length;
					rxState = state_done;
				} 
				else	/*У�����*/
				{
					rxState = waitForStartByte1;	
					dataIndex = 0;
				}
				break;
			case state_done:
				break;
		}
	}			
	return (rxState == state_done);
}

/*�������ӽ���ATKPPacket*/
bool uartlinkSendATKPPacket(atkp_t *packet)
{
	uint8_t cksum=0;
	int i;

	uartPutc(DOWN_BYTE1);
	cksum = DOWN_BYTE1;
	uartPutc(DOWN_BYTE2);
	cksum += DOWN_BYTE2;
	uartPutc((unsigned char)packet->msgID);
	cksum += packet->msgID;
	uartPutc((unsigned char)packet->dataLen);
	cksum += packet->dataLen;

	for (i=0; i < packet->dataLen; i++)
	{
		uartPutc(packet->data[i]);
		cksum += packet->data[i];
	}
	uartPutc(cksum);
	return true;
}
