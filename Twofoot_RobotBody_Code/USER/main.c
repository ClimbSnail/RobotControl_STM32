//21自由度双足AI体感机器人
//Twofoot_RobotBody

//#include "stm32f10x.h"                  // Device header
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
#include "oled.h"
#include "bmp.h"
#include "ADC.h"
#include "MY_USART3.h"
#include "action.h"
#include "DMA.H"
#include "uart5.h"
#include "XFS5152V2.h"

char stepOrder = 0;//用于executeOrder(unsigned char order)函数中前进的步伐控制

/************************************************************************
函数名称：	executeOrder(unsigned char order)
功能描述： 计算每次pwm更新需要多大的增量(用于减速控制)
入口参数： order命令代号
返 回 值： none
其他说明： 执行动画指令 order范围0x00~0x40 总共能容纳65条指令(0x0D因为接受帧
					处理的问题,暂时不能用)
**************************************************************************/
void executeOrder(unsigned char order)
{
    switch(order)
    {
    case 0x01:
        LED=0;
        break;//关闭照明
    case 0x02:
        LED=1;
        break;//开启照明
    case 0x03:
        Infra_red=0;
        break;//关闭激光
    case 0x04:
        Infra_red=1;
        break;//开启激光
    case 0x05:
        actionInit();//动作初始化
        break;
    case 0x06:	//前进
        if( stepOrder )
        {
            forWordOne();
            stepOrder = 0;
        }
        else
        {
            forWordTwo();
            stepOrder = 1;
        }
        break;
    case 0x07:		//后退
//			if( stepOrder ){
//        backOne();
//				stepOrder = 0;
//			}
//			else{
//        backTwo();
//				stepOrder = 1;
//			}
        break;
    case 0x08:
        doAction(calibrationActionArr,25);//执行校准
        break;
    case 0x09:
        doAction(attentionActionArr,25);//执行0号立正
        break;
    case 0x0A://执行1号循环前进
        if( stepOrder )
        {
            doAction(f1ActionArr,50);
            stepOrder = 0;
        }
        else
        {
            doAction(f2ActionArr,50);
            stepOrder = 1;
        }
        break;
    case 0x0B:	//2号循环后退
        if( stepOrder )
        {
            doAction(b1ActionArr,50);
            stepOrder = 0;
        }
        else
        {
            doAction(b2ActionArr,50);
            stepOrder = 1;
        }
        break;
    case 0x0C:
        doAction(turnLActionArr,100);//执行3号左转
        break;
    case 0x0E:
        doAction(turnRActionArr,100);//执行4号右转
        break;
    case 0x0F:
        doAction(rollForwardActionArr,250);//执行5号前滚翻
        break;
    case 0x10:
        doAction(backflipActionArr,275);//执行6号后滚翻
        break;
    case 0x11:
        doAction(FWCActionArr,450);//执行7号俯卧撑
        break;
    case 0x12:
        doAction(abdominalCurActionArr,450);//执行8号仰卧起坐
        break;
    case 0x13:
        doAction(waveActionArr,275);//执行9号挥手
        break;
    case 0x14:
        doAction(bowActionArr,175);//执行10号鞠躬
        break;
    case 0x15:
        doAction(sideslipLActionArr,150);//执行11号左侧滑
        break;
    case 0x16:
        doAction(sideslipRActionArr,150);//执行12号右侧滑 问题
        break;
    case 0x17:
        doAction(DPZCActionArr,725);//执行13号大鹏展翅 问题
        break;
    case 0x18:
        doAction(sitDownActionArr,25);//执行14号下蹲
        break;
    case 0x19:
        doAction(KHDXActionArr,550);//执行15号开怀大大笑
        break;
    case 0x1A:
        doAction(hipHopActionArr,3400);//执行16号街舞 问题
        break;
    case 0x1B:
        doAction(JNStyleActionArr,5675);//执行17号江南style舞蹈
        break;
    case 0x1C:
        doAction(XPGActionArr,4550);//执行18号小苹果舞蹈
        break;
    case 0x1D:
        doAction(laSongActionArr,3450);//执行19号La Song舞蹈
        break;
    case 0x1E:
        doAction(BESActionArr,5150);//执行20号倍儿爽舞蹈
        break;
    case 0x1F:
        doAction(yS1ActionArr,425);//执行演示动作1
        doAction(sitDownActionArr,25);//执行14号下蹲
        doAction(attentionActionArr,25);//执行0号立正
        break;
    case 0x20:
        doAction(shakeHeadActionArr,125);//执行左右摇头
        break;
    case 0x21:
        doAction(f1ActionArr,50);
        delay_ms(200);
        doAction(f2ActionArr,50);
        delay_ms(200);
        doAction(f1ActionArr,50);
        delay_ms(200);
        doAction(f2ActionArr,50);
        delay_ms(200);
        doAction(f1ActionArr,50);
        delay_ms(200);
        doAction(f2ActionArr,50);
        delay_ms(200);
        doAction(yS2ActionArr,675);//执行演示动作2
        break;
    case 0x22:
        break;
    default:
        break;
    }
}

