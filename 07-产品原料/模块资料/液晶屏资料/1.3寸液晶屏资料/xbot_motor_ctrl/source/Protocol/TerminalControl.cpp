/**
  ******************************************************************************
	*�ļ���TerminalControl.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ���������Һ����������Э����ʵ��
  ******************************************************************************  
	*/ 


/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "TerminalControl.h"

/* ���Ͷ��� --------------------------------------------------------------------*/

/* �궨��-----------------------------------------------------------------------*/ 
//����֡���Ⱥ�
#define SEND_BUF_LEN 13
#define RECIVE_BUF_LEN 13

//֡ͷ����
#define FRAME_HEADER_SEND 0XA5
#define FRAME_COMMAND_SEND 0X00

#define FRAME_HEADER_RECIVE 0XA5
// #define FRAME_COMMAND_RECIVE 0X00
//���캯��
TerminalControl::TerminalControl(SerialPort * port)
{
	terminalPort=port;
	reciveChin=new RingChain(RECIVE_BUF_LEN);
	sendBuf=new uint8_t[SEND_BUF_LEN];
	reciveBuf=new uint8_t[RECIVE_BUF_LEN];
	sendBuf[0]=FRAME_HEADER_SEND;
	sendBuf[1]=FRAME_COMMAND_SEND;
	for(int i=2;i<SEND_BUF_LEN;i++)
	{
		sendBuf[i]=0;
	}
	for(int i=0;i<RECIVE_BUF_LEN;i++)
	{
		reciveBuf[i]=0;
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
			if(reciveChin->getData()==0x00||reciveChin->getData()==0x01||reciveChin->getData()==0x02||reciveChin->getData()==0x03)//����λ��ȷ
			{
				for(int i=0;i<RECIVE_BUF_LEN-2;i++)
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
			else
			{
				 reciveChin->previous();//�����ϸ��ڵ�	
			}			
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
	sendBuf[12]=0;
	//����У���
	for(int i=0;i<12;i++)
	{
		sendBuf[12]+=sendBuf[i];
	}
}


