/**
  ******************************************************************************
	*�ļ���Process.cpp
	*���ߣ�������
	*�汾��1.0
	*���ڣ�2016-12-17
	*��Ҫ��
	*��ע��
	*
  ******************************************************************************  
	*/ 
/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "Process.h"
#include "bmp.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdlib.h>
#ifdef __cplusplus
 }
#endif
 
/* ���ʵ��--------------------------------------------------------------------*/

 
 
/**
  * @brief  ���캯��
  * @param  None
  * @retval None
  */
 
 
  Process::Process()
{
	enable_SWD_disable_Jtag();											//	�ر�JTAG��SWD	
	initGPIO();															//	��ʼ��ָʾ��
	initADs();															//	��ʼ��ADͨ��
	initUsarts();														//	��ʼ������
	initTimers();														//	��ʼ����ʱ��	
	initAllData();														//	��ʼ����������
	
	//��ʼ������

	YL_En->setOn();													//
	YL_Set->setOn();
	
	powEn->setOn();
	

	
	

	
//<���������ϴ���������>

	getKeyValue();													//	��ȡAD����ֵ
	dealKeyValue();														  //	������



	updataToZigbee();
	
	SysTick_DelayMs(50);
//</���������Ϸ�������>

}
/**
  * @brief  TIM2��ʱ���жϺ���,,���ڽ��պʹ�����
  * @param  None
  * @retval None
	* @��ʱ��10ms
  */
void Process::runOnTime2(void)
{
	getADKeyValue();
	dealADKeyValue();
	
}

/**
  * @brief  TIM3��ʱ���жϺ���,���ڷ��ʹ�������
  * @param  None
  * @retval None
	* @��ʱ��80ms
  */

void Process::runOnTime3(void)
{	
	
	if(controlStatusFlag)
	{
		updataToZigbee();
	}
	
	
  //sendCheckSignal();
	
	//showOtherSymbol();
}

/**
  * @brief  TIM4��ʱ���жϺ���������ˢ����ʾ����״̬��200ms
  * @param  None
  * @retval None
	* @��ʱ��200ms
  */

void Process::runOnTime4(void)
{


	//��ȡ����ֵ
	//	getPowerValue();
	//showPowerSymbol();
	


//����ģʽ�صƣ���ʱ�ص�Դ
	if(isFreeMode)
	{
	//	ledMode->setOff();
		//��ʱ10S�󣬹ضϵ�Դ

		watchDogTimer++;
		if(watchDogTimer>20)
		{
			watchDogTimer=0;
			powEn->setOff();
		}
		else
		{
			powEn->setOn();
		}
	}
	else
	{
		//ledMode->setOn();
	}
	
	
#ifdef USE_SLEEP

		//ģ�⿴�Ź�����
		watchDogTimer++;
		if(watchDogTimer>200)
		{
			watchDogTimer=0;
		
			SysTick_DelayMs(100);
			PWR_EnterSTANDBYMode();  //�������
			
		
		}
		else if(watchDogTimer<2)
		{
			//���Ѻ�������Ҫ�ĳ�ʼ��
		}
		
	
#endif
	
}



/**
  * @brief  ��ʼ��IO
  * @param  None
  * @retval None
  */
void Process::initGPIO()
{	

	//����ʵ����



	ledKey        = new IoOut(LED_KEY_PORT,LED_KEY_PIN);//�͵�ƽ��Ч
	ledPow        = new IoOut(LED_POW_PORT,LED_POW_PIN);
	
	powEn         = new IoOut(POW_EN_PORT,POW_EN_PIN,true);//g�ߵ�ƽ��Ч
	
	YL_En         = new IoOut(YL_EN_PORT,YL_EN_PIN);
	YL_Set        = new IoOut(YL_SET_PORT,YL_SET_PIN);   //�͵�ƽ��Ч
	
	//zigbeeReset = new IoOut(ZIGBEE_RESET_PORT,ZIGBEE_RESET_PIN);
	

	

}


/**
  * @brief  ��ʼ��AD
  * @param  None
  * @retval None
  */
void Process::initADs()
{
	ADC1_DMA_Init();
}

/**
  * @brief  ��ʼ����ʱ��
  * @param  None
  * @retval None
  */
