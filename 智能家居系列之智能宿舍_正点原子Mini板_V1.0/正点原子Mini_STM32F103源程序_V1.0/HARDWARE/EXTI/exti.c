#include "exti.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h"

extern int number; //宿舍人数，宿舍人数小于等于8

u8 FLAG_1 = 0;
u8 FLAG_2 = 0;
  
 
//外部中断初始化函数
void EXTIX_Init(void)
{
 
	EXTI_InitTypeDef EXTI_InitStructure;
	

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟
	
//	//GPIOA.3 中断线以及中断初始化配置
//  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);

//  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	
	
    //GPIOA.4 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.5	  中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	//EXTI3_NVIC_Config(ENABLE);
	EXTI4_NVIC_Config(ENABLE);
	EXTI5_NVIC_Config(ENABLE);


}

//void EXTI3_NVIC_Config(FunctionalState Cmd)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = Cmd;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
//}

void EXTI4_NVIC_Config(FunctionalState Cmd)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = Cmd;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void EXTI5_NVIC_Config(FunctionalState Cmd)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = Cmd;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


//void EXTI3_IRQHandler(void)
//{
//	printf("HLL\r\n");
//	EXTI_ClearITPendingBit(EXTI_Line3);    //清除LINE5上的中断标志位
//	//EXTI3_NVIC_Config(DISABLE);
//}

void EXTI4_IRQHandler(void)
{
	
	FLAG_1 = 1;
	
	if((FLAG_1 == 1 ) && (FLAG_2 == 0))
		TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	
	EXTI_ClearITPendingBit(EXTI_Line4);    //清除LINE5上的中断标志位
	EXTI4_NVIC_Config(DISABLE);
}
void EXTI9_5_IRQHandler(void)
{
	
	FLAG_2 = 1;
	if((FLAG_2 == 1 ) && (FLAG_1 == 0))
		TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
	
	EXTI_ClearITPendingBit(EXTI_Line5);    //清除LINE5上的中断标志位
	EXTI5_NVIC_Config(DISABLE);
}

