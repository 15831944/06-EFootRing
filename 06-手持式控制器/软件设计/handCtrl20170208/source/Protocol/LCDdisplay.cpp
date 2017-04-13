/**
  ******************************************************************************
	*�ļ���LCDdisplay.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ����СҺ����������Э�飬ʵ��СҺ��������ʾ
	*��ע��
  ******************************************************************************  
	*/ 
	/* ͷ�ļ����� ------------------------------------------------------------------*/
	
#include "LCDdisplay.h"




/**
  * ��  �ܣ�LCDdisplay��Ĺ��캯��
  * ��  ����
	* port��ͨ�ŵĴ��ں�
  * ����ֵ����
  */
LCDdisplay::LCDdisplay(SerialPort *port)
{
	lcddisPort=port;
	//��ʼ�����Ͳ����Ľṹ��
	lcdParameter=new LCDParameter();
	lcdParameter->head=0xaa;
	lcdParameter->command=0x00;
	lcdParameter->end[0]=0xcc;
	lcdParameter->end[1]=0x33;
	lcdParameter->end[2]=0xc3;
	lcdParameter->end[3]=0x3c;
	lcdParameter->len=0x00;

	for(int i=0;i<64;i++)
	{
		lcdParameter->buffer[i]=0x00;
	}
	//Speed=0;
}

/**
  * ��  �ܣ�LCDdisplay�����������
  * ��  ������
  * ����ֵ����
  */
LCDdisplay::~LCDdisplay()
{
	lcddisPort=NULL;
	delete lcdParameter;

}


/*************************************************************************************************
**������LCD���ֹ��ܵ���������
**
**************************************************************************************************/

void LCDdisplay::sendCommand()
{
	while(!(lcddisPort->putChar(lcdParameter->head)));				//����ͷ
	while(!(lcddisPort->putChar(lcdParameter->command)));			//����������
	lcddisPort->write(lcdParameter->buffer,lcdParameter->len);//������������
	lcddisPort->write(lcdParameter->end,4);										//����֡β
}

void LCDdisplay::SetSpace(uint8_t clum,uint8_t row) 								//�����ּ��
{
	lcdParameter->command=0x41;
  lcdParameter->buffer[0]=clum;
  lcdParameter->buffer[1]=row;
  lcdParameter->len=2;
	sendCommand();
}
void LCDdisplay::SetColor(uint16_t fcolor,uint16_t bcolor)					//���õ�ɫ��
{
  lcdParameter->command = 0x40;
  lcdParameter->buffer[0]=fcolor>>8;
  lcdParameter->buffer[1]=fcolor;
  lcdParameter->buffer[2]=bcolor>>8;
  lcdParameter->buffer[3]=bcolor;
  lcdParameter->len=4;
  sendCommand();
}
void LCDdisplay::GetColor(uint8_t mode,uint16_t x,uint16_t y)										//ȡָ��λ����ɫ����ɫ��
{
	if(mode==0)
		lcdParameter->command = 0x42;
	else
		lcdParameter->command = 0x43;
  lcdParameter->buffer[0]=x>>8;
  lcdParameter->buffer[1]=x;
  lcdParameter->buffer[2]=y>>8;
  lcdParameter->buffer[3]=y;
  lcdParameter->len=4;
  sendCommand();
}

void LCDdisplay::backlightOn()												//�򿪱���
{
  lcdParameter->command = 0x5f;
  lcdParameter->len=0;
  sendCommand();
}

void LCDdisplay::backlightOff()												//�رձ���
{
  lcdParameter->command = 0x5e;
  lcdParameter->len=0;
  sendCommand();
}

void LCDdisplay::PWMLight(uint8_t pwm)															//���ñ�������
{
	lcdParameter->command = 0x5f;
  lcdParameter->buffer[0]=pwm;
  lcdParameter->len = 0x01;
  sendCommand();
}
void LCDdisplay::ClearScreen(void)																	//�Ա���ɫ����		
{
  lcdParameter->command = 0x52;
  lcdParameter->len=0;
  sendCommand();
}
void LCDdisplay::DisplayTxt_FixedMode(uint8_t size,uint16_t x,uint16_t y,uint8_t *pdata,uint8_t Len)//��׼�ֿ���ʾ
{
	uint8_t i=0;
	lcdParameter->command   = size;
	lcdParameter->buffer[i++] = x>>8;
  lcdParameter->buffer[i++] = x;
  
  lcdParameter->buffer[i++] = y>>8;
  lcdParameter->buffer[i++] = y;
  while(Len--)
  { 
		lcdParameter->buffer[i++]=*pdata;
		pdata++;
	}
	lcdParameter->len = i;
  sendCommand();
}

