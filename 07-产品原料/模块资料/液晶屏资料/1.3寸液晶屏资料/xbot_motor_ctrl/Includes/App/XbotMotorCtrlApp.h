/**
  ******************************************************************************
	*�ļ���XbotApp.h
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ������Xbot�������ͷ�ļ�
	*��ע�����Ա���кܶྲ̬��Ա������Ϊ���ǻص�����ʹ�ã�����б�Ҫ�Ļ��������޸ĵ��÷�ʽ��Ϊ��ͨ�ĳ�Ա
	*
  ******************************************************************************  
	*/ 
	
/* �����ֹ�ݹ�����Ŀ��ƺ�----------------------------------------------------*/
#ifndef _XBOT_MOTOR_CTRL_APP_H
#define _XBOT_MOTOR_CTRL_APP_H

/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "stm32f10x.h"
// #include "WatchDag.h"
#include "SerialPort.h"
#include "SpiPort.h"
#include "LCDdisplay.h"
#include "TerminalControl.h"
#include "Timer.h"
#include "LEDLighter.h"
#include "Key.h"
#include "STM32_SysTick.h"
#include "ADC.h" 
#include "MotorControl.h"
/* ���Ͷ��� --------------------------------------------------------------------*/

/* �궨��-----------------------------------------------------------------------*/ 


class XbotMotorCtrlApp
{
public:
	XbotMotorCtrlApp();
	~XbotMotorCtrlApp();
	void runOnTime2();								//�����ڶ�ʱ��TIM2�еĺ���
	void runOnTime3();								//�����ڶ�ʱ��TIM3�еĺ���
	void runOnTime4();								//�����ڶ�ʱ��TIM4�еĺ���
	void initLCD();

private:
	LCDdisplay *LCDManager;						//����Һ����
// 	TerminalControl *terminalManager; //��XBOT�����佻��
  MotorControl *motor;							//��е�۵��
	SerialPort *motorPort;						//�ͻ�е��ͨ�ŵĴ���
// 	SerialPort *terminalPort;					//��XBOT�����佻���Ĵ���
	SpiPort *lcdPort;									//��LCDҺ����ͨ�ŵĴ���
	Timer *t1;												//��ʱ��1
	Timer *t2;												//��ʱ��2
	Timer *t3;												//��ʱ��3

	LEDLighter *led;

	Key *key_M;
	Key *key_A;
	Key *key_B;
	Key *key_C;
	Key *key_D;
	int8_t keysValue;				//������ֵ
	int16_t speed_v;
	int16_t speed_u;
	int16_t *maxAngle;
	int16_t *minAngle;
	int16_t maxEle;					//������ֵ
	int16_t eleValue;	 			//��ǰ�ĵ���ֵ
	RingChain *ele;					//��������ͼ�߶Ȼ��� ����ͼ����
	int8_t interfaceIndex; 	//������
	int8_t motorIndex;			//������ ���ö�Ӧ����ĵ�ַ
	int16_t angle;					//�Ƕ�ֵ
	int8_t barValue;
	int8_t ctrlModel;				//����ģʽ 0 �ֶ�  1 �Զ�
	int8_t speedModel;			//�ٶ�ģʽ 0 Vģʽ 1 Uģʽ
	ctrData *fBMotorAngle;	//�����ĵ���Ƕ�
	ctrData *fBMotorEle;	  //�����ĵ������
	int8_t specialCommand;	//����ָ��ı�־
	int8_t speed_direction;	//�ٶȷ��� 0 ֹͣ 1 ������ 2 ������
// 	uint8_t workMode;
// 	uint8_t debugMode;
private:
// 	bool updateLCDdata();
// 	void setKeyPic(uint8_t);
// 	void setWorkMode(uint8_t);
	void disPage_start();	
	void disPage_main(int8_t,int8_t);
  void disPage_auto(int8_t,int8_t);
	void disPage_calibration(int8_t,int8_t);
	void disPage_set(int8_t,int8_t);
	void disPage_warning();	
	void disposeMotorData();
};
#endif /* _XBOT_MOTOR_CTRL_APP_H */









