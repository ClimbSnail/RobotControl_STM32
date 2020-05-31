#include "uart5.h"
#include "MY_USART3.h"	

u8 USART5_RX_BUF[USART5_REC_LEN];//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART5_RX_STA;         		//接收状态标记	
u8 usart5ReceiveSuccess;//一帧数据接收完成标志位 使用后要置0  

void uart5_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC, ENABLE);	//GPIOC  D时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能UART5
	//UART1_TX   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  //UART5_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure); 

  //Uart5 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(UART5, &USART_InitStructure); //初始化串口5
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(UART5, ENABLE);                    //使能串口1 

}


/************************************************************************
函数名称：	UART5_IRQHandler(void)
功能描述： 串口2中断接收服务程序
入口参数： none
返 回 值： none
其他说明： 
**************************************************************************/
void UART5_IRQHandler(void)
{
    u8 Res;
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)//接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(UART5);	//读取接收到的数据
//				if( Res == 0x4F )//处于空闲状态
//					USART3_PutChar(  );
//				if( Res == 0x4E )//正在合成状态
    }

}

/************************************************************************
函数名称：	UART5_PutChar(uint8_t Data)
功能描述： 发送一个字节
入口参数： none
返 回 值： none
其他说明： 
**************************************************************************/
void UART5_PutChar(uint8_t Data)
{
    USART_SendData(UART5, Data);
    while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
}


/************************************************************************
函数名称：	UART5_PutStr (uint8_t *str)  
功能描述： 发送字符串
入口参数： none
返 回 值： none
其他说明： 
**************************************************************************/
void UART5_PutStr (uint8_t *str)  
{  
    while (0 != *str)  
    {  
        UART5_PutChar(*str);  
        str++;  
    }  
} 

