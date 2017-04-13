/**
  ******************************************************************************
	*�ļ���TerminalControl.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*�޸ģ�������  20161226
	*��Ҫ���õ��ӽſ۵�zigbeeͨѶ
	*��ע��
  ******************************************************************************  
	*/ 


/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "TerminalControl.h"

/* ���Ͷ��� --------------------------------------------------------------------*/

/* �궨��-----------------------------------------------------------------------*/ 
//����֡���Ⱥ�
#define SEND_BUF_LEN 						12
#define SEND_DATA_LEN           6
#define RECIVE_BUF_LEN 					12


//֡ͷ����
#define SEND_FRAME_HEADER 			0XA5
#define RECEIVE_FRAME_HEADER    0XA4
//֡ͷ����
#define ROBOT_ADDR 						  0X01



//���캯��
TerminalControl::TerminalControl(SerialPort * p1)
{
	terminalPort=p1; 
	
	reciveChin=new RingChain(RECIVE_BUF_LEN);
	sendBuf=new uint8_t[SEND_BUF_LEN];
	reciveBuf=new uint8_t[RECIVE_BUF_LEN];
	
	for(int i=0;i<SEND_BUF_LEN;i++)
	{
		sendBuf[i]=0;
	}
	for(int i=0;i<RECIVE_BUF_LEN;i++)
	{
		reciveBuf[i]=0;
	}	
	
	sendBuf[0]=SEND_FRAME_HEADER;
	sendBuf[1]=ROBOT_ADDR;
	sendBuf[2]=0x80;
	
}


//��������
TerminalControl::~TerminalControl()
{
	delete reciveChin;
	delete [] sendBuf;
	delete [] reciveBuf;
}

/**
  * ��  �ܣ��������ݵ������ն�
  * ��  ������
  * ����ֵ����
	* ��  ע: 
  */
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
		if(reciveChin->getData()==RECEIVE_FRAME_HEADER)//���֡ͷ	
		{	
			checkSum+=RECEIVE_FRAME_HEADER;
			reciveChin->next();
			if((reciveChin->getData())==ROBOT_ADDR)       // 
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
void TerminalControl::updateSendBuf(uint8_t * temBuf)
{
	uint8_t checksum=0;
	
	for(int i=0;i<SEND_DATA_LEN;i++)
	{
		sendBuf[i+3]=temBuf[i];
	}
	
	//����У���
	for(int i=0;i<SEND_BUF_LEN-1;i++)
	{
		checksum+=sendBuf[i];
	}
	sendBuf[SEND_BUF_LEN-1] = checksum;
}









