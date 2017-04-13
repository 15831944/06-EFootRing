/**
  ******************************************************************************
	*�ļ���Key.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ���������ʵ��
	*��ע��
	*
  ******************************************************************************  
	*/ 
#include "Key.h"

/**
  * ��  �ܣ�������Ĺ��캯��
  * ��  ����
	* 	port�������������ڵ�GPIO�˿�
	*		Pin : �������ź�
  * ����ֵ����
  */
Key::Key(GPIO_TypeDef *port,unsigned int Pin)
{
	keyPort=port;
	keyPin=Pin;
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
	initKey();
}

/**
  * ��  �ܣ����������������
  * ��  ������
  * ����ֵ����
  */
Key::~Key()
{
	keyPort=NULL;
}

/**
  * ��  �ܣ���ʼ���ƵĶ˿�
  * ��  ������
  * ����ֵ����
  */
void Key::initKey()
{
	STM32_GpioOneInit(keyPin,GPIO_Speed_2MHz,GPIO_Mode_IPU,keyPort);	//��������GPIO_Mode_IN_FLOATING
}

/**
  * ��  �ܣ����ذ���ֵ
  * ��  ������
  * ����ֵ��
	* true:��������
	* false:����û�а��£����ߵ�����
  */
bool Key::getValue()
{
	return !GPIO_ReadInputDataBit(keyPort,keyPin);
}

