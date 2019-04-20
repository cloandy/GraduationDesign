#include "exti.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

extern int number; //������������������С�ڵ���8

u8 FLAG_1 = 0;
u8 FLAG_2 = 0;
  
 
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{
 
	EXTI_InitTypeDef EXTI_InitStructure;
	

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��
	
//	//GPIOA.3 �ж����Լ��жϳ�ʼ������
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);

//  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	
	
    //GPIOA.4 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOA.5	  �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	//EXTI3_NVIC_Config(ENABLE);
	EXTI4_NVIC_Config(ENABLE);
	EXTI5_NVIC_Config(ENABLE);


}

//void EXTI3_NVIC_Config(FunctionalState Cmd)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = Cmd;								//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//}

void EXTI4_NVIC_Config(FunctionalState Cmd)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = Cmd;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void EXTI5_NVIC_Config(FunctionalState Cmd)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = Cmd;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


//void EXTI3_IRQHandler(void)
//{
//	printf("HLL\r\n");
//	EXTI_ClearITPendingBit(EXTI_Line3);    //���LINE5�ϵ��жϱ�־λ
//	//EXTI3_NVIC_Config(DISABLE);
//}

void EXTI4_IRQHandler(void)
{
	
	FLAG_1 = 1;
	
	if((FLAG_1 == 1 ) && (FLAG_2 == 0))
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
	
	EXTI_ClearITPendingBit(EXTI_Line4);    //���LINE5�ϵ��жϱ�־λ
	EXTI4_NVIC_Config(DISABLE);
}
void EXTI9_5_IRQHandler(void)
{
	
	FLAG_2 = 1;
	if((FLAG_2 == 1 ) && (FLAG_1 == 0))
		TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
	
	EXTI_ClearITPendingBit(EXTI_Line5);    //���LINE5�ϵ��жϱ�־λ
	EXTI5_NVIC_Config(DISABLE);
}

