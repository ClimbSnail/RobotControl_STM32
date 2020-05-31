/*****************************************************************
**  ���ƣ�YS-XFS5152�����ϳ����׳���
**  ˵�����û���ʹ�ó���ʱ��ֻ��Ҫ��д�� ���ڷ��ͺ���PrintCom()�����ɣ�
         ֱ�ӵ��ñ��ļ����в��ż��ɣ��ڱ�д����ʱ��ע���ж������ϳ�ģ
				 ��ķ���ֵ�����ݷ���ֵ������һ�β��ţ��粻���շ���ֵ���޷���
				 ����һ�β��š�
/*******************************************************************/

#include "XFS5152V2.h"
#include "uart5.h"

/**************оƬ��������*********************/
uchar XFS_StopCom[]= {0xFD,0X00,0X01,0X02}; //ֹͣ�ϳ�
uchar XFS_SuspendCom[]= {0XFD,0X00,0X01,0X03}; //��ͣ�ϳ�
uchar XFS_RecoverCom[]= {0XFD,0X00,0X01,0X04}; //�ָ��ϳ�
uchar XFS_ChackCom[]= {0XFD,0X00,0X01,0X21}; //״̬��ѯ
uchar XFS_PowerDownCom[]= {0XFD,0X00,0X01,0X88}; //����POWER DOWN ״̬����

/***********************************************/

//���������趨
uchar YSpeed[11][6] = {"[s0]","[s1]","[s2]","[s3]","[s4]","[s5]","[s6]","[s7]","[s8]","[s9]","[s10]"};//���ٵ�������
uchar YTone[11][6] = {"[t0]","[t1]","[t2]","[t3]","[t4]","[t5]","[t6]","[t7]","[t8]","[t9]","[t10]"};//�����������
uchar YVolume[11][6] = {"[v0]","[v1]","[v2]","[v3]","[v4]","[v5]","[v6]","[v7]","[v8]","[v9]","[v10]"};//������������
uchar YPeople[6][6] = {"[m3]","[m51]","[m52]","[m53]","[m54]","[m55]"};//������

static uchar YS = 5;//����ָ�� 0-10
static uchar YT = 5;//����ָ�� 0-10
static uchar YV = 8;//����ָ�� 0-10
static uchar YP = 1;//������ָ�� 0-5

/***********************************************************
* ��    �ƣ�  strlen
* ��    �ܣ�  �����ַ�������
* ��ڲ�����  *DAT:�ı�ָ�����
* ���ڲ�����		length;�ı�����
* ˵    ����
* ���÷��������� SYN_FrameInfo
**********************************************************/
unsigned short strlen(uchar *DAT)
{
    unsigned short length;
    for( length = 0; *DAT++ ; length++ );
    return length;
}

/***********************************************************
* ��    �ƣ�  strcpy
* ��    �ܣ�  ����������
* ��ڲ�����  *info  *data:�ı�ָ�����
* ���ڲ�����		void
* ˵    ����
**********************************************************/
void strcpy(uchar *info,uchar *dat)
{
    while( *dat )
    {
        *info++ = *dat++;
    }
		*info = 0;
}


/***********************************************************
* ��    �ƣ�  YS-XFS5051 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���XFS5051оƬ���кϳɲ���
* ��ڲ�����  *data:�ı�ָ�����
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��
* ���÷��������� SYN_FrameInfo
**********************************************************/
void XFS_FrameInfo(uchar *data)
{
    /****************��Ҫ���͵��ı�**********************************/
    unsigned  char Frame_Info[5000]={1,1,1,1,1}; //������ı����� ǰ���һ��Ҫ��Ϊ��0�����洮����ʱ���㴮��Ҫ��
    unsigned  short length;
    unsigned  short pos;
    unsigned  short temp;
		
		//���������������ַ�
		temp = 5;
    strcpy(&Frame_Info[temp],YSpeed[YS]);	//����Ϣ���м�������
		temp = strlen(Frame_Info);
    strcpy(&Frame_Info[temp],YTone[YT]);	//����Ϣ���м������
		temp = strlen(Frame_Info);
    strcpy(&Frame_Info[temp],YVolume[YV]);	//����Ϣ���м�������
		temp = strlen(Frame_Info);
		strcpy(&Frame_Info[temp],YPeople[YP]);	//����Ϣ���м��뷢����
		temp = strlen(Frame_Info);
		
    length = strlen(data); 			//��Ҫ�����ı��ĳ���
    length = length+temp-3;								//�������������ܳ���
    /*****************֡�̶�������Ϣ**************************************/
    Frame_Info[0] = 0xFD ; 			//����֡ͷFD
    Frame_Info[1] = length>>8 ; 	//�������������ȵĸ��ֽ�
    Frame_Info[2] = length%256; 			//�������������ȵĵ��ֽ�
    Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
    Frame_Info[4] = GBK;       //�ı������ʽ��GBK

    /*******************����֡��Ϣ***************************************/
    strcpy(&Frame_Info[temp],data);
//    XFS_I2CSendData(Frame_Info,length+3);
    for( pos = 0 ; pos<length+3 ; pos++ )
    {
        UART5_PutChar(Frame_Info[pos]);//�ô��ڷ���
    }

}

