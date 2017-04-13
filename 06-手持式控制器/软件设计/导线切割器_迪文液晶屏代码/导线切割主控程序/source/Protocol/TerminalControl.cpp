/**
  ******************************************************************************
	*�ļ���TerminalControl.c
	*���ߣ�������
	*�汾��2.0
	*���ڣ�2015-08-31
	*��Ҫ�����ذ����ն�ͨѶ���ʵ��
  ******************************************************************************  
	*/ 


/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "TerminalControl.h"

/* ���Ͷ��� --------------------------------------------------------------------*/

/* �궨��-----------------------------------------------------------------------*/ 
//����֡���Ⱥ�
#define SEND_BUF_LEN 5
#define RECIVE_BUF_LEN 22    //��������֡�ĳ��ȣ����������λ

//֡ͷ����
#define FRAME_HEADER_SEND 0XA5      //֡ͷ
#define FRAME_ID_SEND 0X01					//Ŀ��ID

#define FRAME_HEADER_RECIVE 0XA5			//֡ͷ
#define FRAME_ID_RECIVE  0X00        //Ŀ��ID
// #define FRAME_COMMAND_RECIVE 0X00
//���캯��
TerminalControl::TerminalControl(SerialPort * port)
{
	terminalPort=port;
	reciveChin=new RingChain(RECIVE_BUF_LEN);
	sendBuf=new uint8_t[SEND_BUF_LEN];
	reciveBuf=new uint8_t[RECIVE_BUF_LEN];
	sendBuf[0]=FRAME_HEADER_SEND;
	sendBuf[1]=FRAME_ID_SEND;
	for(int i=2;i<SEND_BUF_LEN;i++)
	{
		sendBuf[i]=0;
	}
	for(int i=0;i<RECIVE_BUF_LEN;i++)
	{
		
		reciveBuf[i]=0;
	//	reciveBuf[i]=0x33;              //wu test  �����޸������õģ�˵��reciveBuf�ǿ��Դ������ݵ�
		
		
		
	}
}
//��������
TerminalControl::~TerminalControl()
{
	delete reciveChin;
	delete [] sendBuf;
	delete [] reciveBuf;
}

void TerminalControl::sendcommand()
{
		terminalPort->write(sendBuf,SEND_BUF_LEN);
}

/**
  * ��  �ܣ����տ����ն˵�����
  * ��  �����������ݵĴ���
  * ����ֵ��
			true:�ɹ�����һ֡
			false:û�н��յ�һ֡����
  */
bool TerminalControl::reciveData()
{
	uint8_t checkSum=0;
	bool readFlag=false;
  //���մ�������
	while (terminalPort->readAllow()&&(readFlag==false))
	{
		reciveChin->setData(terminalPort->getChar());
		reciveChin->next();
		if(reciveChin->getData()==FRAME_HEADER_RECIVE)//���֡ͷ	
		{	
			checkSum+=FRAME_HEADER_RECIVE;
			reciveChin->next();
		//	if(reciveChin->getData()==0x71)
			{
				for(int i=0;i<RECIVE_BUF_LEN-2;i++)//����У���
				{
				   checkSum+=reciveChin->getData();
				   reciveChin->next();
				}
				if(checkSum==reciveChin->getData())//У�����ȷ
				{
					//�����ݴ�������
					for(int i=0;i<RECIVE_BUF_LEN;i++)
					{
					   reciveChin->next();
						
					  reciveBuf[i]=reciveChin->getData();

					}
					readFlag=true;
				}
				reciveChin->next();
			}
// 			else
// 			{
// 				 reciveChin->previous();//�����ϸ��ڵ�	
// 			}			
		 	checkSum=0;
		}	
	}
	return 	readFlag;
}

/**
  * ��  �ܣ����·��͵�����
  * ��  �������µ�����
  * ����ֵ����
  */
void TerminalControl::updateSendBuf(uint8_t data1,uint8_t data2)
{
	sendBuf[2]=data1;
	sendBuf[3]=data2;

	sendBuf[4]=0;
	//����У���
	for(int i=0;i<4;i++)
	{
		sendBuf[4]+=sendBuf[i];
	}
}