void Process::initTimers()
{
	t2=new Timer (TIM2);
	t2->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM2_SubPriority);
	t2->setTimeOut(TIM2_DELAY_TIME);		// �޸ĺ궨���Ըı���ʱ

	t3=new Timer (TIM3);
	t3->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM3_SubPriority);
	t3->setTimeOut(TIM3_DELAY_TIME);		// �޸ĺ궨���Ըı���ʱ
	
	t4=new Timer (TIM4);
	t4->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM4_SubPriority);
	t4->setTimeOut(TIM4_DELAY_TIME);		// �޸ĺ궨���Ըı���ʱ
}

/**
  * @brief  ��ʼ��usart
  * @param  None
  * @retval None
  */
void Process::initUsarts()
{

	zigbeePort = new SerialPort(1,9600);	
	zigbeePort->open();

	zigbeeControl = new TerminalControl(zigbeePort);
}

/**
  * @brief  ��ʼ��ȫ������
  * @param  None
  * @retval None
  */	
void Process::initAllData(void)
{
	

	
	sendData =new u8[6];
	for(int i=0;i<6;i++)
	{
		sendData[i]=0;
	}

	

	
	controlModeFlag=1;           //Ĭ���ǽӽ�����ģʽ
	controlStatusFlag=0;         //Ĭ���Ǳ���
	
	isFreeMode=1;                //Ĭ��������ģʽ
	
	isShocking=false;
	isAlarming=false;

	watchDogTimer=0;
	
	powerValue=0;
	ADKey1Value=0;
	ADKey2Value=0;
	ADKey3Value=0;

	
}


	 
/**
  * @brief  �����û�����
  * @param  None
  * @retval None
  */
void Process::openPeriph()
{
	SysTick_DelayMs(10);												//	��ʱ 10ms

	t2->start();														//	������ʱ��2
	t3->start();														//	������ʱ��3
	t4->start();														//	������ʱ��4
}

//////////////////////////////////////////////end of Init/////////////////////////////////////////////////////////


/**
  * @brief  �����Դ��ѹ
  * @param  None
  * @retval None
  */
void Process::getPowerValue(void)
{
	uint32_t tempValue=0;

	tempValue  = AD_Filter[AD_NUM_POWER]*33*2>>12;

	powerValue=(uint8_t)tempValue;



}


/**
  * @brief  ��ȡ����ֵ
  * @param  None
  * @retval None
  * @author ������
  */
void Process::getKeyValue()
{	
//û��GPIO����	
	
}

/**
  * @brief  ��ȡAD����ֵ
  * @param  None
  * @retval None
  * @author ������
  */
void Process::getADKeyValue()
{	
	uint32_t tempValue=0;

	ADKey1Value  = AD_Filter[AD_NUM_KEY1]*33*3>>12;
	//ADKey2Value  = AD_Filter[AD_NUM_KEY2]*33>>12;
	ADKey3Value  = AD_Filter[AD_NUM_KEY3]*33*3>>12;

	
}

/**
  * @brief  ����AD����ֵ
  * @param  None
  * @retval None
  * @author ������
  */
void Process::dealADKeyValue()
{	
	//����1 ---������ѯ
	if(ADKey3Value>=37)
	{				
		ledPow->setOn();
		watchDogTimer=3;  //�а�������ʱ��ι��
	}
	else
	{
		ledPow->setOff();
	}

	
	

	//����3 ---���  ���º͵���ʱִ��һ��
// 	static uint8_t preADKey3Value=0;
// 	if(preADKey3Value!=ADKey3Value)
// 	{
// 		preADKey3Value=ADKey2Value;
		if(ADKey1Value>5)
		{
// 			updataToZigbee();  //���º�������������1�Σ�Ȼ��򿪷������ݶ�ʱ��
// 			t3->start();
			controlStatusFlag=2;
		//	ledKey->setOn();
		}
		else	
		{
			controlStatusFlag=0;
		//	ledKey->setOff();
// 			t3->stop();          //���𰴼���رշ������ݶ�ʱ��
		}
// 	}

	//�а�������ʱ��ι��
		if(ADKey1Value>5)
	{				
		watchDogTimer=3;  //�а�������ʱ��ι��
	}
 
	

}

/**
  * @brief  ��ȡ��������
  * @param  None
  * @retval None	
  */