void LCDdisplay::DisplayTxt_FixedMode(uint8_t size,uint16_t x,uint16_t y,const char *pdata)//��׼�ֿ���ʾ
{
	uint8_t i=0;
	lcdParameter->command   = size;
	lcdParameter->buffer[i++] = x>>8;
  lcdParameter->buffer[i++] = x;
  
  lcdParameter->buffer[i++] = y>>8;
  lcdParameter->buffer[i++] = y;
  while(*pdata)
  { 
		lcdParameter->buffer[i++]=*pdata;
		pdata++;
	}
	lcdParameter->len = i;
  sendCommand();
}

const uint8_t locktxt[7]={0x20,0x81,0x01,0xFF,0xE0,0x00,0x00}; 
void LCDdisplay::DisplayTxt(uint16_t x,uint16_t y,uint8_t *pdata,uint8_t Len)//ѡ���ֿ���ʾ	
{
	uint8_t i=0;
	lcdParameter->command = 0x98;		//����
  lcdParameter->buffer[0] = x>>8;
  lcdParameter->buffer[1] = x;
  
  lcdParameter->buffer[2] = y>>8;
  lcdParameter->buffer[3] = y;
  
  for(i=4;i<sizeof(locktxt)+4;i++)
  { 
    lcdParameter->buffer[i]=locktxt[i-4];
  }
   
  while(Len--)
  { 
    lcdParameter->buffer[i++] = *pdata;
    pdata++;
  }
 
  lcdParameter->len = i;
  sendCommand();
}

/**********************************************************
(Left,Top,Width,Height)
***********************************************************/
void LCDdisplay::CutPicAndShow(uint8_t Pic_ID ,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye,uint16_t X,uint16_t Y)//����ͼ����ʾ
{
  lcdParameter->command = 0x71;
  lcdParameter->buffer[0] = Pic_ID;
  
  lcdParameter->buffer[1] = Xs>>8;
  lcdParameter->buffer[2] = Xs;
  
  lcdParameter->buffer[3] = Ys>>8;
  lcdParameter->buffer[4] = Ys;
  
  lcdParameter->buffer[5] = (Xe+Xs)>>8;
  lcdParameter->buffer[6] = (Xe+Xs);
  
  lcdParameter->buffer[7] = (Ye+Ys)>>8;
  lcdParameter->buffer[8] = (Ye+Ys);
  //�ٴ���ʾ��λ��
  lcdParameter->buffer[9] = X>>8;
  lcdParameter->buffer[10] = X;
  
  lcdParameter->buffer[11] = Y>>8;
  lcdParameter->buffer[12] = Y;
  
  lcdParameter->len = 0x0D;
  sendCommand(); 
}

/**********************************************************
(Left,Top,Right,Buttom)
***********************************************************/
void LCDdisplay::CutPicAndShow_1(uint8_t Pic_ID ,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye,uint16_t X,uint16_t Y)//����ͼ����ʾ
{
  lcdParameter->command = 0x71;
  lcdParameter->buffer[0] = Pic_ID;
  
  lcdParameter->buffer[1] = Xs>>8;
  lcdParameter->buffer[2] = Xs;
  
  lcdParameter->buffer[3] = Ys>>8;
  lcdParameter->buffer[4] = Ys;
  
  lcdParameter->buffer[5] = Xe>>8;
  lcdParameter->buffer[6] = Xe;
  
  lcdParameter->buffer[7] = Ye>>8;
  lcdParameter->buffer[8] = Ye;
  //�ٴ���ʾ��λ��
  lcdParameter->buffer[9] = X>>8;
  lcdParameter->buffer[10] = X;
  
  lcdParameter->buffer[11] = Y>>8;
  lcdParameter->buffer[12] = Y;
  
  lcdParameter->len = 0x0D;
  sendCommand(); 
}
void LCDdisplay::ConnectTwoPoint(uint8_t cmd,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye)//��ֱ��
{
  lcdParameter->command = cmd;
  lcdParameter->buffer[0]=Xs>>8;
  lcdParameter->buffer[1]=Xs;
  lcdParameter->buffer[2]=Ys>>8;
  lcdParameter->buffer[3]=Ys;
   
  lcdParameter->buffer[4]=Xe>>8;
  lcdParameter->buffer[5]=Xe;
  lcdParameter->buffer[6]=Ye>>8;
  lcdParameter->buffer[7]=Ye;
  
  lcdParameter->len = 0x08;
  sendCommand();
}
void LCDdisplay::ShowFrequency(uint16_t X,uint16_t Y,uint8_t Hmax,uint8_t *Hi,uint8_t Len)//Ƶ����ʾ ����ͼ
{
  lcdParameter->command = 0x75;
  lcdParameter->buffer[0]=X>>8;
  lcdParameter->buffer[1]=X;
  lcdParameter->buffer[2]=Y>>8;
  lcdParameter->buffer[3]=Y;
  lcdParameter->buffer[4]=Hmax;
	for(int i=0;i<Len;i++)
		lcdParameter->buffer[5+i]=Hi[i];
  lcdParameter->len = 0x05+Len;
  sendCommand();
}
void LCDdisplay::DisRecentage(uint8_t cmd,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye)//����������ʾ
{
	lcdParameter->command = cmd;
	lcdParameter->buffer[0]=Xs>>8;
	lcdParameter->buffer[1]=Xs;
	lcdParameter->buffer[2]=Ys>>8;
	lcdParameter->buffer[3]=Ys;
	lcdParameter->buffer[4]=Xe>>8;
	lcdParameter->buffer[5]=Xe;
	lcdParameter->buffer[6]=Ye>>8;
	lcdParameter->buffer[7]=Ye;
	lcdParameter->len = 0x08;
	sendCommand();
}

