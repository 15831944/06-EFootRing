/**
  ******************************************************************************
	*�ļ���Process.cpp
	*���ߣ�������
	*�汾��2.0
	*���ڣ�2017-01-09
	*��Ҫ��
	*��ע��
	*
  ******************************************************************************  
	*/ 

/* ͷ�ļ����� --------------------------------------------------------------------*/
#include "Process.h"
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdlib.h>
#ifdef __cplusplus
 }
#endif

#define TIM2_PERIOD_MS	100
#define TIM3_PERIOD_MS	30
#define TIM4_PERIOD_MS	100
/* ���ʵ��---------------------------------------------------------------------*/

 Process::Process()
{
	enable_SWD_disable_Jtag();								//	�ر�JTAG��SWD	
	
	initIOs();												//	��ʼ��IO
	initADs();												//	��ʼ��ADͨ��
	initUsarts();											//	��ʼ������
	initTimers();											//	��ʼ����ʱ��
	
	initAllData();											//  ��ʼ����������

	  
	YL_EN_Ctrl->setOn();  //YLģ��ȫ�ٹ���
	
	
	//<���Ѻ����Ͻ������ݣ�������>
	runOnTime3();
	SysTick_DelayMs(5);   //�ȴ�YLģ�黽��
	runOnTime3();
	
	//</���Ѻ����Ͻ������ݣ�������>
	//YL_SET_Ctrl->setOn();

	//beepCtrl->setOn();
	
}

void Process::openPeriph(void)
{
	SysTick_DelayMs(10);									//  ��ʱ 10ms
	zigbeePort->open();    										//  ����ͨ�Ŷ˿�1
	
		//  �յ����ݺ��� ������ʱ��2
	//��Ϊû�ӵ�����ʱ
//	t2->start();											//  ������ʱ��2
	t3->start();											//  ������ʱ��3
	t4->start();											//  ������ʱ��4

}

/**
  * @brief  TIM2��ʱ���жϺ���������Զ���뱨��
  * @param  None
  * @retval None
	* @��ʱʱ�䣺100ms
  */
void Process::runOnTime2(void)
{
#ifdef USE_DISTANCE_ALARM
	//static int beepOnTime=0;
	//�����ж�һ������ͨѶ״̬�£������жϣ�����ֻ���ڽӵ�����ʱ���Ŵ򿪶�ʱ��2
	//Զ����
	if(distance==1)
	{
		if(!isFreeMode)
		{
			beepCtrl->setOn();
			redLed->setOn();
		}
		
//watchDogTimer=3;   //Զ���룬һֱι��
		
// 		beepOnTime++;
// 		
// 		//50S�󣬲��ٽ���Զ���⣬Ҳ�ͻ�������ߡ�
// 		if(beepOnTime>500)    
// 		{
// 			distance=0;
// 		}
		
		
	}
	//������
	else if(controlStatus!=1 && !isDecCut)   //û���յ���������,û����
	{
		//beepOnTime=0;
		beepCtrl->setOff();
	}
#endif
}

/**
  * @brief  TIM3��ʱ���жϺ���,���մ�������
  * @param  None
  * @retval None
	* @��ʱʱ�䣺30ms
  */
#define	OVER_TIME	30
void Process::runOnTime3(void)
{
	static uint16_t	overTimerCount=0;
	// ���մ�������
	if(zigbeeControl->reciveData())							
	{
	//  �յ����ݺ��� ������ʱ��2
	//��Ϊû�ӵ�����ʱ
		t2->start();											
		
		overTimerCount=0;									//	��ʱ������λ
		watchDogTimer=3;                  //�յ����ݣ���ι��

		receiveMainData();							 //	��������
		disposal();     								 //��������
		
		distance=0;  //�յ����ݺ����þ���Ϊ0

	}
	//���ճ�ʱ
	else
	{
		
		
		overTimerCount++;									//	û���յ����ݵ����ʱ
		if(overTimerCount>=OVER_TIME*4)   //����ʱ��û���յ����ݺ�ֹͣ����
		{
			overTimerCount=0;
			beepCtrl->setOff();
			
		}
		//���� û���յ����ݺ󣬿��Ƶ��ʱ��
		else if(overTimerCount>=OVER_TIME)
		{		
			overTimerCount=0;
			shockCtrl->setOff();   //ֹͣ���				
			
			resetControlData();   //û�յ����ݺ���Ҫ��λ����ָ��

		}
		//���� ����������������ʱ�����
		else if(overTimerCount>=OVER_TIME/5)
		{
			shockTime=0;
		}
		
		//�����������Χ
		if(overTimerCount>20)
		{
			distance=1;
		}
	}

}

