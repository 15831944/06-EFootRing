
/**
  ******************************************************************************
	*�ļ���sqStack.cpp
	*���ߣ�������
	*�汾��1.0
	*���ڣ�2016-12-15
	*��Ҫ��˳��ջ��ͷ�ļ�
	*��ע��
	*
  ******************************************************************************  
	*/ 


#ifndef SQSTACK_H
#define SQSTACK_H

#define STACKSIZE  5
typedef unsigned char ElemType;

//˳�����
typedef struct 
{
	ElemType data[STACKSIZE];  //����ջԪ��
	int top;   //ջ��
}SqStackType;


//ѭ������
class SqStack
{
public:
	SqStack();
//	void InitStack(SqStackType &st);  //��ʼ��һ����ջ
	int Push( ElemType x);  //��ջ
	int Pop( ElemType &x);   //��ջ
	ElemType Pop();          //��ջ
	int GetTopElem( ElemType &x);   //��ȡջ��Ԫ��
	ElemType GetTopElem();          //����ջ��Ԫ��
	int GetTop();   //����ջ��
	void ClearStack();

	bool IsEmpty();    //�ж�Ϊ��
	bool IsFull();  //�ж϶���


public:
	SqStackType st;

};


#endif  //SQSTACK_H