void LCDdisplay::DisRound(uint8_t cmd,uint16_t type,uint16_t Xs,uint16_t Ys,uint16_t r)	//Բ����ʾ
{
  lcdParameter->command = cmd;
  lcdParameter->buffer[0]=type;
  lcdParameter->buffer[1]=Xs>>8;
  lcdParameter->buffer[2]=Xs;
  lcdParameter->buffer[3]=Ys>>8;
  lcdParameter->buffer[4]=Ys;
  lcdParameter->buffer[5]=r;
  lcdParameter->len = 0x06;
  sendCommand();
}

void LCDdisplay::movePic(uint8_t cmd,uint16_t Xs,uint16_t Ys,uint16_t Xe,uint16_t Ye,uint16_t step)																	//ָ�������ƶ�
{
  lcdParameter->command = cmd;
	lcdParameter->buffer[0]=Xs>>8;
	lcdParameter->buffer[1]=Xs;
	lcdParameter->buffer[2]=Ys>>8;
	lcdParameter->buffer[3]=Ys;
	lcdParameter->buffer[4]=Xe>>8;
	lcdParameter->buffer[5]=Xe;
	lcdParameter->buffer[6]=Ye>>8;
	lcdParameter->buffer[7]=Ye;
	lcdParameter->buffer[8]=step;
  lcdParameter->len = 0x09;
  sendCommand();
}

#define BAR_WIDTH 14
#define BAR_LONG 121
void LCDdisplay::ShowBar(uint16_t Xs,uint16_t Ys,uint8_t step)//��������ʾ
{
//   static unsigned char i=0;
//   if(i==0)
//   {
//     DisRecentage(0x59,Xs,Ys,Xs+BAR_LONG+2,Ys+BAR_WIDTH);
//     i=1;
//   }
  DisRecentage(0x5B,Xs+1,Ys+1,Xs+1+step,Ys+BAR_WIDTH-1);
  DisRecentage(0x5A,Xs+1+step,Ys+1,Xs+BAR_LONG,Ys+BAR_WIDTH-1);
	if(step==BAR_LONG-1)
		DisRecentage(0x5B,Xs+1+step,Ys+1,Xs+1+step,Ys+BAR_WIDTH-1);
		
}

//256ɫСҺ����û��С��̬������ʾ�Ĺ���
void LCDdisplay::ShowCurve(uint16_t X,uint16_t Ys,uint16_t Ye,uint16_t Y,uint16_t Fcolor,uint16_t BColor)						//С��̬������ʾ
{
  lcdParameter->command = 0x74;
  lcdParameter->buffer[0]=X>>8;
  lcdParameter->buffer[1]=X;
  lcdParameter->buffer[2]=Ys>>8;
  lcdParameter->buffer[3]=Ys;
  lcdParameter->buffer[4]=Ye>>8;
  lcdParameter->buffer[5]=Ye;
  lcdParameter->buffer[6]=BColor>>8;
  lcdParameter->buffer[7]=BColor;
  lcdParameter->buffer[8]=Y>>8;
  lcdParameter->buffer[9]=Y;
  lcdParameter->buffer[10]=Fcolor>>8;
  lcdParameter->buffer[11]=Fcolor;
  lcdParameter->len = 0x0C;
  sendCommand();
}
void LCDdisplay::ShowPic(uint8_t picnum)//��ʾͼƬ
{
	lcdParameter->command = 0x70;
	lcdParameter->buffer[0]=picnum;
	lcdParameter->len=0x01;
	sendCommand();
}
void LCDdisplay::ShakeHand(void)//����
{
  lcdParameter->command = 0x00;
  lcdParameter->len=0x00;
  sendCommand();
}


