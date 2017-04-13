/**
  ******************************************************************************
	*�ļ���LCDdisplay.h
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ����СҺ����������Э���ͷ�ļ�
	*��ע���˰汾��Թ����û������Բ����棬
  ******************************************************************************  
	*/ 

/* �����ֹ�ݹ�����Ŀ��ƺ�----------------------------------------------------*/
	#ifndef _LCDDISPLAY_H_
	#define _LCDDISPLAY_H_
	/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "SerialPort.h"

/* ���Ͷ��� --------------------------------------------------------------------*/

//LCDͨ�Ų����Ľṹ��
typedef struct Parameter
{
  uint8_t head;				//֡ͷ
  uint8_t command;		//������
  uint8_t buffer[64];	//����
  uint8_t end[4];    	//֡β
  uint8_t len;	   		//���ݳ���
}LCDParameter;
/* �궨��-----------------------------------------------------------------------*/ 

class LCDdisplay
{
public:
	LCDdisplay(SerialPort *);
	~LCDdisplay();


public:

	void ShowPic(uint8_t picnum);														//��ʾͼƬ



	void SetColor(uint16_t fcolor,uint16_t bcolor);					//���õ�ɫ��
	void ShowBar(uint16_t Xs,uint16_t Ys,uint8_t step);			//��������ʾ
	void DisplayTxt_FixedMode(uint8_t,uint16_t x,uint16_t y,uint8_t *pdata,uint8_t Len);//��׼�ֿ���ʾ
	void DisRecentage(uint8_t cmd,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye);														//����������ʾ
	void CutPicAndShow(uint8_t Pic_ID ,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye,uint16_t X,uint16_t Y);//����ͼ����ʾ
	void movePic(uint8_t,uint16_t,uint16_t,uint16_t,uint16_t,uint16_t);																	//ָ�������ƶ�
	void setLanguageBuf(uint8_t);																																				//�������� 0x00�� Ӣ�ģ� ����������
	void readLanguageBuf();																		//����0 Ӣ�� ���� ����
	void backlightOn();																				//�򿪱���
	void backlightOff();																			//�رձ���
	void ShakeHand(void);																			//����
	uint8_t riceveData();																			//��������

	void SetSpace(uint8_t clum,uint8_t row); 								//�����ּ��
	void GetColor(uint8_t,uint16_t x,uint16_t y);						//ȡָ��λ����ɫ����ɫ��
	void PWMLight(uint8_t pwm);															//���ñ�������
	void ClearScreen(void);																	//�Ա���ɫ����		
	void DisplayTxt(uint16_t x,uint16_t y,uint8_t *pdata,uint8_t Len);					//ѡ���ֿ���ʾ	
	void ConnectTwoPoint(uint8_t cmd,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye);												//��ֱ��
	void ShowFrequency(uint16_t X,uint16_t Y,uint8_t Hmax,uint8_t* Hi,uint8_t Len);		  											//Ƶ����ʾ ����ͼ
	void DisRound(uint8_t cmd,uint16_t type,uint16_t Xs,uint16_t Ys,uint16_t r);															//Բ����ʾ
	void ShowCurve(uint16_t X,uint16_t Ys,uint16_t Ye,uint16_t Y,uint16_t Fcolor,uint16_t BColor);						//С��̬������ʾ
	void CutPicAndShow_1(uint8_t Pic_ID ,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye,uint16_t X,uint16_t Y);//����ͼ����ʾ
	void DisplayTxt_FixedMode(uint8_t size,uint16_t x,uint16_t y,const char *pdata);
private:
	LCDParameter *lcdParameter;		//LCD����ṹ��
	SerialPort *lcddisPort;				//��ʾLCD��ͨ�Ŵ���
	void sendCommand();          //����������LCD������ͨ����������
};
	
	
	#endif


