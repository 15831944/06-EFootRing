/**
  ******************************************************************************
	*�ļ���SpiPort.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2014-01-02
	*��Ҫ��SpiPort���ʵ��
	*��ע��SPI1��SPI3����ͬʱ������ӳ��
  ******************************************************************************  
	*/

#include "SpiPort.h"
#include "string.h"
//���ݲ�ͬ��CPU�ͺ�������
#if defined(STM32F10X_HD)\
		||defined(STM32F10X_HD_VL)\
		||defined(STM32F10X_XL)\
		||defined(STM32F10X_CL)
#define ISSPIPORT(PORT) (((PORT) == 1) || \
                                     ((PORT) == 2) || \
                                     ((PORT) == 3)
#endif

#if defined(STM32F10X_MD)\
		||defined(STM32F10X_MD_VL)
#define ISSPIPORT(PORT) (((PORT) == 1) || \
                                     ((PORT) == 2)
#endif

#if defined(STM32F10X_LD)\
		||defined(STM32F10X_LD_VL)
#define ISSPIPORT(PORT) (((PORT) == 1)
#endif

//SPI��ӳ�俪�� SPI2û����ӳ��
// #define SPIREMAP_1 //���忪������1��ӳ��
// #define SPIREMAP_3 //���忪������1��ӳ��

SpiPort::SpiPort(uint8_t s_port)
{
// 	assert_param(ISSPIPORT(s_port));						//����spi�˿ں��Ƿ�Ϸ�
	port=s_port;
	
	if(port==1)
	{
		SPIX=SPI1;
		SPI_IRQn=SPI1_IRQn;
	}
#if defined(STM32F10X_HD)\
		||defined(STM32F10X_HD_VL)\
		||defined(STM32F10X_XL)\
		||defined(STM32F10X_CL)\
		||defined(STM32F10X_MD)\
		||defined(STM32F10X_MD_VL)
	else if(port==2)
	{
		SPIX=SPI2;
		SPI_IRQn=SPI2_IRQn;
	}
#endif	
	
#if defined(STM32F10X_HD)\
		||defined(STM32F10X_HD_VL)\
		||defined(STM32F10X_XL)\
		||defined(STM32F10X_CL)
	else if(port==3)
	{
		SPIX=SPI3;
		SPI_IRQn=SPI3_IRQn;
	}
#endif
	spiConfig();
}
SpiPort::~SpiPort()
{
	
}

/**
  * ��  �ܣ�����һ������
  * ��  ���������͵�����
  * ����ֵ����
  */
