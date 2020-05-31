#ifndef __XFS5152V2_H
#define __XFS5152V2_H

#include "stdio.h"

//定义编码格式  0x00~0x03
#define GB2312	0x00
#define GBK	0x01
#define BIG5	0x02
#define Unicode 	0x03

#define uchar unsigned char

/**************芯片设置命令*********************/
extern uchar XFS_StopCom[];//停止合成
extern uchar XFS_SuspendCom[];//暂停合成
extern uchar XFS_RecoverCom[];//恢复合成
extern uchar XFS_ChackCom[];//状态查询  芯片空闲返回0x4F
extern uchar XFS_PowerDownCom[];//进入POWER DOWN 状态命令

extern void XFS_FrameInfo(uchar *dat);//文本合成函数
extern void YSOrder(unsigned char order);//语音设置命令


#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_5)//CLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_4)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_4)

void XFS_I2C_Start();
void XFS_I2C_Stop();
//void Ack();
//void NoAck();
void XFS_Test_Ack();
void XFS_SendData(uchar buffer);
void XFS_I2CSendData(uchar s[],unsigned short length);

#endif

