/**
  ******************************************************************************
	*�ļ���Key.cpp
	*���ߣ��Ϻ�
	*�汾��1.0
	*���ڣ�2015-03-09
	*��Ҫ��Key���ͷ�ļ�
	*��ע��
	*
  ******************************************************************************  
	*/ 
#ifndef _IO_OUT_H
	#define _IO_OUT_H

	#include <stdlib.h>			 
	#include <stm32f10x_gpio.h>
	#include  "STM32_GpioInit.h"

	class IoOut
	{
		public:
			IoOut(GPIO_TypeDef *port=GPIOA,unsigned int pin=GPIO_Pin_0, bool bLevel=false);
			~IoOut();

			void setOn();		
			void setOff();	
			void reverseState();	//��תIo��״̬
			
		private:
			void initIo();
			GPIO_TypeDef *ioOutPort; 
			unsigned int ioOutPin;	
			bool ioOutLevel;						
	};

#endif



