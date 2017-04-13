/**
  ******************************************************************************
	*�ļ���MasterCtrlApp.c
	*���ߣ�������
	*�汾��1.0
	*���ڣ�2015-08-31
	*��Ҫ�������и��������ʵ���ļ�
	*��ע������LCD��ʾ
	*
  ******************************************************************************  
	*/ 

/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "MasterCtrlApp.h"
#ifdef __cplusplus
 extern "C" {
#endif
#include <stdlib.h>
#include <math.h>								//��ѧ������
#ifdef __cplusplus
 }
#endif

/* ���Ͷ��� --------------------------------------------------------------------*/

/* �궨��-----------------------------------------------------------------------*/ 

#define LCD_PORT_NUM 					3
#define LCD_PORT_BAUD					115200
#define TERMINAL_PORT_NUM     1      
#define TERMINAL_PORT_BAUD    38400
 #define GREENCOLOR  0x07E0
#define BLACKCOLOR  0X00
#define REDCOLOR    0xF800
#define FONT24 0X6f            //24X24
#define FONT16 0X54   
#define FONT24CH  0X6F


#define XMasterMode   65
#define YMasterMode   22
#define XSlaveMode    205
#define YSlaveMode    22
#define XMasterVol    270
#define YMasterVol    93
#define XSlaveVol     270
#define YSlaveVol     113
#define XShowBar      185
#define YShowBar      42
#define XMasterOrder  65
#define YMasterOrder  42

#define XChannelStatus     65
#define XChannelRES        155

#define XAlarm_1     240
#define YAlarm_1     166
#define XAlarm_2     240
#define YAlarm_2     186

const uint16_t YChannel[8]={86,104,123,142,161,181,199,217};


// #define BRIDGE 0;
// #define Powder 0;
// #define Energy 0;
// const char* masterMode[3]={"����","��ҩ","����"};

//��ʾ��ģʽ
const char* BridgeMode="����";
const char* PowderMode="��ҩ";
const char* EnergyMode="����";
const char* OtherMode="    ";

//��ʾ��״̬
const char* Checking="�����Լ�";
const char* CheckDone="�Լ����";

const char* ReadyCut="׼���и�";
const char* Cuting="�����и�";
const char* CutDone="�и����";
const char* CutError="�и��쳣";

const char* ReadyFire="׼�����";
const char* Firing="���ڵ��";
const char* FireDone="������";
const char* FireError="����쳣";

const char* ReadyShort="׼����·";
const char* Shorting="���ڶ�·";
const char* ShortDone="��·���";
const char* ShortError="��·�쳣";


// const char* OtherMode="  ##  ";

// const char* Stop="��·";
// const char* ResMeasure="����";
// const char* MeasureDone="������";
// const char* FireStart="���";
// const char* FireDone="�����";
// const char* OtherStatus="  ##  ";

const char* Alarm1="ģʽ����";
const char* Alarm2="ģʽ�쳣";	

const char* Alarm3="ͨ������";
const char* Alarm4="ͨ���쳣";
//���캯��
MasterCtrlApp::MasterCtrlApp()
{
		

	led_1=new LEDLighter(GPIOA,GPIO_Pin_1);                //����ִ��ָʾ��
	
	ctrlFuseLed =new LEDLighter(GPIOB,GPIO_Pin_3,true);      //���յƣ��ߵ�ƽ��Ч
	ctrlStartLed=new LEDLighter(GPIOA,GPIO_Pin_11,true);    //�����ƣ��ߵ�ƽ��Ч
	
	keyBridge=new Key(GPIOB,GPIO_Pin_15);
	keyPowder=new Key(GPIOB,GPIO_Pin_14);
	keyEnergy=new Key(GPIOB,GPIO_Pin_13);
	
	keyStart=new Key(GPIOA,GPIO_Pin_12);    //��������
	keyFuse =new Key(GPIOA,GPIO_Pin_15);    //���տ���
	
	terminalPort=new SerialPort(TERMINAL_PORT_NUM,TERMINAL_PORT_BAUD);					//���ն�ͨ�ŵĴ���
	terminalPort->setRS485CtrTx(GPIOA,GPIO_Pin_8);                              //����RS485_EN
	
	lcdPort=new SerialPort(LCD_PORT_NUM,LCD_PORT_BAUD);													//��LCDҺ����ͨ�ŵĴ���
	
	LCDManager=new LCDdisplay(lcdPort);															//����Һ����
	terminalManager=new TerminalControl(terminalPort);							//�ʹӻ�����

	led_1->setLightOn();                     //ֻҪִ�о�һֱ��
	t1=new Timer(TIM2);
	t2=new Timer(TIM3);
	t3=new Timer(TIM4);
	
	t1->setTimeOut(1);
	t1->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM2_SubPriority);

	t2->setTimeOut(15);
	t2->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM3_SubPriority);
	
	t3->setTimeOut(60);
	t3->setPriority(STM32_NVIC_TIM2_PrePriority,STM32_NVIC_TIM4_SubPriority);

	//����Ϊ���ݳ�ʼ��
	for(uint8_t i=0;i<8;i++)
	{
		slaveStatus[i]=stop;
		slaveResister[i]=0xff;
		isActionFinish[i]=false;
	}
	
	keyStartStatus=false;
	slaveMode=otherMode;
	startKeyCount=0;
	
	isSameMode=false;            //���ض˺ʹӻ�ģʽ�Ա�
	isOnLine=false;                 //�Ƿ�ͨ������
	
	isLandLine=false;                  //�Ƿ���������
	isDropLine=true;               //�Ƿ����
	
