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

int number = 0; //������������������С�ڵ���8

//�¶ȴ���������
float temp;
float temperate;

short temper;

int Flag = 0;	//�Զ����Ʊ�־
u8 FlagSun_LED = 0;//LED���Ʊ�־
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
	TIM4_Int_Init(14999,7199);//10Khz�ļ���Ƶ�ʣ�������15000Ϊ1.5s  
	TIM3_Int_Init(14999,7199);//10Khz�ļ���Ƶ�ʣ�������15000Ϊ1.5s  
	TIM2_Int_Init(14999,7199);//10Khz�ļ���Ƶ�ʣ�������15000Ϊ1.5s  

	 //ʹ�����õ��¶ȴ�����
	T_Adc_Init();

	 //ʹ�����õ�DS18B20
//	while(DS18B20_Init())	//DS18B20��ʼ��	
//	{
//		LCD_ShowString(60,130,200,16,16,"DS18B20 Error");
//		delay_ms(200);
//		LCD_Fill(60,130,239,130+16,WHITE);
// 		delay_ms(200);
//	}		

	Smoke_Init();
	 
	POINT_COLOR=BLUE;//��������Ϊ��ɫ	 
	while(1)
	{
		
		nu=KEY_Scan(0);		//�õ���ֵ
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
		if(cnt == 500)//5s����һ������
		{
			printf("%d %d\r\n",number,LED_OUT);
			cnt = 0;
		}
		if(t!=calendar.sec) //��������ָʾ��1S��
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	delay_init();	    	 //��ʱ������ʼ��	  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();
 	LCD_Init();				//��ʼ��LCD				 	
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,10,200,16,16,"GRADUATION DESIGN");	
	LCD_ShowString(60,30,200,16,16,"-----------------");
	LCD_ShowString(60,40,200,16,16,"2019/04/08 Test");	
	while(RTC_Init())		//RTC��ʼ��	��һ��Ҫ��ʼ���ɹ�
	{ 
		LCD_ShowString(60,130,200,16,16,"RTC ERROR!   ");	
		delay_ms(800);
		LCD_ShowString(60,130,200,16,16,"RTC Trying...");	
	}		    						
	//��ʾʱ��
	POINT_COLOR=BLUE;//��������Ϊ��ɫ					 
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
	if(Flag == 0) //�Զ�����ģʽ
	{
		if(number)//�����˵������,����ʱ���жϺ͹����ж�
		{
			if(calendar.hour >= 18) //ʱ���ϵ��жϴ��ڰ���
			{
				FLAG_H = 1;			
			}
			else if(calendar.hour <= 6)//ʱ���ϵ��жϴ����糿
			{
				FLAG_H = 1;
			}
			else						//����ʱ�䴦�ڰ���
			{
				FLAG_H = 0;
			}//ʱ���ж�
			if(Flag_timer == 0)			//��һ�ν����жϻ��ߵȵ�LED������ʱ�����ʱ�ڽ����ж�
			{	
				TIM_Cmd(TIM4, DISABLE);  //ʹ��TIMx����
				if(1 == SUN_RES_INPUT)
				{
					FLAG_SUNRES = 1;	
				}//����ǿ���ж�
				else
					FLAG_SUNRES = 0;
			}
		}
		else if(!number)
		{
			FLAG_H = 0;
			FLAG_SUNRES = 0;
			TIM_Cmd(TIM4, DISABLE);  //ʹ��TIMx����
		}
		
		if((1 == FLAG_H))
		{
			LED_OUT = 0;
		}
		if((1 == FLAG_SUNRES))
		{
			if(0 == FLAG_H)
				TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
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
	
	temperate=temp;//�����¶ȴ������ĵ�ѹֵ
	temperate=(1.43-temperate)/0.0043+25;		//�������ǰ�¶�ֵ
	
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

