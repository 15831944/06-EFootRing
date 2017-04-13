/**
  ******************************************************************************
	*�ļ���IoOut.cpp
	*���ߣ��Ϻ�
	*�汾��1.0
	*���ڣ�2015-03-09
	*��Ҫ��IoOut���ʵ��
	*��ע��
	*
  ******************************************************************************  
	*/ 
	
#include "IoOut.h"
/**
  * ��  �ܣ�IoOut���캯��
  * ��  ����port		=	[GPIOA(Ĭ��),GPIOB,GPIOC,GPIOD,GPIOB,GPIOE,GPIOF,GPIOG]
						Pin			=	[GPIO_Pin_0(Ĭ��)~GPIO_Pin_15]
						bLevel	=	[false(Ĭ��)��true]
  * ����ֵ����
  */
IoOut::IoOut(GPIO_TypeDef *port,unsigned int pin, bool bLevel)
{
	ioOutPort=port;
	ioOutPin=pin;
	ioOutLevel = bLevel;
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
	initIo();
}


IoOut::~IoOut()
{
// 	delete ioOutPort;
}

/**
  * ��  �ܣ���ʼ��Io�Ķ˿�
  */
void IoOut::initIo()
{
	STM32_GpioOneInit(ioOutPin,GPIO_Speed_2MHz,GPIO_Mode_Out_PP,ioOutPort);	//�������
}

/**
  * ��  �ܣ���תIo��״̬
  */
void IoOut::reverseState()	
{
	 GPIO_WriteBit(ioOutPort, ioOutPin, (BitAction)(1 - GPIO_ReadOutputDataBit(ioOutPort, ioOutPin)));
}

void IoOut::setOn()
{
	if(ioOutLevel)
		GPIO_SetBits(ioOutPort, ioOutPin);
	else
		GPIO_ResetBits(ioOutPort, ioOutPin);
}	

void IoOut::setOff()
{
	if(ioOutLevel)
		GPIO_ResetBits(ioOutPort, ioOutPin);
	else
		GPIO_SetBits(ioOutPort, ioOutPin);
}	

