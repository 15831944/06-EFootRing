/**
  ******************************************************************************
	*�ļ���TerminalControl.h
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2014-01-02
	*��Ҫ���������Һ�����������ͷ�ļ�
	*��ע��
	*
  ******************************************************************************  
  */ 
/* �����ֹ�ݹ�����Ŀ��ƺ�----------------------------------------------------*/
#ifndef _TERMINAL_CONTROL_H
#define _TERMINAL_CONTROL_H

/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "SerialPort.h"
#include "DataChain.h"

/* ���Ͷ��� --------------------------------------------------------------------*/

/* �궨��-----------------------------------------------------------------------*/ 

class TerminalControl
{
public:
	TerminalControl(SerialPort *port);	//���캯��
	virtual ~TerminalControl();					//��������
	void updateSendBuf(uint8_t,uint8_t);//���·�������
	void sendcommand();									//�������ݣ���sendBuf�����ݷ��ͳ�ȥ
	bool reciveData();					//�������ݣ������ڻ����������ݷŽ�reciveBuf
	uint8_t* getReciveData(){return reciveBuf;}
private:
	SerialPort *terminalPort;		//���ں�
	RingChain *reciveChin;			//���ݽ�������
	uint8_t *sendBuf; 					//��������
	uint8_t *reciveBuf;					//��������
};

#endif /* _TERMINAL_CONTROL_H */


