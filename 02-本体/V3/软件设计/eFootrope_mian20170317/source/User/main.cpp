
	
/* �궨��-----------------------------------------------------------------------*/ 	

#include "Process.h"

#define CLI()     __set_PRIMASK(1) 							//	�ر����ж�
#define SEI()     __set_PRIMASK(0) 							//	�����ж�

#define	LED_SYS_PORT	GPIOA								//	ϵͳָʾ�ƶ˿�
#define	LED_SYS_PIN		GPIO_Pin_8							//	ϵͳָʾ�����ź�
//ȫ�ֱ���
Process *process;
IoOut *ledExcp;

extern  vu32 TimingDelay ;
ErrorStatus HSEStartUpStatus;

void SYSCLKconfig_STOP(void);
void RCC_Configuration(void);
void EXTI_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void RTC_Configuration (void);
void SysTick_Configuration(void);    
void Delay(vu32 nCount);

 /*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)  /*��ʼʱ��*/
{   
       RCC_DeInit();                                              /* RCC system reset(for debug purpose) */
       RCC_HSEConfig(RCC_HSE_ON);                            /* Enable HSE */
       HSEStartUpStatus = RCC_WaitForHSEStartUp();            /* Wait till HSE is ready */

  if(HSEStartUpStatus == SUCCESS)
  {
   //  FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);            /* Enable Prefetch Buffer */
   //  FLASH_SetLatency(FLASH_Latency_2);             /* Flash 2 wait state */ 
     RCC_HCLKConfig(RCC_SYSCLK_Div1);             /* HCLK = SYSCLK */
     RCC_PCLK2Config(RCC_HCLK_Div1);         /* PCLK2 = HCLK */
     RCC_PCLK1Config(RCC_HCLK_Div2);                                /* PCLK1 = HCLK/2 */
     RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);          /* PLLCLK = 8MHz * 9 = 72 MHz */
     RCC_PLLCmd(ENABLE);                                          /* Enable PLL */

  RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB|    /*����APB2��Χʱ������*/
                                 RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO ,
            ENABLE 
                               );

     while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)                     /* Wait till PLL is ready */
     {

     }
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                           /* Select PLL as system clock source */
     while(RCC_GetSYSCLKSource() != 0x08)                                /* Wait till PLL is used as system clock source */
     {
     }
  }
}


/*******************************************************************************
* Function Name  : ��ͣ��ģʽ�»���֮�� ����ϵͳʱ������HSE���� pll ��Ϊϵͳʱ�ӡ�
* Description    : Inserts a delay time.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  SYSCLKconfig_STOP(void)
  {
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

/*******************************************************************************
* Function Name  : GPIO_Configuration();
* Description    : GPIO SET
* Input          : nCount: None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11|GPIO_Pin_12 | GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//GPIO_InitStructureָ����GPIO_InitTypeDef ��GPIO_InitTypeDef�ṹ������˻�����Ϣ

    /* Configure Key Button GPIO Pin as input floating (Key Button EXTI Line) */

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : GPIO_Configuration();
* Description    : GPIO SET
* Input          : nCount: None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI_Configuration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /*����EXTI_Line17��RTC_Alarm��Ϊ�����ش���*/
  EXTI_ClearITPendingBit(EXTI_Line17);
  EXTI_InitStructure.EXTI_Line = EXTI_Line17 ;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_Init(&EXTI_InitStructure);

}


/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : PTC_clclk 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration (void)
  {
  /*����RTCʱ��Դ*/
  /* �������BKP*/
 PWR_BackupAccessCmd(ENABLE); 
 /*��λ������*/
// BKP_DeInit();
 /*����LSE*/
 RCC_LSEConfig(RCC_LSE_ON); 
 /*�ȴ�LSE��Ч*/
 while (RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET) ;
 /*ѡ��LSE��ΪRTCʱ�� */
 RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); 
 /*����RTCʱ��*/
 RCC_RTCCLKCmd(ENABLE);

 /*����RTC*/
 /*�ȴ�RTC APBͬ��*/
 RTC_WaitForSynchro();

 /*Ԥ��ƵֵΪ1s*/

 RTC_SetPrescaler(32767);

 /*�ȴ����һ��дָ�����*/

 RTC_WaitForLastTask();

 /*����RTC�����ж�*/
 RTC_ITConfig(RTC_IT_ALR, ENABLE);

 /*�ȴ����һ��дָ�����*/
 RTC_WaitForLastTask(); 
  }

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
 {  
       NVIC_InitTypeDef NVIC_InitStruct; 
    /*�����ж�������ַΪ0x8000000*/
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStruct.NVIC_IRQChannel= RTCAlarm_IRQn ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority= 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd= ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel= EXTI9_5_IRQn ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority= 0;
    NVIC_Init(&NVIC_InitStruct);

 }

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : ����1msʱ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(void)
{

}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nTime)
 { 
   /*����SysTick������*/

 }
 
 /*----------------------------------------�ڲ�RCʱ������--------------------------------------*/
void InnerRCC_Configuration(void)
{
  RCC_DeInit();//������RCC�Ĵ�������Ϊȱʡֵ
 
	RCC_HSEConfig(RCC_HSE_OFF);//�ر��ⲿ����
  RCC_HSICmd(ENABLE);//ʹ��HSI  
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}//�ȴ�HSIʹ�ܳɹ�

  if(1)
  {
    //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    //FLASH_SetLatency(FLASH_Latency_2);
   
    RCC_HCLKConfig(RCC_SYSCLK_Div1);   
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    
    //���� PLL ʱ��Դ����Ƶ��
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);                
    RCC_PLLCmd(ENABLE);//ʹ��PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}//�ȴ�PLL��ʼ���ɹ�
 
    //��PLL����Ϊϵͳʱ��Դ
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  
    while(RCC_GetSYSCLKSource() != 0x08){}//�ȴ�����PLLΪʱ��Դ�ɹ�
  }
}
	

