#ifndef __MY_USART3_H
#define __MY_USART3_H
#include "stdio.h"	
#include "sys.h" 
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
#define USART_REC_LEN  			8192  	//�����������ֽ��� 8192
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern u8 usart3ReceiveSuccess;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);


//�Զ���ĵڶ���USART_RX_BUF������ ���ڲ�����������������
#define USART_REC_LEN2  			8192 	//�����������ֽ��� 8192	  	
extern u8  USART_RX_BUF2[USART_REC_LEN2]; //���ջ���,���USART_REC_LEN2���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA2;         		//����״̬���	
extern u8 usart3ReceiveSuccess2;//һ֡���ݽ�����ɱ�־λ ʹ�ú�Ҫ��0 

extern u8 firstdata;//��һ�ؽ��յ���ֵ

extern void USART3_PutChar(uint8_t Data);//����һ���ֽ�
#endif