/**
  * @brief  TIM4��ʱ���жϺ���,��ʱ���ߣ�������߱�������ʾ��ѹֵ
  * @param  None
  * @retval None
	* @��ʱʱ�䣺100ms
  */
void Process::runOnTime4(void)
{
	
	getPowerValue();
	showPowerLed();
	
	detectDecCut();
	
	
#ifdef USE_SLEEP
	watchDogTimer++;
	if(watchDogTimer>50)
	{
		watchDogTimer=0;
//add you code ����֮ǰִ��һЩ����
		SysTick_DelayMs(50);
		PWR_EnterSTANDBYMode();
//	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);   //����˯��
		
	}
	else if(watchDogTimer<2)
	{
		//���Ѻ�ĳ�ʼ��
	}
	
#endif
}



/**
  * @brief  ��ʼ������
  * @param  None
  * @retval None
  */
void Process::initIOs(void)
{

	greenLed		=	new IoOut(PORT_LED_GREEN,PIN_LED_GREEN);
	redLed			=	new	IoOut(PORT_LED_RED,PIN_LED_RED);
	
	beepCtrl		=	new	IoOut(PORT_BEEP,PIN_BEEP,true);
	shockPowCtrl= new IoOut(PORT_SHOCK_POW,PIN_SHOCK_POW);
	shockCtrl		=	new IoOut(PORT_SHOCK,PIN_SHOCK);
	
	
	YL_EN_Ctrl  = new IoOut(PORT_YL_EN,PIN_YL_EN,true);
//	YL_SET_Ctrl = new IoOut(PORT_YL_SET,PIN_YL_SET,true);
	
	


}

/**
  * @brief  ��ʼ��AD
  * @param  None
  * @retval None
  */
void Process::initADs(void)
{
	ADC1_DMA_Init();
}

/**
  * @brief  ��ʼ����ʱ��
  * @param  None
  * @retval None
  */
void Process::initTimers(void)
{
	t2=new Timer (TIM2);
	t2->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM2_SubPriority);
	t2->setTimeOut(TIM2_PERIOD_MS);

	t3=new Timer (TIM3);
	t3->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM3_SubPriority);
	t3->setTimeOut(TIM3_PERIOD_MS);

	t4=new Timer (TIM4);
	t4->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM4_SubPriority);
	t4->setTimeOut(TIM4_PERIOD_MS);
}

/**
  * @brief  ��ʼ��usart
  * @param  None
  * @retval None
  */
void Process::initUsarts(void)
{
	zigbeePort = new SerialPort(1,9600);
	zigbeeControl = new TerminalControl(zigbeePort);
	


}

/**
  * @brief  ��ʼ��ȫ������
  * @param  None
  * @retval None
  */
void Process::initAllData(void)
{
	powerVoltage	=	0;
	decCutADValue =0;
	isDecCut=false;
	
	controlMode 		=	MODE_APPROACH;					//	Ĭ��Ϊ�ӽ�ģʽ
	controlStatus 	=	STATE_DEFAULT;					//	Ĭ��Ϊ�ް�������
	isFreeMode      =1;                       //Ĭ��Ϊ����ģʽ
	
	RoomID		=	ID_ROOM_INVALID;				//	Ĭ��IDΪ��Ч
	TerminalID	=	ID_TERMINAL_INVALID;			//	Ĭ��IDΪ��Ч
	
	watchDogTimer=0;
	shockTime =0;
	
	distance =0;
	
	
	

}


