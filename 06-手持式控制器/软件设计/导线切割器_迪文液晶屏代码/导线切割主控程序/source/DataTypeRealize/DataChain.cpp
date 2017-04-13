/**
  ******************************************************************************
	*�ļ���DataChain.c
	*���ߣ�Ҷ��˳
	*�汾��2.0
	*���ڣ�2013-06-17
	*��Ҫ������ṹ���ʵ��
	*��ע��
	*
  ******************************************************************************  
	*/ 

#include "DataChain.h"

//���캯��
RingChain::RingChain(int len)
{
	if(len<1)
	{
		current=NULL;
		return;
	}
	length=len;
	current=new ChainNode(0,NULL,NULL);
	current->m_next=current;
	current->m_previous=current;
	for(int i=0;i<len-1;i++)
	{
		current->m_next=new ChainNode(0,current,current->m_next);
		current->m_next->m_next->m_previous=current->m_next;
	}
}

//��������
RingChain::~RingChain()
{
// 	delete [] Chains;
	ChainNode *q,*p=current->m_next;	
	while(p!=current)
	{
		q=p->m_next;
		delete (p);
		p=q;
	}
	delete (current);
// 	current=NULL;
}


/**
  * ��  �ܣ���ȡ��ǰ�ڵ������
  * ��  ������
  * ����ֵ����ǰ�ڵ������ֵ
  * 
  */
DATATYPE RingChain::getData()
{
	return current->data;
}

/**
  * ��  �ܣ�����ǰ�ڵ�ָ��ָ���¸��ڵ�
  * ��  ������
  * ����ֵ����
  * 
  */
void RingChain::next()
{
	current=current->m_next;
}

/**
  * ��  �ܣ�����ǰ�ڵ�ָ��ָ���ϸ��ڵ�
  * ��  ������
  * ����ֵ����
  * 
  */
void RingChain::previous()
{
	current=current->m_previous;
}

/**
  * ��  �ܣ���ȡ����ĳ���
  * ��  ������
  * ����ֵ������ĳ���
  * 
  */
int RingChain::getLen()
{
	return length;
}

/**
  * ��  �ܣ����õ�ǰ�ڵ������
  * ��  �������趨��ֵ
  * ����ֵ����
  * 
  */
void RingChain::setData(DATATYPE value)
{
	current->data=value;
}