// 	isCheckFinish=false;
// 	isFireFinish=false;
// 	isMeasureFinish=false;
	isAction=false;                //�Ƿ��Ѿ�����
	isAllActionFinish=true;        //һ��Ҫ��true
	isSystemEnd=false;
	
	slaveProgress=stop;              //��ʼ����Ϊstop
// 	slaveIsActionFinish=false;
// 	slaveIsCheckFirstFinish=false;
	
	masterVoltage=0;
	slaveVoltage =0;
	
	keyStartStatus=false;             //����״̬
	keyFuseStatus =false;
	
	keyBridgeStatus=false;             //����״̬
	keyPowderStatus=false;             //����״̬
	keyEnergyStatus=false;
	
	masterMode=otherMode;
	masterCommand=cmdStop;
	
	TEST=0;          //�������д���
	

	

	
}

MasterCtrlApp::~MasterCtrlApp()
{
	//�ӻ�����
	//ctrlTerminal->setLightOn();
	
	t1->stop();
	t2->stop();
	t3->stop();
	terminalPort->close();
	lcdPort->close();
	delete terminalPort;					//���նˣ�XBOT�����䣩ͨ�ŵĴ���
	delete lcdPort;							//��LCDҺ����ͨ�ŵĴ���
	delete t1;												//��ʱ��1
	delete t2;												//��ʱ��2
	delete t3;												//��ʱ��3
	delete led_1;
	//delete ctrlTerminal;
	delete ctrlStartLed;
	delete keyBridge;
	delete keyStart;
	delete keyPowder;
	delete keyEnergy;
}

/******************************************************************
**��ʼ��
**Modify:������ 7-16
********************************************************************/
void MasterCtrlApp::startWork()
{
	ADC1_Init();
	lcdPort->open();
	SysTick_DelayMs(80);
	LCDManager->ShowPic(0);    //�������ض˻�ӭ����
	terminalPort->open();
	SysTick_DelayMs(1500);
	LCDManager-> SetColor(0x07E0,0x00);

	
	recogniseMode();     //ʶ�����ض�ģʽ
	
	LCDManager->SetColor(REDCOLOR,BLACKCOLOR);//ǰ��ɫ��ɺ�ɫ
	LCDManager->DisplayTxt_FixedMode(FONT16,XAlarm_1,YAlarm_1,Alarm4);
	LCDManager->DisplayTxt_FixedMode(FONT16,XAlarm_2,YAlarm_2,Alarm2);
	LCDManager->SetColor(GREENCOLOR,BLACKCOLOR);//ǰ��ɫ�����ɫ
	
// //�򿪶�ʱ��	
	t1->start();
	t2->start();
	t3->start();

	
}

/******************************************************************
**ʶ������ģʽ
**
********************************************************************/
void MasterCtrlApp::recogniseMode()
{
		//��ȡģʽ�����β���
  keyBridgeStatus=keyBridge->getValue();  //������Բ���ʹ�ð�����ʱ
	keyPowderStatus =keyPowder->getValue();
	keyEnergyStatus=keyEnergy->getValue();	
	
		//����ģʽ����ʶ��

	if(keyPowderStatus)
	{
		masterMode=powderMode;                   //��ҩģʽ
		SysTick_DelayMs(80);
		LCDManager->ShowPic(1);    //���� ��ҩģʽ ��������
		SysTick_DelayMs(80);
	}
	else if(keyBridgeStatus)
	{		
		masterMode=bridgeMode;                   //���Ŷ�·ģʽ
		SysTick_DelayMs(80);
		LCDManager->ShowPic(3);    //���� ����ģʽ ��������
		SysTick_DelayMs(80);
	}
		else
	{
		masterMode=energyMode;                   //����ģʽ
		
		SysTick_DelayMs(80);
		LCDManager->ShowPic(2);    //���� ����ģʽ ��������
		SysTick_DelayMs(80);
	}
	
}

/**
  * ��  �ܣ���ȡ���ذ��ѹֵ
  * ��  ������
  * ����ֵ����
  * ��  ע��	
	*/
void MasterCtrlApp::getPowSample()
{
  uint32_t  tempPowerAD;					//	���ذ��ص�ѹ
	tempPowerAD	=	(uint32_t)After_filter;		//	�������ص�ѹ�ɼ�
	tempPowerAD	=	(tempPowerAD*61*33>>12)/10;		//��ص�ѹ=	tempPowerAD*61*3.3/10,��ص�ѹ�Ŵ�10��
	masterVoltage = uint8_t(tempPowerAD);
}




