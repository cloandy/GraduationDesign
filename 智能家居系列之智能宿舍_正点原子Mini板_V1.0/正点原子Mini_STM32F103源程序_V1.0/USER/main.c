#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "rtc.h"
#include "exti.h"
#include "timer.h"
#include "key.h"
#include "tsensor.h"

u8 FLAG_HCSR501 = 0;
u8 FLAG_SUNRES  = 0;
u8 FLAG_H = 0;

extern u8 FLAG_1;
extern u8 FLAG_2;

int number = 0; //宿舍人数，宿舍人数小于等于8

//温度传感器参数
float temp;
float temperate;

short temper;

int Flag = 0;	//自动控制标志
u8 FlagSun_LED = 0;//LED控制标志
u8 Flag_timer = 0;


void Func_Init(void);
void RTC_Show(void);
void LCD_Show(void);
void Smart_LED(void);
void Number_Students(void);
void Temper_Show(void);
void Smoke_Detect(void);


 int main(void)
 { 
	u8 t;
	u16 cnt; 
	u16 flag = 0;
	u8 nu;
	 
	
 	Func_Init();
	Smart_Led_Init(); 
	Number_Students_Init();
	EXTIX_Init();
	TIM4_Int_Init(14999,7199);//10Khz的计数频率，计数到15000为1.5s  
	TIM3_Int_Init(14999,7199);//10Khz的计数频率，计数到15000为1.5s  
	TIM2_Int_Init(14999,7199);//10Khz的计数频率，计数到15000为1.5s  

	 //使用内置的温度传感器
	T_Adc_Init();

	 //使用外置的DS18B20
//	while(DS18B20_Init())	//DS18B20初始化	
//	{
//		LCD_ShowString(60,130,200,16,16,"DS18B20 Error");
//		delay_ms(200);
//		LCD_Fill(60,130,239,130+16,WHITE);
// 		delay_ms(200);
//	}		

	Smoke_Init();
	 
	POINT_COLOR=BLUE;//设置字体为蓝色	 
	while(1)
	{
		
		nu=KEY_Scan(0);		//得到键值
		switch(nu)
		{				 
			case KEY0_PRES:
				Flag = !Flag;
				break;
			case KEY1_PRES:
				if(Flag == 1)
					LED_OUT = !LED_OUT;
				break;
			case WKUP_PRES:	
				 if(Flag == 1)
				 {
					BEEP_OUT = !BEEP_OUT;
					FLY_OUT = !FLY_OUT;		
				 }
						
				break;
			default:
				delay_ms(10);	
		} 
		if(flag == 10)//0.1s
		{
			Temper_Show();
			#if 0
			printf("HC_SR501_1 Value is %d.\t",HC_SR501_1_INPUT);
			printf("HC_SR501_2 Value is %d.\t",HC_SR501_2_INPUT);
			printf("SUN_RES Value is %d.\t",SUN_RES_INPUT);
			
			printf("E18_D80NK1 Value is %d.\t",E18_D80NK1_INPUT);
			printf("E18_D80NK2 Value is %d.\r\n",E18_D80NK2_INPUT);
			
			#else
			LCD_ShowNum(160,130,HC_SR501_1_INPUT,4,16);	
			LCD_ShowNum(160,150,HC_SR501_2_INPUT,4,16);	
			LCD_ShowNum(160,170,SUN_RES_INPUT,4,16);	
			LCD_ShowNum(160,190,E18_D80NK1_INPUT,4,16);	
			LCD_ShowNum(160,210,E18_D80NK2_INPUT,4,16);	
			LCD_ShowNum(160,230,     number     ,4,16);	
			
			//LCD_ShowNum(160,250,   temper/10  ,4,16);
			LCD_ShowNum(160,250,  (u8)temperate ,4,16);	
			LCD_ShowNum(160,270,   MQ_INPUT     ,4,16);	
			LCD_ShowNum(160,290,     Flag       ,4,16);	
			
			#endif
		}
		if(flag == 50) //0.5s
		{
			Smart_LED();
			Smoke_Detect();
			flag = 0;
			
		}
		if(cnt == 500)//5s发送一次数据
		{
			printf("%d %d\r\n",number,LED_OUT);
			cnt = 0;
		}
		if(t!=calendar.sec) //程序运行指示灯1S闪
		{
			t=calendar.sec;
			 
			RTC_Show();

			DS0_OUT=!DS0_OUT;
		}	
		flag ++;
		cnt ++;
	}  											    
}	



