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
	#include "SpiPort.h"
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
#define USE_OLED
/* ���Ŷ���--------------------------------------------------------------------*/


#define	USE_PERIPH
#ifdef	USE_PERIPH

	
	//<���� ���Ŷ���>
	#define KEYNUM1_PORT           GPIOA             //���ְ���1��
	#define KEYNUM1_PIN            GPIO_Pin_2
	#define KEYNUM2_PORT           GPIOB            //���ְ���2��
	#define KEYNUM2_PIN            GPIO_Pin_0
	#define KEYNUM3_PORT           GPIOA            //����3
	#define KEYNUM3_PIN            GPIO_Pin_15
	#define KEYNUM4_PORT           GPIOB           //����4
	#define KEYNUM4_PIN            GPIO_Pin_3	
	#define KEYNUM5_PORT           GPIOB           //����5
	#define KEYNUM5_PIN            GPIO_Pin_1  	
	#define KEYNUM6_PORT           GPIOB            //����6
	#define KEYNUM6_PIN            GPIO_Pin_2      
	#define KEYNUM7_PORT           GPIOA            //����7
	#define KEYNUM7_PIN            GPIO_Pin_11 
	#define KEYNUM8_PORT           GPIOA            //����8
	#define KEYNUM8_PIN            GPIO_Pin_12
	#define KEYNUM9_PORT           GPIOB             //����9
	#define KEYNUM9_PIN            GPIO_Pin_10
	#define KEYNUM0_PORT           GPIOB              //����0
	#define KEYNUM0_PIN            GPIO_Pin_11
	
	#define KEYMODE_PORT           GPIOB              //ģʽ����
	#define KEYMODE_PIN            GPIO_Pin_12
	#define KEYALARM_PORT           GPIOB              //��������
	#define KEYALARM_PIN            GPIO_Pin_13
	#define KEYSHOCK_PORT           GPIOB              //�������
	#define KEYSHOCK_PIN            GPIO_Pin_14
	#define KEYDEL_PORT           GPIOA                //�˸񰴼�
	#define KEYDEL_PIN            GPIO_Pin_8
	#define KEYCHECK_PORT					GPIOB							//��ѯ�������
	#define KEYCHECK_PIN					GPIO_Pin_15
	
	
	//ָʾ��
	#define LED_PORT              GPIOB
	#define LED_PIN               GPIO_Pin_7
	
	
	//ʹ�ܵ�Դ
	#define POW_EN_PORT              GPIOB
	#define POW_EN_PIN               GPIO_Pin_6
	
	//ʹ������ģ��
	#define YL_EN_PORT           GPIOB                //SETA
	#define YL_EN_PIN            GPIO_Pin_5 
	#define YL_SET_PORT           GPIOB                //SETA
	#define YL_SET_PIN            GPIO_Pin_4 

	

	
	//</���� ���Ŷ���>
	

	
//</���Ŷ˿ڶ���>
	
//<AD ��ض���>
	/* AD˳����--------------------------------------------------------------------*/

	#define	NUM_POWER			   	0										//	��Դ��ѹ




	//</AD ��ض���>

#endif
	//��ʱ��ʱ��
	#define	TIM2_DELAY_TIME		50
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
		
		private:
		
			u8* OLEDString;                  //��ʾ���ַ���
			SqStack *sqStack;                //��Ű������ݵ�ջ
			
			u8 *sendData;
			u8 *receiveData;
			u8 *showData;
		


			Timer		         *t2,*t3,*t4;								//	��ʱ��2,��ʱ��3,��ʱ��4			
			SerialPort       *zigbeePort;
			TerminalControl  *zigbeeControl;
		//	LCDdisplay *LCDCtrl;						            //����Һ����
		//	SpiPort *lcdPort;									          //��LCDҺ����ͨ�ŵ�SPI��

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
			Key   *keyCheck;
		
			//IO
			IoOut   *Led;
			IoOut   *Pow_EN,*YL_Set,*YL_EN;
			IoOut   *zigbeeReset;
			
			u8 controlData;
			u8 controlModeFlag;  //��ʶ����ģʽ
			u8 controlStatusFlag;  //��ʶ����״̬
			u8 isFreeMode;
			u8 checkPowerMode;				//��ѯ�������
			
			uint16_t	RoomID;						//	���յ��ķ���ID
			uint16_t	TerminalID;					//	���յ����ն�ID
			int Data;
			
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
			bool    returnKeyEvent();               //  �����Ƿ��а���
			bool    returnKeyValue();             

			void		dealKeyValue(void);							//	��������������

			void 		showOtherSymbol();          //��ʾOLED�ַ���
			void    showPowerValue();
			void    showControlMode();          //��ʾ����ģʽ  �ӽ��Ͷ���
			void    showControlStatus();        //��ʾ����״̬  ����;���
			void    disPage_start();
			void    showTitleName();

			void    showNoPower();
			
			void    updataToZigbee();         //���·�������
		
		  void		receiveMainData(void);     //���ձ�������

			void    SYSCLKconfig_STOP(void);   //��ͣ��״̬���Ѻ���Ҫ����ʱ��
			void		enable_SWD_disable_Jtag(void);				//	ʹ��SWD���ر�JTAG
			void		testWithUsart(void);						//	ͨ�����ڲ�������
			
			
			void 		itoa(int, u8*);
	};
	
#endif 
/*--------------------------------- End of Process.h -----------------------------*/
