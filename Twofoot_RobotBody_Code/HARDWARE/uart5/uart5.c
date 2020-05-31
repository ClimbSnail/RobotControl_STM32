#include "uart5.h"
#include "MY_USART3.h"	

u8 USART5_RX_BUF[USART5_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART5_RX_STA;         		//����״̬���	
u8 usart5ReceiveSuccess;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0  

void uart5_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC, ENABLE);	//GPIOC  Dʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��UART5
	//UART1_TX   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  //UART5_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  //Uart5 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(UART5, &USART_InitStructure); //��ʼ������5
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���1 

}


/************************************************************************
�������ƣ�	UART5_IRQHandler(void)
���������� ����2�жϽ��շ������
��ڲ����� none
�� �� ֵ�� none
����˵���� 
**************************************************************************/
void UART5_IRQHandler(void)
{
    u8 Res;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res = USART_ReceiveData(UART5);	//��ȡ���յ�������
//				if( Res == 0x4F )//���ڿ���״̬
//					USART3_PutChar(  );
//				if( Res == 0x4E )//���ںϳ�״̬
    }

}

/************************************************************************
�������ƣ�	UART5_PutChar(uint8_t Data)
���������� ����һ���ֽ�
��ڲ����� none
�� �� ֵ�� none
����˵���� 
**************************************************************************/
void UART5_PutChar(uint8_t Data)
{
    USART_SendData(UART5, Data);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
}


/************************************************************************
�������ƣ�	UART5_PutStr (uint8_t *str)  
���������� �����ַ���
��ڲ����� none
�� �� ֵ�� none
����˵���� 
**************************************************************************/
void UART5_PutStr (uint8_t *str)  
{  
    while (0 != *str)  
    {  
        UART5_PutChar(*str);  
        str++;  
    }  
} 

