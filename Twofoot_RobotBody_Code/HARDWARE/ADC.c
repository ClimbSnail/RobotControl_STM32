 #include "adc.h"
 #include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK miniSTM32开发板
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/7
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
	   
 u16 AD_Value[8];		   
//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//CA4  5 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//PC0~3  4路作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	 //PC0/1/2/3 作为模拟通道输入引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//------------------------------------ADC设置--------------------------------------------------------

	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;        //ADC工作模式:ADC1和ADC2工作在独立模式
		ADC_InitStructure.ADC_ScanConvMode =ENABLE;        //多信道扫描模式
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //模数转换工作在连续转换模式
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //外部触发转换关闭
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        //ADC数据右对齐
		ADC_InitStructure.ADC_NbrOfChannel = 8;        //此处开6个信道（可开的为1~16）
		ADC_Init(ADC1, &ADC_InitStructure);        //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
		
		//ADC常规信道配置
		//ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );                
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );                
		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5 );                
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, ADC_SampleTime_239Cycles5 );

          ADC_DMACmd(ADC1, ENABLE);       //使能ADC1的DMA传输         
         ADC_Cmd(ADC1, ENABLE);           //使能指定的ADC1
         ADC_ResetCalibration(ADC1);        //复位指定的ADC1的校准寄存器
         while(ADC_GetResetCalibrationStatus(ADC1));        //获取ADC1复位校准寄存器的状态,设置状态则等待
         ADC_StartCalibration(ADC1);                //开始指定ADC1的校准状态
         while(ADC_GetCalibrationStatus(ADC1));                //获取指定ADC1的校准程序,设置状态则等待
}		

////获得ADC值
////ch:通道值 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//设置指定ADC的规则组通道，一个序列，采样时间
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
//}

//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 	 


//数组排序
void sort(u16 *first,u16 *end)//first为排序的头指针 end为尾指针
{
    u16 *i;
    u16 *j;
    u16 temp;
    for( i = first ; i<end ; i++ )
        for( j = i+1 ; j<=end ; j++ )
            if( *i >= *j )
            {
                temp = *i;
                *i = *j;
                *j = temp;
            }
}


//得到一次八路ADC过滤后的值
void getOneADC()
{
    u16 AD_Value_temp[8][1000];
    u16 i;
    u8 j;
    u32 addCount[8] = {0};
    u16 avg[8];
		//八路ADC同时获取1000次数值
    for( i = 0 ; i < 1000 ; i++ )
    {
        for( j = 0; j<8 ; j++ )
        {
            AD_Value_temp[j][i] = AD_Value[j];
        }
        delay_ms(1);//延时为了数据平稳
    }
		//这八路ADC采集的1000次数据各个都拿去排序
    for( j = 0 ; j<8 ; j++ )
        sort(&AD_Value_temp[j][0],&AD_Value_temp[j][999]);
		//各个都取中间600次数据计算求和
    for( i = 200 ; i<800 ; i++ )
    {
        for( j = 0 ; j<8 ; j++ )
            addCount[j] += AD_Value_temp[j][i];
    }
		//求出八路采集的平均值作为标准初始化的数值
    for( j = 0 ; j<8 ; j++ )
        avg[j] = addCount[j]/600;
}


//初始ADC归一化
void adcToOne()
{
	
}