/**
  * ��  �ܣ���ȡ���ذ尴������
  * ��  ������
  * ����ֵ����
  * ��  ע��	
	*/
void MasterCtrlApp::getCommand()
{
	
	
//�������Ҫ����������һ�����£�������������������İ���״̬һ����λ��������λ��ֱ��������
	if(!keyStartStatus)
	{
		keyFuseStatus=keyFuse->getValue(5);
		keyStartStatus=keyStart->getValue(5);
	}
	
	

	
	
//�ж�����ģʽ�ʹӻ�ģʽ�Ƿ�һ��
	if(masterMode==slaveMode)
		isSameMode=true;
	else 
		isSameMode=false;	
	

//������� ��������һЩ�ӻ�����
	if(!isOnLine)
	{

		isSameMode=false;
	}

	
//���¿�ʼִ�в�������
	if(isOnLine && isSameMode && !isSystemEnd)	         //ͨ��������ģʽһ��
	{
// 		if(!keyStartStatus && !isMeasureFinish)
// 		{
// 			if(!keyBridgeStatus || keyPowderStatus)
// 				masterCommand=cmdCheckFirst;                  //�������
// 			else 
// 				masterCommand=cmdStop;                   //ֹͣ����
// 		}
		//��ҩģʽ
		if(masterMode==powderMode)
		{
			if(slaveStatus[0]==stop)
			{
				masterCommand=cmdCheckFirst;
			}
			else if(!isAction  && slaveStatus[0]!=checkSecond)
			{
				if(keyStartStatus)
					masterCommand=cmdReadyAction;		
				else
					masterCommand=cmdCheckFirst;
			}
			else if(isAction && slaveStatus[0]==actionStart)
			{
				masterCommand=cmdAction; 
			}
			else if(slaveStatus[0]==actionDone)
			{
				masterCommand=cmdCheckSecond;                     //��ѯ�и��Ƿ����
// 				confirmActionFinish();  //ע��  
			}
			else if(slaveStatus[0]==checkSecond)
			{
				confirmActionFinish();//�޸ģ���ӣ�����˵����ɺ���ֵ���쳣������ 20161010
				masterCommand=cmdStop;
				isSystemEnd=true;
			}
//	else if()
// 			else if(!isAction)
// 			{
// 				
// 			}
				
			
// 			if(keyStartStatus && !isAction)  //����������£�����Ԥ�������֪ͨ�ն˷�����������
// 			{
// 				masterCommand=cmdReadyAction;
// 			}
// 			if(slaveIsActionFinish)  //����յ�������ɣ����͵ڶ��β������裬�Բ�ѯ�����Ƿ���ɡ�
// 			{
// 				masterCommand=cmdCheckSecond;                     //��ѯ�и��Ƿ����
// 				confirmActionFinish();
// 			}	
// 							
// 			
		}
		//����ģʽ
		else if(masterMode==energyMode)
		{
			if(keyStartStatus && !isAction)  //����������£�����Ԥ�������֪ͨ�ն˷�����������
			{
				masterCommand=cmdReadyAction;
			}
			else if(isAction)
			{
				masterCommand=cmdAction; 
				isSystemEnd=true;
			}
			else 
			{
				masterCommand=cmdStop;
			}			
		}
		//����ģʽ
		else if(masterMode==bridgeMode)
		{
			
			if(keyStartStatus && !isAction)  //����������£�����Ԥ�������֪ͨ�ն˷�����������
			{
				masterCommand=cmdReadyAction;
			}
			else if(isAction)
			{
				masterCommand=cmdAction; 
				isSystemEnd=true;
			}
			else 
			{
				masterCommand=cmdStop;
			}
			
		}	

	}
	//ϵͳ��������
	else if(isSystemEnd)
	{
		if(masterMode==powderMode)
			masterCommand=cmdCheckSecond;
		
	}
	else if(isOnLine)
	{
		if(masterMode==powderMode)
			masterCommand=cmdStop;

	}	
	else
	{
		;
		//slaveMode=otherMode;   //otherMode���ǲ���ʾ
	}


}
/**
  * ��  �ܣ�����������
  * ��  ������
  * ����ֵ����
  * ��  �ģ������� 11-11
  * ��  ע��	
	*/
void MasterCtrlApp::progressBarCount()
{
	if(isOnLine && isSameMode)	         //ͨ��������ģʽһ��
	{
			if(keyStartStatus)  //����ʱ��
			{
				startKeyCount++;
				if(startKeyCount>STARTKEY_MAX_TIMER)
				{
					masterCommand=cmdAction;           //������������
					isAction=true;
					startKeyCount=STARTKEY_MAX_TIMER;
				}
			}
			else
			{
				startKeyCount=0;

			}
		
	}
}
/**
  * ��  �ܣ��ж϶����Ƿ����
  * ��  ������
  * ����ֵ����
  * ��  �ģ������� 11-17
  * ��  ע��	
	*/
void MasterCtrlApp::confirmActionFinish()
{
	//static uint16_t temslaveResister[8];
	
	if(masterCommand==cmdCheckSecond)
	{
		for(int i=0;i<8;i++)
		{
			if(slaveResister[i]>200)
				isActionFinish[i]=true;	
			
			if(!isActionFinish[i])
				isAllActionFinish=false;
		}
		
		
	}
}

