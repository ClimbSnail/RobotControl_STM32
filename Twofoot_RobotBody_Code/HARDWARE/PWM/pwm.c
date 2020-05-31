#include "pwm.h"
#include "led.h"
#include "sys.h"
#include "action.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//PWM  ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 
u8 pos = 0;	//�ڶ�ʱ����ѭ����־��

void TIM6_Int_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC->APB1ENR|=1<<4; //TIM6 ʱ��ʹ��
	TIM6->ARR=arr; //�趨�������Զ���װֵ
	TIM6->PSC=psc; //Ԥ��Ƶ�� 7200,�õ� 10Khz �ļ���ʱ��
	TIM6->DIER|=1<<0; //��������ж�
	TIM6->CR1|=0x01; //ʹ�ܶ�ʱ�� 3
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; //TIM6 �ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5; //�����ȼ� 3 ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

/************************************************************************
�������ƣ�	TIM6_IRQHandler(void)
���������� TIM6��ʱ���жϣ�50ms(Ԥ��ֵ)�ж�һ�μ���ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
��ڲ����� none
�� �� ֵ�� none
����˵���� �ɸ���TIM6��ʱ���Ķ�ʱʱ��(�����action.h���eachFrequencyTime���������λ����ʱ��)
**************************************************************************/
void TIM6_IRQHandler(void)
{
	if(TIM6->SR&0X0001) 
	{
        for( pos = 0 ; pos<12 ; pos++ )	//ѭ�����㵱ǰ��·��pwm��ֵӦ����Ϊ����
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
        upData();	//������nowPwm���ݸ������
	}	
	//����ж�
	TIM6->SR&=~(1<<0);//����жϱ�־λ
}
//void TIM2_Int_Init(u16 arr,u16 psc)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC->APB1ENR|=1<<0; //TIM6 ʱ��ʹ��
//	TIM2->ARR=arr; //�趨�������Զ���װֵ//�պ� 1ms
//	TIM2->PSC=psc; //Ԥ��Ƶ�� 7200,�õ� 10Khz �ļ���ʱ��
//	TIM2->DIER|=1<<0; //��������ж�
//	TIM2->CR1|=0x01; //ʹ�ܶ�ʱ�� 2
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //TIM3 �ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3 ��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);
//}

//void TIM2_IRQHandler(void)
//{
//	if(TIM2->SR&0X0001) 
//	{
//		LED1=~LED1;
//		LED2=~LED2;
//	}
//	//����ж�
//	TIM2->SR&=~(1<<0);//����жϱ�־λ
//}
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
	 GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	
   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11; //TIM_CH1-----CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx	
	
	TIM1->CCMR1|=1<<3; //CH1 Ԥװ��ʹ��
	TIM1->CCMR1|=1<<11; //CH1 Ԥװ��ʹ��
	TIM1->CCMR2|=1<<3; //CH1 Ԥװ��ʹ��
	TIM1->CCMR2|=1<<11; //CH1 Ԥװ��ʹ��
	TIM1->CCMR1|=7<<4; //CH1 PWM2 ģʽ
	TIM1->CCMR1|=7<<12; //CH1 PWM2 ģʽ
	TIM1->CCMR2|=7<<4; //CH1 PWM2 ģʽ	
	TIM1->CCMR2|=7<<12; //CH1 PWM2 ģʽ
  TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM1->CCER|=1<<4; //OC1 ���ʹ��
	TIM1->CCER|=1<<5;
	TIM1->CCER|=1<<8; //OC1 ���ʹ��
	TIM1->CCER|=1<<9;
	TIM1->CCER|=1<<12; //OC1 ���ʹ��
	TIM1->CCER|=1<<13;
		
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1  
}
void TIM8_PWM_Init(u16 arr,u16 psc)
{		

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	                                                                     	
   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_7 | GPIO_Pin_8; //TIM_CH1-----CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx	
	
	TIM8->CCMR1|=1<<3; //CH1 Ԥװ��ʹ��
	TIM8->CCMR1|=1<<11; //CH1 Ԥװ��ʹ��
	TIM8->CCMR2|=1<<3; //CH1 Ԥװ��ʹ��
	TIM8->CCMR2|=1<<11; //CH1 Ԥװ��ʹ��
	TIM8->CCMR1|=7<<4; //CH1 PWM2 ģʽ
	TIM8->CCMR1|=7<<12; //CH1 PWM2 ģʽ
	TIM8->CCMR2|=7<<4; //CH1 PWM2 ģʽ	
	TIM8->CCMR2|=7<<12; //CH1 PWM2 ģʽ
  TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	 
	TIM8->CCER|=1<<4; //OC1 ���ʹ��
	TIM8->CCER|=1<<5;
	TIM8->CCER|=1<<8; //OC1 ���ʹ��
	TIM8->CCER|=1<<9;
	TIM8->CCER|=1<<12; //OC1 ���ʹ��
	TIM8->CCER|=1<<13;
		
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM1  											  
}  	  	

