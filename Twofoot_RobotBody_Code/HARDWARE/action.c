#include "action.h"
#include "delay.h"
#include "XFS5152V2.h"
#include "MY_USART3.h"

//上电后就是这个状态(初始值就是一个站立状态)
short nowPwmL[12] = {1500,1500,1500, 1500,1700,2300, 1900,1500,1800, 1700,1000,1500};
short nowPwmR[12] = {1500,1500,1500, 1500,1300,700, 1100,1500,1200, 1300,2000,1500};//目标Pwm
short targetPwmL[12] = {1500,1500,1500, 1500,1700,2300, 1900,1500,1800, 1700,1000,1500};
short targetPwmR[12] = {1500,1500,1500, 1500,1300,700, 1100,1500,1200, 1300,2000,1500};//目标Pwm
//每次定时中断时要增加的舵机量
short addPwmR[12]= {0,0,0, 0,0,0, 0,0,0, 0,0,0};
short addPwmL[12]= {0,0,0, 0,0,0, 0,0,0, 0,0,0};
//舵机的安装误差
short errorValueL[12] = {0,0,0, 0,0,0, 0,0,0, 0,0,0};
short errorValueR[12] = {0,0,0, 0,0,0, 0,0,0, 0,0,0};

void actionInit();//动作初始化
void standUp();//站起来
void sitDown();//蹲下
void halfSitDown();//半蹲
void upData(void);//pwm数据更新
void forWordOne();//前进1
void forWordTwo();//前进2
void backOne();//后退1
void backTwo();//后退2
void countAddPwm(short temp_pwmL[12],short temp_pwmR[12],short frequency);//传入pwm以及减速倍率,定时器6开启时将自动缓慢生效

void actionInit()//动作初始化	所有舵机归中位1500us
{
    short temp_pwmL[12] = {1500,1500,1500, 1500,1500,1500, 1500,1500,1500, 1500,1500,1500};
    short temp_pwmR[12] = {1500,1500,1500, 1500,1500,1500, 1500,1500,1500, 1500,1500,1500};
    countAddPwm(temp_pwmL,temp_pwmR,20);//计算每次pwm更新需要多大的增量(用于减速控制)
}

void standUp()//站起来
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,0,1600, 1200,1500,0};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,0,1400, 1800,1500,0};
    countAddPwm(temp_pwmL,temp_pwmR,20);
}

void sitDown()//蹲下
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,0,2200, 2300,500,0};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,0,800, 700,2500,0};
    countAddPwm(temp_pwmL,temp_pwmR,20);//计算每次pwm更新需要多大的增量(用于减速控制)
}

void halfSitDown()//半蹲
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,0,1800, 1700,1000,0};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,0,1200, 1300,2000,0};
    countAddPwm(temp_pwmL,temp_pwmR,20);//计算每次pwm更新需要多大的增量(用于减速控制)
}

void open()//张脚
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,1700,0, 0,0,1700};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,1300,0, 0,0,1300};
    countAddPwm(temp_pwmL,temp_pwmR,20);//计算每次pwm更新需要多大的增量(用于减速控制)
}

void rightLeaning()//右倾
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,1700,0, 0,0,1700};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,1700,0, 0,0,1700};
    countAddPwm(temp_pwmL,temp_pwmR,20);//计算每次pwm更新需要多大的增量(用于减速控制)
}

void leftLeaning()//左倾
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,1300,0, 0,0,1300};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,1300,0, 0,0,1300};
    countAddPwm(temp_pwmL,temp_pwmR,20);//计算每次pwm更新需要多大的增量(用于减速控制)
}

void forWordOne()//前进1
{
    short temp_pwmL1[12] = {0,1500,0, 0,0,0, 1250,0,1900, 1600,0,0};//左腿稍升高
    short temp_pwmR1[12] = {0,0,0, 0,0,0, 750,0,1000, 1300,1800,0};//抬右腿

    short temp_pwmL2[12] = {0,1000,0, 0,0,0, 0,1500,1300, 1400,1000,1600};//降左腿
    short temp_pwmR2[12] = {0,0,0, 0,0,0, 0,1500,1300, 1400,0,1600};

    short temp_pwmL3[12] = {0,500,0, 0,0,0, 0,1400,0, 0,0,1650};
    short temp_pwmR3[12] = {0,0,0, 0,0,0, 0,1350,0, 0,0,1650};

    countAddPwm(temp_pwmL1,temp_pwmR1,10);//计算每次pwm更新需要多大的增量(用于减速控制)
    delay_ms(700);
    countAddPwm(temp_pwmL2,temp_pwmR2,10);//计算每次pwm更新需要多大的增量(用于减速控制)
    delay_ms(700);
    countAddPwm(temp_pwmL3,temp_pwmR3,5);//计算每次pwm更新需要多大的增量(用于减速控制)
    delay_ms(300);
}