/**
  * ��  �ܣ���ȡ�ӻ�����
  * ��  ������
  * ����ֵ����
  * ��  �ģ������� 11-11
  * ��  ע��	
	*/
void MasterCtrlApp::getSlaveData()
{
	static int count=0;
	if(terminalManager->reciveData())
	{
		uint8_t *temReciveData=terminalManager->getReciveData();
		
		slaveMode=Mode(temReciveData[2]);                       //�����ն�ģʽ
		slaveVoltage=temReciveData[3] + (temReciveData[4]<<8);  //�����ն˵�ѹ
		
		//�����ն�״̬
		for(uint8_t i=0;i<8;i++)
		{
			slaveStatus[i]=SlaveStatus(temReciveData[i+5]);
		}
		//�����ն˵���
		for(uint8_t i=0;i<8;i++)
		{
			slaveResister[i] = temReciveData[i+13];
		}
		isOnLine=true;	
		count=0;
	}
	else
	{
		if(count<MAX_DROP_FRAME) 
			count++;
		else                 //����1000��û���յ���Ϊͨ�Ź���
			isOnLine=false;
	}
}
/*
  * ��  ��:������յ��Ĵӻ�����
  * ��  ������
  * ����ֵ����
  * ��  ע��	����״̬ע��
  *         �������Ѿ�����������  2016 10 09
	*/


void MasterCtrlApp::dealSlaveData()
{
// 	if(slaveStatus[0]==checkFirst)
// 	{ ;   }

// 	if(slaveStatus[0]==checkFirstDone)
// 	{
// 		slaveIsCheckFirstFinish=true;
// 	}
// 	if(slaveStatus[0]==actionDone)
// 	{
// 		slaveIsActionFinish=true;
// 	}

// 	if(slaveStatus[0]==checkSecondDone)
// 	{
// 		isCheckFinish=true;
// 	}

	slaveProgress=slaveStatus[0];
	
	
	
	
		
}

/**
  * ��  �ܣ���ʾ����LED
  * ��  ������
  * ����ֵ����
  * ��  ע��	����״̬ע��
	*/
void MasterCtrlApp::showKeyLed()
{
	if(keyStartStatus)
		ctrlStartLed->setLightOn();
	else
		ctrlStartLed->setLightOff();
	
	if(keyFuseStatus)
		ctrlFuseLed->setLightOn();
	else
		ctrlFuseLed->setLightOff();	
		
}

/**
  * ��  �ܣ����·��͵�����
  * ��  ������
  * ����ֵ����
  * ��  ע��	����״̬ע��
	*/

void MasterCtrlApp::updateSlavedata()
{
	terminalManager->updateSendBuf(masterMode,masterCommand);
	
	//test data
	//terminalManager->updateSendBuf(TEST,isDropLine);
	terminalManager->sendcommand();
}



/**************************************************************************************
**������LCD��ʾ���ܺ���**
***************************************************************************************/

/**
  * ��  �ܣ���ʾ���ض˺ʹӻ�ģʽ
  * ��  ������
  * ����ֵ����
  * 
  */
void MasterCtrlApp::showMode()
{
		//��ʾ���ض�ģʽ
	static Mode temMasterMode=otherMode;
	static Mode temSlaveMode=otherMode;
	static bool showModeTemIsOnLine=false;
	
	if(temMasterMode!=masterMode)
	{
		temMasterMode=masterMode;
		if(masterMode==bridgeMode)
			LCDManager->DisplayTxt_FixedMode(FONT16,XMasterMode,YMasterMode,BridgeMode);//masterModeBridge
		else if(masterMode==powderMode)
			LCDManager->DisplayTxt_FixedMode(FONT16,XMasterMode,YMasterMode,PowderMode);
		else
			LCDManager->DisplayTxt_FixedMode(FONT16,XMasterMode,YMasterMode,EnergyMode);	
	}
	
		//��ʾ�ӻ�ģʽ
	
	//���߻��������ò���ֵ
	if(showModeTemIsOnLine!=isOnLine)
	{
		showModeTemIsOnLine=isOnLine;
		temSlaveMode=otherMode;
	}	
	
	//��ʾ
	if(temSlaveMode!=slaveMode)
	{
		temSlaveMode=slaveMode;
		if(!isOnLine)
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveMode,YSlaveMode,"        ");
		else if(slaveMode==0x01)
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveMode,YSlaveMode,BridgeMode);
		else if(slaveMode==0x02)
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveMode,YSlaveMode,PowderMode);
		else if(slaveMode==0x03)
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveMode,YSlaveMode,EnergyMode);
		else
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveMode,YSlaveMode,OtherMode);	
	}
}
/**
  * ��  �ܣ���ʾ���ض�����
  * ��  ������
  * ����ֵ����
	* ��  �ģ���ģʽ��ʾ
  */
