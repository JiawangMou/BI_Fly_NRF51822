#ifndef __PINOUT_H
#define __PINOUT_H

#define LED_PIN 			2
#define BUTTON_PIN 			17	
#define PM_VCCEN_PIN 		21	/* LDOʹ�ܿ��ƽ� */
#define PM_CHG_STATE_PIN 	1	
#define PM_VBAT_SINK_PIN 	3	
#define USB_CONNECTED_PIN	7	/*�ж�USB�Ƿ��������*/

#define RADIO_PAEN_PIN 		25
#define RADIO_PATX_DIS_PIN 	22

/* ��STM32�������� */
#define STM_BOOT0_PIN 		5	
#define STM_NRST_PIN 		27	
#define UART_RX_PIN 		30
#define UART_TX_PIN 		31
#define UART_RTS_PIN 		26

#define AIN_VBAT 			ADC_CONFIG_PSEL_AnalogInput4	/*��ѹ�ɼ�����*/
#define AIN_VBAT_DIVIDER 	ADC_CONFIG_INPSEL_AnalogInputOneThirdPrescaling	/*ģ������1/3��ѹ*/
#define ADC_SCALER 			(2.0/1.0)	/*�����ѹ*/
#define ADC_DIVIDER 		(3.3/1.0)	/*��ѹ���ű���*/

#endif //__PINOUT_H