#define	POWER_OFF_COUNT_DOWN			10							//	�ػ���ʱ,5s�Ժ��Զ��ػ�
#define	POWER_OFF_HARD_WARE_COUNT_DOWN	50							//	ǿ�ƹػ���ʱ
void Process::dealKeyValue()
{
//  	static 	bool	preNumdKey[14]		={true};


// 		
// 	//��һ��Num1��
//  	if (preNumdKey[1] != keyNum1->FinalState)					//	
//  	{
//  		preNumdKey[1]=keyNum1->FinalState;
// 		if (keyNum1->FinalState==false)
// 		{
// 			if(!sqStack->IsFull())
// 			{
// 				sqStack->Push('1');			
// 			
// 				OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 			}
// 			Led->reverseState();
// 			
// 		}

//  	}
// 	
// 		//��һ��Num2��
//  	if (preNumdKey[2] != keyNum2->FinalState)					//	
//  	{
//  		preNumdKey[2]=keyNum2->FinalState;
// 		if (keyNum2->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())
// 			{
// 			sqStack->Push('2');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 	
// 			//��һ��Num3��
//  	if (preNumdKey[3] != keyNum3->FinalState)					//	
//  	{
//  		preNumdKey[3]=keyNum3->FinalState;
// 		if (keyNum3->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('3');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 				//��һ��Num4��
//  	if (preNumdKey[4] != keyNum4->FinalState)					//	
//  	{
//  		preNumdKey[4]=keyNum4->FinalState;
// 		if (keyNum4->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('4');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 					//��һ��Num5��
//  	if (preNumdKey[5] != keyNum5->FinalState)					//	
//  	{
//  		preNumdKey[5]=keyNum5->FinalState;
// 		if (keyNum5->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('5');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 	
// 					//��һ��Num6��
//  	if (preNumdKey[6] != keyNum6->FinalState)					//	
//  	{
//  		preNumdKey[6]=keyNum6->FinalState;
// 		if (keyNum6->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('6');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 	
// 					//��һ��Num7��
//  	if (preNumdKey[7] != keyNum7->FinalState)					//	
//  	{
//  		preNumdKey[7]=keyNum7->FinalState;
// 		if (keyNum7->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('7');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 	
// 					//��һ��Num8��
//  	if (preNumdKey[8] != keyNum8->FinalState)					//	
//  	{
//  		preNumdKey[8]=keyNum8->FinalState;
// 		if (keyNum8->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('8');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 	
// 					//��һ��Num9��
//  	if (preNumdKey[9] != keyNum9->FinalState)					//	
//  	{
//  		preNumdKey[9]=keyNum9->FinalState;
// 		if (keyNum9->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('9');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 					//��һ��Num0��
//  	if (preNumdKey[0] != keyNum0->FinalState)					//	
//  	{
//  		preNumdKey[0]=keyNum0->FinalState;
// 		if (keyNum0->FinalState==false)
// 		{
// 			
// 			if(!sqStack->IsFull())       //����
// 			{
// 			sqStack->Push('0');
// 			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
// 				
// 			}

// 			Led->reverseState();
// 			
// 		}

//  	}
// 		//��һ��DEL��
//  	if (preNumdKey[10] != keyDel->FinalState)					//	
//  	{
//  		preNumdKey[10]=keyDel->FinalState;
// 		if (keyDel->FinalState==false)
// 		{
// 			if(!sqStack->IsEmpty())                                       //�ǿ�
// 			{
// 				OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,' ',16);		//��ʾ�ո񣬴������		
// 				sqStack->Pop();

// 			}
// 			
// 		}

//  	}
// 	
// 			//��һ��MODE��
// //  	if (preNumdKey[11] != keyMode->FinalState)					//	
// //  	{
// //  		preNumdKey[11]=keyMode->FinalState;
// // 		if (keyMode->FinalState==false)
// // 		{
// // 			controlModeFlag=1-controlModeFlag;         //ʵ��1��0���л�
// // 			
// // 			showControlMode();     //
// // 		
// // 			//��ֹ����״̬����
// // 			controlStatusFlag=0;

// // 			
// // 		}

// //  	}
// 	
// 	
// 				//��һ��SHOCK��
//  	if (preNumdKey[12] != keyShock->FinalState)					
//  	{
//  		preNumdKey[12]=keyShock->FinalState;
// 		if (keyShock->FinalState==false)
// 		{
// 			controlStatusFlag=2;         //���

// 		//	showControlStatus();
// 			
// 		}

//  	}
// 	
// 	
// 					//��һ��ALARM��
//  	if (preNumdKey[13] != keyAlarm->FinalState)					//	
//  	{
//  		preNumdKey[13]=keyAlarm->FinalState;
// 		if (keyAlarm->FinalState==false)
// 		{
// 			controlStatusFlag=1;         //����

// 		//	showControlStatus();
// 			
// 		}

//  	}
// 	
// 	//ι��
// 	static int keyActionFlag=0;
// 	if(!keyMode->FinalState || !keyAlarm->FinalState || !keyShock->FinalState || !keyDel->FinalState
// 		 || !keyNum1->FinalState || !keyNum2->FinalState || !keyNum3->FinalState || !keyNum4->FinalState
// 		 || !keyNum5->FinalState || !keyNum6->FinalState || !keyNum7->FinalState || !keyNum8->FinalState
// 	   || !keyNum9->FinalState || !keyNum0->FinalState)
// 	{
// 		watchDogTimer=3;//��ô���3����������ˢ����ʾ
// 		
// 		//�а����������������� 2107-01-13
// 		//Ϊ�˷�ֹ���°�����һֱ�������ݣ�����ֻ������2֡����һ֡���ڻ��ѣ��ڶ�֡����Ч����
// 		if(keyActionFlag==0 || keyActionFlag==1)
// 		{
// 			
// 			//������������֮ǰ�ȹرն�ʱ����������
// 			t3->stop();
// 			updataToZigbee();
// 			
// 			keyActionFlag++;
// 			
// 			//�ڶ��η����갴�����ݺ󣬴򿪶�ʱ��3����ʱ��������
// 			if(keyActionFlag==1)
// 			{
// 				t3->start();
// 			}
// 			
// 				
// 		}
// 		
// 		
// 	}
// 	//�����ɿ�״̬
// 	else
// 	{
// 		keyActionFlag=0;
// 	}


}