void MasterCtrlApp::showMasterCommand()
{
	//��ʾ���ض�����
	static MasterCommand temmasterCommand=cmdOther;
	if(temmasterCommand==cmdStop)    //��֪��˵��ԭ���ܳ�ʼ��ΪcmdOther
		temmasterCommand=cmdOther;
	
	//���߻��������ò���ֵ
	//Add code  here

	//������ж���Ϊ��ͨѶ���߶����õ��жϣ����û�е������������´���ֻ��Ҫ����һ�μ���
	if(temmasterCommand!=masterCommand ) 
	{
		temmasterCommand=masterCommand;	
		
		//��ҩģʽ
		if(masterMode==powderMode)
		{
			if(masterCommand==cmdStop)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,Checking);
			else if(masterCommand==cmdCheckFirst)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,CheckDone);
			else if(masterCommand==cmdReadyAction)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,ReadyFire);
			else if(masterCommand==cmdAction)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,Firing);
			else if(isAllActionFinish)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,FireDone);
			else
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,FireError);				
				
		}
		//����ģʽ
		else if(masterMode==energyMode)
		{
			if(!isOnLine)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,Checking);
			else if(masterCommand==cmdStop)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,CheckDone);		
			else if(masterCommand==cmdReadyAction)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,ReadyCut);
			else if(masterCommand==cmdAction)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,CutDone);			

		}
		//����ģʽ
		else if(masterMode==bridgeMode)											
		{
			if(!isOnLine)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,Checking);
			else if(masterCommand==cmdStop)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,CheckDone);		
			else if(masterCommand==cmdReadyAction)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,ReadyShort);
			else if(masterCommand==cmdAction)
				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,ShortDone);
// 			else if(isAllActionFinish)
// 				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,ShortDone);		
// 			else 
// 				LCDManager->DisplayTxt_FixedMode(FONT16,XMasterOrder,YMasterOrder,ShortError);
		}
			

	}
		
}
/**
  * ��  �ܣ���ʾִ�ж���������
  * ��  ������
  * ����ֵ����
  */
void MasterCtrlApp::showProgressBar()
{
	//��ʾ������
	static int temstartKeyCount=-1;
	int startKeyCount_1; 
	
	progressBarCount();
	
	if(temstartKeyCount!=startKeyCount)
	{
		temstartKeyCount=startKeyCount;
		startKeyCount_1=int((120.0/STARTKEY_MAX_TIMER)*double(startKeyCount));    //��������������ֵ�����Ŵ�
		if(startKeyCount_1<=144)
		{
			if(startKeyCount_1>100)
			{
				LCDManager->SetColor(REDCOLOR,BLACKCOLOR);//ǰ��ɫ��ɺ�ɫ
				LCDManager->ShowBar(XShowBar,YShowBar,startKeyCount_1);
				LCDManager->SetColor(GREENCOLOR,BLACKCOLOR);//ǰ��ɫ�����ɫ	
			}
			else
			{
				LCDManager->ShowBar(XShowBar,YShowBar,startKeyCount_1);
			}
		}
	}
// 	else if(temstartKeyCount==-1)
// 		LCDManager->ShowBar(XShowBar,YShowBar,0);\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

}
/**
  * ��  �ܣ���ʾ���ض˵�ѹ
  * ��  ������
  * ����ֵ����
  */
void MasterCtrlApp::showVoltage()
{

	char TempChar[5] = {0};
	static uint8_t temMasterVol = 0xff;  
	static uint16_t temSlaveVol = 0xffff;
	static bool showVoltageTemIsOnLine=false;


//��ʾ���ض˵�ѹ
	if(temMasterVol>masterVoltage+1 || temMasterVol<masterVoltage-1)      //ʩ������������
	{
		temMasterVol=masterVoltage;
 		
		uint162str_voltage(temMasterVol,(char *)TempChar,5);
		
		if(temMasterVol<74 )
		{
			LCDManager->SetColor(0xF800,0x00);//ǰ��ɫ��ɺ�ɫ
			LCDManager->DisplayTxt_FixedMode(FONT16,XMasterVol,YMasterVol,(char *)TempChar);
			LCDManager->SetColor(0x07E0,0x00);//ǰ��ɫ�����ɫ
		}
		else
		{
			LCDManager->DisplayTxt_FixedMode(FONT16,XMasterVol,YMasterVol,(char *)TempChar);
		}
	}	
	
//��ʾ�ӻ���ѹ
	
//���߻��������ò���ֵ
	if(showVoltageTemIsOnLine!=isOnLine)
	{
		showVoltageTemIsOnLine=isOnLine;
		temSlaveVol=0xffff;
	}	
	
	//��ʾ
	if(temSlaveVol>slaveVoltage+1 || temSlaveVol<slaveVoltage-1)      //ʩ������������
	{
		temSlaveVol=slaveVoltage;
 		
		uint162str_voltage(temSlaveVol,(char *)TempChar,5);
		
		if(!isOnLine)
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveVol,YSlaveVol,"     ");
		else if(temSlaveVol<111 )
		{
			LCDManager->SetColor(0xF800,0x00);//ǰ��ɫ��ɺ�ɫ
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveVol,YSlaveVol,(char *)TempChar);
			LCDManager->SetColor(0x07E0,0x00);//ǰ��ɫ�����ɫ
		}
		else
		{
			LCDManager->DisplayTxt_FixedMode(FONT16,XSlaveVol,YSlaveVol,(char *)TempChar);
		}
	}

	
	
}
/**
  * ��  �ܣ���ʾ�ӻ�8ͨ������
  * ��  ������
  * ����ֵ����
  */