int main(void)
{ 
	CLI();
	
	InnerRCC_Configuration();
	
	NVIC_PriorityGroupConfig(STM32_NVIC_PriorityGroup);		//	�ж����ȼ�����
	SysTick_Init();											//	SysTick��ʼ��

//	ledExcp = new IoOut(LED_SYS_PORT,LED_SYS_PIN);
//	ledExcp->setOn();
	//SysTick_DelayMs(20);
	process = new Process();
	
	process->openPeriph();
	
	SEI();

	
		
	//</������ز���>

	
	//RCC_Configuration();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE );
  GPIO_Configuration();
  EXTI_Configuration();

  NVIC_Configuration();

	PWR_WakeUpPinCmd(ENABLE);
		//</������ز���>

	while(1)
	{  
//		���Դ���
//		process->testLed->reverseState();
//		SysTick_DelayMs(500);
//		ledExcp->reverseState();
	}
}

extern "C" {
	
		
	void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
		

		SystemInit();
  }
}
	
	void TIM2_IRQHandler(void)
	{ 
		if (Timer::getITStatus(TIM2)!= RESET) 
		{
			Timer::clearITPendingBit(TIM2);
			Timer::stop(TIM2);
				process->runOnTime2();
			Timer::start(TIM2);
		}
	}
	void TIM3_IRQHandler(void)
	{ 
		if (Timer::getITStatus(TIM3)!= RESET) 
		{
			Timer::clearITPendingBit(TIM3);
			Timer::stop(TIM3);
				process->runOnTime3();
			Timer::start(TIM3);
		}
	}
	void TIM4_IRQHandler(void)
	{ 
		if (Timer::getITStatus(TIM4)!= RESET) 
		{
			Timer::clearITPendingBit(TIM4);
			Timer::stop(TIM4);
				process->runOnTime4();
			Timer::start(TIM4);
		}
	}

	/**
	  * @����˵����  	���жϺ�������NMI�쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			��NMI���ֹ���ʱ,�쳣ָʾ��ledExcp�ر�
	  */
	void NMI_Handler(void)
	{
//		ledExcp->setOff();
	}

	/**
	  * @����˵����  	���жϺ�������Ӳ�����쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			��Ӳ�����ֹ���ʱ,�쳣ָʾ��ledExcp��200�����Ƶ����˸
	  */
	void HardFault_Handler(void)
	{
		/* ������Ӳ�������쳣ʱ������ѭ�� */
	  while (1)
	  {
			SysTick_DelayMs(100);
//			ledExcp->reverseState();
	  }
	}

	/**
	  * @����˵����  	���жϺ��������ڴ�����쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			���ڴ������ֹ���ʱ,�쳣ָʾ��ledExcp��500�����Ƶ����˸
	  */
	void MemManage_Handler(void)
	{
	  /* �������ڴ�����쳣ʱ������ѭ�� */
		ledExcp->setOff();
	  while (1)
	  {
			SysTick_DelayMs(500);	
//			ledExcp->reverseState();
	  }
	}

	/**
	  * @����˵����  	���жϺ����������߹����쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			�����߳��ֹ���ʱ,�쳣ָʾ��ledExcp��1���Ƶ����˸
	  */
	void BusFault_Handler(void)
	{
	  /* �����������쳣ʱ������ѭ�� */
	  while (1)
	  {
			SysTick_DelayMs(1000);	
//			ledExcp->reverseState();
	  }
	}

	/**
	  * @����˵����  	���жϺ�������ʹ�ù����쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			��ʹ�ó��ֹ���ʱ,�쳣ָʾ��ledExcp��2���Ƶ����˸
	  */
	void UsageFault_Handler(void)
	{
	  /* ������ʹ�ô����쳣ʱ������ѭ�� */
	  while (1)
	  {
			SysTick_DelayMs(2000);	
//			ledExcp->reverseState();
	  }
	}

	/**
	  * @����˵����  	���жϺ�������SVCcall�����쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			��SVCcall���ֹ���ʱ,�쳣ָʾ��ledExcpϨ��
	  */
	void SVC_Handler(void)
	{
//		ledExcp->setOff();
	}

	/**
	  * @����˵����  	���жϺ���������Լ��ӹ����쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			�����Լ��ӳ��ֹ���ʱ,�쳣ָʾ��ledExcpϨ��
	  */
	void DebugMon_Handler(void)
	{
//		ledExcp->setOff();
	}

	/**
	  * @����˵����  	���жϺ�������PendSVC�쳣
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			��PendSVC���ֹ���ʱ,�쳣ָʾ��ledExcpϨ��
	  */
	void PendSV_Handler(void)
	{
//		ledExcp->setOff();
	}

	/**
	  * @����˵����  	���жϺ�������ϵͳ1msʱ��
	  * @������			��
	  * @����ֵ�� 		��
	  *	@��ע�� 			���жϲ���CLI()Ӱ�죬����ϵͳ���ж϶���û��Ӱ��
	  */
	void SysTick_Handler(void)
	{
		  
	}

}



