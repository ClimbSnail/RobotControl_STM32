#include "pwm.h"
#include "led.h"
#include "sys.h"
#include "action.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//PWM  驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 
u8 pos = 0;	//在定时器里循环标志用

void TIM6_Int_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB1ENR|=1<<4; //TIM6 时钟使能
	TIM6->ARR=arr; //设定计数器自动重装值
	TIM6->PSC=psc; //预分频器 7200,得到 10Khz 的计数时钟
	TIM6->DIER|=1<<0; //允许更新中断
	TIM6->CR1|=0x01; //使能定时器 3
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; //TIM6 中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //从优先级 3 级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
	NVIC_Init(&NVIC_InitStructure);
}

/************************************************************************
函数名称：	TIM6_IRQHandler(void)
功能描述： TIM6定时器中断，50ms(预设值)中断一次计算每次pwm更新需要多大的增量(用于减速控制)
入口参数： none
返 回 值： none
其他说明： 可更改TIM6定时器的定时时间(需更改action.h里的eachFrequencyTime舵机减缓单位倍率时间)
**************************************************************************/
void TIM6_IRQHandler(void)
{
	if(TIM6->SR&0X0001) 
	{
        for( pos = 0 ; pos<12 ; pos++ )	//循环计算当前各路的pwm的值应自增为多少
        {
            if( addPwmR[pos]>0 )	
            {
                if( nowPwmR[pos]<targetPwmR[pos]-addPwmR[pos] )
                    nowPwmR[pos] += addPwmR[pos];
                else
                    nowPwmR[pos] = targetPwmR[pos];
            }

            if( addPwmR[pos]<0 )
            {
                if( nowPwmR[pos]>targetPwmR[pos]-addPwmR[pos] )
                    nowPwmR[pos] += addPwmR[pos];
                else
                    nowPwmR[pos] = targetPwmR[pos];
            }
        }

        for( pos = 0 ; pos<12 ; pos++ )
        {
            if( addPwmL[pos]>0 )
            {
                if( nowPwmL[pos]<targetPwmL[pos]-addPwmL[pos] )
                    nowPwmL[pos] += addPwmL[pos];
                else
                    nowPwmL[pos] = targetPwmL[pos];
            }

            if( addPwmL[pos]<0 )
            {
                if( nowPwmL[pos]>targetPwmL[pos]-addPwmL[pos] )
                    nowPwmL[pos] += addPwmL[pos];
                else
                    nowPwmL[pos] = targetPwmL[pos];
            }
        }
        upData();	//立即将nowPwm数据更新输出
	}	
	//溢出中断
	TIM6->SR&=~(1<<0);//清除中断标志位
}
//void TIM2_Int_Init(u16 arr,u16 psc)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC->APB1ENR|=1<<0; //TIM6 时钟使能
//	TIM2->ARR=arr; //设定计数器自动重装值//刚好 1ms
//	TIM2->PSC=psc; //预分频器 7200,得到 10Khz 的计数时钟
//	TIM2->DIER|=1<<0; //允许更新中断
//	TIM2->CR1|=0x01; //使能定时器 2
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM3 中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 3 级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
//	NVIC_Init(&NVIC_InitStructure);
//}