void forWordTwo()//前进2
{
    short temp_pwmL4[12] = {0,1500,0, 0,0,0, 2000,1400,2000, 1700,1200,1650};//抬左腿
    short temp_pwmR4[12] = {0,0,0, 0,0,0, 1500,1350,1100, 1400,0,1650};//右腿稍升高

    short temp_pwmL5[12] = {0,2000,0, 0,0,0, 0,1500,1700, 1600,0,1400};
    short temp_pwmR5[12] = {0,0,0, 0,0,0, 0,1500,1700, 1600,2000,1400};//降右腿

    short temp_pwmL6[12] = {0,2500,0, 0,0,0, 0,1650,0, 0,0,1350};
    short temp_pwmR6[12] = {0,0,0, 0,0,0, 0,1600,0, 0,0,1350};

    countAddPwm(temp_pwmL4,temp_pwmR4,10);	//计算每次pwm更新需要多大的增量(用于减速控制)
    delay_ms(700);
    countAddPwm(temp_pwmL5,temp_pwmR5,10);	//计算每次pwm更新需要多大的增量(用于减速控制)
    delay_ms(700);
    countAddPwm(temp_pwmL6,temp_pwmR6,5);	//计算每次pwm更新需要多大的增量(用于减速控制)
    delay_ms(300);
}


/************************************************************************
函数名称：	countAddPwm(short temp_pwmL[12],short temp_pwmR[12],short frequency)
功能描述： 计算每次pwm更新需要多大的增量(用于减速控制)
入口参数： temp_pwmL temp_pwmR frequency:减缓倍率
返 回 值： none
其他说明： 该函数传入pwm以及减速倍率(单位减缓时间预设值为50ms),定时器6开启时
					将自动缓慢生效
**************************************************************************/
void countAddPwm(short temp_pwmL[12],short temp_pwmR[12],short frequency)
{
    u8 i;
    for( i = 0 ; i<12 ; i++ )
    {
        if( temp_pwmL[i]>0 )
        {
            targetPwmL[i] = temp_pwmL[i]-errorValueL[i];
            addPwmL[i] = (temp_pwmL[i]-nowPwmL[i])/frequency;
        }
        if( temp_pwmR[i]>0 )
        {
            targetPwmR[i] = temp_pwmR[i]-errorValueR[i];
            addPwmR[i] = (temp_pwmR[i]-nowPwmR[i])/frequency;
        }
    }
}

/************************************************************************
函数名称：	upData(void)
功能描述： 立即将nowPwm数据更新输出
入口参数： none
返 回 值： none
其他说明： 该函数将nowPwm数据直接输出到定义好的IO端口
**************************************************************************/
void upData(void)
{
    L1 = nowPwmL[0];
    L2 = nowPwmL[1];
    L3 = nowPwmL[2];

    L4 = nowPwmL[3];
    L5 = nowPwmL[4];
    L6 = nowPwmL[5];

    L7 = nowPwmL[6];
    L8 = nowPwmL[7];
    L9 = nowPwmL[8];

    L10 = nowPwmL[9];
    L11 = nowPwmL[10];
    L12 = nowPwmL[11];

    R1 = nowPwmR[0];
    R2 = nowPwmR[1];
    R3 = nowPwmR[2];

    R4 = nowPwmR[3];
    R5 = nowPwmR[4];
    R6 = nowPwmR[5];

    R7 = nowPwmR[6];
    R8 = nowPwmR[7];
    R9 = nowPwmR[8];

    R10 = nowPwmR[9];
    R11 = nowPwmR[10];
    R12 = nowPwmR[11];
}

/************************************************************************
函数名称：	doAction(const short *actionArr,short length) 
功能描述： 执行动作数组
入口参数： 指针型的actionArr length
返 回 值： none
其他说明： 该函数将传入的动作数组指针actionArr，以及actionArr的长度length(一定
						要是25的倍数 每组第25位为执行的时间间隔)有兴趣的可根据需要自行修改。
**************************************************************************/
void doAction(const short *actionArr,short length) 
{
    short i = 0;
    char j = 0;
    short temp_pwmL[12];//临时变量
    short temp_pwmR[12];
//	for( i = 0 ; i<length ; i+=25 )	//不带动作中途中断功能
    for( i = 0 ; i<length && USART_RX_BUF[0] ; i+=25 )	//需要接收到停止命令的时候，当前动作才可以被打断
    {
        for( j = 0 ; j<12 ; j++ )
        {
            temp_pwmL[j] = actionArr[i+j];
            temp_pwmR[j] = actionArr[i+12+j];
        }
        countAddPwm(temp_pwmL,temp_pwmR,actionArr[i+24]/eachFrequencyTime);//50ms为定时器更新pwm的周期
        delay_ms(actionArr[i+24]);//时间延缓 舵机减速控制的一部分
	
				//添加以下代码 可以在做动作的同时进行语音对话
        if( usart3ReceiveSuccess2 )//如果语音缓冲区表明接收到完成指令帧
        {
            if( USART_RX_BUF2[0] == 'Y' )//语音命令帧头标志
                if( USART_RX_BUF2[1] == 'S' )
                    YSOrder( USART_RX_BUF2[2] );//语音设置命令
                else if( USART_RX_BUF2[1] == 'T' ) //语音转发
                    XFS_FrameInfo( &USART_RX_BUF2[2] );
            usart3ReceiveSuccess2 = 0;//标志位清零

        }
				
    }
}
