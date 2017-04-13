/**
  ******************************************************************************
	*�ļ���Process.h
	*���ߣ�������
	*�汾��2.0
	*���ڣ�2017-01-09
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


//#define USE_SLEEP
/*--�궨��-����----------------------------------------------------------*/




//	������
#define	PIN_BEEP			GPIO_Pin_3	
#define	PORT_BEEP			GPIOA

//	�������
#define	PIN_SHOCK_POW		GPIO_Pin_2
#define	PORT_SHOCK_POW			GPIOA

#define	PIN_SHOCK		GPIO_Pin_1
#define	PORT_SHOCK			GPIOA

//	ָʾ��
//��·��û��

// �̵�
#define	PIN_LED_GREEN		GPIO_Pin_5	
#define	PORT_LED_GREEN		GPIOA

//	���
#define	PIN_LED_RED			GPIO_Pin_6	
#define	PORT_LED_RED		GPIOA
 
//����ģ��ʹ�ܿ��� YL_EN
#define	PIN_YL_EN		GPIO_Pin_7
#define	PORT_YL_EN		GPIOA

//����ģ��ʹ�ܿ��� YL_SET
#define	PIN_YL_SET		GPIO_Pin_8
#define	PORT_YL_SET		GPIOA

//�豸�𻵼�� CutDect
#define	PIN_CUT_DECT		GPIO_Pin_11
#define	PORT_CUT_DECT		GPIOA



/*--�궨�壺���������--------------------------------------------------------*/


/*--�궨��:AD-----------------------------------------------------------------*/
//	��ѹ�ɼ�
#define	NUM_AD				1
#define NUM_POWER			0

//	����ţ���001��ʼ��ÿ�������ڵ��豸ID����01��ʼ
#define	ID_ROOM				101		//	ʵ�ʷ����
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


	
	private:
		SerialPort 	*zigbeePort;

		TerminalControl *zigbeeControl;
		Timer		*t2;							//	��ʱ��2
		Timer		*t3;							//	��ʱ��3
		Timer		*t4;							//	��ʱ��4
	
		IoOut		*greenLed,*redLed;				//	���̵�
		IoOut		*beepCtrl;					     //	�����ն����������
		IoOut   *shockPowCtrl;           //	�����ն�������
		IoOut		*shockCtrl;						   //	�����ն�������
		IoOut   *YL_EN_Ctrl,*YL_SET_Ctrl;  //����YL�豸��EN����SET


	



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
		void    showPowerStatus();

		

};
	
#endif 
/*--------------------------------- End of Process.h -----------------------------*/
