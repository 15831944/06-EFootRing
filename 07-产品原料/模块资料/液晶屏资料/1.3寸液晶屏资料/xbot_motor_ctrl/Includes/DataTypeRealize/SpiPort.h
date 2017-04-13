/**
  ******************************************************************************
	*�ļ���SpiPort.h
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2014-01-02
	*��Ҫ��SpiPort���ͷ�ļ�
	*
  ******************************************************************************  
	*/

#ifndef _SPIPORT_H
#define _SPIPORT_H
#include <stdlib.h>	
#include <stdint.h>	
#include "stm32f10x.h"
#include "STM32_GpioInit.h"
#include "STM32F10x_PriorityControl.h"

class SpiPort
{
	public:
		SpiPort(uint8_t);
		~SpiPort();
	public:
		void putData(uint8_t);
		void open();							  //��spi
		void close();								//�ر�spi
		void setPriority(uint8_t,uint8_t);
	private:
		void spiConfig();//spi��ʼ��
		void spiGpioInit();															 //spi�˿�����											
	private:
		int port;										//spi�˿ں�
		int SPI_IRQn;								//�жϺ�
		SPI_TypeDef *SPIX;
};

#endif

