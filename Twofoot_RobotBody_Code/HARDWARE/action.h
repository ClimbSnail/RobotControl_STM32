#ifndef __ACTION_H
#define __ACTION_H

#include "delay.h"
#include "sys.h"
#include "pwm.h"

#define eachFrequencyTime 50		//���������λʱ��(Ԥ��50ms �ɸ���)

extern void actionInit();//������ʼ��
extern void standUp();//վ����
extern void sitDown();//����
extern void halfSitDown();//���
extern void leftLeaning();//����
extern void rightLeaning();//����
extern void open();//�Ž�
extern void forWordOne();//ǰ��1
extern void forWordTwo();//ǰ��2
extern void backOne();//����1
extern void backTwo();//����2
extern void upData(void);//������nowPwm���ݸ������
extern void countAddPwm(short temp_pwmL[12],short temp_pwmR[12],short frequency);//����ÿ��pwm������Ҫ��������(���ڼ��ٿ���)

extern short targetPwmR[12];//����Ŀ��PWM
extern short targetPwmL[12];//����Ŀ��PWM
extern short nowPwmR[12];//���ڵ�Pwm
extern short nowPwmL[12];
extern short addPwmR[12];//ÿ��������Pwm��
extern short addPwmL[12];

extern void doAction(const short *actionArr,short length); //ִ�ж������麯��

//�������Աද����
extern const short yS2ActionArr[675] __attribute__((at(0x0801D098)));//0x0801D098~0x0801D5DD 		  ��ʾ����2
extern const short yS1ActionArr[425] __attribute__((at(0x0801CD46)));//0x0801CD46~0x0801D097 		  ��ʾ����1
extern const short shakeHeadActionArr[125] __attribute__((at(0x0801CC4C)));//0x0801CC4C~0x0801CD45 		  ����ҡͷ


//�������ֻ������Ķ���������
extern const short calibrationActionArr[25] __attribute__((at(0x0801CC1A)));//0x0801CC1A~0x0801CC4C 		  У׼
extern const short attentionActionArr[25] __attribute__((at(0x0801CBE8)));//0x0801CBE8~0x0801CC19 		   	0������
extern const short f1ActionArr[50] __attribute__((at(0x0801CB84)));//0x0801CB84~0x0801CBE7 		   				  1��ѭ��ǰ��1
extern const short f2ActionArr[50] __attribute__((at(0x0801CB20)));//0x0801CB20~0x0801CB83 		   				  1��ѭ��ǰ��2
extern const short b1ActionArr[50] __attribute__((at(0x0801CABC)));//0x0801CABC~0x0801CB1F 		   					2��ѭ������1
extern const short b2ActionArr[50] __attribute__((at(0x0801CA58)));//0x0801CA58~0x0801CABB 		   					2��ѭ������2
extern const short turnLActionArr[100] __attribute__((at(0x0801C990)));//0x0801C990~0x0801CA57 		   		  3����ת
extern const short turnRActionArr[100] __attribute__((at(0x0801C8C8)));//0x0801C8C8~0x0801C98F 		   			4����ת
extern const short rollForwardActionArr[250] __attribute__((at(0x0801C6D4)));//0x0801C6D4~0x0801C8C7 			5��ǰ����
extern const short backflipActionArr[275] __attribute__((at(0x0801C4AE)));//0x0801C4AE~0x0801C6D3 	    	6�ź����
extern const short FWCActionArr[450] __attribute__((at(0x0801C12A)));//0x0801C12A~0x0801C4AD 		   				7�Ÿ��Գ�
extern const short abdominalCurActionArr[450] __attribute__((at(0x0801BDA6)));//0x0801BDA6~0x0801C129		  8����������
extern const short waveActionArr[275] __attribute__((at(0x0801BB80)));//0x0801BB80~0x0801BDA5		 					9�Ż���
extern const short bowActionArr[175] __attribute__((at(0x0801BA22)));//0x0801BA22~0x0801BB7F							10�žϹ�
extern const short sideslipLActionArr[150] __attribute__((at(0x0801B8F6)));//0x0801B8F6~0x0801BA21				11����໬
extern const short sideslipRActionArr[150] __attribute__((at(0x0801B7CA)));//0x0801B7CA~0x0801B8F5				12���Ҳ໬
extern const short DPZCActionArr[725] __attribute__((at(0x0801B220)));//0x0801B220~0x0801B7C9							13�Ŵ���չ��
extern const short sitDownActionArr[25] __attribute__((at(0x0801B1EE)));//0x0801B1EE~0x0801B21F						14���¶�
extern const short KHDXActionArr[550] __attribute__((at(0x0801ADA2)));//0x0801ADA2~0x0801B1ED							15�ſ������Ц
extern const short hipHopActionArr[3400] __attribute__((at(0x08019312)));//0x08019312~0x0801ADA1					16�Ž���
extern const short JNStyleActionArr[5675] __attribute__((at(0x080166BC)));//0x080166BC~0x08019311					17�Ž���style�赸
extern const short XPGActionArr[4550] __attribute__((at(0x08014330)));//0x08014330~0x080166BB							18��Сƻ���赸
extern const short laSongActionArr[3450] __attribute__((at(0x0801283C)));//0x0801283C~0x0801432F					19��La Song�赸
extern const short BESActionArr[5150] __attribute__((at(0x08010000)));//0x08010000~0x0801283B 						20�ű���ˬ�赸


#endif

