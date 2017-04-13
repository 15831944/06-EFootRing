/**
  ******************************************************************************
	*�ļ���Process.cpp
	*���ߣ��Ϻ�
	*�汾��2.0
	*���ڣ�2015-03
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

#define TIM2_PERIOD_MS	5
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
	
	SysTick_DelayMs(1000);	
	testLed->setOn();
//	shockCtr->setOn();
	//beepCtr->setOn();
	initAllData();											//  ��ʼ����������
	SysTick_DelayMs(100);
//	shockCtr->setOff();
	testLed->setOff();
	beepCtr->setOff();
}

void Process::openPeriph(void)
{
	SysTick_DelayMs(10);									//  ��ʱ 10ms
	com1st->open();    										//  ����ͨ�Ŷ˿�1
	
	t2->start();											//  ������ʱ��2
	t3->start();											//  ������ʱ��3
	t4->start();											//  ������ʱ��4

}

/**
  * @brief  TIM2��ʱ���жϺ���������
  * @param  None
  * @retval None
  */
void Process::runOnTime2(void)
{
	getPowerValue();
	getKeyValue();
}

/**
  * @brief  TIM3��ʱ���жϺ���,���µ���ֵ���Դ��ѹ
  * @param  None
  * @retval None
  */
#define	OVER_TIME	1000
void Process::runOnTime3(void)
{
	static uint16_t	overTimerCount=0;
	if(protocol->com1st_Receive())							//  ���մ�������
	{
		overTimerCount=0;									//	��ʱ������λ
		testLed->reverseState();
		dealMainData();										//	��������
//		protocol->com1st_Send();							//	com1st��������
	}
	else
	{
		overTimerCount++;									//	��ʱ����
		if(overTimerCount>=OVER_TIME)
		{
	//		beepCtr->setOn();								//	����������
			resetTerminal();
			overTimerCount=0;
		}
	}

}

/**
  * @brief  TIM4��ʱ���жϺ���
  * @param  None
  * @retval None
  */
void Process::runOnTime4(void)
{

}


/**
  * @brief  ������������
  * @param  None
  * @retval None
  */
void Process::dealMainData(void)
{
	uint8_t* dataFromMain;								//	������������ص�����
	uint16_t roomID1,roomID2,roomID3;
	uint8_t	terminalID1,terminalID2;
	
	dataFromMain = protocol->com1st_GetData();			//	��ȡ��������
	
	revMode		=	dataFromMain[3]&0x03;				//	��ȡģʽ
	revKeyState	=	(dataFromMain[3]>>2)&0x03;			//	��ȡ����״̬
	roomID1		=	dataFromMain[4];					//	��ȡ��������λ
	roomID2		=	dataFromMain[5];					//	��ȡ������м�λ
	roomID3		=	dataFromMain[6];					//	��ȡ��������λ
	terminalID1	=	dataFromMain[7];					//	��ȡ��������λ
	terminalID2	=	dataFromMain[8];					//	��ȡ��������λ
	
	revRoomID 		=	roomID1*100 + roomID2*10 + roomID3;	//	������յ��ķ����
	revTerminalID	=	terminalID1*10 + terminalID2;		//	������յ��ı����
	
	disposal(revMode);
	

	
}

#define	KEY_PRESS_LONG_TIME	100
void Process::disposal(uint8_t mode)
{
	static	uint8_t lastMode=MODE_APPROACH;
	static	uint16_t keyCount=0;
	static	uint8_t	preKeyState=KEY_DEFAULT;
	
	
	if(lastMode!=mode)	//	ǰ������ģʽ��һ����λ
	{
		keyState=KEY_DEFAULT;
		resetTerminal();
		lastMode=mode;
	}
	else				//	ǰ������ģʽһ�£�ִ�в���
	{
		//<�ӽ�ģʽ>
		if(MODE_APPROACH==mode)	//	�ӽ�ģʽ
		{
		//	if(preKeyState!=revKeyState) //���δ���
		//	{
				if(revKeyState==STATE_ALARM)
				{
					keyState = KEY_ALARM_SHOT;
					
				}
				else if(revKeyState==STATE_SHOCK)
				{
					keyState = KEY_SHOCK_SHOT;
				}
				else
				{
					keyState = KEY_DEFAULT;
				}
				preKeyState=revKeyState;
				keyCount=0;
		//	}
			/*
			else	//	if(preKeyState==revKeyState)   
			{
				keyCount++;      //���keycountû��������������һֱִ�У�
				if(keyCount>=KEY_PRESS_LONG_TIME)
				{
					if(preKeyState==STATE_ALARM)
					{
						keyState = KEY_ALARM_LONG;
					}
					else if(preKeyState==STATE_SHOCK)
					{
						keyState = KEY_SHOCK_LONG;
					}
				}
			}
			*/
			operate();
		}
		//</�ӽ�ģʽ>
		//<����ģʽ>
		else if(MODE_POINT==mode)	//	����ģʽ
		{
			if(ID_ROOM==revRoomID&&ID_TERMINAL==revTerminalID)	//	IDһ��
			{
				if(preKeyState!=revKeyState)
				{
					if(preKeyState==STATE_ALARM)
					{
						keyState = KEY_ALARM_SHOT;
						
					}
					else if(preKeyState==STATE_SHOCK)
					{
						keyState = KEY_SHOCK_SHOT;
					}
					preKeyState=revKeyState;
					keyCount=0;
				}
				/*
				else	//	if(preKeyState==revKeyState)
				{
					keyCount++;
					if(keyCount>=KEY_PRESS_LONG_TIME)
					{
						if(preKeyState==STATE_ALARM)
						{
							keyState = KEY_ALARM_LONG;
						}
						else if(preKeyState==STATE_SHOCK)
						{
							keyState = KEY_SHOCK_LONG;
						}
					}
				}
				*/
				operate();
			}
			else
			{}
		}
		//</����ģʽ>
		else
		{}
		
	}
}