/**
  * @brief  ��ʾ����ģʽ
  * @param  None
  * @retval None
  * @author:������
  */
// #define SHOCKTIME  5
// #define ALARMTIME  5
// void Process::showControlStatus()
// {
// #ifdef USE_OLED
// 	
// 		static int shockTimeCount=0,alarmTimeCount=0;
// 	if(2==controlStatusFlag)     //���ڵ��
// 	{
// 		alarmTimeCount=0;           //�������þ�����ʱ
// 		shockTimeCount++;
// 		if(shockTimeCount>SHOCKTIME)
// 		{
// 			shockTimeCount=0;
// 			controlStatusFlag=0;
// 		}
// 		
// 		//��ʾ���
// 		OLED_ShowEFootCHinese(96,2,6);
// 		OLED_ShowEFootCHinese(112,2,7);
// 	}
// 	else if(1==controlStatusFlag) //���ھ���
// 	{
// 		shockTimeCount=0;           //�������õ����ʱ
// 		alarmTimeCount++;
// 		if(alarmTimeCount>ALARMTIME)
// 		{
// 			alarmTimeCount=0;
// 			controlStatusFlag=0;
// 		}
// 		 //��ʾ����
// 	
// 		OLED_ShowEFootCHinese(96,2,8);
// 		OLED_ShowEFootCHinese(112,2,9);
// 	}
// 	else
// 	{
// 		OLED_ShowString(96,2,noAction,16);  //û�ж�������ʾ�հ�
// 	}

// #endif
// 	
// }

	/**
  * @brief  ���·�������
  * @param  None
  * @retval None
  * @author:������
  */

void Process::updataToZigbee()
{
	//��������
	sendData[0]=(isFreeMode<<4)+(controlStatusFlag <<2)+controlModeFlag;
	
	//����п��ƶ���ʱ���򲻸��±���ID�ţ������ڶ���ʱ����ı��˱���ID�ţ�Ȼ��������ID��ת�ơ�
	//�����ڶ���ʱ���ı䱾��ID������Ч�ġ�

	
	//��������
	zigbeeControl->updateSendBuf(sendData);
	zigbeeControl->sendcommand();
	
}



	/**
  * @brief  ���·�������
  * @param  None
  * @retval None
  * @author:������ 2017-01-14
  */
void Process::sendCheckSignal()
{
	static char counter=0;
	counter++;
	if(counter>100)
		counter=0;
	
		zigbeePort->putChar(0XFF);
		zigbeePort->putChar(0XFF);
		zigbeePort->putChar(counter);		
}

/**
  * @brief  ʹ��SWD���ر�JTAG
  * @param  None
  * @retval None
  */
void Process::enable_SWD_disable_Jtag()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//	ʹ��AFIOʱ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//	�ر�JTAG��������ΪGPIO��
}






/**
  * @brief  ���Դ��룬�������
  * @param  None
  * @retval None
  */
void Process::testWithUsart(void)
{
	uint8_t aa;
	aa=AD_Filter[1]>>4;													//	���Դ���	

//	testLed->reverseState();
}

/*--------------------------------- End of Process.cpp -----------------------------*/
