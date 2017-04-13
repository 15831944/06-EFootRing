
	
/* �궨��-----------------------------------------------------------------------*/ 	

#include "Process.h"

#define CLI()     __set_PRIMASK(1) 							//	�ر����ж�
#define SEI()     __set_PRIMASK(0) 							//	�����ж�

#define	LED_SYS_PORT	GPIOA								//	ϵͳָʾ�ƶ˿�
#define	LED_SYS_PIN		GPIO_Pin_8							//	ϵͳָʾ�����ź�
//ȫ�ֱ���
Process *process;
IoOut *ledExcp;

int main(void)
{ 
	CLI();
	NVIC_PriorityGroupConfig(STM32_NVIC_PriorityGroup);		//	�ж����ȼ�����
	SysTick_Init();											//	SysTick��ʼ��

//	ledExcp = new IoOut(LED_SYS_PORT,LED_SYS_PIN);
//	ledExcp->setOn();
	SysTick_DelayMs(200);
	process = new Process();
	process->openPeriph();
	SEI();
//	ledE/tOff();

	while(1)
	{  
//		���Դ���
//		process->testLed->reverseState();
//		SysTick_DelayMs(500);
//		ledExcp->reverseState();
	}
}

extern "C" {
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