void Process::resetTerminal(void)
{
	beepCtr->setOff();
	shockCtr->setOff();
}

#define	BEEP_10SEC	3000	//	������10s
#define	SHOCK_3s	3000	//	�����3s
void Process::operate( void)
{
	static uint16_t operateAlarmCount=0;
	static uint16_t operateShockCount=0;
	static uint16_t operateShockLongCount=0;
	static bool	operateAlarmOnce=false;
	static bool operateShockOnce=false;
	
	//����
	if(keyState==KEY_ALARM_SHOT)
	{		


			operateAlarmCount++;
			if(operateAlarmCount<=BEEP_10SEC/TIM3_PERIOD_MS)
			{
				beepCtr->setOn();
			}
			else
			{
				beepCtr->setOff();
			}
			
	}
	//���
	else if(keyState==KEY_SHOCK_SHOT)
	{

			operateShockCount++;
			if(operateShockCount<=SHOCK_3s/TIM3_PERIOD_MS)
			{
				shockCtr->setOn();
			}
			else
			{
				shockCtr->setOff();
			}
	}
	else
	{
		beepCtr->setOff();
		shockCtr->setOff();
		operateShockCount=0;
	}
	/*
	else if(keyState==KEY_ALARM_LONG)
	{
		operateAlarmOnce=false;
		operateShockOnce=false;
		operateShockLongCount=0;
		operateAlarmCount=0;
		operateShockCount=0;
		beepCtr->setOn();
		shockCtr->setOff();
	}
	*/
	
	//���
	/*
	else if(keyState==KEY_SHOCK_SHOT)
	{
		operateAlarmOnce=false;
		operateShockLongCount=0;
		operateAlarmCount=0;
		operateShockCount++;
		beepCtr->setOff();
		if(!operateShockOnce)
		{
			if(operateShockCount<=SHOCK_3s/TIM3_PERIOD_MS)
			{
				shockCtr->setOn();
				operateShockOnce=false;
			}
			else
			{
				operateShockCount=0;
				operateShockOnce=true;
			}
		}
		
	}
	else if(keyState==KEY_SHOCK_LONG)
	{
		operateAlarmOnce=false;
		operateShockOnce=false;
		operateAlarmCount=0;
		operateShockCount=0;
		beepCtr->setOff();
		if(operateShockLongCount<SHOCK_3s/TIM3_PERIOD_MS)
		{
			shockCtr->setOn();
		}
		else if(operateShockLongCount<(2*SHOCK_3s/TIM3_PERIOD_MS))
		{
			shockCtr->setOff();
		}
		else if(operateShockLongCount>=(2*SHOCK_3s/TIM3_PERIOD_MS))
		{
			operateShockLongCount=0;
		}
		operateShockLongCount++;
	}
	else
	{
	}
	*/
}


/**
  * @brief  �����Դ��ѹ���Ŵ�10��
  * @param  None
  * @retval None
  */
void Process::getPowerValue(void)
{
	uint32_t tempValue=0;

	adValue[NUM_POWER] = AD_Filter[NUM_POWER];
	tempValue  = adValue[NUM_POWER]*33*3>>12;
//	powerValue.u16_data = (uint16_t)tempValue;
	powerVoltage=(uint8_t)tempValue;
}

/**
  * @brief  ����com1s_t��Ҫ���͵�����
  * @param  None
  * @retval None
  */
void Process::setCom1Data(void)
{
//	com1st_DataToSend[0] = currentMode;
//	com1st_DataToSend[1]= powerValue.u8_data[0];
//	com1st_DataToSend[2]= powerValue.u8_data[1];
	

}


void Process::getKeyValue(void)
{
	for(uint8_t index=0;index<NUM_KEY;index++) 					//  ��ȡ���а�����Ϣ
			keys[index]->getValue();							//  ��ȡ������Ϣ
}


/**
  * @brief  ��ʼ������
  * @param  None
  * @retval None
  */
void Process::initIOs(void)
{
	testLed			=	new IoOut(PORT_LED_TEST,PIN_LED_TEST);
	greenLed		=	new IoOut(PORT_LED_GREEN,PIN_LED_GREEN);
	redLed			=	new	IoOut(PORT_LED_RED,PIN_LED_RED);
	
	beepCtr			=	new	IoOut(PORT_BEEP,PIN_BEEP,true);
	shockCtr		=	new IoOut(PORT_SHOCK,PIN_SHOCK,true);
	
	keys[NUM_KEY_DETECT]	= new Key(PORT_KEY_DETECT, PIN_KEY_DETECT);	
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
	com1st = new SerialPort(1,38400);

	protocol = new Protocol(com1st);
}

/**
  * @brief  ��ʼ��ȫ������
  * @param  None
  * @retval None
  */
void Process::initAllData(void)
{
	powerVoltage	=	0;
	revMode 		=	MODE_APPROACH;					//	Ĭ��Ϊ�ӽ�ģʽ
	revKeyState 	=	STATE_DEFAULT;					//	Ĭ��Ϊ�ް�������
	revRoomID		=	ID_ROOM_INVALID;				//	Ĭ��IDΪ��Ч
	revTerminalID	=	ID_TERMINAL_INVALID;			//	Ĭ��IDΪ��Ч
	
	keyState		=	KEY_DEFAULT;
	
	setCom1Data();										//	������Ҫ���͵�����
	protocol->com1st_UpdateSendBuf(com1st_DataToSend);	//	������Ҫ���͵�����
	
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






