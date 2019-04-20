#include "led.h"



//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DS0_RCC, ENABLE);	 //ʹ��PA �˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = DS0_Pin;				 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(DS0_GPIO, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.8
	GPIO_SetBits(DS0_GPIO,DS0_Pin);						 //PA.8 ����� 
}


void Smart_Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(HC_SR501_1_RCC, ENABLE);	 	 //ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = HC_SR501_1_Pin;		 //HC-SR501�����������̽ͷ����ˣ���������-->PA.1 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_Init(HC_SR501_1_GPIO, &GPIO_InitStructure);	 //�����趨������ʼ��GPIOA.1
	
	RCC_APB2PeriphClockCmd(HC_SR501_2_RCC, ENABLE);	 	//ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = HC_SR501_2_Pin;		//HC-SR501�����������̽ͷ����ˣ���������-->PA.6 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
	GPIO_Init(HC_SR501_2_GPIO, &GPIO_InitStructure);	//�����趨������ʼ��GPIOA.6
	
	
	RCC_APB2PeriphClockCmd(SUN_RES_RCC, ENABLE);	    //ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = SUN_RES_Pin;			//��������������ˣ���������-->PA.3 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
	GPIO_Init(SUN_RES_GPIO, &GPIO_InitStructure);		//�����趨������ʼ��GPIOA.3
	
	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);			 //ʹ��PA �˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = LED_Pin;				 //LED0-->PA.7 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);			 //�����趨������ʼ��GPIOA.7
	GPIO_SetBits(LED_GPIO,LED_Pin);						 //PA.7 ����� 
	LED_OUT = 1;
			
}

void Number_Students_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(E18_D80NK1_RCC, ENABLE);	 	 //ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = E18_D80NK1_Pin;		 //E18_D80NK�����������ˣ���������-->PA.4 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_Init(E18_D80NK1_GPIO, &GPIO_InitStructure);	 //�����趨������ʼ��GPIOA.4
	
	RCC_APB2PeriphClockCmd(E18_D80NK2_RCC, ENABLE);	 	//ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = E18_D80NK2_Pin;		//E18_D80NK�����������ˣ���������-->PA.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
	GPIO_Init(E18_D80NK2_GPIO, &GPIO_InitStructure);	//�����趨������ʼ��GPIOA.5
}

void Smoke_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_RCC, ENABLE);			 //ʹ��PD �˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = BEEP_Pin;				 //BEEP-->PD.2 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);			 //�����趨������ʼ��GPIOD.2
	GPIO_SetBits(BEEP_GPIO,BEEP_Pin);				     //PD.2 ����� 
	BEEP_OUT = 0;
	
	
	RCC_APB2PeriphClockCmd(FLY_RCC, ENABLE);			 //ʹ��PA �˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = FLY_Pin;				 //-->PA.12 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_Init(FLY_GPIO, &GPIO_InitStructure);			 //�����趨������ʼ��GPIOA.12
	FLY_OUT = 0;
	
	//����
	RCC_APB2PeriphClockCmd(MQ_RCC, ENABLE);	 	 		//ʹ��PA�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = MQ_Pin;		 		//MQ ����������������������-->PA.11 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_Init(MQ_GPIO, &GPIO_InitStructure);	 		//�����趨������ʼ��GPIOA.11
	
	
	
	
}

