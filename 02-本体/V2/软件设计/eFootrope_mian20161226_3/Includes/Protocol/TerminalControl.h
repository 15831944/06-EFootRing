/**
  ******************************************************************************
	*�ļ���TerminalControl.h
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ��Xbot���ն˽������ͷ�ļ�
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



class TerminalControl
{
public:
	TerminalControl(SerialPort *);
	virtual ~TerminalControl();
	void updateSendBuf(uint8_t * temBuf);
	void sendcommand();
	bool reciveData();                //�������ݣ������ڻ����������ݶ�ȡ��reciveBuf
	uint8_t* getReciveData(){return reciveBuf;}  //���ؽ��յ�����

private:
	SerialPort *terminalPort;		//���ߴ��ں�	
	RingChain *reciveChin;
	uint8_t *sendBuf; 
	uint8_t *reciveBuf;
private:
	

};

#endif /* _TERMINAL_CONTROL_H */


