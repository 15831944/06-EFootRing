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
 	const u8 Coordinate_X[15]={0,8,16,32,40,48,56,64,72,80,88,96,104,112,120};  //Һ����X ��������
 	 u8 pinPointControl[]="Pin Point";
	 u8 approachControl[]="Approach ";
	 u8 shock[]="Shock";
	 u8 alarm[]="Alarm";
	 u8 noAction[]="     ";
 
 
/**
  * @brief  ���캯��
  * @param  None
  * @retval None
  */
 
 
  Process::Process()
{
	enable_SWD_disable_Jtag();											//	�ر�JTAG��SWD	
	initGPIO();															//	��ʼ��ָʾ��
	Pow_EN->setOn();	//����������ʹ�ܵ�Դ
	
	SysTick_DelayMs(100);   //��ʱ�ȴ�Һ�����ϵ��ȶ�
	
	initADs();															//	��ʼ��ADͨ��
	initUsarts();														//	��ʼ������
	initTimers();														//	��ʼ����ʱ��	
	initOLED();
	initAllData();														//	��ʼ����������
	
	//��ʼ������

	Led->setOn();													//	


	showControlMode();                         //��ʾ����ģʽ
	
	

	

}
/**
  * @brief  TIM2��ʱ���жϺ���,10ms,���ڽ��պʹ�����
  * @param  None
  * @retval None
  */
void Process::runOnTime2(void)
{
	static int index=0;
	if(index>100)
	{
		index=0;
	}
	index++;
	

	getKeyValue();													//	��ȡAD����ֵ
	dealKeyValue();														  //	������

	
	

}

/**
  * @brief  TIM3��ʱ���жϺ���,50ms
  * @param  None
  * @retval None
	*��ʱ��80ms
  */
void Process::runOnTime3(void)
{	
	if(controlStatusFlag)
	{
		updataToZigbee();
	}
	
	
	showOtherSymbol();
}

/**
  * @brief  TIM4��ʱ���жϺ���������ˢ����ʾ����״̬��200ms
  * @param  None
  * @retval None
  */

void Process::runOnTime4(void)
{
	//static int sleepTimer=0;
	
	showControlStatus();

	//��ȡ����ֵ
	getPowerValue();
	showPowerSymbol();

//��ʱ�ض�POW_EN
	watchDogTimer++;
	if(watchDogTimer>100)
	{
		watchDogTimer=0;
		Pow_EN->setOff();
	}
	
	
#ifdef USE_SLEEP
	//ģ�⿴�Ź�����
	watchDogTimer++;
	if(watchDogTimer>200)
	{
		watchDogTimer=0;
		OLED_Display_Off();//�ر�Һ������ʾ
		SysTick_DelayMs(100);
	//	PWR_EnterSTANDBYMode();  //�������
		
	
		PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);   //����ͣ��
		//SysTick_DelayMs(10);
	//	SYSCLKconfig_STOP();		
	}
	else if(watchDogTimer<2)
	{
		OLED_Display_On();
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
	keyNum1    = new Key(KEYNUM1_PORT,KEYNUM1_PIN);    //���ְ���1
	keyNum2    = new Key(KEYNUM2_PORT,KEYNUM2_PIN);     //���ְ���2
	keyNum3    = new Key(KEYNUM3_PORT,KEYNUM3_PIN);    //���ְ���3
	keyNum4    = new Key(KEYNUM4_PORT,KEYNUM4_PIN);    //���ְ���4
	keyNum5    = new Key(KEYNUM5_PORT,KEYNUM5_PIN);    //���ְ���5
	keyNum6    = new Key(KEYNUM6_PORT,KEYNUM6_PIN);    //���ְ���6
	keyNum7    = new Key(KEYNUM7_PORT,KEYNUM7_PIN);    //���ְ���7
	keyNum8    = new Key(KEYNUM8_PORT,KEYNUM8_PIN);    //���ְ���8
	keyNum9    = new Key(KEYNUM9_PORT,KEYNUM9_PIN);    //���ְ���9
	keyNum0    = new Key(KEYNUM0_PORT,KEYNUM0_PIN);    //���ְ���0

	
	keyDel     = new Key(KEYDEL_PORT,KEYDEL_PIN);      //�˸��
	keyMode    = new Key(KEYMODE_PORT,KEYMODE_PIN);   //ģʽ����
	keyShock   = new Key(KEYSHOCK_PORT,KEYSHOCK_PIN);
	keyAlarm   = new Key(KEYALARM_PORT,KEYALARM_PIN);
	
	Led        = new IoOut(LED_PORT,LED_PIN);
	Pow_EN = new IoOut(POW_EN_PORT,POW_EN_PIN,true);
	

	

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

	zigbeePort = new SerialPort(2,9600);	
	zigbeePort->open();

	zigbeeControl = new TerminalControl(zigbeePort);
}
/**
  * @brief  ��ʼ��Һ����
  * @param  None
  * @retval None
  * @author ������
  */
