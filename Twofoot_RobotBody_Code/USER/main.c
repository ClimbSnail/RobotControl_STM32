//21���ɶ�˫��AI��л�����
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

char stepOrder = 0;//����executeOrder(unsigned char order)������ǰ���Ĳ�������

/************************************************************************
�������ƣ�	executeOrder(unsigned char order)
���������� ����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
��ڲ����� order�������
�� �� ֵ�� none
����˵���� ִ�ж���ָ�� order��Χ0x00~0x40 �ܹ�������65��ָ��(0x0D��Ϊ����֡
					���������,��ʱ������)
**************************************************************************/
void executeOrder(unsigned char order)
{
    switch(order)
    {
    case 0x01:
        LED=0;
        break;//�ر�����
    case 0x02:
        LED=1;
        break;//��������
    case 0x03:
        Infra_red=0;
        break;//�رռ���
    case 0x04:
        Infra_red=1;
        break;//��������
    case 0x05:
        actionInit();//������ʼ��
        break;
    case 0x06:	//ǰ��
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
    case 0x07:		//����
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
        doAction(calibrationActionArr,25);//ִ��У׼
        break;
    case 0x09:
        doAction(attentionActionArr,25);//ִ��0������
        break;
    case 0x0A://ִ��1��ѭ��ǰ��
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
    case 0x0B:	//2��ѭ������
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
        doAction(turnLActionArr,100);//ִ��3����ת
        break;
    case 0x0E:
        doAction(turnRActionArr,100);//ִ��4����ת
        break;
    case 0x0F:
        doAction(rollForwardActionArr,250);//ִ��5��ǰ����
        break;
    case 0x10:
        doAction(backflipActionArr,275);//ִ��6�ź����
        break;
    case 0x11:
        doAction(FWCActionArr,450);//ִ��7�Ÿ��Գ�
        break;
    case 0x12:
        doAction(abdominalCurActionArr,450);//ִ��8����������
        break;
    case 0x13:
        doAction(waveActionArr,275);//ִ��9�Ż���
        break;
    case 0x14:
        doAction(bowActionArr,175);//ִ��10�žϹ�
        break;
    case 0x15:
        doAction(sideslipLActionArr,150);//ִ��11����໬
        break;
    case 0x16:
        doAction(sideslipRActionArr,150);//ִ��12���Ҳ໬ ����
        break;
    case 0x17:
        doAction(DPZCActionArr,725);//ִ��13�Ŵ���չ�� ����
        break;
    case 0x18:
        doAction(sitDownActionArr,25);//ִ��14���¶�
        break;
    case 0x19:
        doAction(KHDXActionArr,550);//ִ��15�ſ������Ц
        break;
    case 0x1A:
        doAction(hipHopActionArr,3400);//ִ��16�Ž��� ����
        break;
    case 0x1B:
        doAction(JNStyleActionArr,5675);//ִ��17�Ž���style�赸
        break;
    case 0x1C:
        doAction(XPGActionArr,4550);//ִ��18��Сƻ���赸
        break;
    case 0x1D:
        doAction(laSongActionArr,3450);//ִ��19��La Song�赸
        break;
    case 0x1E:
        doAction(BESActionArr,5150);//ִ��20�ű���ˬ�赸
        break;
    case 0x1F:
        doAction(yS1ActionArr,425);//ִ����ʾ����1
        doAction(sitDownActionArr,25);//ִ��14���¶�
        doAction(attentionActionArr,25);//ִ��0������
        break;
    case 0x20:
        doAction(shakeHeadActionArr,125);//ִ������ҡͷ
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
        doAction(yS2ActionArr,675);//ִ����ʾ����2
        break;
    case 0x22:
        break;
    default:
        break;
    }
}

/************************************************************************
�������ƣ�	receiveDataConvertToPwmValue()
���������� �������ģʽ ���յ�����Ϣ֡ת��ΪPwm�źŲ�������Ч ��һ��ͬʱ����ִ��
					60������
��ڲ����� none
�� �� ֵ�� none
����˵���� 
**************************************************************************/
void receiveDataConvertToPwmValue()
{
    short len = 1;//��ʶ��������ʱ��bufferָ��
    short pwmL[12];//��ŵ�ǰ�������pwmֵ
    short pwmR[12];//��ŵ�ǰ�������pwmֵ
    short actionNum = 0;//��Ŷ�����ĸ���ֵ
    short executionTime;//��ŵ�ǰ������Ҫִ�е�ʱ��
    char flag;
    char i;

    //������֡ͷ������(������ĸ���)
    while( USART_RX_BUF[len]==' ' ) len++;

    while( USART_RX_BUF[len]<58 && USART_RX_BUF[len]>47 )
    {
        actionNum = actionNum*10+USART_RX_BUF[len++]-48;
    }

    usart3ReceiveSuccess = 0;//��־λ����
    //����actionNum������
    while( actionNum-- && !usart3ReceiveSuccess )//��־λ���±���һ�Ļ�˵�����µ�ָ�� ��ֹͣ��ǰ����
    {
        executionTime = 0;
        flag = 0;
        for( i = 0 ; i<12 ; i++)
        {
            pwmL[i] = 0;
            pwmR[i] = 0;
        }
        //����L���������ֵ
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
        //����R���������ֵ
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
        //�������鶯����Ҫִ�е�ʱ��
        while( USART_RX_BUF[len]==' ' ) len++;
        while( USART_RX_BUF[len]<58 && USART_RX_BUF[len]>47 )
        {
            executionTime = executionTime*10+USART_RX_BUF[len++]-48;
        }

        countAddPwm(pwmL,pwmR,executionTime/eachFrequencyTime);//����pwm�Լ����ٱ���5,��ʱ��6����ʱ���Զ�������Ч
        delay_ms(executionTime);//�ȴ���������
    }
}

