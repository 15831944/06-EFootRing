#include "ADC.h" 

// #define ADC1_DR_Address    ((uint32_t)0x4001244C)
int16_t AD_Value[50];   //AD����ֵ
int16_t After_filter;   //AD�˲��� 

void ADC1_GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);      //����DMAʱ�� 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);     //����ADC1ʱ��   
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������   
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

//ADC1���ã������ⲿ���룬������ڲ��¶ȴ�������
void ADC1_Configuration(void)   
{   
    ADC_InitTypeDef ADC_InitStructure;   
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 	//ת��ģʽΪ���������н���ȷǳ�������ѡ��   
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;       	//ɨ�跽ʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //����ת������    
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�ⲿ������ֹ
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;             //�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = STM32_ADC_CHANNEL_N;     		 //����ת�����г���Ϊ1��һͨ��    
    ADC_Init(ADC1, &ADC_InitStructure);                                //����ADC
       
    ADC_TempSensorVrefintCmd(ENABLE);    //ADC�����¶ȴ�����ʹ�ܣ�Ҫʹ��Ƭ���¶ȴ��������м�Ҫ��������    
     //���������ADC���裬ADCͨ����ת������˳�򣬲���ʱ�� 
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);  //�Ƕ�ֵ
//     ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 2, ADC_SampleTime_239Cycles5);  //�ٶ�ֵ

    ADC_Cmd(ADC1, ENABLE);      // Enable ADC1   
  
    ADC_DMACmd(ADC1, ENABLE);     // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����    
      
    // ������ADC�Զ�У׼����������ִ��һ�Σ���֤����     
    ADC_ResetCalibration(ADC1);      
    while(ADC_GetResetCalibrationStatus(ADC1));    
    ADC_StartCalibration(ADC1);    
    while(ADC_GetCalibrationStatus(ADC1));   
    // ADC�Զ�У׼����---------------    
     ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ADC����    
}  

//DMA���֣�(���ж��˲�)
void ADC1_DMA_Configuration(void)   
{   
    DMA_InitTypeDef DMA_InitStructure;   
      
    DMA_DeInit(DMA1_Channel1);   
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));//ADC1_DR_Address;   
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value;   
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;   
    //BufferSize=2����ΪADCת��������2��ͨ��    
    //������ã�ʹ����1�������AD_Value[0]������2�������AD_Value[1]    
    DMA_InitStructure.DMA_BufferSize = STM32_ADC_CHANNEL_N*STM32_ADC_CACHEL_M;//����DMA�ڴ���ʱ�������ĳ��� word
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          //����DMA���������ģʽ��һ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;   								//����DMA���ڴ����ģʽ��
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   //���������ֳ� 256
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;           //�ڴ������ֳ� 1024
    //ѭ��ģʽ������Bufferд�����Զ��ص���ʼ��ַ��ʼ����    
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   														//����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;   												//����DMA�����ȼ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;																	//����DMA��2��memory�еı����������
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);   																//����DMAͨ��
    //������ɺ�����DMAͨ��    
    DMA_Cmd(DMA1_Channel1, ENABLE);																								
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE); 															//ʹ��DMA��������ж�    
} 
void ADC1_NVIC_Configure(void) //DMA�ж����ȼ�����
{ 
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);          // Enable the DMA Interrupt 
}
void ADC1_Init(void)
{
    ADC1_GPIO_Configure();
    ADC1_Configuration();
    ADC1_DMA_Configuration();
    ADC1_NVIC_Configure();
}
//����ʹ��ADC��������ݴ�λ������
void ReEnableAdc(void)
{
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

//�жϽ�������
void DMA1_Channel1_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC1) != RESET)
  { 
    DMA_ClearITPendingBit(DMA1_IT_TC1); //���ȫ���жϱ�־    
    DMA_ClearFlag(DMA1_FLAG_TC1); 
    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
    FilteAdcVaule(); 
  }
}
//�˲����֣�����ֵ�˲��� 
void FilteAdcVaule(void)
{
	int32_t sum = 0;
	uint8_t count;
	for ( count=0;count<STM32_ADC_CACHEL_M;count++)
	{
		sum += AD_Value[count];
	}
	After_filter=sum/STM32_ADC_CACHEL_M;
	sum=0;
}

int16_t getADCValue( )
{
	int16_t temData=After_filter;
	if(temData<50)
		temData=0;
	else if(temData>4049)
	{
		temData=3999;
	}
	else
	{
		temData=temData-50;
	}
	return temData;
}
 










