/**
  ******************************************************************************
	*�ļ���MasterCtrlApp.h
	*���ߣ�������
	*�汾��1.0
	*���ڣ�2015-08-31
	*��Ҫ�������и�������
	*��ע��
	*
  ******************************************************************************  
	*/ 
	
/* �����ֹ�ݹ�����Ŀ��ƺ�----------------------------------------------------*/
#ifndef _MASTER_CTRL_APP_H
#define _MASTER_CTRL_APP_H

/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "SerialPort.h"
#include "LCDdisplay.h"
#include "TerminalControl.h"
#include "Timer.h"
#include "LEDLighter.h"
#include "Key.h"
#include "ADC.h"
#include "STM32_SysTick.h"

/* ���Ͷ��� --------------------------------------------------------------------*/
typedef enum 
{
	otherMode=0,
	bridgeMode,
	powderMode,
	energyMode
	
}Mode;

typedef enum 
{
	stop=0,
	checkFirst,
	checkFirstDone,
	actionStart,
	actionDone,
	checkSecond,
	checkSecondDone
	
}SlaveStatus;

typedef enum
{
	cmdStop=0,
	cmdCheckFirst=1,
	cmdCheckSecond=2,
	cmdReadyAction=3,
	cmdAction=4,
	cmdOther=5
	
}MasterCommand;



/* �궨��-----------------------------------------------------------------------*/ 
#define MAX_DROP_FRAME  1000       //ͨѶ���ʧ֡��
#define STARTKEY_MAX_TIMER    20.0 //���������ӳ�

/* �ඨ��-----------------------------------------------------------------------*/
class MasterCtrlApp
{
public:
	MasterCtrlApp();
	~MasterCtrlApp();

 	void runOnTime2();												//�����ڶ�ʱ��TIM2�еĺ���
	void runOnTime3();												//�����ڶ�ʱ��TIM3�еĺ���
	void runOnTime4();												//�����ڶ�ʱ��TIM4�еĺ���
	void startWork();
	
private:
	LCDdisplay *LCDManager;										//����Һ����
	TerminalControl *terminalManager; 				//��XBOT�����佻��
	SerialPort *terminalPort;									//���նˣ�XBOT�����䣩ͨ�ŵĴ���
	SerialPort *lcdPort;											//��LCDҺ����ͨ�ŵĴ���
	Timer *t1;																//��ʱ��1
	Timer *t2;																//��ʱ��2
	Timer *t3;															  //��ʱ��3

	LEDLighter *led_1;
	LEDLighter *led_2;
	LEDLighter *ctrlFuseLed;        //���տ��ص�
	LEDLighter *ctrlStartLed;        //�������ص�

	
	Key *keyStart;                   //��������
	Key *keyFuse;									   //���տ���
	Key *keyPowder;                  //��ҩģʽ����
	Key *keyBridge;                  //����ģʽ����
	Key *keyEnergy;                  //����ģʽ����
	
	bool keyStartStatus;             //����״̬
	bool keyFuseStatus;

	bool keyBridgeStatus;            //����״̬
	bool keyPowderStatus;            //����״̬
	bool keyEnergyStatus;            //����ģʽ����״̬

	
	uint8_t masterVoltage;
	
	Mode masterMode;
	MasterCommand masterCommand;            //���ض˿���ָ��
// 	uint8_t sendCommand[2];         //���շ��ʹӻ���ָ��
	
	Mode slaveMode;                //�ӻ�ģʽ
	uint16_t slaveVoltage;            //�ӻ���ѹ
	SlaveStatus slaveStatus[8];           //�ӻ�8·״̬
	SlaveStatus slaveProgress;            //��¼�ӻ���ִ�н���
	uint8_t slaveResister[8];        //�ӻ�8·����ֵ

	bool isSameMode;                 //���ض˺ʹӻ�ģʽ�Ա�
	bool isOnLine;                   //�Ƿ�ͨ������
	
// 	bool isMeasureFinish;            //�Ƿ�������
// 	bool isFireFinish;               //�Ƿ������
// 	bool isCheckFinish;              //�Ƿ��ѯ���
// 	bool slaveIsActionFinish;
// 	bool slaveIsCheckFirstFinish;
	
	bool isAction;
	bool isActionFinish[8];
	bool isAllActionFinish;
	bool isSystemEnd;
	
	bool isLandLine;
	bool isDropLine;
	
	int startKeyCount;               //����������ʱ	
	unsigned int TEST;                //�������ڲ���



private:
	void recogniseMode();                          //ʶ������ģʽ
  void getPowSample();                           //��ȡ���ص�ѹֵ
  void getCommand();                             //��ȡ��������
	void getSlaveData();                           //��ȡ�ӻ�����
	void showKeyLed();                             //��ʾ���ص�
	void updateLCDdata();                          //����LCD����
	void dealSlaveData();
	void updateSlavedata();                        //���·��ʹӻ�����
	void progressBarCount();                       //����������
	void confirmActionFinish();                    //ȷ�϶������
	

//������LCD����ʾ����
	void showMode();
	void showVoltage();
	void showSlaveStatus();
	void showSlaveStatus1();
	void showSlaveResister();
	void showMasterCommand();
	void showProgressBar();
	void showAlarmInfo();
	void showSignalIndicator();
	void showLCD();
	void signLandLine();
	void signDropLine();    //��ǵ���

//�ַ���ת��
	void double2str(double num, char *str,int n,int size);
	void int2str(int num, char *str,int size);
	void uint162str_voltage(uint16_t num,char *str,int size);
	void uint82str_res(uint8_t num,char *str ,int size);
};
#endif /* _MASTER_CTRL_APP_H */









