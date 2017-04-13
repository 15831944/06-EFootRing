/**
  ******************************************************************************
	*�ļ���LEDLighter.h
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ��LED�����ͷ�ļ�
	*��ע��
	*
  ******************************************************************************  
	*/ 
#ifndef _LEDLIGHTER_H
#define _LEDLIGHTER_H

	#include <stdlib.h>			 
	#include <stm32f10x_gpio.h>
	#include  "STM32_GpioInit.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LEDLighter
{
public:
	LEDLighter(GPIO_TypeDef *port,unsigned int Pin);
	~LEDLighter();
// 	void setPort(GPIO_TypeDef *port);
// 	void setPin(unsigned int Pin);
// 	void initLighter();
	void setLightOn(){setLightStatus(true);}		//����
	void setLightOff(){setLightStatus(false);}	//�ص�
	void reverseLight();	//��ת�Ƶ�״̬
	void setLightStatus(bool light);
private:
	void initLighter();
	GPIO_TypeDef *lighterPort;
	unsigned int lighterPin;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



