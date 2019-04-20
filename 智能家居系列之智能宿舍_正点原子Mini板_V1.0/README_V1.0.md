# 实现内容
+ 室内智能灯光控制系统：  
    + 硬件材料：光敏电阻、LED照明灯、后备纽扣电池、HC-SR501人体红外感应电子模块传感器热释电探头感应开关(2个)
    + 软件资源：GPIO口4个(分别对应四个硬件)、定时器(白天的LED照明灯的时间控制)、RTC实时时钟(时间显示+LED照明灯的时间判断)
    + 软件编程思路：
        + 使用RTC实时时钟在屏幕上显示实时时间
        + 利用RTC时间判断：
        + 在某一时间段内处于傍晚及其晚上，LED照明灯自动打开；在某一时间段内处于早晨及其白天，LED照明灯需要触发条件才可以打开
        + 触发条件为：
            + 第一条件为：
                + 在特定的LED照明灯打开时间内。
            + 第二条件为：
                + 在满足第一条件的基础上，当宿舍内有人(人数由下个部分实现)时，并且室内的光照强度明显弱时，LED照明灯灯将开启部分时间。  
    + 出现的问题：
        + 选用的GPIO口和JLINK的程序下载口有相同的部分：
            + 一般情况下，当板子下载完程序后并不会将JKINK移除
            + 这就导致了一种现象，当外部传感器将电平信号回传MCU时，由于JKINK的存在，会使得电平信号难以确定
            + 而传感器明确的电平信号通过电平指示灯由人眼可以明确知晓，而MCU得到的信号确与人观察的信号相背离，进而可能认为传感器坏掉
        + 定时器问题，在初始化定时器的时候，将定时器失能`TIM_Cmd(TIM2, DISABLE);`，但是定时器却依然进入一次，后续不在进入
            + 解决方法：在LED照明灯定时时，将进入判断条件的下限值和上限值右移。 
    + 得到的经验：
        + 在设计传感器时，PCB板上应该有传感器的电平信号指示灯
        + 在做测试的时候，应该移除JKINK  

`主要程序实现`

```
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
	GPIO_InitStructure.GPIO_Pin = LED_Pin;				 //LED -->PA.7 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO口速度为50MHz
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);			 //根据设定参数初始化GPIOA.7
	GPIO_SetBits(LED_GPIO,LED_Pin);						 //PA.7 输出高 
	LED_OUT = 1;
			
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


//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
							 
}

void TIM4_IRQHandler(void)   //TIM4中断
{
	static u8 cnt = 0;
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		cnt ++;
		if((cnt > 1) && (cnt <= 11))
		{
			Flag_timer = 1;
			LED_OUT = 0; //定时15s
		}
			
		else
		{
			LED_OUT = 1;
			cnt = 1;
			Flag_timer = 0;
			
		}
	}
}

```
+ 室内人数统计系统：
    + 硬件系统：2个红外管
    + 软件资源：GPIO口(2个)、配置为外部中断、通用定时器（2个）
    + 软件编程思路：
        + 需要解决以下几个问题：  
            1. 2个红外管确定人的进出，在直观上很好理解，在程序上不好控制。
            2. 2个红外管的电平跳变捕捉，在人的进出时，只捕获一次，且每个管子都得捕获一次。  
            3. 尽可能的移除干扰状态，比如：只触发一个管子。
        + 实现步骤：
            + 将两个GPIO口配置为外部中断，使用红外管的电平跳变来检测是否有人通过。
            + 定义两个`中断进入`标志位，通过两个标志位的先后等于1的顺序来确定人的进入和出去，进而解决了问题b。
            + 当其中一个标志位等于1(红外管已触发)时，而另外一个标志位等于0(红外管未触发)时，开启相应的定时器中断，判断是否在定时时间内触发了另一个管子，进而解决了问题c。
            + 在定时器中断中判断另一个管子的标志位是否等于1：若等于1，则进行相应的人数加减,`对人数还应该进行相应的人数下限处理`，因为人数不可能小于0，`而不对人数上限进行处理`，因为可能有串宿舍的；若不等于1，则说明没有人进出，则重新开启外部中断，且标志位复位，进行下一轮判断。
            + 在人进出时，电平跳变只检测一次。
    + 出现的问题：
        + 对人数统计进行了多次的程序测试：
            + 1.使用外部中断判断
            + 2.使用外部中断+主程序控制
            + 3.使用外部中断+定时器控制 `此效果最好`
        + 对于红外管的问题：
            + 在某宝买红外管时，一定要确认下红外管是否有杜邦头！！ 真的很重要，手里没工具，焊杜邦头很麻烦  
            + 红外管后面的指示灯是当检测到障碍物时才会亮，才会有电平变化。
            + 如果刚开始测试的时候出现无论怎么弄都不能触发的情况，这是可以尝试着旋转红外管号码的旋钮，`不得不吐槽以下，后面的旋钮真小，小的平口螺丝刀都插不进去，还是用的刀片，emmm....` 
            + 本人在刚拿到红外管，测试的时候就有这个问题，一度以为是坏的.....
            + 检测红外管好坏的方法可以较为黑暗的环境下，用眼看一下红外管的头部是否有红外光源射出。

