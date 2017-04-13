#include "STM32F10x_PriorityControl.h"

void setSystemPriorityGroup()
{
		NVIC_PriorityGroupConfig(STM32_NVIC_PriorityGroup);
}
/**
  * ��  �ܣ������жϵ����ȼ�
  * ��  ����
	* NVIC_IRQChannel���жϺ�
	* NVIC_IRQChannelPreemptionPriority����ռ���ȼ�
	* NVIC_IRQChannelSubPriority����Ӧ���ȼ�
  * ����ֵ����
  */
void setIRQPriority(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel 	= NVIC_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
// 	NVIC_Init(&NVIC_InitStructure);
}