void Process::initOLED()
{

		OLED_Init();			//��ʼ��OLED  
		OLED_Clear()  	; 
	

}

/**
  * @brief  ��ʼ��ȫ������
  * @param  None
  * @retval None
  */	
void Process::initAllData(void)
{
	
	OLEDString=new u8[20];
	for(int i=0;i<20;i++)
	{
		OLEDString[i]=i+48;
		
	}
	
	sendData =new u8[6];
	for(int i=0;i<6;i++)
	{
		sendData[i]=0;
	}
	controlData=0;
	
	sqStack=new SqStack();
	
	controlModeFlag=1;           //Ĭ���ǽӽ�����ģʽ
	controlStatusFlag=0;         //Ĭ����ʲô������û��
	isFreeMode=1;
	isShocking=false;
	isAlarming=false;

	watchDogTimer=0;
	powerValue=0;

	
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

	tempValue  = AD_Filter[NUM_POWER]*33*2>>12;

	powerValue=(uint8_t)tempValue;
	
// 	OLED_ShowChar(0,0,powerValue/10%10+48,16);
// 	OLED_ShowChar(16,0,powerValue%10+48,16);
// 	
	

}


/**
  * @brief  ��ȡ����ֵ
  * @param  None
  * @retval None
  * @author ������
  */