/************************************************************************
函数名称：	receiveDataConvertToPwmValue()
功能描述： 数组调试模式 接收到的信息帧转换为Pwm信号并缓慢生效 可一次同时调试执行
					60个动作
入口参数： none
返 回 值： none
其他说明： 
**************************************************************************/
void receiveDataConvertToPwmValue()
{
    short len = 1;//标识处理数据时的buffer指针
    short pwmL[12];//存放当前动作组的pwm值
    short pwmR[12];//存放当前动作组的pwm值
    short actionNum = 0;//存放动作组的个数值
    short executionTime;//存放当前动作组要执行的时间
    char flag;
    char i;

    //解析出帧头的数据(动作组的个数)
    while( USART_RX_BUF[len]==' ' ) len++;

    while( USART_RX_BUF[len]<58 && USART_RX_BUF[len]>47 )
    {
        actionNum = actionNum*10+USART_RX_BUF[len++]-48;
    }

    usart3ReceiveSuccess = 0;//标志位清零
    //解析actionNum次数据
    while( actionNum-- && !usart3ReceiveSuccess )//标志位重新被置一的话说明有新的指令 需停止当前动作
    {
        executionTime = 0;
        flag = 0;
        for( i = 0 ; i<12 ; i++)
        {
            pwmL[i] = 0;
            pwmR[i] = 0;
        }
        //解析L动作数组的值
        while( flag<12 )
        {
            while( USART_RX_BUF[len++]!=' ' );
            while( USART_RX_BUF[len]<58 && USART_RX_BUF[len]>47 )
            {
                pwmL[flag] = pwmL[flag]*10+USART_RX_BUF[len++]-48;
            }
            if(  USART_RX_BUF[len-1]<58 && USART_RX_BUF[len-1]>47  )
                flag++;
        }
        //解析R动作数组的值
        flag = 0;
        while( flag<12 )
        {
            while( USART_RX_BUF[len++]!=' ' );
            while( USART_RX_BUF[len]<58 && USART_RX_BUF[len]>47 )
            {
                pwmR[flag] = pwmR[flag]*10+USART_RX_BUF[len++]-48;
            }
            if(  USART_RX_BUF[len-1]<58 && USART_RX_BUF[len-1]>47  )
                flag++;
        }
        //解析此组动作组要执行的时间
        while( USART_RX_BUF[len]==' ' ) len++;
        while( USART_RX_BUF[len]<58 && USART_RX_BUF[len]>47 )
        {
            executionTime = executionTime*10+USART_RX_BUF[len++]-48;
        }

        countAddPwm(pwmL,pwmR,executionTime/eachFrequencyTime);//传入pwm以及减速倍率5,定时器6开启时将自动缓慢生效
        delay_ms(executionTime);//等待动作结束
    }
}

/************************************************************************
函数名称：	feelingDataParsing()
功能描述： 体感模式数据解析并执行
入口参数： none
返 回 值： none
其他说明： 目前还未添加此功能
**************************************************************************/
void feelingDataParsing()
{
    short pwmL[12];//存放当前动作组的pwm值
    short pwmR[12];//存放当前动作组的pwm值
    char i;
    for( i = 0 ; i<12 ; i++ )
    {
        pwmL[i] = USART_RX_BUF[i+1]<<8+USART_RX_BUF[i+2];
        pwmR[i] = USART_RX_BUF[i+25]<<8+USART_RX_BUF[i+26];
    }
//		countAddPwm(pwmL,pwmR,executionTime/50);//传入pwm以及减速倍率5,定时器6开启时将自动缓慢生效
//		delay_ms(executionTime);//等待动作结束
}