//void TIM2_IRQHandler(void)
//{
//	if(TIM2->SR&0X0001) 
//	{
//		LED1=~LED1;
//		LED2=~LED2;
//	}
//	//溢出中断
//	TIM2->SR&=~(1<<0);//清除中断标志位
//}
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIO外设时钟使能
	                                                                     	
   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; //TIM_CH1-----CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx	
	
	TIM1->CCMR1|=1<<3; //CH1 预装载使能
	TIM1->CCMR1|=1<<11; //CH1 预装载使能
	TIM1->CCMR2|=1<<3; //CH1 预装载使能
	TIM1->CCMR2|=1<<11; //CH1 预装载使能
	TIM1->CCMR1|=7<<4; //CH1 PWM2 模式
	TIM1->CCMR1|=7<<12; //CH1 PWM2 模式
	TIM1->CCMR2|=7<<4; //CH1 PWM2 模式	
	TIM1->CCMR2|=7<<12; //CH1 PWM2 模式
  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM1->CCER|=1<<4; //OC1 输出使能
	TIM1->CCER|=1<<5;
	TIM1->CCER|=1<<8; //OC1 输出使能
	TIM1->CCER|=1<<9;
	TIM1->CCER|=1<<12; //OC1 输出使能
	TIM1->CCER|=1<<13;
		
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1  
}
void TIM8_PWM_Init(u16 arr,u16 psc)
{		

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIO外设时钟使能
	                                                                     	
   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_7 | GPIO_Pin_8; //TIM_CH1-----CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx	
	
	TIM8->CCMR1|=1<<3; //CH1 预装载使能
	TIM8->CCMR1|=1<<11; //CH1 预装载使能
	TIM8->CCMR2|=1<<3; //CH1 预装载使能
	TIM8->CCMR2|=1<<11; //CH1 预装载使能
	TIM8->CCMR1|=7<<4; //CH1 PWM2 模式
	TIM8->CCMR1|=7<<12; //CH1 PWM2 模式
	TIM8->CCMR2|=7<<4; //CH1 PWM2 模式	
	TIM8->CCMR2|=7<<12; //CH1 PWM2 模式
  TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM8->CCER|=1<<4; //OC1 输出使能
	TIM8->CCER|=1<<5;
	TIM8->CCER|=1<<8; //OC1 输出使能
	TIM8->CCER|=1<<9;
	TIM8->CCER|=1<<12; //OC1 输出使能
	TIM8->CCER|=1<<13;
		
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM8, ENABLE);  //使能TIM1  											  
}  	  	

void TIM2_PWM_Init(u16 arr,u16 psc)
{		 		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<0; 	//TIM2时钟使能    
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟	
	RCC->APB2ENR|=1<<3;    	//使能PORTB时钟	
	GPIOA->CRH&=0X0FFFFFFF;	//A15输出
	GPIOA->CRH|=0XB0000000;	//复用功能输出 
	GPIOB->CRL&=0XFFFF0FFF;	//B3输出
	GPIOB->CRL|=0X0000B000;	//复用功能输出 
	GPIOB->CRH&=0XFFFF00FF;	//B10B11输出
	GPIOB->CRH|=0X0000BB00;	//复用功能输出 
	RCC->APB2ENR|=1<<0;     //开启辅助时钟
	AFIO->MAPR&=0XFFFFFCFF; //清除MAPR的[8:9]   
	AFIO->MAPR|=3<<8; //全映射
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 


	TIM2->ARR=arr;			//设定计数器自动重装值 
	TIM2->PSC=psc;			//预分频器不分频
	
	TIM2->CCMR1|=7<<4;  	//CH1 PWM2模式
	TIM2->CCMR1|=1<<3; 	//CH1预装载使能	   
  TIM2->CCER|=1<<0;   	//OC1 输出使能 高电平有效
	TIM2->CCER|=1<<1;
	
	TIM2->CCMR1|=7<<12;  	//CH2 PWM2模式
	TIM2->CCMR1|=1<<11; 	//CH2预装载使能	   
	TIM2->CCER|=1<<4;   	//OC2 输出使能 高电平
	TIM2->CCER|=1<<5;
	
	TIM2->CCMR2 |= 7<<4;
	TIM2->CCMR2 |= 1<<3;
	TIM2->CCER|=1<<8;
	TIM2->CCER|=1<<9;
	
	TIM2->CCMR2 |= 7<<12;
	TIM2->CCMR2 |= 1<<11;
	TIM2->CCER|=1<<12;
	TIM2->CCER|=1<<13;

	TIM2->CR1=0x0080;   	//ARPE使能 
	TIM2->CR1|=0x01;    	//使能定时器2											  
}  	



void TIM3_PWM_Init(u16 arr,u16 psc)