/**
  * @brief  ������������
  * @param  None
  * @retval None
  */
void Process::receiveMainData(void)
{
	uint8_t* dataFromMain;								//	������������ص�����
	uint16_t roomID1,roomID2,roomID3;
	uint16_t	terminalID1,terminalID2;
	
	dataFromMain = zigbeeControl->getReciveData();			//��ȡ�����յ�������
	
	//<just for test 2017 01 11 > 
// 	if(dataFromMain[2]==0x80)
// 	{
// 		greenLed->setOn();
// 		redLed->setOff();
// 			
// 	}
	//</just for test>
	
	
	controlMode		=	dataFromMain[3]&0x03;				//	��ȡ����ģʽ
	controlStatus	=	(dataFromMain[3]>>2)&0x03;			//	��ȡ����״̬
	isFreeMode    = (dataFromMain[3]>>4)&0x01;			//	��ȡ����״̬
	
	
	roomID1		=	dataFromMain[4];					//	��ȡ��������λ
	roomID2		=	dataFromMain[5];					//	��ȡ������м�λ
	roomID3		=	dataFromMain[6];					//	��ȡ��������λ
	terminalID1	=	dataFromMain[7];					//	��ȡ��������λ
	terminalID2	=	dataFromMain[8];					//	��ȡ��������λ
	
	RoomID 		  =	roomID1*100 + roomID2*10 + roomID3;	//	������յ��ķ����
	TerminalID	=	terminalID1*10 + terminalID2;		//	������յ��ı����
	

	

	
}



/**
  * @brief  ����������ݣ�������ִ����
  * @param  None
  * @retval None
  */
// #define	BEEP_10SEC	500	//	������3s
// #define	SHOCK_3s	500	//	�����2s
void Process::disposal()
{
	static	uint8_t lastMode=MODE_APPROACH;
	static  uint8_t preControlStatus=0;

	static uint16_t operateAlarmCount=0;

	
	
	if(lastMode!=controlMode)	//�л��˿���ģʽ
	{
		lastMode=controlMode;		
		resetTerminal();
		
		operateAlarmCount=0; //����������
			
	}
	//û���л�����ģʽ
	else				
	{
		//<�ӽ�ģʽ>
		if(MODE_APPROACH==controlMode)	//	�ӽ�ģʽ
		{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////���´��븴��3��///////////////////////////////////////////////
		//һ�ΰ���ֻ�������Σ�����һ�Σ�����һ��
		//ȡ���������β�����ԭ���ɿ��������ղ������ݣ����ԾͲ��ᴦ�����ݡ�
		//	if(preControlStatus!=controlStatus)
			{
				preControlStatus=controlStatus;
				if(controlStatus==STATE_ALARM)  //����
				{
					beepCtrl->setOn();
					redLed->setOn();
						
				}
				else if(controlStatus==STATE_SHOCK)//���
				{
				//		shockTime=0;    //���µ������ʼ��ʱ���ʱ��
								//��ʱ�رյ�������ó��ڵ��
					shockTime++;
					if(shockTime>6)
					{
						//shockTime����resetControlData�︴λ
						shockPowCtrl->setOff();
						shockCtrl->setOff();				
					}
					else 
					{
						
						shockPowCtrl->setOn();
						shockCtrl->setOn();
						redLed->setOn();
					}

				}
				//���û���ߣ�û������Ұ��Χ��û�п��ƶ����ź�
				else  
				{
					if(!isDecCut && (distance!=0) ) 
					{
						beepCtrl->setOff();
						redLed->setOff();
					}
						
						shockPowCtrl->setOff();
						shockCtrl->setOff();
						
				}
			}
//////////////////////////////////////////////////////////////////////////////////////////////////////		
			
		//</�ӽ�ģʽ>
		}
/*
		//<����ģʽ>
		if(MODE_POINT==controlMode)	//	�ӽ�ģʽ
		{
			//�����Ϊ��
			if(TerminalID==ID_TERMINAL_INVALID)    
			{
				if(RoomID==ID_ROOM)
				{
					if(controlStatus==STATE_ALARM)  //����
					{
						operateAlarmCount++;
					 if(operateAlarmCount<=BEEP_10SEC/TIM3_PERIOD_MS)
					 {
							beepCtrl->setOn();

					 }
					 else
					 {
							beepCtrl->setOff();
					
					 }
							
					}
					else if(controlStatus==STATE_SHOCK)//���
					{
						
						operateShockCount++;
						if(operateShockCount<=SHOCK_3s/TIM3_PERIOD_MS)
						{
							shockCtrl->setOn();
					
						}
						else
						{
							shockCtrl->setOff();
						
						}	
					}
					else  //Ĭ��ģʽ����������
					{
						operateAlarmCount=0; //����������
						operateShockCount=0;	
						//�ر�ִ����
						beepCtrl->setOff();
						shockCtrl->setOff();	
		
						
					}
				}
			}
			//����Ų�Ϊ��
			else if(TerminalID==ID_TERMINAL && RoomID==ID_ROOM)  //����źͷ���Ŷ���Ӧʱ
			{
				if(controlStatus==STATE_ALARM)  //����
				{
					operateAlarmCount++;
				 if(operateAlarmCount<=BEEP_10SEC/TIM3_PERIOD_MS)
				 {
						beepCtrl->setOn();
				
				 }
				 else
				 {
						beepCtrl->setOff();
					
				 }
						
				}
				else if(controlStatus==STATE_SHOCK)//���
				{
					
					operateShockCount++;
					if(operateShockCount<=SHOCK_3s/TIM3_PERIOD_MS)
					{
						shockCtrl->setOn();
						
					}
					else
					{
						shockCtrl->setOff();
						
					}	
				}
				else  //Ĭ��ģʽ����������
				{
					operateAlarmCount=0; //����������
					operateShockCount=0;	
					//�ر�ִ����
					beepCtrl->setOff();
					shockCtrl->setOff();	

					
				}				

			}
			
		}
			
*/
		//</����ģʽ>
	

	}
	
}

