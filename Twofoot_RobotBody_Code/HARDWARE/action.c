#include "action.h"
#include "delay.h"
#include "XFS5152V2.h"
#include "MY_USART3.h"

//�ϵ��������״̬(��ʼֵ����һ��վ��״̬)
short nowPwmL[12] = {1500,1500,1500, 1500,1700,2300, 1900,1500,1800, 1700,1000,1500};
short nowPwmR[12] = {1500,1500,1500, 1500,1300,700, 1100,1500,1200, 1300,2000,1500};//Ŀ��Pwm
short targetPwmL[12] = {1500,1500,1500, 1500,1700,2300, 1900,1500,1800, 1700,1000,1500};
short targetPwmR[12] = {1500,1500,1500, 1500,1300,700, 1100,1500,1200, 1300,2000,1500};//Ŀ��Pwm
//ÿ�ζ�ʱ�ж�ʱҪ���ӵĶ����
short addPwmR[12]= {0,0,0, 0,0,0, 0,0,0, 0,0,0};
short addPwmL[12]= {0,0,0, 0,0,0, 0,0,0, 0,0,0};
//����İ�װ���
short errorValueL[12] = {0,0,0, 0,0,0, 0,0,0, 0,0,0};
short errorValueR[12] = {0,0,0, 0,0,0, 0,0,0, 0,0,0};

void actionInit();//������ʼ��
void standUp();//վ����
void sitDown();//����
void halfSitDown();//���
void upData(void);//pwm���ݸ���
void forWordOne();//ǰ��1
void forWordTwo();//ǰ��2
void backOne();//����1
void backTwo();//����2
void countAddPwm(short temp_pwmL[12],short temp_pwmR[12],short frequency);//����pwm�Լ����ٱ���,��ʱ��6����ʱ���Զ�������Ч

void actionInit()//������ʼ��	���ж������λ1500us
{
    short temp_pwmL[12] = {1500,1500,1500, 1500,1500,1500, 1500,1500,1500, 1500,1500,1500};
    short temp_pwmR[12] = {1500,1500,1500, 1500,1500,1500, 1500,1500,1500, 1500,1500,1500};
    countAddPwm(temp_pwmL,temp_pwmR,20);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
}

void standUp()//վ����
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,0,1600, 1200,1500,0};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,0,1400, 1800,1500,0};
    countAddPwm(temp_pwmL,temp_pwmR,20);
}

void sitDown()//����
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,0,2200, 2300,500,0};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,0,800, 700,2500,0};
    countAddPwm(temp_pwmL,temp_pwmR,20);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
}

void halfSitDown()//���
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,0,1800, 1700,1000,0};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,0,1200, 1300,2000,0};
    countAddPwm(temp_pwmL,temp_pwmR,20);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
}

void open()//�Ž�
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,1700,0, 0,0,1700};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,1300,0, 0,0,1300};
    countAddPwm(temp_pwmL,temp_pwmR,20);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
}

void rightLeaning()//����
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,1700,0, 0,0,1700};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,1700,0, 0,0,1700};
    countAddPwm(temp_pwmL,temp_pwmR,20);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
}

void leftLeaning()//����
{
    short temp_pwmL[12] = {0,0,0, 0,0,0, 0,1300,0, 0,0,1300};
    short temp_pwmR[12] = {0,0,0, 0,0,0, 0,1300,0, 0,0,1300};
    countAddPwm(temp_pwmL,temp_pwmR,20);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
}

void forWordOne()//ǰ��1
{
    short temp_pwmL1[12] = {0,1500,0, 0,0,0, 1250,0,1900, 1600,0,0};//����������
    short temp_pwmR1[12] = {0,0,0, 0,0,0, 750,0,1000, 1300,1800,0};//̧����

    short temp_pwmL2[12] = {0,1000,0, 0,0,0, 0,1500,1300, 1400,1000,1600};//������
    short temp_pwmR2[12] = {0,0,0, 0,0,0, 0,1500,1300, 1400,0,1600};

    short temp_pwmL3[12] = {0,500,0, 0,0,0, 0,1400,0, 0,0,1650};
    short temp_pwmR3[12] = {0,0,0, 0,0,0, 0,1350,0, 0,0,1650};

    countAddPwm(temp_pwmL1,temp_pwmR1,10);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
    delay_ms(700);
    countAddPwm(temp_pwmL2,temp_pwmR2,10);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
    delay_ms(700);
    countAddPwm(temp_pwmL3,temp_pwmR3,5);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
    delay_ms(300);
}

