/**
  ******************************************************************************
	*�ļ���Protocol.h
	*���ߣ��Ϻ�
	*�汾��1.0
	*���ڣ�2015-05-28
	*��Ҫ������İ塢�󰴼��塢�Ұ�����ͨ��
	*��ע��
	*
  ******************************************************************************  
  */ 
/* �����ֹ�ݹ�����Ŀ��ƺ�----------------------------------------------------*/
#ifndef _PROTOCOL_H
	#define _PROTOCOL_H

	/* ͷ�ļ����� ------------------------------------------------------------------*/
	#include "SerialPort.h"
	#include "DataChain.h"
	#include "IoOut.h"
	
	#define	COMN1_485_PIN				GPIO_Pin_0
	#define	COMN1_485_PORT				GPIOC
	
	#define COMN1_SEND_LENGTH 			12
	#define COMN1_SEND_HEAD 			0xA4
	#define	COMN1_SEND_DATA_LENGTH		10
	#define COMN1_SEND_ID 				0x00
	

	#define COMN1_RECEIVE_LENGTH 		12
	#define COMN1_RECEIVE_HEAD 			0xA4
	#define	COMN1_RECEIVE_DATA_LENGTH	10
	#define COMN1_RECEIVE_ID 			0x01

	#define	COMN2_485_PIN				GPIO_Pin_0
	#define	COMN2_485_PORT				GPIOC
	
	#define COMN2_SEND_LENGTH 			13
	#define COMN2_SEND_HEAD 			0xA5
	#define	COMN2_SEND_DATA_LENGTH		10
	#define COMN2_SEND_ID 				0x02

	#define COMN2_RECEIVE_LENGTH 		7
	#define COMN2_RECEIVE_HEAD 			0xA5
	#define	COMN2_RECEIVE_DATA_LENGTH	2
	#define COMN2_RECEIVE_ID 			0x02

	class Protocol
	{
		public:
			Protocol(SerialPort *comNum);								//	���캯��
			Protocol(SerialPort *comNum1,SerialPort *comNum2);			//	���캯��
			~Protocol();												//	��������
			
			void		com1st_Init(void);								//	com1st��ʼ��
			void 		com1st_UpdateSendBuf(uint8_t*);					//	com1st��������
			void 		com1st_Send(void);								//	com1st��������
			bool 		com1st_Receive(void);							//	com1st��������
			uint8_t* 	com1st_GetData(void);							//	��ȡcom1st���յ�������
			void 		com1st_DealData(void);							//	com1st������յ�������
			
			void		com2nd_Init(void);								//	com2nd��ʼ��
			void 		com2nd_UpdateSendBuf(uint8_t*);					//	com2nd��������
			void 		com2nd_Send(void);								//	com2nd��������
			bool 		com2nd_Receive(void);							//	com2nd��������
			uint8_t* 	com2nd_GetData(void);							//	��ȡcom2nd���յ�������
			void 		com2nd_DealData(void);							//	com2nd������յ�������
			
		private:
			SerialPort 	*com1st;										//	com1st�˿�
			uint8_t		*com1st_SendBuf;								//	com1st��������
			uint8_t		*com1st_ReceiveBuf;								//	com1st��������
			RingChain 	*com1st_ReceiveChain;							//	com1st������������
			
			SerialPort 	*com2nd;										//	com2nd�˿�
			uint8_t		*com2nd_SendBuf;								//	com2nd��������
			uint8_t		*com2nd_ReceiveBuf;								//	com2nd��������
			RingChain 	*com2nd_ReceiveChain;							//	com2nd������������
	};

#endif
