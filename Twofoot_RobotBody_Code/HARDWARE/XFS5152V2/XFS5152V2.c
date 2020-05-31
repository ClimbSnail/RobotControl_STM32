/*****************************************************************
**  名称：YS-XFS5152语音合成配套程序
**  说明：用户在使用程序时，只需要编写好 串口发送函数PrintCom()；即可，
         直接调用本文件进行播放即可，在编写程序时，注意判断语音合成模
				 块的返回值，根据返回值进行下一次播放，如不接收返回值则无法进
				 行下一次播放。
/*******************************************************************/

#include "XFS5152V2.h"
#include "uart5.h"

/**************芯片设置命令*********************/
uchar XFS_StopCom[]= {0xFD,0X00,0X01,0X02}; //停止合成
uchar XFS_SuspendCom[]= {0XFD,0X00,0X01,0X03}; //暂停合成
uchar XFS_RecoverCom[]= {0XFD,0X00,0X01,0X04}; //恢复合成
uchar XFS_ChackCom[]= {0XFD,0X00,0X01,0X21}; //状态查询
uchar XFS_PowerDownCom[]= {0XFD,0X00,0X01,0X88}; //进入POWER DOWN 状态命令

/***********************************************/

//声音参数设定
uchar YSpeed[11][6] = {"[s0]","[s1]","[s2]","[s3]","[s4]","[s5]","[s6]","[s7]","[s8]","[s9]","[s10]"};//语速调整参数
uchar YTone[11][6] = {"[t0]","[t1]","[t2]","[t3]","[t4]","[t5]","[t6]","[t7]","[t8]","[t9]","[t10]"};//语调调整参数
uchar YVolume[11][6] = {"[v0]","[v1]","[v2]","[v3]","[v4]","[v5]","[v6]","[v7]","[v8]","[v9]","[v10]"};//音量调整参数
uchar YPeople[6][6] = {"[m3]","[m51]","[m52]","[m53]","[m54]","[m55]"};//发音人

static uchar YS = 5;//语速指针 0-10
static uchar YT = 5;//音调指针 0-10
static uchar YV = 8;//音量指针 0-10
static uchar YP = 1;//发音人指针 0-5

/***********************************************************
* 名    称：  strlen
* 功    能：  计算字符串长度
* 入口参数：  *DAT:文本指针变量
* 出口参数：		length;文本长度
* 说    明：
* 调用方法：例： SYN_FrameInfo
**********************************************************/
unsigned short strlen(uchar *DAT)
{
    unsigned short length;
    for( length = 0; *DAT++ ; length++ );
    return length;
}

/***********************************************************
* 名    称：  strcpy
* 功    能：  串拷贝函数
* 入口参数：  *info  *data:文本指针变量
* 出口参数：		void
* 说    明：
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
* 名    称：  YS-XFS5051 文本合成函数
* 功    能：  发送合成文本到XFS5051芯片进行合成播放
* 入口参数：  *data:文本指针变量
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。
* 调用方法：例： SYN_FrameInfo
**********************************************************/
void XFS_FrameInfo(uchar *data)
{
    /****************需要发送的文本**********************************/
    unsigned  char Frame_Info[5000]={1,1,1,1,1}; //定义的文本长度 前五个一定要设为非0，后面串连接时计算串长要用
    unsigned  short length;
    unsigned  short pos;
    unsigned  short temp;
		
		//连接声音参数的字符
		temp = 5;
    strcpy(&Frame_Info[temp],YSpeed[YS]);	//在信息串中加入语速
		temp = strlen(Frame_Info);
    strcpy(&Frame_Info[temp],YTone[YT]);	//在信息串中加入语调
		temp = strlen(Frame_Info);
    strcpy(&Frame_Info[temp],YVolume[YV]);	//在信息串中加入音量
		temp = strlen(Frame_Info);
		strcpy(&Frame_Info[temp],YPeople[YP]);	//在信息串中加入发音人
		temp = strlen(Frame_Info);
		
    length = strlen(data); 			//需要发送文本的长度
    length = length+temp-3;								//计算数据区的总长度
    /*****************帧固定配置信息**************************************/
    Frame_Info[0] = 0xFD ; 			//构造帧头FD
    Frame_Info[1] = length>>8 ; 	//构造数据区长度的高字节
    Frame_Info[2] = length%256; 			//构造数据区长度的低字节
    Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
    Frame_Info[4] = GBK;       //文本编码格式：GBK

    /*******************发送帧信息***************************************/
    strcpy(&Frame_Info[temp],data);
//    XFS_I2CSendData(Frame_Info,length+3);
    for( pos = 0 ; pos<length+3 ; pos++ )
    {
        UART5_PutChar(Frame_Info[pos]);//用串口发送
    }

}