void forWordTwo()//ǰ��2
{
    short temp_pwmL4[12] = {0,1500,0, 0,0,0, 2000,1400,2000, 1700,1200,1650};//̧����
    short temp_pwmR4[12] = {0,0,0, 0,0,0, 1500,1350,1100, 1400,0,1650};//����������

    short temp_pwmL5[12] = {0,2000,0, 0,0,0, 0,1500,1700, 1600,0,1400};
    short temp_pwmR5[12] = {0,0,0, 0,0,0, 0,1500,1700, 1600,2000,1400};//������

    short temp_pwmL6[12] = {0,2500,0, 0,0,0, 0,1650,0, 0,0,1350};
    short temp_pwmR6[12] = {0,0,0, 0,0,0, 0,1600,0, 0,0,1350};

    countAddPwm(temp_pwmL4,temp_pwmR4,10);	//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
    delay_ms(700);
    countAddPwm(temp_pwmL5,temp_pwmR5,10);	//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
    delay_ms(700);
    countAddPwm(temp_pwmL6,temp_pwmR6,5);	//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
    delay_ms(300);
}


/************************************************************************
�������ƣ�	countAddPwm(short temp_pwmL[12],short temp_pwmR[12],short frequency)
���������� ����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)
��ڲ����� temp_pwmL temp_pwmR frequency:��������
�� �� ֵ�� none
����˵���� �ú�������pwm�Լ����ٱ���(��λ����ʱ��Ԥ��ֵΪ50ms),��ʱ��6����ʱ
					���Զ�������Ч
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
�������ƣ�	upData(void)
���������� ������nowPwm���ݸ������
��ڲ����� none
�� �� ֵ�� none
����˵���� �ú�����nowPwm����ֱ�����������õ�IO�˿�
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
�������ƣ�	doAction(const short *actionArr,short length) 
���������� ִ�ж�������
��ڲ����� ָ���͵�actionArr length
�� �� ֵ�� none
����˵���� �ú���������Ķ�������ָ��actionArr���Լ�actionArr�ĳ���length(һ��
						Ҫ��25�ı��� ÿ���25λΪִ�е�ʱ����)����Ȥ�Ŀɸ�����Ҫ�����޸ġ�
**************************************************************************/
void doAction(const short *actionArr,short length) 
{
    short i = 0;
    char j = 0;
    short temp_pwmL[12];//��ʱ����
    short temp_pwmR[12];
//	for( i = 0 ; i<length ; i+=25 )	//����������;�жϹ���
    for( i = 0 ; i<length && USART_RX_BUF[0] ; i+=25 )	//��Ҫ���յ�ֹͣ�����ʱ�򣬵�ǰ�����ſ��Ա����
    {
        for( j = 0 ; j<12 ; j++ )
        {
            temp_pwmL[j] = actionArr[i+j];
            temp_pwmR[j] = actionArr[i+12+j];
        }
        countAddPwm(temp_pwmL,temp_pwmR,actionArr[i+24]/eachFrequencyTime);//50msΪ��ʱ������pwm������
        delay_ms(actionArr[i+24]);//ʱ���ӻ� ������ٿ��Ƶ�һ����
	
				//������´��� ��������������ͬʱ���������Ի�
        if( usart3ReceiveSuccess2 )//��������������������յ����ָ��֡
        {
            if( USART_RX_BUF2[0] == 'Y' )//��������֡ͷ��־
                if( USART_RX_BUF2[1] == 'S' )
                    YSOrder( USART_RX_BUF2[2] );//������������
                else if( USART_RX_BUF2[1] == 'T' ) //����ת��
                    XFS_FrameInfo( &USART_RX_BUF2[2] );
            usart3ReceiveSuccess2 = 0;//��־λ����

        }
				
    }
}
