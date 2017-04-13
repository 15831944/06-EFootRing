/**
  ******************************************************************************
	*�ļ���Process.h
	*���ߣ��Ϻ�
	*�汾��2.0
	*���ڣ�
	*��Ҫ��
	*��ע��
	*
  ******************************************************************************  
	*/ 
	
/* �����ֹ�ݹ�����Ŀ��ƺ�----------------------------------------------------*/
#ifndef _PROCESS_H
#define _PROCESS_H

/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "DataTypeConvertion.h"
#include "stm32f10x.h"
//#include "Protocol.h"
#include "TerminalControl.h"
#include "SerialPort.h"
#include "Timer.h"
#include "IoOut.h"
#include "Key.h"
#include "STM32_SysTick.h"
#include "STM32_ADC.h"
#include "stdint.h"

/*--�궨��-����----------------------------------------------------------*/

//#define USE_SLEEP

//	����
#define	NUM_KEY				1
#define	NUM_KEY_DETECT		0   			
#define	PIN_KEY_DETECT		GPIO_Pin_10
#define	PORT_KEY_DETECT		GPIOB

//	������
#define	PIN_BEEP			GPIO_Pin_3	
#define	PORT_BEEP			GPIOA

//	�������
#define	PIN_SHOCK			GPIO_Pin_2
#define	PORT_SHOCK			GPIOA

//	ָʾ��
#define	PIN_LED_TEST		GPIO_Pin_8	
#define	PORT_LED_TEST		GPIOB

// �̵�
#define	PIN_LED_GREEN		GPIO_Pin_5	
#define	PORT_LED_GREEN		GPIOB

//	���
#define	PIN_LED_RED			GPIO_Pin_6	
#define	PORT_LED_RED		GPIOB


/*--�궨�壺���������--------------------------------------------------------*/


/*--�궨��:AD-----------------------------------------------------------------*/
//	��ѹ�ɼ�
#define	NUM_AD				1
#define NUM_POWER			0

//	����ţ���001��ʼ��ÿ�������ڵ��豸ID����01��ʼ
#define	ID_ROOM				001		//	ʵ�ʷ����
#define	ID_TERMINAL			01		//	ʵ�ʱ����
#define	ID_ROOM_INVALID		000		//	��Ч�����
#define	ID_TERMINAL_INVALID	0x00	//	��Ч�����

//	ģʽ������ģʽ/�ӽ�ģʽ
#define	MODE_POINT			0X00
#define	MODE_APPROACH		0X01

//	����״̬������/���/Ĭ��
#define	STATE_ALARM			0X01
#define	STATE_SHOCK			0X02
#define STATE_DEFAULT		0X00

#define	KEY_DEFAULT			0X00	//	Ĭ��
#define	KEY_ALARM_SHOT		0X01	//	�������̰�
#define	KEY_ALARM_LONG		0X02	//	����������
#define	KEY_SHOCK_SHOT		0X03	//	������̰�
#define	KEY_SHOCK_LONG		0X04	//	���������

class Process
{
	public:
					Process();
					~Process();
		void 		runOnTime2(void);				//	�����ڶ�ʱ��TIM2�еĺ���
		void 		runOnTime3(void);				//	�����ڶ�ʱ��TIM3�еĺ���
		void 		runOnTime4(void);				//	�����ڶ�ʱ��TIM4�еĺ���
		void 		openPeriph(void);				//	��������

		IoOut 		*testLed;						//	����ָʾ��
	
	private:
		SerialPort 	*zigbeePort;
		//Protocol 	*protocol;
		TerminalControl *zigbeeControl;
		Timer		*t2;							//	��ʱ��2
		Timer		*t3;							//	��ʱ��3
		Timer		*t4;							//	��ʱ��4
	
		IoOut		*greenLed,*redLed;				//	���̵�
		IoOut		*beepCtr;					//	�����ն����ͨ��
		IoOut		*shockCtr;						//	�����ն������ѹ
	
		uint32_t    adValue[NUM_AD];				//	ADͨ��


		uint8_t		keyState;
		uint8_t		powerVoltage;
		uint8_t		controlMode;						//����ģʽģʽ
		uint8_t		controlStatus;					//����״̬
	
		uint16_t	RoomID;						//	���յ��ķ���ID
		uint16_t	TerminalID;					//	���յ����ն�ID
		uint16_t  watchDogTimer;    //ģ�⿴�Ź�

		void		enable_SWD_disable_Jtag(void);	//	ʹ��SWD���ر�JTAG
		void		receiveMainData(void);				//	������������
		void	    initAllData(void);				//	ȫ�����ݳ�ʼ����������־λ
		void		resetControlData(void);			//	���ÿ�������
		void		initIOs(void);					//	��ʼ��GPIO
		void	    initADs(void);					//	��ʼ��ADͨ��
		void	    initUsarts(void);				//	��ʼ������
		void	    initTimers(void);				//	��ʼ����ʱ��
		void		resetTerminal(void);			//	��λ
		void		disposal();				//	��������,��ִ�в���


		

		void		getPowerValue(void);			//	��ȡ��ѹֵ

		

};
	
#endif 
/*--------------------------------- End of Process.h -----------------------------*/