void Process::resetTerminal(void)
{
	beepCtrl->setOff();
	shockCtrl->setOff();
}



/**
  * @brief  �����Դ��ѹ���Ŵ�10��
  * @param  None
  * @retval None
  */
void Process::getPowerValue(void)
{
	
	if(controlStatus!=STATE_SHOCK)
	{
		powerVoltage  = AD_Filter[AD_NUM_POWER]*33*3>>12;
	}
	

}


/**
  * @brief  ��ʾ����״̬
  * @param  None
  * @retval None
  */
void Process::showPowerLed()
{
	if(powerVoltage>=37)
	{
		//redLed->setOff();
		greenLed->setOn();
	}
	else
	{
	//	redLed->setOn();
		greenLed->setOff();
	}
}

void Process::detectDecCut()
{
	//�����ʱ�򲻼����߱���
	if(controlStatus!=STATE_SHOCK)
	{
		decCutADValue  = AD_Filter[AD_NUM_DECCUT]*33>>12;//
	}
	
	
	//�߶ϱ���
	if(decCutADValue>15)
	{
		isDecCut=true;
		beepCtrl->setOn();
		redLed->setOn();
		
		watchDogTimer=3;   //�����ˣ�һֱι��
	}
	else if(controlStatus!=1)
	{
		isDecCut=false;
		beepCtrl->setOff();
		redLed->setOff();
	}
	
}

/**
  * @brief  ���û�ȫ������
  * @param  None
  * @retval None
  */
void Process::resetControlData(void)
{
	controlStatus=0;
	
}

/**
  * @brief  ʹ��SWD���ر�JTAG
  * @param  None
  * @retval None
  */
void Process::enable_SWD_disable_Jtag(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//	ʹ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//	�ر�JTAG��������ΪGPIO��
}
/*--------------------------------- End of Process.cpp -----------------------------*/






