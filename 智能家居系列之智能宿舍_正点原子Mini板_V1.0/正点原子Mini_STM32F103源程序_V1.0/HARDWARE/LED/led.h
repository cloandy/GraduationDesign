#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

/**************程序运行指示灯******************/
#define DS0_RCC		RCC_APB2Periph_GPIOA
#define DS0_Pin		GPIO_Pin_8
#define DS0_GPIO	GPIOA
#define DS0_OUT		PAout(8)	// PA8	

/************智能灯光管理系统***********************************************/
//HC_SR501_1人体红外
#define HC_SR501_1_RCC		RCC_APB2Periph_GPIOA
#define HC_SR501_1_Pin		GPIO_Pin_1
#define HC_SR501_1_GPIO		GPIOA
#define HC_SR501_1_INPUT	GPIO_ReadInputDataBit(HC_SR501_1_GPIO,HC_SR501_1_Pin)//读取输入值
#define HC_SR501_1_ON		1
#define HC_SR501_1_OFF		0

//HC_SR501人体红外
#define HC_SR501_2_RCC		RCC_APB2Periph_GPIOA
#define HC_SR501_2_Pin		GPIO_Pin_6
#define HC_SR501_2_GPIO		GPIOA
#define HC_SR501_2_INPUT	GPIO_ReadInputDataBit(HC_SR501_2_GPIO,HC_SR501_2_Pin)//读取输入值
#define HC_SR501_2_ON		1
#define HC_SR501_2_OFF		0

//光敏电阻
#define SUN_RES_RCC			RCC_APB2Periph_GPIOA
#define SUN_RES_Pin			GPIO_Pin_3
#define SUN_RES_GPIO		GPIOA
#define SUN_RES_INPUT		GPIO_ReadInputDataBit(SUN_RES_GPIO,SUN_RES_Pin)//读取输入值
#define SUN_RES_ON			1
#define SUN_RES_OFF			0

//LED灯 PA7
#define LED_RCC				RCC_APB2Periph_GPIOA
#define LED_Pin				GPIO_Pin_7
#define LED_GPIO			GPIOA
#define LED_OUT				PAout(7)


/*******************人数智能监控系统************************************/
#define	E18_D80NK1_RCC		RCC_APB2Periph_GPIOA
#define	E18_D80NK1_Pin		GPIO_Pin_4
#define	E18_D80NK1_GPIO		GPIOA
#define	E18_D80NK1_INPUT	GPIO_ReadInputDataBit(E18_D80NK1_GPIO,E18_D80NK1_Pin)	
#define	E18_D80NK1_ON		1
#define	E18_D80NK1_OFF		0

#define	E18_D80NK2_RCC		RCC_APB2Periph_GPIOA
#define	E18_D80NK2_Pin		GPIO_Pin_5
#define	E18_D80NK2_GPIO		GPIOA
#define	E18_D80NK2_INPUT	GPIO_ReadInputDataBit(E18_D80NK2_GPIO,E18_D80NK2_Pin)
#define	E18_D80NK2_ON		1
#define	E18_D80NK2_OFF		0

/*******************室内防火报警************************************/
#define BEEP_RCC			RCC_APB2Periph_GPIOD
#define BEEP_Pin			GPIO_Pin_2
#define BEEP_GPIO			GPIOD
#define BEEP_OUT			PDout(2)

#define FLY_RCC				RCC_APB2Periph_GPIOA
#define FLY_Pin				GPIO_Pin_12
#define FLY_GPIO			GPIOA
#define FLY_OUT				PAout(12)


#define	MQ_RCC				RCC_APB2Periph_GPIOA
#define	MQ_Pin				GPIO_Pin_11
#define	MQ_GPIO				GPIOA
#define MQ_INPUT			GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define	MQ_ON				1
#define	MQ_OFF				0


void LED_Init(void);//初始化
void Smart_Led_Init(void);
void Number_Students_Init(void);
void Smoke_Init(void);
		 				    
#endif