/************************************************************************
函数名称：	checkAndDealY( void )
功能描述： 检查并处理语音指令
入口参数： none
返 回 值： none
其他说明： 开头‘YS’为语音设置指令 开头‘YT’为语音转发指令
**************************************************************************/
char checkAndDealY( void )
{
    if( usart3ReceiveSuccess2 )//如果语音缓冲区表明接收到完成指令帧
    {
        if( USART_RX_BUF2[0] == 'Y' )//语音命令帧头标志
            if( USART_RX_BUF2[1] == 'S' )
                YSOrder( USART_RX_BUF2[2] );//语音设置命令
            else if( USART_RX_BUF2[1] == 'T' ) //语音转发
                XFS_FrameInfo( &USART_RX_BUF2[2] );
        usart3ReceiveSuccess2 = 0;//标志位清零

        return 1;
    }
    else
        return 0;
}

/************************************************************************
函数名称：	checkAndDealActionDebug( void )
功能描述： 检查并处理动作及调试指令
入口参数： none
返 回 值： none
其他说明： 开头‘D’为Debug模式 其余为动作指令
**************************************************************************/
char checkAndDealActionDebug( void )
{
    if( usart3ReceiveSuccess )//是否已接收完整帧
    {
        if( USART_RX_BUF[0] == 'D' )//数组调试模式帧头标志
            receiveDataConvertToPwmValue();//数组调试模式 可一次同时调试执行60个动作

        while( USART_RX_BUF[0] && USART_RX_BUF[0] < 0x41 )//添加循环的目的是只要发送一次指令就可以不停执行一个动作 直到下一条指令
            executeOrder(USART_RX_BUF[0]);//执行动作指令 需要接收到停止命令的时候，当前动作才可以被打断

        usart3ReceiveSuccess = 0;//标志位清零
        return 1;
    }
    else
        return 0;
}

int main(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    u16 adcx;
    delay_init();	    	 //延时函数初始化

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置优先级分组
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    DMA_Config(DMA1_Channel1,(u32)ADC1_DR_Address,(u32)&AD_Value,8);//cndtr对应所开信道个数
    Adc_Init();		//ADC初始化
    DMA_Cmd(DMA1_Channel1, ENABLE);      //启动DMA通道
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  //软件启动AD转换

    LED_Init();
    LED = 0;//关闭照明
    Infra_red = 0;//关闭红外
    delay_ms(800);
    OLED_Init();			//初始化OLED
    OLED_Clear();
    OLED_ShowCHinese(0,0,0);//中
    OLED_ShowNum(6,3,23,2,16);
    uart_init(115200);

    uart5_init(9600);//与语音模块相连接

    TIM1_PWM_Init(5000-1,72-1);    //不分频。PWM频率=72000/(899+1)=80Khz
    TIM3_PWM_Init(5000-1,72-1);
    TIM2_PWM_Init(5000-1,72-1);
    TIM4_PWM_Init(5000-1,72-1);
    TIM5_PWM_Init(5000-1,72-1);
    TIM8_PWM_Init(5000-1,72-1);
    TIM6_Int_Init(eachFrequencyTime*10-1,7199);   //50ms eachFrequencyTime值为50
//    actionInit();//动作初始化	所有舵机归中位1500us
		doAction(attentionActionArr,25);//执行0号立正 实际上刚上电就是这个状态
    while( 0 )
    {

        delay_ms(1500);
        XFS_FrameInfo("显示ADC");//语音合成模块播放
//				OLED_ShowNum(0,0,AD_Value[0],4,16);//显示ADC的值
//        OLED_ShowNum(70,0,AD_Value[1],4,16);//显示ADC的值
//        OLED_ShowNum(0,2,AD_Value[2],4,16);//显示ADC的值
//        OLED_ShowNum(70,2,AD_Value[3],4,16);//显示ADC的值
//        OLED_ShowNum(0,4,AD_Value[4],4,16);//显示ADC的值
//        OLED_ShowNum(70,4,AD_Value[5],4,16);//显示ADC的值
//        OLED_ShowNum(0,6,AD_Value[6],4,16);//显示ADC的值
//        OLED_ShowNum(70,6,AD_Value[7],4,16);//显示ADC的值
    }
    while(1)
    {
        checkAndDealActionDebug();	//检查并处理动作及调试指令
        checkAndDealY();	//检查并处理语音指令
    }
}

