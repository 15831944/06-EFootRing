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
 
 
 const u8 Coordinate_X[11]={192,66,80,94,108,122,136,150,164,178,220};  //������ʾҺ����X��������
 const u8 Coordinate_P[11]={66,80,94,108,122,136,150,164,178,220};  //������ʾҺ����X��������
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
	

	ledPow->setOn();
	
	


}
/**
  * @brief  TIM2��ʱ���жϺ���,,���ڽ��պʹ�����
  * @param  None
  * @retval None
	* @��ʱ��20ms
  */
void Process::runOnTime2(void)
{
	static int index=0;
	if(index>100)
	{
		//showControlMode();
		showPower();
		
		ledPow->reverseState();
		index=0;
		//showControlMode();
	}
	
	showControlMode();
	showPower();
	index++;
	

	getKeyValue();													//	��ȡAD����ֵ
	dealKeyValue();														  //	������




}

/**
  * @brief  TIM3��ʱ���жϺ���,���ʹ��ڿ�������
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

}

/**
  * @brief  TIM4��ʱ���жϺ���������ˢ����ʾ����״̬��200ms
  * @param  None
  * @retval None
	* @��ʱ��200ms
  */

void Process::runOnTime4(void)
{
	//static int sleepTimer=0;
	
	//����ʹ���Ǵ�
#ifndef TEST
	//showControlStatus();
#endif

	//��ȡ����ֵ
//	getPowerValue();
	//showPowerSymbol();
	
#ifdef USE_SLEEP
	//ģ�⿴�Ź�����
	watchDogTimer++;
	if(watchDogTimer>200)
	{
		watchDogTimer=0;
		OLED_Display_Off();//�ر�Һ������ʾ
		SysTick_DelayMs(100);
		PWR_EnterSTANDBYMode();  //�������
		
	
	//	PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);   //����ͣ��
		//SysTick_DelayMs(10);
	//	SYSCLKconfig_STOP();		
	}
	else if(watchDogTimer<2)
	{
		//���Ѻ�ĳ�ʼ��
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
	
	ledPow       = new IoOut(LED_POW_PORT,LED_POW_PIN);
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

	zigbeePort = new SerialPort(3,9600);	
	zigbeePort->open();
	zigbeeControl = new TerminalControl(zigbeePort);
	
	lcdPort    = new SerialPort(1,38400);
	lcdPort->open();
	LCDControl    = new LCDdisplay(lcdPort);

	
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
	controlData=0;
	
	sqStack=new SqStack();
	
	controlModeFlag=1;           //Ĭ���ǽӽ�����ģʽ
	controlStatusFlag=0;         //Ĭ�����޲���
	isFreeMode=1;                //Ĭ��������ģʽ
	
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

void Process::startWork()
{
	showStartPic();
	showMainWin();
	
	
	//openPeriph();
	
	
	
	openPeriph();
	SysTick_DelayMs(10);												//	��ʱ 10ms
	
	//showControlMode();
	
}
//////////////////////////////////////////////end of Init/////////////////////////////////////////////////////////


/**
  * @brief  �����Դ��ѹ
  * @param  None
  * @retval None
  */
void Process::getPowerValue(void)
{
	powerValue  = AD_Filter[NUM_POWER]*33*11>>12;

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


	//����Ƕ�����ƣ���ʹ�����ְ���
	if(0==controlModeFlag)
	{		
		//��һ��Num1��
		if (preNumdKey[1] != keyNum1->FinalState)					//	
		{
			preNumdKey[1]=keyNum1->FinalState;
			if (keyNum1->FinalState==false)
			{
				if(!sqStack->IsFull())
				{
					sqStack->Push(1);			
					showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
				}

				
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
				sqStack->Push(2);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}


				
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
				sqStack->Push(3);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}

		
				
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
				sqStack->Push(4);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}


				
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
				sqStack->Push(5);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}


				
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
				sqStack->Push(6);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}


				
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
				sqStack->Push(7);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}


				
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
				sqStack->Push(8);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}


				
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
				sqStack->Push(9);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}


				
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
				sqStack->Push(0);
				showNumber(sqStack->GetTop(),sqStack->GetTopElem() );
					
				}

				
			}

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
				showNumber(sqStack->GetTop(),10 );		//��ʾ�հױ������������		
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
			
		}
		else
		{
			controlStatusFlag=0;         //û�в���
		}
		showControlStatus();

 	}
	//��һ��ALARM��
	else if (preNumdKey[13] != keyAlarm->FinalState)					//	
 	{
 		preNumdKey[13]=keyAlarm->FinalState;
		if (keyAlarm->FinalState==false)
		{
			controlStatusFlag=1;         //����		
			
		}
				else
		{
			controlStatusFlag=0;         //û�в���
		}
		showControlStatus();


 	}

	
	//ι��
	static int keyActionFlag=0;
	if(!keyMode->FinalState || !keyAlarm->FinalState || !keyShock->FinalState || !keyDel->FinalState
		 || !keyNum1->FinalState || !keyNum2->FinalState || !keyNum3->FinalState || !keyNum4->FinalState
		 || !keyNum5->FinalState || !keyNum6->FinalState || !keyNum7->FinalState || !keyNum8->FinalState
	   || !keyNum9->FinalState || !keyNum0->FinalState)
	{
		watchDogTimer=3;//��ô���3����������ˢ����ʾ
	
/*		
		//�а����������������� 2107-01-13
		//Ϊ�˷�ֹ���°�����һֱ�������ݣ�����ֻ������2֡����һ֡���ڻ��ѣ��ڶ�֡����Ч����
		if(keyActionFlag==0 || keyActionFlag==1)
		{
			
			//������������֮ǰ�ȹرն�ʱ����������
			t3->stop();
		//	updataToZigbee();
			
			keyActionFlag++;
			
			//�ڶ��η����갴�����ݺ󣬴򿪶�ʱ��3����ʱ��������
			if(keyActionFlag==1)
			{
				t3->start();
			}
			
				
		}
		
	*/
	}
	//�����ɿ�״̬
	else
	{
		keyActionFlag=0;
	}


}


