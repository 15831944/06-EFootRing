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
	//#include "Protocol.h"
	#include "TerminalControl.h"
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
//#define TEST
/* ���Ŷ���--------------------------------------------------------------------*/


#define	USE_PERIPH
#ifdef	USE_PERIPH

	
	//<���� ���Ŷ���>



//��������
//YL_EN���Ŷ���

// 	#define YL_EN_PORT                GPIOA
// 	#define YL_EN_PIN                 GPIO_Pin_12
// 	#define YL_SET_PORT                GPIOA
// 	#define YL_SET_PIN                 GPIO_Pin_11	
// 	
// 	#define POW_EN_PORT                GPIOA
// 	#define POW_EN_PIN                 GPIO_Pin_1

//LED����
	#define LED_POW1_PORT              GPIOA
	#define LED_POW1_PIN               GPIO_Pin_0	
	
	#define LED_POW2_PORT              GPIOA
	#define LED_POW2_PIN               GPIO_Pin_7


	
	//</���� ���Ŷ���>
	

	
//</���Ŷ˿ڶ���>
	
//<AD ��ض���>
	/* AD˳����--------------------------------------------------------------------*/


	#define	AD_NUM_KEY1		   	0									//	����1
	#define	AD_NUM_KEY2		   	1									//	����2


//</AD ��ض���>

#endif
	//��ʱ��ʱ��
	#define	TIM2_DELAY_TIME		10
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
		

			
			u8 *sendData;
			u8 *receiveData;

		


			Timer		         *t2,*t3,*t4;								//	��ʱ��2,��ʱ��3,��ʱ��4			
			SerialPort       *zigbeePort;
			TerminalControl  *zigbeeControl;

			//<����ʵ������>


		
		
			//IO
			IoOut   *ledPow1,*ledPow2;
// 			IoOut   *powEn;
// 			IoOut   *YL_En;
// 			IoOut   *YL_Set;


			u8 controlModeFlag;    //��ʶ����ģʽ
			u8 controlStatusFlag;  //��ʶ����״̬
			u8 isFreeMode;        
			
			u8 powerValue;   //��ѹֵ
			//AD����ֵ
			u8 ADKey1Value;
			u8 ADKey2Value;
		//	u8 ADKey3Value;
			
			bool isShocking;       //���ڵ��
			bool isAlarming;        //���ھ���
			
			int watchDogTimer;
					
	
			
private:
			void	    initAllData(void);							//	ȫ�����ݳ�ʼ����������־λ
			void			initGPIO(void);							  	//	��ʼ��IO
			void	    initADs(void);								//	��ʼ��ADͨ��
			void	    initUsarts(void);							//	��ʼ������
			void	    initTimers(void);							//	��ʼ����ʱ��		

			
			void    getPowerValue();
			void	  getKeyValue(void);							//	��ȡ����ֵ
			void    getADKeyValue();                //	��ȡAD����ֵ

			void		dealKeyValue(void);							//	��������������
			void    dealADKeyValue();


			void    updataToZigbee();         //���·�������
			void    sendCheckSignal();        //���ͼ���źţ����ڷ���FF FF AA
		
			

			void		enable_SWD_disable_Jtag(void);				//	ʹ��SWD���ر�JTAG
			void		testWithUsart(void);						//	ͨ�����ڲ�������
	};
	
#endif 
/*--------------------------------- End of Process.h -----------------------------*/
