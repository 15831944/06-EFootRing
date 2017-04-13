
/**
  ******************************************************************************
	*�ļ���sqStack.cpp
	*���ߣ�������
	*�汾��1.0
	*���ڣ�2016-12-15
	*��Ҫ��˳��ջ��ʵ��
	*��ע����ʵ��FILO�����ݴ���ͬʱ��������ı���ջ������
	*
  ******************************************************************************  
	*/ 
	
#include "sqStack.h"

SqStack::SqStack()
{
	st.top=-1;
	
};

//void SqStack::InitStack()

int SqStack::Push(ElemType x)
{
	if(st.top==STACKSIZE-1)  //ջ��
		return 0;
	else
	{
		st.top++;
		st.data[st.top]=x;
		return 1;
	}
	
}

int SqStack::Pop(ElemType &x)
{
	if(st.top==-1)        //ջ��
		return 0;
	else
	{
		x=st.data[st.top];
		st.top--;
		return 1;			
	}	
}

ElemType SqStack::Pop()
{
	if(st.top==-1)        //ջ��
		return 0;
	else
	{
		return 	st.data[st.top--];	//ע��һ����Ҫ�ȷ��أ����Լ�		
	}	
}

int SqStack::GetTopElem(ElemType &x)
{
	if(st.top==-1)        //ջ��
		return 0;
	else
	{
		x=st.data[st.top];
		return 1;
	}
}

ElemType SqStack::GetTopElem()
{
	if(st.top==-1)        //ջ��
		return 0;
	else
	{
		return st.data[st.top];		
	}
}

int SqStack::GetTop()
{
	return st.top;
}

bool SqStack::IsEmpty()
{
	if(st.top==-1)
		return true;
	else
		return false;
}

bool SqStack::IsFull()
{
	if(st.top==STACKSIZE-1)  //ջ��
		return true;
	else
	{
		return false;
	}
}

void SqStack::ClearStack()
{
	//���ջ
	st.top=-1;	
		
}


