/**
  ******************************************************************************
	*�ļ���LCDdisplay.h
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ����СҺ����������Э���ͷ�ļ�
	*��ע��
  ******************************************************************************  
	*/ 

/* �����ֹ�ݹ�����Ŀ��ƺ�----------------------------------------------------*/
	#ifndef _LCDDISPLAY_H_
	#define _LCDDISPLAY_H_
	/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "SpiPort.h"
#include "STM32_SysTick.h"
#include "string.h"
/* �궨��------------------------------------------------------------------*/
/*********************************************************
�˿ڶ���
*********************************************************/
/* ���Ͷ��� --------------------------------------------------------------------*/




class LCDdisplay
{
public:
	LCDdisplay(SpiPort *);
	~LCDdisplay();

public:
	void clearScreen(uint8_t);				//����
	void enabledDisplay();						//ʹ����ʾ
	void disabledDisplay();						//������ʾ
  void setContrast(uint8_t);				//������Ļ�ĶԱȶ�
	bool setStartLine(int);						//������ʼ�е�ַ
	bool setPageAddress(uint8_t);			//����ҳ��ַ
	bool setColumnAddress(uint8_t);		//�����е�ַ
	bool setXY(uint8_t,uint8_t);			//������ʼ����
	void writeMediumChar(uint8_t,bool color=true,bool fill=true);			//��ʾ�к��ַ�6*8
	void writeMediumCharLine(uint8_t,uint8_t,const char*,uint8_t,bool color=true,bool fill=true,uint8_t start=0);			//��ʾ�к��ַ���

	void writeMediumChar_1(uint8_t,uint8_t,uint8_t,bool color=true,bool fill=true,bool underline=false);			//������ʾ�к��ַ�6*16
	void writeMediumCharLine_1(uint8_t,uint8_t,const char*,uint8_t,bool color=true,bool fill=true,bool underline=false);			//������ʾ�к��ַ���

	void writeLargeChar(uint8_t,uint8_t,uint8_t,bool color=true,bool fill=true,bool underline=false);			//��ʾ����ַ�8*16
	void writeLargeChar_1(uint8_t,uint8_t,uint8_t,bool color=true);			//��ʾ����ַ�10*16 ���½Ǵ���
	void writeLargeCharLine(uint8_t,uint8_t,const char*,uint8_t len=1,bool color=true,bool fill=true,bool underline=false);			//��ʾ����ַ���
	void writeHanzi(uint8_t,uint8_t,const char*,bool color=true,uint8_t fill=0,uint8_t underline=0);					//��ʾ����12*16
	void writeHanziLine(uint8_t,uint8_t,const char*,uint8_t len=1,bool color=true,uint8_t fill=0,uint8_t underline=false,uint8_t start=0);					//��ʾ�����ַ���
	void displayUpArrows(uint8_t xPos,uint8_t yPos,bool color,bool rim);
	void displayDownArrows(uint8_t xPos,uint8_t yPos,bool color,bool rim);
	void displayHoldOff(uint8_t xPos,uint8_t yPos);
	void displayVertical(uint8_t xPos,uint8_t yPos);
	void displayLevel(uint8_t xPos,uint8_t yPos);
	void displayHoldOn(uint8_t xPos,uint8_t yPos);
	void displayProgressBar(uint8_t xPos,uint8_t yPos,int8_t value);
	void disLargeNum(uint8_t xPos,uint8_t yPos,int8_t data,bool color=true,bool fill=true,bool underline=false);
	uint8_t disMediumNum(uint8_t xPos,uint8_t yPos,int16_t data,bool color=true,bool fill=true);
	void sendCommand(uint8_t);				//��������
	void sendData(uint8_t);						//��������
public:
// 	void sendCommand(uint8_t);				//��������
// 	void sendData(uint8_t);						//��������
	void LcdInit();



// 		void LcdCmd(u8 cmd);
// 		void LcdDat(u8 cmd); 
// 		void LcdDisplay(uint8_t on);
// 		void LcdSetPageAddress(uint8_t page);
// 		void LcdSetColumnAddress(uint8_t column);
// 		void LcdSetXy(u8 x,u8 y);
// 		void LcdSetStartLine(int line);
// 		void LcdContrast(u8 contrast);
// 		void LcdDelay(void);
// 	void Lcdclear();
// void LcdDrawStart(u16 x0, u16 y0, u16 x1, u16 y1, enum DrawDir _dir);
// void LcdDrawStop(void);
// 	void LcdDrawPixel(u16 color);
// void LcdDrawPixelXY(u16 x, u16 y, u16 color);
// 	void LcdDrawRLE(const u8 *data, int len, u32 color);
private:
		SpiPort *lcddisPort;				//��ʾLCD��ͨ��SPI��
	
};

#endif