void TIM2_PWM_Init(u16 arr,u16 psc)
{		 		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB , ENABLE);
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<0; 	//TIM2ʱ��ʹ��    
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��	
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTBʱ��	
	GPIOA->CRH&=0X0FFFFFFF;	//A15���
	GPIOA->CRH|=0XB0000000;	//���ù������ 
	GPIOB->CRL&=0XFFFF0FFF;	//B3���
	GPIOB->CRL|=0X0000B000;	//���ù������ 
	GPIOB->CRH&=0XFFFF00FF;	//B10B11���
	GPIOB->CRH|=0X0000BB00;	//���ù������ 
	RCC->APB2ENR|=1<<0;     //��������ʱ��
	AFIO->MAPR&=0XFFFFFCFF; //���MAPR��[8:9]   
	AFIO->MAPR|=3<<8; //ȫӳ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE); 


	TIM2->ARR=arr;			//�趨�������Զ���װֵ 
	TIM2->PSC=psc;			//Ԥ��Ƶ������Ƶ
	
	TIM2->CCMR1|=7<<4;  	//CH1 PWM2ģʽ
	TIM2->CCMR1|=1<<3; 	//CH1Ԥװ��ʹ��	   
  TIM2->CCER|=1<<0;   	//OC1 ���ʹ�� �ߵ�ƽ��Ч
	TIM2->CCER|=1<<1;
	
	TIM2->CCMR1|=7<<12;  	//CH2 PWM2ģʽ
	TIM2->CCMR1|=1<<11; 	//CH2Ԥװ��ʹ��	   
	TIM2->CCER|=1<<4;   	//OC2 ���ʹ�� �ߵ�ƽ
	TIM2->CCER|=1<<5;
	
	TIM2->CCMR2 |= 7<<4;
	TIM2->CCMR2 |= 1<<3;
	TIM2->CCER|=1<<8;
	TIM2->CCER|=1<<9;
	
	TIM2->CCMR2 |= 7<<12;
	TIM2->CCMR2 |= 1<<11;
	TIM2->CCER|=1<<12;
	TIM2->CCER|=1<<13;

	TIM2->CR1=0x0080;   	//ARPEʹ�� 
	TIM2->CR1|=0x01;    	//ʹ�ܶ�ʱ��2											  
}  	



void TIM3_PWM_Init(u16 arr,u16 psc)

{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<1; 	//TIM3ʱ��ʹ��    
	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��	
	RCC->APB2ENR|=1<<3;    	//ʹ��PORTAʱ��	
	GPIOA->CRL&=0X00FFFFFF;	//PA6/PA7���
	GPIOA->CRL|=0XBB000000;	//���ù������ 
  GPIOB->CRL &= 0XFFFFFF00;            //PB0/PB1���
	GPIOB->CRL |= 0X000000BB;              //���ù������

	
//	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
//	AFIO->MAPR&=0XFFFFF3FF; //���MAPR��[11:10]   û����ӳ��

	TIM3->ARR=arr;			//�趨�������Զ���װֵ 
	TIM3->PSC=psc;			//Ԥ��Ƶ������Ƶ
	
	TIM3->CCMR1|=7<<4;  	//CH1 PWM2ģʽ
	TIM3->CCMR1|=1<<3; 	//CH1Ԥװ��ʹ��	   
  TIM3->CCER|=1<<0;   	//OC1 ���ʹ�� �ߵ�ƽ��Ч
	TIM3->CCER|=1<<1;
	
	TIM3->CCMR1|=7<<12;  	//CH2 PWM2ģʽ
	TIM3->CCMR1|=1<<11; 	//CH2Ԥװ��ʹ��	   
	TIM3->CCER|=1<<4;   	//OC2 ���ʹ�� �ߵ�ƽ
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


	TIM3->CR1=0x0080;   	//ARPEʹ�� 
	TIM3->CR1|=0x01;    	//ʹ�ܶ�ʱ��3 											  
}  	