void SpiPort::putData(uint8_t Data)
{
	uint16_t temp = Data;
	while (SPI_I2S_GetFlagStatus(SPIX, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIX, temp); 
	while(SPI_I2S_GetFlagStatus(SPIX,SPI_I2S_FLAG_RXNE) == RESET); 
	temp = (SPI_I2S_ReceiveData(SPIX));
}

void SpiPort::open()
{
  SPI_Cmd(SPIX, ENABLE);
}

void SpiPort::close()
{
  SPI_Cmd(SPIX, DISABLE);
}

void SpiPort::setPriority(uint8_t Preemption,uint8_t Sub)
{
	uint8_t temPreemptionPriority = Preemption;
	uint8_t temSubPriority = Sub;
	setIRQPriority(SPI_IRQn,temPreemptionPriority,temSubPriority);
}

void SpiPort::spiConfig()
{
	SPI_InitTypeDef   SPI_InitStructure;
  spiGpioInit();																												//��ʼ��SPI���ź�ʱ��
	SPI_Cmd(SPIX, DISABLE);																								//ʧ��SPI
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//˫���������ȫ˫��ģʽ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;													//����ΪSPI������ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;											//SPI���ݴ�С������8λ֡�ṹ����
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;														//�豸����״̬ʱͬ��ʱ��SCK��״̬��HIGH��ʾ�ߵ�ƽ��LOW��ʾ�͵�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;													//ʱ����λ��1��ʾ��ͬ��SCK�������ر߲�����2��ʾż���ر߲���
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;															//NSS���������Ƭѡ
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//ʱ�ӵ�Ԥ��Ƶֵ ֵԽС�ٶ�Խ��
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;										//MSB��λ��ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;															//CRCУ��͵Ķ���ʽ
//   SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;												//
  SPI_Init(SPIX, &SPI_InitStructure);																		//��ʼ��SPI��������
  SPI_SSOutputCmd(SPIX, ENABLE);																				//ʹ��SPI1_NSS Ƭѡ����
}

void SpiPort::spiGpioInit()
{
	switch(port)
	{
		case 1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
#ifndef 	SPIREMAP_1  //spi1û����ӳ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			STM32_GpioOneInit(GPIO_Pin_4,GPIO_Speed_50MHz, GPIO_Mode_Out_PP,GPIOA); 				//SPI1_NSS  Ƭѡ �������
			STM32_GpioOneInit(GPIO_Pin_5,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOA);					//SPI1_SCK  ʱ�� �����������
			STM32_GpioOneInit(GPIO_Pin_6,GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING,GPIOA);		//SPI1_MOSO ��������/�ӻ���� ��������
			STM32_GpioOneInit(GPIO_Pin_7,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOA);					//SPI1_MISI �������/�ӻ����� �����������
// 			GPIO_ResetBits(GPIOA, GPIO_Pin_4);//Ƭѡ����
#else									//spi1������ӳ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			STM32_GpioOneInit(GPIO_Pin_15,GPIO_Speed_50MHz, GPIO_Mode_Out_PP,GPIOA); 				//SPI1_NSS  Ƭѡ �������
			STM32_GpioOneInit(GPIO_Pin_3,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOB);					//SPI1_SCK  ʱ�� �����������
			STM32_GpioOneInit(GPIO_Pin_4,GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING,GPIOB);		//SPI1_MOSO ��������/�ӻ���� ��������
			STM32_GpioOneInit(GPIO_Pin_5,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOB);					//SPI1_MISI �������/�ӻ����� �����������
// 			GPIO_ResetBits(GPIOA, GPIO_Pin_15);//Ƭѡ����
#endif
			break;
#if defined(STM32F10X_MD)\
		||defined(STM32F10X_MD_VL)\
		||defined(STM32F10X_HD)\
		||defined(STM32F10X_HD_VL)\
		||defined(STM32F10X_XL)\
		||defined(STM32F10X_CL)		
		case 2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE );														//SPI2ʱ��ʹ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
			STM32_GpioOneInit(GPIO_Pin_12,GPIO_Speed_50MHz, GPIO_Mode_Out_PP,GPIOB); 				//SPI2_NSS  Ƭѡ �������
			STM32_GpioOneInit(GPIO_Pin_13,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOB);					//SPI2_SCK  ʱ�� �����������
			STM32_GpioOneInit(GPIO_Pin_14,GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING,GPIOB);		//SPI2_MOSO ��������/�ӻ���� ��������
			STM32_GpioOneInit(GPIO_Pin_15,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOB);					//SPI2_MISI �������/�ӻ����� �����������
// 			GPIO_ResetBits(GPIOB, GPIO_Pin_12);//Ƭѡ����
			break;
#endif
		
#if defined(STM32F10X_HD)\
		||defined(STM32F10X_HD_VL)\
		||defined(STM32F10X_XL)\
		||defined(STM32F10X_CL)		
		case 3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
#ifndef 	SPIREMAP_3  //spi3û����ӳ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
			STM32_GpioOneInit(GPIO_Pin_15,GPIO_Speed_50MHz, GPIO_Mode_Out_PP,GPIOA); 				//SPI3_NSS  Ƭѡ �������
			STM32_GpioOneInit(GPIO_Pin_3,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOB);					//SPI3_SCK  ʱ�� �����������
			STM32_GpioOneInit(GPIO_Pin_4,GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING,GPIOB);		//SPI3_MOSO ��������/�ӻ���� ��������
			STM32_GpioOneInit(GPIO_Pin_5,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOB);					//SPI3_MISI �������/�ӻ����� �����������
// 			GPIO_ResetBits(GPIOA, GPIO_Pin_15);//Ƭѡ����
#else									//spi3������ӳ��
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
			STM32_GpioOneInit(GPIO_Pin_4,GPIO_Speed_50MHz, GPIO_Mode_Out_PP,GPIOA); 				//SPI3_NSS  Ƭѡ �������
			STM32_GpioOneInit(GPIO_Pin_10,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOC);					//SPI3_SCK  ʱ�� �����������
			STM32_GpioOneInit(GPIO_Pin_11,GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING,GPIOC);		//SPI3_MOSO ��������/�ӻ���� ��������
			STM32_GpioOneInit(GPIO_Pin_12,GPIO_Speed_50MHz, GPIO_Mode_AF_PP,GPIOC);					//SPI3_MISI �������/�ӻ����� �����������
// 			GPIO_ResetBits(GPIOA, GPIO_Pin_4);//Ƭѡ����
#endif
			break;
#endif
	}
}