`主要程序实现`
```
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		if(FLAG_2 == 1)
		{
			number ++;
		}
		
		EXTI4_NVIC_Config(ENABLE);
		EXTI5_NVIC_Config(ENABLE);
		TIM_Cmd(TIM3, DISABLE);  //使能TIMx外设
		FLAG_1 = 0;
		FLAG_2 = 0;
	}
}

void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		
		if(FLAG_1 == 1)
		{
			number --;
			if(number < 0)
				number = 0;
		}		
		EXTI4_NVIC_Config(ENABLE);
		EXTI5_NVIC_Config(ENABLE);
		TIM_Cmd(TIM2, DISABLE);  //使能TIMx外设
		FLAG_1 = 0;
		FLAG_2 = 0;
		
	}
}


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
```
+ 室内防火防烟雾报警处理系统：
    + 硬件材料：MQ-2 火灾烟雾检测传感器、小型风扇模块、5V蜂鸣器模块(低电平触发)、两路继电器模块、DS18B20(可选)、4节电池和电池盒子
    + 软件资源：GPIO口(3个，2输出，1输入)、MCU内部的温度传感器(其温度略高于外界温度)
    + 软件编程：
        + 利用MCU内部的温度传感器， 使用ADC得到所需要的温度信息。
        + MQ-2 火灾烟雾检测传感器为数字量输出，容易得到所需要的数据。
        + 使用以上两个传感器参数作为主参考量，当MQ-2检测到火灾烟雾，`或者` 温度达到指定的限值，MCU通过GPIO引脚的电平0/1信号驱动继电器的开合，进而驱动小型风扇模块和5V蜂鸣器的通断
        + 程序整体较为简单
    + 出现的问题：
        + 蜂鸣器问题：
            + 使用的是5V蜂鸣器，以为供电电压为5V就可以，直接将信号端接入MCU引脚，其现象是：无论GPIO引脚处于置位/复位状态，蜂鸣器响个不停；将信号端接入VCC端，蜂鸣器停止，得出了蜂鸣器的信号端电平状态与MCU的电平状态不匹配，所以使用继电器控制蜂鸣器。   
    + 因程序较为简单，遂不在列出程序。

+ 物联网Air202_S5模块控制系统：
    + 虽然说是一个系统，不过就是使用公司提供的芯片+外围，其使用Lua脚本语言来进行开发。 
    + 将单片机的所需要传输的信息通过串口传送到物联网芯片，然后数据上网。
    + 关于Air202_S5这个模块和其他的物联网模块相比，有一点比较好，就是可以绑定虚拟SIM，无需实体SIM
    + 物联网平台，使用阿里云，较为方便，即可以实现数据可视化，也可以实现数据的下发，使用HTML语言还可以通过网页的制作，实现特定的功能实现。
    + 其程序较大，不在此列出。

+ 总结：
    + 本设计较为简单，前前后后花费了2.5天时间完成，其程序源码上传[Github]()和百度云
    + 还得用Altium Designer 画一个原理图... 