{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<1; 	//TIM3时钟使能    
	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟	
	RCC->APB2ENR|=1<<3;    	//使能PORTA时钟	
	GPIOA->CRL&=0X00FFFFFF;	//PA6/PA7输出
	GPIOA->CRL|=0XBB000000;	//复用功能输出 
  GPIOB->CRL &= 0XFFFFFF00;            //PB0/PB1输出
	GPIOB->CRL |= 0X000000BB;              //复用功能输出

	
//	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
//	AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]   没有重映像

	TIM3->ARR=arr;			//设定计数器自动重装值 
	TIM3->PSC=psc;			//预分频器不分频
	
	TIM3->CCMR1|=7<<4;  	//CH1 PWM2模式
	TIM3->CCMR1|=1<<3; 	//CH1预装载使能	   
  TIM3->CCER|=1<<0;   	//OC1 输出使能 高电平有效
	TIM3->CCER|=1<<1;
	
	TIM3->CCMR1|=7<<12;  	//CH2 PWM2模式
	TIM3->CCMR1|=1<<11; 	//CH2预装载使能	   
	TIM3->CCER|=1<<4;   	//OC2 输出使能 高电平
	TIM3->CCER|=1<<5;
	
	TIM3->CCMR2 |= 7<<4;
	TIM3->CCMR2 |= 1<<3;
	TIM3->CCER|=1<<8;
	TIM3->CCER|=1<<9;	
	
	TIM3->CCMR2 |= 7<<12;
	TIM3->CCMR2 |= 1<<11;
	TIM3->CCER|=1<<12;
	TIM3->CCER|=1<<13;	
	
	TIM3->CCMR2 |= 7<<4;


	TIM3->CR1=0x0080;   	//ARPE使能 
	TIM3->CR1|=0x01;    	//使能定时器3 											  
}  	


void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<2; 	//TIM4时钟使能    
	GPIOB->CRL&=0X00FFFFFF;	//PB6/PB7输出
	GPIOB->CRL|=0XBB000000;	//复用功能输出 
  GPIOB->CRH&= 0XFFFFFF00; //PB8/PB9输出
	GPIOB->CRH|= 0X000000BB; //复用功能输出

	
//	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
//	AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]   没有重映像

	TIM4->ARR=arr;			//设定计数器自动重装值 
	TIM4->PSC=psc;			//预分频器不分频
	
	TIM4->CCMR1|=7<<4;  	//CH1 PWM2模式
	TIM4->CCMR1|=1<<3; 	//CH1预装载使能	   
  TIM4->CCER|=1<<0;   	//OC1 输出使能 高电平有效
	TIM4->CCER|=1<<1;
	
	TIM4->CCMR1|=7<<12;  	//CH2 PWM2模式
	TIM4->CCMR1|=1<<11; 	//CH2预装载使能	   
	TIM4->CCER|=1<<4;   	//OC2 输出使能 高电平
	TIM4->CCER|=1<<5;	
	
	TIM4->CCMR2 |= 7<<4;
	TIM4->CCMR2 |= 1<<3;
	TIM4->CCER|=1<<8;
	TIM4->CCER|=1<<9;
	
	TIM4->CCMR2 |= 7<<12;
	TIM4->CCMR2 |= 1<<11;
	TIM4->CCER|=1<<12;
	TIM4->CCER|=1<<13;	
	

	TIM4->CR1=0x0080;   	//ARPE使能 
	TIM4->CR1|=0x01;    	//使能定时器3 											  
}  	




void TIM5_PWM_Init(u16 arr,u16 psc)
{		 					 
	//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<3; 	//TIM5时钟使能    
	GPIOA->CRL&=0XFFFF0000;	//PA0/PA1/PA2/PA3输出
	GPIOA->CRL|=0X0000BBBB;	//复用功能输出 

	
//	RCC->APB2ENR|=1<<0;     //开启辅助时钟	   
//AFIO->MAPR&=0XFFFFF3FF; //清除MAPR的[11:10]   没有重映像

	TIM5->ARR=arr;			//设定计数器自动重装值 
	TIM5->PSC=psc;			//预分频器不分频
	
	TIM5->CCMR1|=7<<4;  	//CH1 PWM2模式
	TIM5->CCMR1|=1<<3; 	//CH1预装载使能	   
  TIM5->CCER|=1<<0;   	//OC1 输出使能 高电平有效
	TIM5->CCER|=1<<1;
	
	TIM5->CCMR1|=7<<12;  	//CH2 PWM2模式
	TIM5->CCMR1|=1<<11; 	//CH2预装载使能	   
	TIM5->CCER|=1<<4;   	//OC2 输出使能 高电平
	TIM5->CCER|=1<<5;
	
	TIM5->CCMR2 |= 7<<4;
	TIM5->CCMR2 |= 1<<3;
	TIM5->CCER|=1<<8;
	TIM5->CCER|=1<<9;
	
	TIM5->CCMR2 |= 7<<12;
	TIM5->CCMR2 |= 1<<11;
	TIM5->CCER|=1<<12;
	TIM5->CCER|=1<<13;
	
	TIM5->CCMR2 |= 7<<4;


	TIM5->CR1=0x0080;   	//ARPE使能 
	TIM5->CR1|=0x01;    	//使能定时器5											  
}  	