/***********************************************************
* 名    称：  YSOrder 声音设置命令执行函数
* 功    能：  设置语速语调音量以及人声
* 入口参数：  order为命令参数  范围0x00-0x07
* 出口参数：
* 说    明： 
* 调用方法：例： 本程序中在主函数中检测语音设置命令到来时执行
**********************************************************/
void YSOrder(unsigned char order)//语音设置命令
{
    unsigned  char temp[20];
    unsigned  char num[3];
    unsigned  short length;
    switch( order )
    {
    case 0x01:
        YS = (YS+1)%11;//语速加快
        break;
    case 0x02:
        YS = (YS+11-1)%11;//语速减慢
        break;
    case 0x03:
        YT = (YT+1)%11;//语调提升
        break;
    case 0x04:
        YT = (YT+11-1)%11;//语调降低
        break;
    case 0x05:
        YV = (YV+1)%11;//音量加
        break;
    case 0x06:
        YV = (YV+11-1)%11;//音量减
        break;
    case 0x07:
        YP = (YP+1)%6;//换一种声音
        XFS_FrameInfo("我现在的声音是这样的");
        break;
    case 0x08://恢复默认值
        YS = 5;//语速指针 0-10
        YT = 5;//音调指针 0-10
        YV = 5;//音量指针 0-10
        YP = 1;//发音人指针 0-5
        XFS_FrameInfo("声音已恢复默认设置");
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
        strcpy(temp,"语速已调到");
        length = strlen(temp);
        strcpy(&temp[length],num);
        length = strlen(temp);
        strcpy(&temp[length],"级");
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
        strcpy(temp,"语调已调到");
        length = strlen(temp);
        strcpy(&temp[length],num);
        length = strlen(temp);
        strcpy(&temp[length],"级");
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
        strcpy(temp,"音量已调到");
        length = strlen(temp);
        strcpy(&temp[length],num);
        length = strlen(temp);
        strcpy(&temp[length],"级");
        XFS_FrameInfo(temp);
    }
}

//uchar flag;
//uint idata ucSendBuffer[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
//uint idata ucReceData;
//uint idata ucReceiveBuffer[8];//从器件中读出的多字节数据暂存区
//void delay_10ms();
//void ACK();
//void NoACK();
/* ********************************************************
**名称：I2C_Start
**功能：启动I2C
**输入：无
**返回：无
******************************************************* */
void XFS_I2C_Start()
{
    OLED_SCLK_Set() ;
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}
/**********************************************************
**名称：I2C_Stop
**功能：停止I2C
**输入：无
**返回：无
**********************************************************/
void XFS_I2C_Stop()
{
    OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();

}

/********************************************************
**名称：Test_Ack()
**功能：检测应答位
*********************************************************/
void XFS_Test_Ack()
{

    //GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
    //GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
    //OLED_SCL = 1;
    //delay_us(50000);
    /*	while(1)
    	{
    		if(!OLED_SDA)				//判断是否接收到OLED 应答信号
    		{
    			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
    			//GPIOB->CRH |= 0x00030000;
    			return;
    		}
    	}
    */
    OLED_SCLK_Set() ;
    OLED_SCLK_Clr();
}

/********************************************************
**名称：SendData()
**功能：发送一字节数据
**输入：buffer
**返回：
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
功能描述： 	发送多字节数据
入口参数：	DAT:带发送的数据
返 回 值： 	none
其他说明：suba-数据地址，*s-写入的数据，n-写入的字节数
**************************************************************************/
void XFS_I2CSendData(uchar dat[],unsigned short length)
{
    unsigned short i;
    XFS_I2C_Start();//启动I2C
    XFS_SendData(0x80);//发送器件地址

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

