/**
  ******************************************************************************
	*�ļ���Process.h
	*���ߣ�������
	*�汾��1.0
	*���ڣ�2016-12-17
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

	#include "TerminalControl.h"
	#include "LCDdisplay.h" 
	
	#include "SerialPort.h"
	#include "Timer.h"
	#include "IoOut.h"
	#include "Key.h"
	#include "STM32_SysTick.h"
	#include "STM32_ADC.h" 
	#include "stdint.h"
	
	#include "oled.h"
	#include "sqStack.h"
	
//#define USE_SLEEP	

/* ���Ŷ���--------------------------------------------------------------------*/


#define	USE_PERIPH
#ifdef	USE_PERIPH

	
	//<���� ���Ŷ���>
//10������
	#define KEYNUM1_PORT           GPIOB             //���ְ���1��
	#define KEYNUM1_PIN            GPIO_Pin_1
	#define KEYNUM2_PORT           GPIOB            //���ְ���2��
	#define KEYNUM2_PIN            GPIO_Pin_2
	#define KEYNUM3_PORT           GPIOB            //����3
	#define KEYNUM3_PIN            GPIO_Pin_12
	#define KEYNUM4_PORT           GPIOB           //����4
	#define KEYNUM4_PIN            GPIO_Pin_13	
	#define KEYNUM5_PORT           GPIOA           //����5
	#define KEYNUM5_PIN            GPIO_Pin_11
 	
	#define KEYNUM6_PORT           GPIOA            //����6
	#define KEYNUM6_PIN            GPIO_Pin_5      
	#define KEYNUM7_PORT           GPIOA            //����7
	#define KEYNUM7_PIN            GPIO_Pin_6 
	#define KEYNUM8_PORT           GPIOA           //����8
	#define KEYNUM8_PIN            GPIO_Pin_7
	#define KEYNUM9_PORT           GPIOB             //����9
	#define KEYNUM9_PIN            GPIO_Pin_0
	#define KEYNUM0_PORT           GPIOA              //����0
	#define KEYNUM0_PIN            GPIO_Pin_12
	
//5�����ܼ�	20170118
	#define KEYDEL_PORT           GPIOA                //�˸񰴼�
	#define KEYDEL_PIN            GPIO_Pin_4	
	#define KEYENTER_PORT         GPIOB                //�˸񰴼�
	#define KEYENTER_PIN          GPIO_Pin_9
	#define KEYMODE_PORT          GPIOB              //ģʽ���� 
	#define KEYMODE_PIN           GPIO_Pin_8
	#define KEYALARM_PORT           GPIOB              //��������
	#define KEYALARM_PIN            GPIO_Pin_7
	#define KEYSHOCK_PORT           GPIOB              //�������
	#define KEYSHOCK_PIN            GPIO_Pin_6
	
//3��ָʾ�� 20170118
	#define LED1_PORT              GPIOA
	#define LED1_PIN               GPIO_Pin_8
	#define LED2_PORT              GPIOB
	#define LED2_PIN               GPIO_Pin_15
	#define LED_POW_PORT           GPIOB
	#define LED_POW_PIN            GPIO_Pin_14
	

	
	//</���� ���Ŷ���>
	

	
//</���Ŷ˿ڶ���>
	
//<AD ��ض���>
	/* AD˳����--------------------------------------------------------------------*/

#define	NUM_POWER			   	0										//	��Դ��ѹ

	//</AD ��ض���>

#endif
	//��ʱ��ʱ��
	#define	TIM2_DELAY_TIME		20
	#define	TIM3_DELAY_TIME		80
	#define	TIM4_DELAY_TIME		200

	/* ��̬�������� --------------------------------------------------------------------*/
	


	/* ������� --------------------------------------------------------------------*/
	class Process
	{
		public:
						Process();
						~Process();
			void 		runOnTime2(void);							//	�����ڶ�ʱ��TIM2�еĺ���
			void 		runOnTime3(void);							//	�����ڶ�ʱ��TIM3�еĺ���
			void 		runOnTime4(void);							//	�����ڶ�ʱ��TIM4�еĺ���
			void 		openPeriph(void);							//	��������	
			void    startWork(void);             //��ʼ����
		
		private:
		
			u8* OLEDString;                  //��ʾ���ַ���
			SqStack *sqStack;                //��Ű������ݵ�ջ
			
			u8 *sendData;
			u8 *receiveData;

		


			Timer		         *t2,*t3,*t4;								//	��ʱ��2,��ʱ��3,��ʱ��4			
			SerialPort       *zigbeePort;               //������
			SerialPort       *lcdPort;                  //LCD������
			TerminalControl  *zigbeeControl;            //����ͨѶ������
			LCDdisplay       *LCDControl;								//Һ������ʾ������
			

			//<����ʵ������>

			Key     *keyNum1;
			Key     *keyNum2;
			Key     *keyNum3;
			Key     *keyNum4;
			Key     *keyNum5;
			Key     *keyNum6;
			Key     *keyNum7;
			Key     *keyNum8;
			Key     *keyNum9;
			Key     *keyNum0;
			
			Key   *keyDel;
			Key   *keyMode;
			Key   *keyShock;
			Key   *keyAlarm;
				
			//IO
			IoOut   *ledPow;
			IoOut   *led1,*led2;
			
			u8 controlData;
			u8 controlModeFlag;  //��ʶ����ģʽ
			u8 controlStatusFlag;  //��ʶ����״̬
			u8 isFreeMode;
			
			u8 powerValue;   //��ѹֵ
			bool isShocking;       //���ڵ��
			bool isAlarming;        //���ھ���
			
			int watchDogTimer;
					
	
			
private:
			void	    initAllData(void);							//	ȫ�����ݳ�ʼ����������־λ
			void			initGPIO(void);							  	//	��ʼ��IO
			void	    initADs(void);								//	��ʼ��ADͨ��
			void	    initUsarts(void);							//	��ʼ������
			void	    initTimers(void);							//	��ʼ����ʱ��		
			void      initOLED(void);                //��ʼ��Һ����
			
			void    getPowerValue();
			void	  getKeyValue(void);							//	��ȡ����ֵ

			void		dealKeyValue(void);							//	��������������
      bool    anyKeyDown();                  //û�в���������

//<LCD��ʾ>
			void    showStartPic();
			void    showMainWin();
			void 		showInputWin(bool flag);             //��ʾ������봰��
			void    showNumber(int position,int num);    //��ʾ����
			void    showPower();                //��ʾ����
			void    showControlMode();          //��ʾ����ģʽ  �ӽ��Ͷ���
			void    showControlStatus();        //��ʾ����״̬  ����;���
//</LCD��ʾ>

			void    updataToZigbee();         //���·�������
			void    sendCheckSignal();        //���ͼ���źţ����ڷ���FF FF AA
		
			

			void    SYSCLKconfig_STOP(void);   //��ͣ��״̬���Ѻ���Ҫ����ʱ��
			void		enable_SWD_disable_Jtag(void);				//	ʹ��SWD���ر�JTAG
			void		testWithUsart(void);						//	ͨ�����ڲ�������
	};
	
#endif 
/*--------------------------------- End of Process.h -----------------------------*/