void MasterCtrlApp::showSlaveResister()
{
	char *strSlaveRES;
	static uint8_t temslaveResister[8]={0};//={0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff};
	static bool showSlaveResisterTemIsOnLine=false;
	

	//���߻��������ò���ֵ
	if(showSlaveResisterTemIsOnLine!=isOnLine)
	{
		showSlaveResisterTemIsOnLine=isOnLine;
		for(int i=0;i<8;i++)
		{
				temslaveResister[i]=0;
		}
	}
	
	//��ʾ����ֵ
	for(int i=0;i<8;i++)
	{

		if((temslaveResister[i]!=slaveResister[i]) && (masterMode!=bridgeMode))
		{
			temslaveResister[i]=slaveResister[i];
			if(!isOnLine)
			{
				LCDManager->CutPicAndShow(10,0,40,30,15,XChannelRES,YChannel[i]);    //��ʾ������
			}
			else if(slaveResister[i]==0xff)
				LCDManager->CutPicAndShow(10,0,40,30,15,XChannelRES,YChannel[i]);    //��ʾ������
				//strSlaveRES="    ";
			else 
				//strSlaveRES="have";
			//int2str((int)slaveResister[i],strSlaveRES,4);
			{
			uint82str_res(slaveResister[i],strSlaveRES,3);
			
			LCDManager->DisplayTxt_FixedMode(FONT16,XChannelRES,YChannel[i],strSlaveRES);		
			}
		}
		
	}
}
/*
   ��  �ܣ���ʾ�ӻ�8ͨ��״̬
	 ˵  ����ͨ���ӻ�����������ʾ8ͨ��״̬
   ��  ������
   ����ֵ����
	 ��  �ģ���ͬ��ģʽ��ʾ��״̬��һ���������յ����ն�״ֵ̬�ǹ̶��ġ�
	 ˵���� �ú����Ѿ�������ʹ��
  */