bool Process::anyKeyDown()
{
	//���������
	if(!keyMode->FinalState || !keyAlarm->FinalState || !keyShock->FinalState || !keyDel->FinalState
		 || !keyNum1->FinalState || !keyNum2->FinalState || !keyNum3->FinalState || !keyNum4->FinalState
		 || !keyNum5->FinalState || !keyNum6->FinalState || !keyNum7->FinalState || !keyNum8->FinalState
	   || !keyNum9->FinalState || !keyNum0->FinalState)
	{
		
		return true;
	}
	
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////LCD��ʾ//////////////////////////////////////////////


/**
  * @brief  ��ʾ��������
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showStartPic()
{
	SysTick_DelayMs(80);
	LCDControl->ShowPic(0);    //�������ض˻�ӭ����
	SysTick_DelayMs(1000);
}
/**
  * @brief  ��ʾ���ؽ���
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showMainWin()
{
	SysTick_DelayMs(80);
	LCDControl->ShowPic(1);    //���ز�������
	SysTick_DelayMs(80);
}
/**
  * @brief  ��ʾ������봰��
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showInputWin(bool flag)
{
	if(flag)
	{
		LCDControl->CutPicAndShow(2,120,119,188,55,66,141);    //��ʾ��������
	}
	else
	{
		LCDControl->CutPicAndShow(2,120,3,188,55,66,141);    //��ʾ����ɫ
	}
	
}

/**
  * @brief  ��ʾ����
  * @param  None
  * @retval pisiton:��ʾ�ĵ�pisiton������
						num��   ��ʾ������
						�磺���µĵ�position��������num
  * @author:������
  
  */
void Process::showNumber(int position,int num)
{

	if(num>9)
	{
		LCDControl->CutPicAndShow(3,Coordinate_X[10],95,14,24,Coordinate_P[position],141);    //��ʾ�հױ���
	}
	else
	{
		LCDControl->CutPicAndShow(3,Coordinate_X[num],95,14,24,Coordinate_P[position],141);    //��ʾ�������
	}
	
	
	
}

/**
  * @brief  ��ʾ����ͼ��
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showPower()
{
	getPowerValue();
	static u8 prePowerValue=0;
	if(prePowerValue!=powerValue)
	{
		prePowerValue=powerValue;
	//��ʾ����ͼ��
		if(powerValue>120)
			LCDControl->CutPicAndShow(2,10,15,60,30,240,102);    //��ʾ100
		else if(powerValue>115)
			LCDControl->CutPicAndShow(2,10,57,60,30,240,102);    //��ʾ80
		else if(powerValue>113)
			LCDControl->CutPicAndShow(2,10,102,60,30,240,102);    //��ʾ60
		else if(powerValue>111)
			LCDControl->CutPicAndShow(2,10,147,60,30,240,102);    //��ʾ40
		else 
			LCDControl->CutPicAndShow(2,10,191,60,30,240,102);    //��ʾ20
			
	}
	
	
}
/**
  * @brief  ��ʾ����ģʽ
  * @param  None
  * @retval None
  * @author:������
  */
void Process::showControlMode()
{
	static u8 preControlModeFlag=0xff;
	
	if(preControlModeFlag!=controlModeFlag)
	{
		preControlModeFlag=controlModeFlag;
	//�ӽ�����
		if(1==controlModeFlag)
		{
			LCDControl->CutPicAndShow(2,219,61,60,30,240,59);    //��ʾ �ӽ�����
			showInputWin(false);
		}
		//�������
		else if(0==controlModeFlag)
		{
			LCDControl->CutPicAndShow(2,119,61,60,30,240,59);    //��ʾ �������
			showInputWin(true);
		}
		//���ջ
		sqStack->ClearStack();
		
	}
	
	
}

/**
  * @brief  ��ʾ����ģʽ
  * @param  None
  * @retval None
  * @author:������
  */

void Process::showControlStatus()
{
	if(2==controlStatusFlag)     //���ڵ��
	{

		//��ʾ���
		LCDControl->CutPicAndShow(2,137,203,40,30,251,203);    //��ʾ ���

	}
	else if(1==controlStatusFlag) //���ھ���
	{

		 //��ʾ����
		LCDControl->CutPicAndShow(2,209,203,40,30,251,203);    //��ʾ ����

	}
	else
	{
		//û�ж�������ʾ�հ�
		LCDControl->CutPicAndShow(2,260,203,40,30,251,203);    //��ʾ �հ�
	}
	
}


////////////////////////////////////////////////////LCD��ʾ��///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	//if(0==controlStatusFlag)
	{
		for(int i=0;i<=sqStack->GetTop();i++)
		{
			sendData[i+1] = sqStack->st.data[i];
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
