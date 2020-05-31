#ifndef __XFS5152V2_H
#define __XFS5152V2_H

#include "stdio.h"

//��������ʽ  0x00~0x03
#define GB2312	0x00
#define GBK	0x01
#define BIG5	0x02
#define Unicode 	0x03

#define uchar unsigned char

/**************оƬ��������*********************/
extern uchar XFS_StopCom[];//ֹͣ�ϳ�
extern uchar XFS_SuspendCom[];//��ͣ�ϳ�
extern uchar XFS_RecoverCom[];//�ָ��ϳ�
extern uchar XFS_ChackCom[];//״̬��ѯ  оƬ���з���0x4F
extern uchar XFS_PowerDownCom[];//����POWER DOWN ״̬����

extern void XFS_FrameInfo(uchar *dat);//�ı��ϳɺ���
extern void YSOrder(unsigned char order);//������������


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

