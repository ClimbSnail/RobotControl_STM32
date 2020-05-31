#include "sys.h"
#include "MY_USART3.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART3_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;       //接收状态标记	  
u8 usart3ReceiveSuccess = 0;  //一帧数据接收完成标志位 检测到了要置0
u8  USART_RX_BUF2[USART_REC_LEN2]; //接收缓冲,最大USART_REC_LEN2个字节.末字节为换行符 
u16 USART_RX_STA2 = 0;         		//接收状态标记	
u8 usart3ReceiveSuccess2 = 0;//一帧数据接收完成标志位 使用后要置0 

u8 firstdata;//第一回接收到的值
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//GPIOC时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口1 

}

//void USART3_IRQHandler(void)                	//串口1中断服务程序
//{
//    u8 Res;
//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//    OSIntEnter();
//#endif
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
//    {
//        Res =USART_ReceiveData(USART3);	//读取接收到的数据

//        if((USART_RX_STA&0x8000)==0)//接收未完成
//        {
//            if(USART_RX_STA&0x4000)//接收到了0x0d
//            {
//                if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
//                else
//                {
//                    USART_RX_STA|=0x8000;	//接收完成了
//                    USART_RX_STA = 0;//自己添加的 达到循环覆盖信息的作用
//                    usart3ReceiveSuccess = 1;//接收一一帧完成标志位
//                }
//            }
//            else //还没收到0X0D
//            {
//                if(Res==0x0d)USART_RX_STA|=0x4000;
//                else
//                {
//                    USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
//                    USART_RX_STA++;
//                    if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收
//                }
//            }
//        }
//    }
//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
//    OSIntExit();
//#endif
//}

/************************************************************************
函数名称：	USART3_IRQHandler(void) 
功能描述： 接收数据  存在缓冲区USART_RX_BUF或USART_RX_BUF2中
入口参数： none
返 回 值： none
其他说明： 
**************************************************************************/
void USART3_IRQHandler(void)                	//串口1中断服务程序
{
    u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res =USART_ReceiveData(USART3);	//读取接收到的数据
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
                if((USART_RX_STA2&0x8000)==0)//接收未完成
                {
                    if(USART_RX_STA2&0x4000)//接收到了0x0d
                    {
                        if(Res!=0x0a)
                            USART_RX_STA2=0;//接收错误,重新开始
                        else
                        {
                            USART_RX_STA2|=0x8000;	//接收完成了
                            USART_RX_STA2 = 0;//自己添加的 达到循环覆盖信息的作用
                            usart3ReceiveSuccess2 = 1;//接收一一帧完成标志位
                        }
                    }
                    else //还没收到0X0D
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
                                USART_RX_STA2=0;//接收数据错误,重新开始接收
                        }
                    }
                }
            }
            else
            {
                if((USART_RX_STA&0x8000)==0)//接收未完成
                {
                    if(USART_RX_STA&0x4000)//接收到了0x0d
                    {
                        if(Res!=0x0a)
                            USART_RX_STA=0;//接收错误,重新开始
                        else
                        {
                            USART_RX_STA|=0x8000;	//接收完成了
                            USART_RX_STA = 0;//自己添加的 达到循环覆盖信息的作用
                            usart3ReceiveSuccess = 1;//接收一一帧完成标志位
                        }
                    }
                    else //还没收到0X0D
                    {
                        if(Res==0x0d)USART_RX_STA|=0x4000;
                        else
                        {
                            USART_RX_BUF[USART_RX_STA&0X3FFF] = Res ;
                            USART_RX_STA++;
                            if(USART_RX_STA>(USART_REC_LEN-1))
                                USART_RX_STA=0;//接收数据错误,重新开始接收
                        }
                    }
                }
            }
        }
    }
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntExit();
#endif
}


#endif	

/************************************************************************
函数名称：	USART3_PutChar(uint8_t Data)
功能描述： 发送字符
入口参数： none
返 回 值： none
其他说明： 
**************************************************************************/
void USART3_PutChar(uint8_t Data)
{
    USART_SendData(USART3, Data);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}