void MasterCtrlApp::showSlaveStatus()
{
	char *strSlaveStatus;
	static uint8_t temslaveStatus[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

	if(masterMode==powderMode)
	{
		for(int i=0;i<8;i++)
	{
		if(temslaveStatus[i]!=slaveStatus[i])
		{
			temslaveStatus[i]=slaveStatus[i];
				if(slaveStatus[i]==checkFirst)
					strSlaveStatus=(char*)CheckDone;
				else if(slaveStatus[i]==checkFirstDone)
					strSlaveStatus=(char*)CheckDone;
				else if(slaveStatus[i]==actionStart)
					strSlaveStatus=(char*)Firing;
				else if(slaveStatus[i]==checkSecondDone)
					strSlaveStatus=(char*)FireDone;
				else if(slaveStatus[i]==stop)
					strSlaveStatus=(char*)Checking;
				else	strSlaveStatus=(char*)Checking;
				
			LCDManager->DisplayTxt_FixedMode(FONT16,XChannelStatus,YChannel[i],strSlaveStatus);
// 			return;
		}
	}
	}
	else if(masterMode==energyMode)
	{
		
	}
	else if(masterMode==bridgeMode)
	{
		
	}
	
}
/*
   ��  �ܣ���ʾ�ӻ�8ͨ��״̬
	 ˵  ����ͨ��������������ʾ8ͨ��״̬
   ��  ������
   ����ֵ����
	 ��  �ģ���ͬ��ģʽ��ʾ��״̬��һ���������յ����ն�״ֵ̬�ǹ̶��ġ�
  */
void MasterCtrlApp::showSlaveStatus1()
{
	char *strSlaveStatus;
	static MasterCommand temMasterCommand1=cmdOther;	
	static bool showSlaveStatusTemIsOnLine=false;
	
	//���߻��������ò���ֵ
	if(showSlaveStatusTemIsOnLine!=isOnLine)
	{
		showSlaveStatusTemIsOnLine=isOnLine;
		temMasterCommand1=cmdOther;
		TEST++;
	}
	
	
	if(temMasterCommand1!=masterCommand )
	{
		temMasterCommand1=masterCommand;

		if(masterMode==powderMode)       //��ҩģʽ  
		{
			for(int i=0;i<8;i++)
			{
				if(!isOnLine)
					strSlaveStatus="        ";
				else if(masterCommand==cmdStop)
					strSlaveStatus=(char*)Checking;
				else if(masterCommand==cmdCheckFirst)
					strSlaveStatus=(char*)CheckDone;
				else if(masterCommand==cmdReadyAction)
					strSlaveStatus=(char*)ReadyFire;
				else if(masterCommand==cmdAction)
					strSlaveStatus=(char*)Firing;
				else if(isActionFinish[i])
					strSlaveStatus=(char*)FireDone;
				else 
					strSlaveStatus=(char*)FireError;
				
				LCDManager->DisplayTxt_FixedMode(FONT16,XChannelStatus,YChannel[i],strSlaveStatus);			

			}
		}
		else if(masterMode==energyMode)    //����ģʽ  ֻ��2ͨ��
		{
			for(int i=0;i<2;i++)
			{
				if(!isOnLine)
					strSlaveStatus="        ";
				else if((masterCommand==cmdStop) && isSameMode)  //��������ģʽ��ͬ
					strSlaveStatus=(char*)CheckDone;
				else if(masterCommand==cmdReadyAction)
					strSlaveStatus=(char*)ReadyCut;
				else if(masterCommand==cmdAction)
					strSlaveStatus=(char*)CutDone;
				
// 				else if(isActionFinish[i])
// 					strSlaveStatus=(char*)CutDone;
// 				else 
// 					strSlaveStatus=(char*)CutError;
				
				LCDManager->DisplayTxt_FixedMode(FONT16,XChannelStatus+40,YChannel[i],strSlaveStatus);			

			}
		
		}
		else if(masterMode==bridgeMode)   //����ģʽ
		{
			for(int i=0;i<8;i++)
			{
				if(!isOnLine)
					strSlaveStatus="        ";
				else if((masterCommand==cmdStop) && isSameMode) //��������ģʽ��ͬ
					strSlaveStatus=(char*)CheckDone;
				else if(masterCommand==cmdReadyAction)
					strSlaveStatus=(char*)ReadyShort;
				else if(masterCommand==cmdAction)
					strSlaveStatus=(char*)ShortDone;
				
// 				else if(isActionFinish[i])
// 					strSlaveStatus=(char*)CutDone;
// 				else 
// 					strSlaveStatus=(char*)CutError;
				
				LCDManager->DisplayTxt_FixedMode(FONT16,XChannelStatus+40,YChannel[i],strSlaveStatus);			

			}
		
		}
	
	}

}



/**
  * ��  �ܣ���ʾ������Ϣ
  * ��  ������
  * ����ֵ����
  */
void MasterCtrlApp::showAlarmInfo()
{
	//��ʾͨѶ������Ϣ
	static bool temIsOnLine=false;
	static bool temIsSameMode=false;
	if(temIsOnLine!=isOnLine)
	{
		temIsOnLine=isOnLine;	
		if(isOnLine)
		{
			LCDManager->DisplayTxt_FixedMode(FONT16,XAlarm_1,YAlarm_1,Alarm3);	
		}
		else
		{
			LCDManager->SetColor(REDCOLOR,BLACKCOLOR);//ǰ��ɫ��ɺ�ɫ
			LCDManager->DisplayTxt_FixedMode(FONT16,XAlarm_1,YAlarm_1,Alarm4);
			LCDManager->SetColor(GREENCOLOR,BLACKCOLOR);//ǰ��ɫ�����ɫ				
		}
	}
	//��ʾģʽ������Ϣ
	if(temIsSameMode!=isSameMode)
	{
		temIsSameMode=isSameMode;
		if(isSameMode)
		{
			LCDManager->DisplayTxt_FixedMode(FONT16,XAlarm_2,YAlarm_2,Alarm1);	
		}
		else
		{
			LCDManager->SetColor(REDCOLOR,BLACKCOLOR);//ǰ��ɫ��ɺ�ɫ
			LCDManager->DisplayTxt_FixedMode(FONT16,XAlarm_2,YAlarm_2,Alarm2);
			LCDManager->SetColor(GREENCOLOR,BLACKCOLOR);//ǰ��ɫ�����ɫ				
		}
	}
}
/**
  * ��  �ܣ���ʾ�ź�ָʾ��
  * ��  ������
  * ����ֵ����
  * 
  */
void MasterCtrlApp::showSignalIndicator()
{
	static uint16_t reversalIndicatorCount=0;
	
	if(isOnLine)
	{
		reversalIndicatorCount++;
		if(reversalIndicatorCount<=1)
			LCDManager->CutPicAndShow(10,80,2,15,15,300,1);    //��ʾ��ɫ��
		else 
			LCDManager->CutPicAndShow(10,60,20,15,15,300,1);    //��ʾ��ɫ��
		
		if(reversalIndicatorCount>=2)
			reversalIndicatorCount=0;
	}
	else
	{
		LCDManager->CutPicAndShow(10,100,20,15,15,300,1);      //��ʾ��ɫ��
	}

}
/**
  * ��  �ܣ���ʾ����LCD��Ϣ
  * ��  ������
  * ����ֵ����
  * 
  */
void MasterCtrlApp::showLCD()
{
	static uint8_t index=0;   //���ڷִ���ʾ
	if(index==0)
	{
		showMode();
		showMasterCommand();
		showProgressBar();
		showAlarmInfo();
		showVoltage();
		showSignalIndicator();
		index++;
	}
	else if(index==1)
	{
		//if(isOnLine)
		//����Ǵ���ģʽ����ʾ���裬������ʾ
		if(masterMode==powderMode)
			showSlaveResister();
		index++;
	}
	else if(index==2)
	{
		//if(isOnLine)
			showSlaveStatus1();
		
			signDropLine();
			signLandLine();
		
		index=0;
	}
}

//////////////////////////////////////////��ʾLCD����///////////////////////////////////////////////////////////////////


/**
  * ��  ��:�������߱�־
  * ��  ������
  * ����ֵ����
  * ʹ��˵����һ��Ҫ��֤�������ĵ���Ƶ������͵�
  */
void MasterCtrlApp::signLandLine()
{
	static bool temIsLandLine=true;
  isLandLine=false;
	if(temIsLandLine!=isOnLine)
	{
		temIsLandLine=isOnLine;
		if(isOnLine)   //��������	
		{
			isLandLine=true;
		}
	}
	
}

void MasterCtrlApp::signDropLine()
{
	static bool temIsDropLine=true;
	isDropLine=false;
	if(temIsDropLine!= (!isOnLine))
	{
		temIsDropLine=(!isOnLine);
		if(!isOnLine)
		{
			isDropLine=true;
		}
		
	}

}

/**
  * ��  �ܣ������ڶ�ʱ��2�еĺ���,��Ҫ�����Ǽ������
  * ��  ������
  * ����ֵ����
  * 
  */

void MasterCtrlApp::runOnTime2()
{
	getSlaveData();
	
}
/**
  * ��  �ܣ������ڶ�ʱ��3�еĺ���,��Ҫ�����Ǽ������
  * ��  ������
  * ����ֵ����
  * 
  */
void MasterCtrlApp::runOnTime3()
{
	getPowSample();	
	getCommand();
}


void MasterCtrlApp::runOnTime4()
{
//	dealSlaveData();
	updateSlavedata();//���͵��ӻ�
	showLCD();//Һ������ʾ
	//clearLCD();
	showKeyLed(); 
}





/**-------------------------------------------------------------------------------
  * ��  �ܣ�������ת��Ϊ�ַ���
  * ��  ����num
  * ����ֵ��
  *-----------------------------------------------------------------------------*/
void MasterCtrlApp::double2str(double num, char *str,int n,int size)
{
	char buf[10] = "";
	int i = 0;
	int len = 0;
	int temSize=size;
	int temp = num*pow(10.0, n);
	temp = temp < 0 ? -temp: temp;
	if (str == NULL)
	{
		return;
	}
	
	do
	{
		buf[i++] = (temp % 10) + '0';
		if(i==n)
			buf[i++] ='.';
		temp = temp / 10;
	}
	while(temp!=0||i<n+1);
	if(i==n+1)
		buf[i++] ='0';
	 len = num < 0 ? ++i: i;
	 if(temSize<len)
		 temSize=len;
	 for(int k=len;k<temSize;k++)
	 {
			str[k]=' ';//ʹ�ÿո�ռλ
	 }
	 str[temSize] = 0;//���λ����/0
	 while(1)
	 {
			 i--;
			 if (buf[len-i-1] ==0)
			 {
					 break;
			 }
			 str[i] = buf[len-i-1];
	 }
	 if (i == 0 )
	 {
			 str[i] = '-';
	 }
}

void MasterCtrlApp::int2str(int num, char *str,int size)
{
	int temSize=size;
	char buf[10] = "";
	int i = 0;
	int len = 0;
	int temp = num < 0 ? -num: num;
	 if (str == NULL)
	 {
			 return;
	 }
	 do
	 {
			 buf[i++] = (temp % 10) + '0';
			 temp = temp / 10;
	 }while(temp);
	 len = num < 0 ? ++i: i;
	 if(temSize<len)
		 temSize=len;
	 for(int k=len;k<temSize;k++)
	 {
			str[k]=' ';//ʹ�ÿո�ռλ
	 }
	 str[temSize] = 0;
	 while(1)
	 {
			 i--;
			 if (buf[len-i-1] ==0)
			 {
					 break;
			 }
			 str[i] = buf[len-i-1];
	 }
	 if (i == 0 )
	 {
			 str[i] = '-';
	 }
}


void MasterCtrlApp::uint162str_voltage(uint16_t num,char *str,int size)
{
	
	uint8_t i=0;
	
	if(num==0)
		str[i++] = '0';
	else
	{
				 if(num>=100)
						str[i++] = num/100+'0';  
				 str[i++] = (num%100)/10+'0';
				 str[i++] = '.';
				 str[i++] = num%10 + '0';
	 }
	 str[i++] =  'V';

	while(i<size)
		str[i++] = ' ';
	str[i]=0;	


}

void MasterCtrlApp::uint82str_res(uint8_t num,char *str,int size)
{
	uint8_t i=0;
	
	if(num==0)
		str[i++] = '0';
	else
	{
				 if(num>=100)
						str[i++] = num/100+'0';  
				 if(num>=10)
				 str[i++] = (num%100)/10+'0';			
				 
				 str[i++] = num%10 + '0';
	 }
	
	while(i<size)
		str[i++] = ' ';
	str[i]=0;	
}