void Process::getKeyValue()
{

	
	keyNum1->getValue(); //1
	keyNum2->getValue(); //2
	keyNum3->getValue();
	keyNum4->getValue();
	keyNum5->getValue();
	keyNum6->getValue();
	keyNum7->getValue();
	keyNum8->getValue();
	keyNum9->getValue();
	keyNum0->getValue(); //0
	
	keyDel->getValue();
	keyMode->getValue();
	keyShock->getValue();
	keyAlarm->getValue();
	

	
	
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
 	static 	bool	preNumdKey[14]		={true};


		
	//��һ��Num1��
 	if (preNumdKey[1] != keyNum1->FinalState)					//	
 	{
 		preNumdKey[1]=keyNum1->FinalState;
		if (keyNum1->FinalState==false)
		{
			if(!sqStack->IsFull())
			{
				sqStack->Push('1');			
			
				OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
			}
			Led->reverseState();
			
		}

 	}
	
		//��һ��Num2��
 	if (preNumdKey[2] != keyNum2->FinalState)					//	
 	{
 		preNumdKey[2]=keyNum2->FinalState;
		if (keyNum2->FinalState==false)
		{
			
			if(!sqStack->IsFull())
			{
			sqStack->Push('2');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
	
			//��һ��Num3��
 	if (preNumdKey[3] != keyNum3->FinalState)					//	
 	{
 		preNumdKey[3]=keyNum3->FinalState;
		if (keyNum3->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('3');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
				//��һ��Num4��
 	if (preNumdKey[4] != keyNum4->FinalState)					//	
 	{
 		preNumdKey[4]=keyNum4->FinalState;
		if (keyNum4->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('4');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
					//��һ��Num5��
 	if (preNumdKey[5] != keyNum5->FinalState)					//	
 	{
 		preNumdKey[5]=keyNum5->FinalState;
		if (keyNum5->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('5');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
	
					//��һ��Num6��
 	if (preNumdKey[6] != keyNum6->FinalState)					//	
 	{
 		preNumdKey[6]=keyNum6->FinalState;
		if (keyNum6->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('6');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
	
					//��һ��Num7��
 	if (preNumdKey[7] != keyNum7->FinalState)					//	
 	{
 		preNumdKey[7]=keyNum7->FinalState;
		if (keyNum7->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('7');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
	
					//��һ��Num8��
 	if (preNumdKey[8] != keyNum8->FinalState)					//	
 	{
 		preNumdKey[8]=keyNum8->FinalState;
		if (keyNum8->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('8');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
	
					//��һ��Num9��
 	if (preNumdKey[9] != keyNum9->FinalState)					//	
 	{
 		preNumdKey[9]=keyNum9->FinalState;
		if (keyNum9->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('9');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
					//��һ��Num0��
 	if (preNumdKey[0] != keyNum0->FinalState)					//	
 	{
 		preNumdKey[0]=keyNum0->FinalState;
		if (keyNum0->FinalState==false)
		{
			
			if(!sqStack->IsFull())       //����
			{
			sqStack->Push('0');
			OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,sqStack->GetTopElem(),16);
				
			}

			Led->reverseState();
			
		}

 	}
		//��һ��DEL��
 	if (preNumdKey[10] != keyDel->FinalState)					//	
 	{
 		preNumdKey[10]=keyDel->FinalState;
		if (keyDel->FinalState==false)
		{
			if(!sqStack->IsEmpty())                                       //�ǿ�
			{
				OLED_ShowChar(Coordinate_X[sqStack->GetTop()],0,' ',16);		//��ʾ�ո񣬴������		
				sqStack->Pop();

			}
			
		}

 	}
	
			//��һ��MODE��
 	if (preNumdKey[11] != keyMode->FinalState)					//	
 	{
 		preNumdKey[11]=keyMode->FinalState;
		if (keyMode->FinalState==false)
		{
			controlModeFlag=1-controlModeFlag;         //ʵ��1��0���л�
			
			showControlMode();     //
		
			//��ֹ����״̬����
			controlStatusFlag=0;

			
		}

 	}
	
	
				//��һ��SHOCK��
 	if (preNumdKey[12] != keyShock->FinalState)					
 	{
 		preNumdKey[12]=keyShock->FinalState;
		if (keyShock->FinalState==false)
		{
			controlStatusFlag=2;         //���

			showControlStatus();
			
		}

 	}
	
	
					//��һ��ALARM��
 	if (preNumdKey[13] != keyAlarm->FinalState)					//	
 	{
 		preNumdKey[13]=keyAlarm->FinalState;
		if (keyAlarm->FinalState==false)
		{
			controlStatusFlag=1;         //����

			showControlStatus();
			
		}

 	}
	
	//ι��
	if(!keyMode->FinalState || !keyAlarm->FinalState || !keyShock->FinalState || !keyDel->FinalState
		 || !keyNum1->FinalState || !keyNum2->FinalState || !keyNum3->FinalState || !keyNum4->FinalState
		 || !keyNum5->FinalState || !keyNum6->FinalState || !keyNum7->FinalState || !keyNum8->FinalState
	   || !keyNum9->FinalState || !keyNum0->FinalState)
	{
		watchDogTimer=3;//��ô���3����������ˢ����ʾ
	}


}

/**
  * @brief  ��ʾOLED�ַ�����
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showOtherSymbol()
{


	//��ʾ�м����
	if(sqStack->GetTop()>2)
	{
		OLED_ShowChar(24,0,'-',16);		//��ʾ����
	}
	else
	{
		OLED_ShowChar(24,0,' ',16);		//��ʾ�ո񣬴������
	}
	
	//��ʾ����ͼ��
	
}

/**
  * @brief  ��ʾOLED����ͼ��
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showPowerSymbol()
{
	//��ʾ����ͼ��

	

	if(	powerValue>41)
			OLED_DrawBMP(110,0,127,1,POWER_100);
	else if(	powerValue>39)
			OLED_DrawBMP(110,0,127,1,POWER_80);
	else if(	powerValue>37)
			OLED_DrawBMP(110,0,127,1,POWER_60);
	else if(	powerValue>35)
			OLED_DrawBMP(110,0,127,1,POWER_40);
	else 
			OLED_DrawBMP(110,0,127,1,POWER_20);
	
	
}
/**
  * @brief  ��ʾ����ģʽ
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showControlMode()
{
#ifdef USE_OLED
	
	if(controlModeFlag)
	{
		//�ӽ�ģʽ
	//	OLED_ShowString(0,2,approachControl,16);
		OLED_ShowEFootCHinese(0,2,0);
		OLED_ShowEFootCHinese(16,2,1);	
		OLED_ShowEFootCHinese(32,2,2);
		OLED_ShowEFootCHinese(48,2,3);		
		
	}
	else
	{
		//����ģʽ
		//OLED_ShowString(0,2,pinPointControl,16);
		OLED_ShowEFootCHinese(0,2,4);
		OLED_ShowEFootCHinese(16,2,5);	
		OLED_ShowEFootCHinese(32,2,2);
		OLED_ShowEFootCHinese(48,2,3);	
	}
		
#endif

}

/**
  * @brief  ��ʾ����ģʽ
  * @param  None
  * @retval None
  * @author:������
  */
#define SHOCKTIME  15
#define ALARMTIME  15
void Process::showControlStatus()
{
#ifdef USE_OLED
	
		static int shockTimeCount=0,alarmTimeCount=0;
	if(2==controlStatusFlag)     //���ڵ��
	{
		alarmTimeCount=0;           //�������þ�����ʱ
		shockTimeCount++;
		if(shockTimeCount>SHOCKTIME)
		{
			shockTimeCount=0;
			controlStatusFlag=0;
		}
		
		//��ʾ���
		OLED_ShowEFootCHinese(96,2,6);
		OLED_ShowEFootCHinese(112,2,7);
	}
	else if(1==controlStatusFlag) //���ھ���
	{
		shockTimeCount=0;           //�������õ����ʱ
		alarmTimeCount++;
		if(alarmTimeCount>ALARMTIME)
		{
			alarmTimeCount=0;
			controlStatusFlag=0;
		}
		 //��ʾ����
	
		OLED_ShowEFootCHinese(96,2,8);
		OLED_ShowEFootCHinese(112,2,9);
	}
	else
	{
		OLED_ShowString(96,2,noAction,16);  //û�ж�������ʾ�հ�
	}

#endif
	
}

	/**
  * @brief  ���·�������
  * @param  None
  * @retval None
  * @author:������
  */

void Process::updataToZigbee()
{
	//��������
	sendData[0]=(isFreeMode<<4) +(controlStatusFlag <<2)+controlModeFlag;
	
	//����п��ƶ���ʱ���򲻸��±���ID�ţ������ڶ���ʱ����ı��˱���ID�ţ�Ȼ��������ID��ת�ơ�
	//�����ڶ���ʱ���ı䱾��ID������Ч�ġ�
	if(0==controlStatusFlag)
	{
		for(int i=0;i<=sqStack->GetTop();i++)
		{
			sendData[i+1] = sqStack->st.data[i]-'0';
		}
		for(int i=sqStack->GetTop()+1;i<5;i++)
		{
			sendData[i+1] = 0;
		}
	}
	

	
	//��������
	zigbeeControl->updateSendBuf(sendData);
	zigbeeControl->sendcommand();
	
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


/*******************************************************************************
* Function Name  : ��ͣ��ģʽ�»���֮�� ����ϵͳʱ������HSE���� pll ��Ϊϵͳʱ�ӡ�
* Description    : Inserts a delay time.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  Process::SYSCLKconfig_STOP(void)
{
		ErrorStatus HSEStartUpStatus;
    RCC_HSEConfig(RCC_HSE_ON); /*HSESʹ��*/  
    HSEStartUpStatus = RCC_WaitForHSEStartUp(); /*�ȴ�*/
   if(HSEStartUpStatus == SUCCESS) 
   { 
      RCC_PLLCmd(ENABLE);/*ʹ��*/
         while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)== RESET); /*�ȴ�PLL��Ч*/      
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);/*��PLL��Ϊϵͳʱ��*/
			while(RCC_GetSYSCLKSource() != 0x08);/*�ȴ�*/
   } 
}
/*--------------------------------- End of Process.cpp -----------------------------*/
