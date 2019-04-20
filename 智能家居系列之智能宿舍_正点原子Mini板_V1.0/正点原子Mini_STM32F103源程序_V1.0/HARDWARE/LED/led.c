#include "led.h"



//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DS0_RCC, ENABLE);	 //使能PA 端口时钟

	GPIO_InitStructure.GPIO_Pin = DS0_Pin;				 //LED0-->PA.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(DS0_GPIO, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.8
	GPIO_SetBits(DS0_GPIO,DS0_Pin);						 //PA.8 输出高 
}


void Smart_Led_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(HC_SR501_1_RCC, ENABLE);	 	 //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = HC_SR501_1_Pin;		 //HC-SR501人体红外热释探头输入端（数字量）-->PA.1 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_Init(HC_SR501_1_GPIO, &GPIO_InitStructure);	 //根据设定参数初始化GPIOA.1
	
	RCC_APB2PeriphClockCmd(HC_SR501_2_RCC, ENABLE);	 	//使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = HC_SR501_2_Pin;		//HC-SR501人体红外热释探头输入端（数字量）-->PA.6 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入
	GPIO_Init(HC_SR501_2_GPIO, &GPIO_InitStructure);	//根据设定参数初始化GPIOA.6
	
	
	RCC_APB2PeriphClockCmd(SUN_RES_RCC, ENABLE);	    //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = SUN_RES_Pin;			//光敏传感器输入端（数字量）-->PA.3 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入
	GPIO_Init(SUN_RES_GPIO, &GPIO_InitStructure);		//根据设定参数初始化GPIOA.3
	
	RCC_APB2PeriphClockCmd(LED_RCC, ENABLE);			 //使能PA 端口时钟
	GPIO_InitStructure.GPIO_Pin = LED_Pin;				 //LED0-->PA.7 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);			 //根据设定参数初始化GPIOA.7
	GPIO_SetBits(LED_GPIO,LED_Pin);						 //PA.7 输出高 
	LED_OUT = 1;
			
}

void Number_Students_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(E18_D80NK1_RCC, ENABLE);	 	 //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = E18_D80NK1_Pin;		 //E18_D80NK红外避障输入端（数字量）-->PA.4 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_Init(E18_D80NK1_GPIO, &GPIO_InitStructure);	 //根据设定参数初始化GPIOA.4
	
	RCC_APB2PeriphClockCmd(E18_D80NK2_RCC, ENABLE);	 	//使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = E18_D80NK2_Pin;		//E18_D80NK红外避障输入端（数字量）-->PA.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//上拉输入
	GPIO_Init(E18_D80NK2_GPIO, &GPIO_InitStructure);	//根据设定参数初始化GPIOA.5
}

void Smoke_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BEEP_RCC, ENABLE);			 //使能PD 端口时钟
	GPIO_InitStructure.GPIO_Pin = BEEP_Pin;				 //BEEP-->PD.2 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);			 //根据设定参数初始化GPIOD.2
	GPIO_SetBits(BEEP_GPIO,BEEP_Pin);				     //PD.2 输出高 
	BEEP_OUT = 0;
	
	
	RCC_APB2PeriphClockCmd(FLY_RCC, ENABLE);			 //使能PA 端口时钟
	GPIO_InitStructure.GPIO_Pin = FLY_Pin;				 //-->PA.12 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(FLY_GPIO, &GPIO_InitStructure);			 //根据设定参数初始化GPIOA.12
	FLY_OUT = 0;
	
	//烟雾
	RCC_APB2PeriphClockCmd(MQ_RCC, ENABLE);	 	 		//使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = MQ_Pin;		 		//MQ 烟雾报警传感器（数字量）-->PA.11 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
	GPIO_Init(MQ_GPIO, &GPIO_InitStructure);	 		//根据设定参数初始化GPIOA.11
	
	
	
	
}