/************************************************************************
�������ƣ�	feelingDataParsing()
���������� ���ģʽ���ݽ�����ִ��
��ڲ����� none
�� �� ֵ�� none
����˵���� Ŀǰ��δ��Ӵ˹���
**************************************************************************/
void feelingDataParsing()
{
    short pwmL[12];//��ŵ�ǰ�������pwmֵ
    short pwmR[12];//��ŵ�ǰ�������pwmֵ
    char i;
    for( i = 0 ; i<12 ; i++ )
    {
        pwmL[i] = USART_RX_BUF[i+1]<<8+USART_RX_BUF[i+2];
        pwmR[i] = USART_RX_BUF[i+25]<<8+USART_RX_BUF[i+26];
    }
//		countAddPwm(pwmL,pwmR,executionTime/50);//����pwm�Լ����ٱ���5,��ʱ��6����ʱ���Զ�������Ч
//		delay_ms(executionTime);//�ȴ���������
}

/************************************************************************
�������ƣ�	checkAndDealY( void )
���������� ��鲢��������ָ��
��ڲ����� none
�� �� ֵ�� none
����˵���� ��ͷ��YS��Ϊ��������ָ�� ��ͷ��YT��Ϊ����ת��ָ��
**************************************************************************/
char checkAndDealY( void )
{
    if( usart3ReceiveSuccess2 )//��������������������յ����ָ��֡
    {
        if( USART_RX_BUF2[0] == 'Y' )//��������֡ͷ��־
            if( USART_RX_BUF2[1] == 'S' )
                YSOrder( USART_RX_BUF2[2] );//������������
            else if( USART_RX_BUF2[1] == 'T' ) //����ת��
                XFS_FrameInfo( &USART_RX_BUF2[2] );
        usart3ReceiveSuccess2 = 0;//��־λ����

        return 1;
    }
    else
        return 0;
}

/************************************************************************
�������ƣ�	checkAndDealActionDebug( void )
���������� ��鲢������������ָ��
��ڲ����� none
�� �� ֵ�� none
����˵���� ��ͷ��D��ΪDebugģʽ ����Ϊ����ָ��
**************************************************************************/
char checkAndDealActionDebug( void )
{
    if( usart3ReceiveSuccess )//�Ƿ��ѽ�������֡
    {
        if( USART_RX_BUF[0] == 'D' )//�������ģʽ֡ͷ��־
            receiveDataConvertToPwmValue();//�������ģʽ ��һ��ͬʱ����ִ��60������

        while( USART_RX_BUF[0] && USART_RX_BUF[0] < 0x41 )//���ѭ����Ŀ����ֻҪ����һ��ָ��Ϳ��Բ�ִͣ��һ������ ֱ����һ��ָ��
            executeOrder(USART_RX_BUF[0]);//ִ�ж���ָ�� ��Ҫ���յ�ֹͣ�����ʱ�򣬵�ǰ�����ſ��Ա����

        usart3ReceiveSuccess = 0;//��־λ����
        return 1;
    }
    else
        return 0;
}

int main(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    u16 adcx;
    delay_init();	    	 //��ʱ������ʼ��

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�������ȼ�����
    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    DMA_Config(DMA1_Channel1,(u32)ADC1_DR_Address,(u32)&AD_Value,8);//cndtr��Ӧ�����ŵ�����
    Adc_Init();		//ADC��ʼ��
    DMA_Cmd(DMA1_Channel1, ENABLE);      //����DMAͨ��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  //�������ADת��

    LED_Init();
    LED = 0;//�ر�����
    Infra_red = 0;//�رպ���
    delay_ms(800);
    OLED_Init();			//��ʼ��OLED
    OLED_Clear();
    OLED_ShowCHinese(0,0,0);//��
    OLED_ShowNum(6,3,23,2,16);
    uart_init(115200);

    uart5_init(9600);//������ģ��������

    TIM1_PWM_Init(5000-1,72-1);    //����Ƶ��PWMƵ��=72000/(899+1)=80Khz
    TIM3_PWM_Init(5000-1,72-1);
    TIM2_PWM_Init(5000-1,72-1);
    TIM4_PWM_Init(5000-1,72-1);
    TIM5_PWM_Init(5000-1,72-1);
    TIM8_PWM_Init(5000-1,72-1);
    TIM6_Int_Init(eachFrequencyTime*10-1,7199);   //50ms eachFrequencyTimeֵΪ50
//    actionInit();//������ʼ��	���ж������λ1500us
		doAction(attentionActionArr,25);//ִ��0������ ʵ���ϸ��ϵ�������״̬
    while( 0 )
    {

        delay_ms(1500);
        XFS_FrameInfo("��ʾADC");//�����ϳ�ģ�鲥��
//				OLED_ShowNum(0,0,AD_Value[0],4,16);//��ʾADC��ֵ
//        OLED_ShowNum(70,0,AD_Value[1],4,16);//��ʾADC��ֵ
//        OLED_ShowNum(0,2,AD_Value[2],4,16);//��ʾADC��ֵ
//        OLED_ShowNum(70,2,AD_Value[3],4,16);//��ʾADC��ֵ
//        OLED_ShowNum(0,4,AD_Value[4],4,16);//��ʾADC��ֵ
//        OLED_ShowNum(70,4,AD_Value[5],4,16);//��ʾADC��ֵ
//        OLED_ShowNum(0,6,AD_Value[6],4,16);//��ʾADC��ֵ
//        OLED_ShowNum(70,6,AD_Value[7],4,16);//��ʾADC��ֵ
    }
    while(1)
    {
        checkAndDealActionDebug();	//��鲢������������ָ��
        checkAndDealY();	//��鲢��������ָ��
    }
}

