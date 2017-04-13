/**
  ******************************************************************************
	*�ļ���Key.cpp
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ���������ʵ��
	*��ע��
	*
  ******************************************************************************  
	*/ 
#include "Key.h"
// typedef enum
// { GPIO_Mode_AIN = 0x0,						//ģ������
//   GPIO_Mode_IN_FLOATING = 0x04,  //��������
//   GPIO_Mode_IPD = 0x28,					//����������
//   GPIO_Mode_IPU = 0x48,					//����������
//   GPIO_Mode_Out_OD = 0x14,				//��©���
//   GPIO_Mode_Out_PP = 0x10,				//�������
//   GPIO_Mode_AF_OD = 0x1C,				//���ù��ܵĿ�©���
//   GPIO_Mode_AF_PP = 0x18					//���ù��ܵ��������
// }GPIOMode_TypeDef;
/**
  * ��  �ܣ�������Ĺ��캯��
  * ��  ����
	* 	port�������������ڵ�GPIO�˿�
	*		Pin : �������ź�
  * ����ֵ����
  */
//Key::Key(GPIO_TypeDef *port,unsigned int Pin)
Key::Key(GPIO_TypeDef *port,unsigned int Pin, bool bLevel)
{
	keyPort=port;
	keyPin=Pin;
	keyLevel=bLevel;
	
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

Key::Key()
{
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
	STM32_GpioOneInit(keyPin,GPIO_Speed_2MHz,GPIO_Mode_IPU,keyPort);	//��������
	number=0;
	value=false;
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
	if(keyLevel)
		return GPIO_ReadInputDataBit(keyPort,keyPin);
	else
		return !GPIO_ReadInputDataBit(keyPort,keyPin);
}
/**
  * ��  �ܣ����ذ���ֵ��������
  * ��  ������
  * ����ֵ��
	* true:��������
	* false:����û�а��£����ߵ�����
  */
bool Key::getValue(int maxNumber)
{
	bool temValue= getValue();
	if(temValue!=value)
	{
		if(number>maxNumber)
		{
			value=!value;
			number=0;
		}
		else
		{
			number++;
		}
	}
	return value;
}