void TIM4_PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<2; 	//TIM4ʱ��ʹ��    
	GPIOB->CRL&=0X00FFFFFF;	//PB6/PB7���
	GPIOB->CRL|=0XBB000000;	//���ù������ 
  GPIOB->CRH&= 0XFFFFFF00; //PB8/PB9���
	GPIOB->CRH|= 0X000000BB; //���ù������

	
//	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
//	AFIO->MAPR&=0XFFFFF3FF; //���MAPR��[11:10]   û����ӳ��

	TIM4->ARR=arr;			//�趨�������Զ���װֵ 
	TIM4->PSC=psc;			//Ԥ��Ƶ������Ƶ
	
	TIM4->CCMR1|=7<<4;  	//CH1 PWM2ģʽ
	TIM4->CCMR1|=1<<3; 	//CH1Ԥװ��ʹ��	   
  TIM4->CCER|=1<<0;   	//OC1 ���ʹ�� �ߵ�ƽ��Ч
	TIM4->CCER|=1<<1;
	
	TIM4->CCMR1|=7<<12;  	//CH2 PWM2ģʽ
	TIM4->CCMR1|=1<<11; 	//CH2Ԥװ��ʹ��	   
	TIM4->CCER|=1<<4;   	//OC2 ���ʹ�� �ߵ�ƽ
	TIM4->CCER|=1<<5;	
	
	TIM4->CCMR2 |= 7<<4;
	TIM4->CCMR2 |= 1<<3;
	TIM4->CCER|=1<<8;
	TIM4->CCER|=1<<9;
	
	TIM4->CCMR2 |= 7<<12;
	TIM4->CCMR2 |= 1<<11;
	TIM4->CCER|=1<<12;
	TIM4->CCER|=1<<13;	
	

	TIM4->CR1=0x0080;   	//ARPEʹ�� 
	TIM4->CR1|=0x01;    	//ʹ�ܶ�ʱ��3 											  
}  	




void TIM5_PWM_Init(u16 arr,u16 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<3; 	//TIM5ʱ��ʹ��    
	GPIOA->CRL&=0XFFFF0000;	//PA0/PA1/PA2/PA3���
	GPIOA->CRL|=0X0000BBBB;	//���ù������ 

	
//	RCC->APB2ENR|=1<<0;     //��������ʱ��	   
//AFIO->MAPR&=0XFFFFF3FF; //���MAPR��[11:10]   û����ӳ��

	TIM5->ARR=arr;			//�趨�������Զ���װֵ 
	TIM5->PSC=psc;			//Ԥ��Ƶ������Ƶ
	
	TIM5->CCMR1|=7<<4;  	//CH1 PWM2ģʽ
	TIM5->CCMR1|=1<<3; 	//CH1Ԥװ��ʹ��	   
  TIM5->CCER|=1<<0;   	//OC1 ���ʹ�� �ߵ�ƽ��Ч
	TIM5->CCER|=1<<1;
	
	TIM5->CCMR1|=7<<12;  	//CH2 PWM2ģʽ
	TIM5->CCMR1|=1<<11; 	//CH2Ԥװ��ʹ��	   
	TIM5->CCER|=1<<4;   	//OC2 ���ʹ�� �ߵ�ƽ
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


	TIM5->CR1=0x0080;   	//ARPEʹ�� 
	TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��5											  
}  	

