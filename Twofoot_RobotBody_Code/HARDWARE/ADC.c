 #include "adc.h"
 #include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK miniSTM32������
//ADC ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
	   
 u16 AD_Value[8];		   
//��ʼ��ADC
//�������ǽ��Թ���ͨ��Ϊ��
//����Ĭ�Ͻ�����ͨ��0~3																	   
void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//CA4  5 ��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//PC0~3  4·��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	 //PC0/1/2/3 ��Ϊģ��ͨ����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//------------------------------------ADC����--------------------------------------------------------

	ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;        //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
		ADC_InitStructure.ADC_ScanConvMode =ENABLE;        //���ŵ�ɨ��ģʽ
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //ģ��ת������������ת��ģʽ
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;   //�ⲿ����ת���ر�
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        //ADC�����Ҷ���
		ADC_InitStructure.ADC_NbrOfChannel = 8;        //�˴���6���ŵ����ɿ���Ϊ1~16��
		ADC_Init(ADC1, &ADC_InitStructure);        //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���
		
		//ADC�����ŵ�����
		//ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );                
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5 );                
		ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5 );                
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, ADC_SampleTime_239Cycles5 );
		ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, ADC_SampleTime_239Cycles5 );

          ADC_DMACmd(ADC1, ENABLE);       //ʹ��ADC1��DMA����         
         ADC_Cmd(ADC1, ENABLE);           //ʹ��ָ����ADC1
         ADC_ResetCalibration(ADC1);        //��λָ����ADC1��У׼�Ĵ���
         while(ADC_GetResetCalibrationStatus(ADC1));        //��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�
         ADC_StartCalibration(ADC1);                //��ʼָ��ADC1��У׼״̬
         while(ADC_GetCalibrationStatus(ADC1));                //��ȡָ��ADC1��У׼����,����״̬��ȴ�
}		

////���ADCֵ
////ch:ͨ��ֵ 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

//	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
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


//��������
void sort(u16 *first,u16 *end)//firstΪ�����ͷָ�� endΪβָ��
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


//�õ�һ�ΰ�·ADC���˺��ֵ
void getOneADC()
{
    u16 AD_Value_temp[8][1000];
    u16 i;
    u8 j;
    u32 addCount[8] = {0};
    u16 avg[8];
		//��·ADCͬʱ��ȡ1000����ֵ
    for( i = 0 ; i < 1000 ; i++ )
    {
        for( j = 0; j<8 ; j++ )
        {
            AD_Value_temp[j][i] = AD_Value[j];
        }
        delay_ms(1);//��ʱΪ������ƽ��
    }
		//���·ADC�ɼ���1000�����ݸ�������ȥ����
    for( j = 0 ; j<8 ; j++ )
        sort(&AD_Value_temp[j][0],&AD_Value_temp[j][999]);
		//������ȡ�м�600�����ݼ������
    for( i = 200 ; i<800 ; i++ )
    {
        for( j = 0 ; j<8 ; j++ )
            addCount[j] += AD_Value_temp[j][i];
    }
		//�����·�ɼ���ƽ��ֵ��Ϊ��׼��ʼ������ֵ
    for( j = 0 ; j<8 ; j++ )
        avg[j] = addCount[j]/600;
}


//��ʼADC��һ��
void adcToOne()
{
	
}

