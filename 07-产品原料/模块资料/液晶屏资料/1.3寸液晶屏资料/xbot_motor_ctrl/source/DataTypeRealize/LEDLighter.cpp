/**
  ******************************************************************************
	*�ļ���LEDLighter.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ��LED�����ʵ��
	*��ע��
	*
  ******************************************************************************  
	*/ 
	
#include "LEDLighter.h"
LEDLighter::LEDLighter(GPIO_TypeDef *port,unsigned int Pin)
{
	lighterPort=port;
	lighterPin=Pin;
	if(port==GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	}
	else if(port==GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	}
	else if(port==GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	}
	else if(port==GPIOD)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	}
	else if(port==GPIOE)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	}
	else if(port==GPIOF)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	}
	else if(port==GPIOG)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	}
	initLighter();
}

LEDLighter::~LEDLighter()
{
// 	delete lighterPort;
}

/**
  * ��  �ܣ���ʼ���ƵĶ˿�
  * ��  ������
  * ����ֵ����
  */
void LEDLighter::initLighter()
{
	STM32_GpioOneInit(lighterPin,GPIO_Speed_2MHz,GPIO_Mode_Out_PP,lighterPort);	//�������
}

/**
  * ��  �ܣ���ת�ȵ�����״̬
  * ��  ������
  * ����ֵ����
  */
void LEDLighter::reverseLight()	//��ת�Ƶ�״̬
{
	 GPIO_WriteBit(lighterPort, lighterPin, (BitAction)(1 - GPIO_ReadOutputDataBit(lighterPort, lighterPin)));
}

/**
  * ��  �ܣ����õȵ�����
  * ��  ����
	*			true:����
	*			false:Ϩ��
  * ����ֵ����
  */
void LEDLighter::setLightStatus(bool light)
{
	if(light)
		GPIO_ResetBits(lighterPort, lighterPin);
	else
		GPIO_SetBits(lighterPort, lighterPin);
}