/***********************************************************
* ��    �ƣ�  YSOrder ������������ִ�к���
* ��    �ܣ�  ����������������Լ�����
* ��ڲ�����  orderΪ�������  ��Χ0x00-0x07
* ���ڲ�����
* ˵    ���� 
* ���÷��������� �����������������м���������������ʱִ��
**********************************************************/
void YSOrder(unsigned char order)//������������
{
    unsigned  char temp[20];
    unsigned  char num[3];
    unsigned  short length;
    switch( order )
    {
    case 0x01:
        YS = (YS+1)%11;//���ټӿ�
        break;
    case 0x02:
        YS = (YS+11-1)%11;//���ټ���
        break;
    case 0x03:
        YT = (YT+1)%11;//�������
        break;
    case 0x04:
        YT = (YT+11-1)%11;//�������
        break;
    case 0x05:
        YV = (YV+1)%11;//������
        break;
    case 0x06:
        YV = (YV+11-1)%11;//������
        break;
    case 0x07:
        YP = (YP+1)%6;//��һ������
        XFS_FrameInfo("�����ڵ�������������");
        break;
    case 0x08://�ָ�Ĭ��ֵ
        YS = 5;//����ָ�� 0-10
        YT = 5;//����ָ�� 0-10
        YV = 5;//����ָ�� 0-10
        YP = 1;//������ָ�� 0-5
        XFS_FrameInfo("�����ѻָ�Ĭ������");
        break;
    default:
        break;
    }
    if( order == 0x01 || order == 0x02 )
    {
        if( YS == 10 )
        {
            num[0] = 49;
            num[1] = 48;
            num[2] = 0;
        }
        else
        {
            num[0] = YS+48;
            num[1] = 0;
        }
        strcpy(temp,"�����ѵ���");
        length = strlen(temp);
        strcpy(&temp[length],num);
        length = strlen(temp);
        strcpy(&temp[length],"��");
        XFS_FrameInfo(temp);
    }
    else if( order == 0x03 || order == 0x04 )
    {
        if( YT == 10 )
        {
            num[0] = 49;
            num[1] = 48;
            num[2] = 0;
        }
        else
        {
            num[0] = YT+48;
            num[1] = 0;
        }
        strcpy(temp,"����ѵ���");
        length = strlen(temp);
        strcpy(&temp[length],num);
        length = strlen(temp);
        strcpy(&temp[length],"��");
        XFS_FrameInfo(temp);
    }
    else if( order == 0x05 || order == 0x06 )
    {

        if( YV == 10 )
        {
            num[0] = 49;
            num[1] = 48;
            num[2] = 0;
        }
        else
        {
            num[0] = YV+48;
            num[1] = 0;
        }
        strcpy(temp,"�����ѵ���");
        length = strlen(temp);
        strcpy(&temp[length],num);
        length = strlen(temp);
        strcpy(&temp[length],"��");
        XFS_FrameInfo(temp);
    }
}

//uchar flag;
//uint idata ucSendBuffer[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
//uint idata ucReceData;
//uint idata ucReceiveBuffer[8];//�������ж����Ķ��ֽ������ݴ���
//void delay_10ms();
//void ACK();
//void NoACK();
/* ********************************************************
**���ƣ�I2C_Start
**���ܣ�����I2C
**���룺��
**���أ���
******************************************************* */
void XFS_I2C_Start()
{
    OLED_SCLK_Set() ;
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}
/**********************************************************
**���ƣ�I2C_Stop
**���ܣ�ֹͣI2C
**���룺��
**���أ���
**********************************************************/
void XFS_I2C_Stop()
{
    OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();

}

/********************************************************
**���ƣ�Test_Ack()
**���ܣ����Ӧ��λ
*********************************************************/
void XFS_Test_Ack()
{

    //GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊ��������ģʽ
    //GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
    //OLED_SCL = 1;
    //delay_us(50000);
    /*	while(1)
    	{
    		if(!OLED_SDA)				//�ж��Ƿ���յ�OLED Ӧ���ź�
    		{
    			//GPIOB->CRH &= 0XFFF0FFFF;	//����PB12Ϊͨ���������ģʽ
    			//GPIOB->CRH |= 0x00030000;
    			return;
    		}
    	}
    */
    OLED_SCLK_Set() ;
    OLED_SCLK_Clr();
}

/********************************************************
**���ƣ�SendData()
**���ܣ�����һ�ֽ�����
**���룺buffer
**���أ�
*******************************************************/
void XFS_SendData(uchar buffer)
{
    unsigned char i;
    unsigned char m;
    OLED_SCLK_Clr();
    for(i=0; i<8; i++)
    {
        m=buffer;
        //OLED_SCLK_Clr();
        m=m&0x80;
        if(m==0x80)
        {
            OLED_SDIN_Set();
        }
        else OLED_SDIN_Clr();
        buffer=buffer<<1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
    }
}
/************************************************************************
���������� 	���Ͷ��ֽ�����
��ڲ�����	DAT:�����͵�����
�� �� ֵ�� 	none
����˵����suba-���ݵ�ַ��*s-д������ݣ�n-д����ֽ���
**************************************************************************/
void XFS_I2CSendData(uchar dat[],unsigned short length)
{
    unsigned short i;
    XFS_I2C_Start();//����I2C
    XFS_SendData(0x80);//����������ַ

    XFS_Test_Ack();
//  if(flag==0) return(0);

    for(i=0; i<length; i++)
    {
        XFS_SendData(dat[i]);
        XFS_Test_Ack();
//  	if(flag==0) return(0);
    }
    XFS_I2C_Stop();
}

