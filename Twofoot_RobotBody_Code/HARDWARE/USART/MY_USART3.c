#include "sys.h"
#include "MY_USART3.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/8/18
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//********************************************************************************
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART3_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA=0;       //����״̬���	  
u8 usart3ReceiveSuccess = 0;  //һ֡���ݽ�����ɱ�־λ ��⵽��Ҫ��0
u8  USART_RX_BUF2[USART_REC_LEN2]; //���ջ���,���USART_REC_LEN2���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART_RX_STA2 = 0;         		//����״̬���	
u8 usart3ReceiveSuccess2 = 0;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0 

u8 firstdata;//��һ�ؽ��յ���ֵ
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//GPIOCʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 

}

//void USART3_IRQHandler(void)                	//����1�жϷ������
//{
//    u8 Res;
//#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//    OSIntEnter();
//#endif
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//    {
//        Res =USART_ReceiveData(USART3);	//��ȡ���յ�������

//        if((USART_RX_STA&0x8000)==0)//����δ���
//        {
//            if(USART_RX_STA&0x4000)//���յ���0x0d
//            {
//                if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
//                else
//                {
//                    USART_RX_STA|=0x8000;	//���������
//                    USART_RX_STA = 0;//�Լ���ӵ� �ﵽѭ��������Ϣ������
//                    usart3ReceiveSuccess = 1;//����һһ֡��ɱ�־λ
//                }
//            }
//            else //��û�յ�0X0D
//            {
//                if(Res==0x0d)USART_RX_STA|=0x4000;
//                else
//                {
//                    USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//                    USART_RX_STA++;
//                    if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
//                }
//            }
//        }
//    }
//#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
//    OSIntExit();
//#endif
//}

/************************************************************************
�������ƣ�	USART3_IRQHandler(void) 
���������� ��������  ���ڻ�����USART_RX_BUF��USART_RX_BUF2��
��ڲ����� none
�� �� ֵ�� none
����˵���� 
**************************************************************************/
void USART3_IRQHandler(void)                	//����1�жϷ������
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
        if( USART_RX_STA == 0 )
        {
            firstdata = Res;
            USART_RX_BUF[USART_RX_STA++] = Res ;
        }
        else if( USART_RX_STA2 == 0 )
        {
            firstdata = Res;
            USART_RX_BUF2[USART_RX_STA2++] = Res ;
        }
        else
        {
            if( firstdata == 'Y' )
            {
                if((USART_RX_STA2&0x8000)==0)//����δ���
                {
                    if(USART_RX_STA2&0x4000)//���յ���0x0d
                    {
                        if(Res!=0x0a)
                            USART_RX_STA2=0;//���մ���,���¿�ʼ
                        else
                        {
                            USART_RX_STA2|=0x8000;	//���������
                            USART_RX_STA2 = 0;//�Լ���ӵ� �ﵽѭ��������Ϣ������
                            usart3ReceiveSuccess2 = 1;//����һһ֡��ɱ�־λ
                        }
                    }
                    else //��û�յ�0X0D
                    {
                        if(Res==0x0d){
														USART_RX_BUF2[USART_RX_STA2] = 0x00; 
                            USART_RX_STA2|=0x4000;
												}
                        else
                        {
                            USART_RX_BUF2[USART_RX_STA2&0X3FFF] = Res ;
                            USART_RX_STA2++;
                            if(USART_RX_STA2>(USART_REC_LEN2-1))
                                USART_RX_STA2=0;//�������ݴ���,���¿�ʼ����
                        }
                    }
                }
            }
            else
            {
                if((USART_RX_STA&0x8000)==0)//����δ���
                {
                    if(USART_RX_STA&0x4000)//���յ���0x0d
                    {
                        if(Res!=0x0a)
                            USART_RX_STA=0;//���մ���,���¿�ʼ
                        else
                        {
                            USART_RX_STA|=0x8000;	//���������
                            USART_RX_STA = 0;//�Լ���ӵ� �ﵽѭ��������Ϣ������
                            usart3ReceiveSuccess = 1;//����һһ֡��ɱ�־λ
                        }
                    }
                    else //��û�յ�0X0D
                    {
                        if(Res==0x0d)USART_RX_STA|=0x4000;
                        else
                        {
                            USART_RX_BUF[USART_RX_STA&0X3FFF] = Res ;
                            USART_RX_STA++;
                            if(USART_RX_STA>(USART_REC_LEN-1))
                                USART_RX_STA=0;//�������ݴ���,���¿�ʼ����
                        }
                    }
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}


#endif	

/************************************************************************
�������ƣ�	USART3_PutChar(uint8_t Data)
���������� �����ַ�
��ڲ����� none
�� �� ֵ�� none
����˵���� 
**************************************************************************/
void USART3_PutChar(uint8_t Data)
{
    USART_SendData(USART3, Data);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}


