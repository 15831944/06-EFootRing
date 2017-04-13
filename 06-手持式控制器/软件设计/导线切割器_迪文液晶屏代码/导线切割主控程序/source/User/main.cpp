/**
  ******************************************************************************
	*�ļ���main.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ������������
	*��ע��
	*
  ******************************************************************************  
	*/ 
	
/* �궨��-----------------------------------------------------------------------*/ 	

//���Ź�ֻ��λһ��
// #define RESET_ONE

#include "STM32_SysTick.h"
#include "MasterCtrlApp.h"
	
// #include <rt_heap.h>
//  #define HEAP_SIZE   (0x300)              //���ڴ�صĳ���3000*4=16K STM32��RAM�ܴ�С��32K
//  int32_t  heap_zone[HEAP_SIZE] = {0};       //���ڴ�����ȫ��ʼ��Ϊ0


#define CLI()     __set_PRIMASK(1) //�ر����ж�
#include <stm32f10x.h>
#define SEI()     __set_PRIMASK(0) //�����ж�
	
//ȫ�ֱ���
MasterCtrlApp *myApp;
LEDLighter *LED_EXCEPTION;//
int main(void)
{ 
	//SystemInit(); //�������Ӧ��ע�͵�����Ϊ�������ļ������Ѿ�ִ����
	CLI();
	NVIC_PriorityGroupConfig(STM32_NVIC_PriorityGroup);	//���ȼ�����
	SysTick_Init();																			//1msϵͳʱ�ӳ�ʼ��

	SysTick_DelayMs(1000);															//��ʱ1�룬�ȴ��豸����,СҺ�������� ��Ҫ1Sʱ��
	LED_EXCEPTION=new LEDLighter(GPIOA,GPIO_Pin_2);     //�����쳣ָʾ��
	LED_EXCEPTION->setLightOn();                          //������������ʱ����
	
	//����JTAG����
	#if 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable ,ENABLE);
	#endif
	
	myApp=new MasterCtrlApp();
	SEI();
	myApp->startWork();
	while(1)
	{  
	
	
	}
}



/**/
extern "C" {
	void TIM2_IRQHandler(void)
	{ 
		if (Timer::getITStatus(TIM2)!= RESET) 
		{
			Timer::clearITPendingBit(TIM2);
			Timer::stop(TIM2);
			myApp->runOnTime2();
			Timer::start(TIM2);
		}
	}
	
	void TIM3_IRQHandler(void)
	{ 
		if (Timer::getITStatus(TIM3)!= RESET) 
		{
			Timer::clearITPendingBit(TIM3);
			Timer::stop(TIM3);
			myApp->runOnTime3();
			Timer::start(TIM3);
		}
	}
	
	void TIM4_IRQHandler(void)
	{ 
		if (Timer::getITStatus(TIM4)!= RESET) 
		{
			Timer::clearITPendingBit(TIM4);
			Timer::stop(TIM4);
			myApp->runOnTime4();
			Timer::start(TIM4);
		}
	}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	LED_EXCEPTION->setLightOff();
}

/**
  * @����˵����  	���жϺ�������Ӳ�����쳣
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			��Ӳ�����ֹ���ʱ,�쳣ָʾ��LED_EXCEPTION��200�����Ƶ����˸
  */
void HardFault_Handler(void)
{
// 	__IO int32_t ReturnAddr=0x00000000; 
// 	ReturnAddr = __get_MSP();//���ض�ջָ�� ������DEBUGʱ��������ĵ�ַ
// 	ReturnAddr = *(int32_t *)(ReturnAddr+32); 
	/* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		SysTick_DelayMs(50);
		LED_EXCEPTION->reverseLight();
  }
}

/**
  * @����˵����  	���жϺ��������ڴ�����쳣
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			���ڴ������ֹ���ʱ,�쳣ָʾ��LED_EXCEPTION��500�����Ƶ����˸
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
	LED_EXCEPTION->setLightOff();
  while (1)
  {
		SysTick_DelayMs(500);	
		LED_EXCEPTION->reverseLight();
  }
}

/**
  * @����˵����  	���жϺ����������߹����쳣
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			�����߳��ֹ���ʱ,�쳣ָʾ��LED_EXCEPTION��1���Ƶ����˸
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
		SysTick_DelayMs(1000);	
		LED_EXCEPTION->reverseLight();
  }
}

/**
  * @����˵����  	���жϺ�������ʹ�ù����쳣
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			��ʹ�ó��ֹ���ʱ,�쳣ָʾ��LED_EXCEPTION��2���Ƶ����˸
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
		SysTick_DelayMs(2000);	
		LED_EXCEPTION->reverseLight();
  }
}

/**
  * @����˵����  	���жϺ�������SVCcall�����쳣
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			��SVCcall���ֹ���ʱ,�쳣ָʾ��LED_EXCEPTIONϨ��
  */
void SVC_Handler(void)
{
	LED_EXCEPTION->setLightOff();
}

/**
  * @����˵����  	���жϺ���������Լ��ӹ����쳣
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			�����Լ��ӳ��ֹ���ʱ,�쳣ָʾ��LED_EXCEPTIONϨ��
  */
void DebugMon_Handler(void)
{
	LED_EXCEPTION->setLightOff();
}

/**
  * @����˵����  	���жϺ�������PendSVC�쳣
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			��PendSVC���ֹ���ʱ,�쳣ָʾ��LED_EXCEPTIONϨ��
  */
void PendSV_Handler(void)
{
	LED_EXCEPTION->setLightOff();
}

/**
  * @����˵����  	���жϺ�������ϵͳ1msʱ��
  * @������				��
  * @����ֵ�� 		��
	*	@��ע�� 			���жϲ���CLI()Ӱ�죬����ϵͳ���ж϶���û��Ӱ��
  */
void SysTick_Handler(void)
{
	SysTick_TimingDelay_Decrement();   
}

}



