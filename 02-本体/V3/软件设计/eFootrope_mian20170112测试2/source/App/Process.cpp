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

#define TIM2_PERIOD_MS	10
#define TIM3_PERIOD_MS	10
#define TIM4_PERIOD_MS	100
/* ���ʵ��---------------------------------------------------------------------*/

 Process::Process()
{
	enable_SWD_disable_Jtag();								//	�ر�JTAG��SWD	
	
	initIOs();												//	��ʼ��IO
	initADs();												//	��ʼ��ADͨ��
	initUsarts();											//	��ʼ������
	initTimers();											//	��ʼ����ʱ��
	
	//SysTick_DelayMs(1000);	

//	shockCtr->setOn();
	//beepCtr->setOn();
	initAllData();											//  ��ʼ����������
//	SysTick_DelayMs(100);
//	shockCtr->setOff();

	//��ʼ��һЩ����
	greenLed->setOff();
	redLed->setOn();


	//
	  
	YL_EN_Ctrl->setOn();  //YLģ��ȫ�ٹ���
	
	
	//<���Ѻ����Ͻ������ݣ�������>
	runOnTime3();
	SysTick_DelayMs(5);   //�ȴ�YLģ�黽��
	runOnTime3();
	
	//</���Ѻ����Ͻ������ݣ�������>
	//YL_SET_Ctrl->setOn();

	
}

void Process::openPeriph(void)
{
	SysTick_DelayMs(10);									//  ��ʱ 10ms
	zigbeePort->open();    										//  ����ͨ�Ŷ˿�1
	
	t2->start();											//  ������ʱ��2
	t3->start();											//  ������ʱ��3
	t4->start();											//  ������ʱ��4

}

/**
  * @brief  TIM2��ʱ���жϺ���������
  * @param  None
  * @retval None
	* @��ʱʱ�䣺10ms
  */
void Process::runOnTime2(void)
{
	//getPowerValue();

}

/**
  * @brief  TIM3��ʱ���жϺ���,���µ���ֵ���Դ��ѹ
  * @param  None
  * @retval None
	* @��ʱʱ�䣺10ms
  */
#define	OVER_TIME	1000
void Process::runOnTime3(void)
{
	static uint16_t	overTimerCount=0;
	// ���մ�������
	if(zigbeeControl->reciveData())							
	{

		
		overTimerCount=0;									//	��ʱ������λ
		watchDogTimer=3;                  //�յ����ݣ���ι��

		receiveMainData();										//	��������
		disposal();      //��������

	}
	//���ճ�ʱ
	else
	{
		overTimerCount++;									//	��ʱ����
		if(overTimerCount>=OVER_TIME)
		{
			overTimerCount=0;
			resetTerminal();    //û���յ����ݣ���λ��

		}
	}

}

/**
  * @brief  TIM4��ʱ���жϺ���,��ʱ����
  * @param  None
  * @retval None
	* @��ʱʱ�䣺100ms
  */
void Process::runOnTime4(void)
{
	
	getPowerValue();
	//showPowerStatus();
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
	if(dataFromMain[2]==0x80)
	{
		greenLed->setOn();
		redLed->setOff();
		

		
	}
	//</just for test>
	
	
	controlMode		=	dataFromMain[3]&0x03;				//	��ȡ����ģʽ
	controlStatus	=	(dataFromMain[3]>>2)&0x03;			//	��ȡ����״̬
	
	roomID1		=	dataFromMain[4];					//	��ȡ��������λ
	roomID2		=	dataFromMain[5];					//	��ȡ������м�λ
	roomID3		=	dataFromMain[6];					//	��ȡ��������λ
	terminalID1	=	dataFromMain[7];					//	��ȡ��������λ
	terminalID2	=	dataFromMain[8];					//	��ȡ��������λ
	
	RoomID 		=	roomID1*100 + roomID2*10 + roomID3;	//	������յ��ķ����
	TerminalID	=	terminalID1*10 + terminalID2;		//	������յ��ı����
	

	

	
}



/**
  * @brief  ����������ݣ�������ִ����
  * @param  None
  * @retval None
  */
#define	BEEP_10SEC	500	//	������3s
#define	SHOCK_3s	500	//	�����2s
void Process::disposal()
{
	static	uint8_t lastMode=MODE_APPROACH;

	//static	uint8_t	preKeyState=KEY_DEFAULT;
	static uint16_t operateAlarmCount=0;
	static uint16_t operateShockCount=0;
	
	
	if(lastMode!=controlMode)	//�л��˿���ģʽ
	{
		lastMode=controlMode;		
		resetTerminal();
		
		operateAlarmCount=0; //����������
		operateShockCount=0;
		
		
	}
	//û���л�����ģʽ
	else				
	{
		//<�ӽ�ģʽ>
		if(MODE_APPROACH==controlMode)	//	�ӽ�ģʽ
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
					shockPowCtrl->setOn();
					shockCtrl->setOn();
					
				}
				else
				{
					shockPowCtrl->setOff();
					shockCtrl->setOff();
					
				}	
			}
			else  //Ĭ��ģʽ����������
			{
				operateAlarmCount=0; //����������
				operateShockCount=0;	
				//�ر�ִ����
				beepCtrl->setOff();
				
				shockPowCtrl->setOff();
				shockCtrl->setOff();	
					
			}
		//</�ӽ�ģʽ>
		}
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
	uint32_t tempValue=0;

	tempValue  = AD_Filter[NUM_POWER]*33*6>>12;
//	powerValue.u16_data = (uint16_t)tempValue;
	powerVoltage=(uint8_t)tempValue;
}

/**
  * @brief  ��ʾ����״̬
  * @param  None
  * @retval None
  */
void Process::showPowerStatus()
{
	if(powerVoltage>72)
	{
		redLed->setOff();
		greenLed->setOn();
	}
	else
	{
		redLed->setOn();
		greenLed->setOff();
	}
}


/**
  * @brief  ��ʼ������
  * @param  None
  * @retval None
  */
void Process::initIOs(void)
{

	greenLed		=	new IoOut(PORT_LED_GREEN,PIN_LED_GREEN,true);
	redLed			=	new	IoOut(PORT_LED_RED,PIN_LED_RED,true);
	
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
	//ADC1_DMA_Init();
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
	controlMode 		=	MODE_APPROACH;					//	Ĭ��Ϊ�ӽ�ģʽ
	controlStatus 	=	STATE_DEFAULT;					//	Ĭ��Ϊ�ް�������
	RoomID		=	ID_ROOM_INVALID;				//	Ĭ��IDΪ��Ч
	TerminalID	=	ID_TERMINAL_INVALID;			//	Ĭ��IDΪ��Ч
	
	watchDogTimer=0;
	

}

/**
  * @brief  ���û�ȫ������
  * @param  None
  * @retval None
  */
void Process::resetControlData(void)
{
	
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