void Func_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);	 	//串口初始化为9600
	LED_Init();				//初始化与LED连接的硬件接口
	KEY_Init();
 	LCD_Init();				//初始化LCD				 	
	POINT_COLOR=RED;//设置字体为红色 
	LCD_ShowString(60,10,200,16,16,"GRADUATION DESIGN");	
	LCD_ShowString(60,30,200,16,16,"-----------------");
	LCD_ShowString(60,40,200,16,16,"2019/04/08 Test");	
	while(RTC_Init())		//RTC初始化	，一定要初始化成功
	{ 
		LCD_ShowString(60,130,200,16,16,"RTC ERROR!   ");	
		delay_ms(800);
		LCD_ShowString(60,130,200,16,16,"RTC Trying...");	
	}		    						
	//显示时间
	POINT_COLOR=BLUE;//设置字体为蓝色					 
	LCD_ShowString(80,60,200,16,16,"    -  -     ");	   
	LCD_ShowString(80,82,200,16,16,"  :  :  ");	
	
	LCD_Show();
}

void RTC_Show(void)
{
	LCD_ShowNum(80,60,calendar.w_year,4,16);									  
	LCD_ShowNum(120,60,calendar.w_month,2,16);									  
	LCD_ShowNum(144,60,calendar.w_date,2,16);	
	switch(calendar.week)
	{
		case 0:
			LCD_ShowString(80,108,200,16,16,"Sunday   ");
			break;
		case 1:
			LCD_ShowString(80,108,200,16,16,"Monday   ");
			break;
		case 2:
			LCD_ShowString(80,108,200,16,16,"Tuesday  ");
			break;
		case 3:
			LCD_ShowString(80,108,200,16,16,"Wednesday");
			break;
		case 4:
			LCD_ShowString(80,108,200,16,16,"Thursday ");
			break;
		case 5:
			LCD_ShowString(80,108,200,16,16,"Friday   ");
			break;
		case 6:
			LCD_ShowString(80,108,200,16,16,"Saturday ");
			break;  
	}
	LCD_ShowNum(80,82,calendar.hour,2,16);									  
	LCD_ShowNum(104,82,calendar.min,2,16);									  
	LCD_ShowNum(128,82,calendar.sec,2,16);
}


void LCD_Show(void)
{
	LCD_ShowString(20,130,200,16,16,"HC_SR501_1_INPUT:");
	LCD_ShowString(20,150,200,16,16,"HC_SR501_1_INPUT:");
	LCD_ShowString(20,170,200,16,16,"  SUN_RES_INPUT :");
	LCD_ShowString(20,190,200,16,16,"E18_D80NK1_INPUT:");
	LCD_ShowString(20,210,200,16,16,"E18_D80NK2_INPUT:");
	
	LCD_ShowString(20,230,200,16,16,"Number_Students:");
	
	LCD_ShowString(20,250,200,16,16,"Number_Temper  :");
	LCD_ShowString(20,270,200,16,16,"Number_MQ_2    :");
	LCD_ShowString(20,290,200,16,16,"Automation_Flag:");
}
 	

void Smart_LED(void)
{
	if(Flag == 0) //自动控制模式
	{
		if(number)//在有人的情况下,进行时间判断和光照判断
		{
			if(calendar.hour >= 18) //时间上的判断处于傍晚
			{
				FLAG_H = 1;			
			}
			else if(calendar.hour <= 6)//时间上的判断处于早晨
			{
				FLAG_H = 1;
			}
			else						//其他时间处于白天
			{
				FLAG_H = 0;
			}//时间判断
			if(Flag_timer == 0)			//第一次进入判断或者等到LED照明的时间结束时在进行判断
			{	
				TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
				if(1 == SUN_RES_INPUT)
				{
					FLAG_SUNRES = 1;	
				}//光照强度判断
				else
					FLAG_SUNRES = 0;
			}
		}
		else if(!number)
		{
			FLAG_H = 0;
			FLAG_SUNRES = 0;
			TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
		}
		
		if((1 == FLAG_H))
		{
			LED_OUT = 0;
		}
		if((1 == FLAG_SUNRES))
		{
			if(0 == FLAG_H)
				TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
		}
		if((0 == FLAG_H) && (0 == FLAG_SUNRES))
		{
			LED_OUT = 1;
		}
	}
}


void Temper_Show(void)
{
	u16 adcx;
	
	adcx=T_Get_Adc_Average(ADC_CH_TEMP,10);
	temp=(float)adcx*(3.3/4096);
	
	temperate=temp;//保存温度传感器的电压值
	temperate=(1.43-temperate)/0.0043+25;		//计算出当前温度值
	
	//temper = DS18B20_Get_Temp();	
	
	
}

void Smoke_Detect(void)
{
	if(Flag == 0)
	{
		if((MQ_INPUT == 0) || ((u8)temperate > 40))
		{
			delay_ms(100);
			if((MQ_INPUT == 0) || ((u8)temperate > 40))
			{
				BEEP_OUT = 1;
				FLY_OUT = 1;
			}
		}
		else
		{
			BEEP_OUT = 0;
			FLY_OUT = 0;
		}
	}
}

