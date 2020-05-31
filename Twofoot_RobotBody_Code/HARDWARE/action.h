#ifndef __ACTION_H
#define __ACTION_H

#include "delay.h"
#include "sys.h"
#include "pwm.h"

#define eachFrequencyTime 50		//舵机减缓单位时间(预设50ms 可更改)

extern void actionInit();//动作初始化
extern void standUp();//站起来
extern void sitDown();//蹲下
extern void halfSitDown();//半蹲
extern void leftLeaning();//左倾
extern void rightLeaning();//右倾
extern void open();//张脚
extern void forWordOne();//前进1
extern void forWordTwo();//前进2
extern void backOne();//后退1
extern void backTwo();//后退2
extern void upData(void);//立即将nowPwm数据更新输出
extern void countAddPwm(short temp_pwmL[12],short temp_pwmR[12],short frequency);//计算每次pwm更新需要多大的增量(用于减速控制)

extern short targetPwmR[12];//储存目标PWM
extern short targetPwmL[12];//储存目标PWM
extern short nowPwmR[12];//现在的Pwm
extern short nowPwmL[12];
extern short addPwmR[12];//每次自增的Pwm量
extern short addPwmL[12];

extern void doAction(const short *actionArr,short length); //执行动作数组函数

//以下是自编动作组
extern const short yS2ActionArr[675] __attribute__((at(0x0801D098)));//0x0801D098~0x0801D5DD 		  演示动作2
extern const short yS1ActionArr[425] __attribute__((at(0x0801CD46)));//0x0801CD46~0x0801D097 		  演示动作1
extern const short shakeHeadActionArr[125] __attribute__((at(0x0801CC4C)));//0x0801CC4C~0x0801CD45 		  左右摇头


//以下是乐幻索尔的动作组数据
extern const short calibrationActionArr[25] __attribute__((at(0x0801CC1A)));//0x0801CC1A~0x0801CC4C 		  校准
extern const short attentionActionArr[25] __attribute__((at(0x0801CBE8)));//0x0801CBE8~0x0801CC19 		   	0号立正
extern const short f1ActionArr[50] __attribute__((at(0x0801CB84)));//0x0801CB84~0x0801CBE7 		   				  1号循环前进1
extern const short f2ActionArr[50] __attribute__((at(0x0801CB20)));//0x0801CB20~0x0801CB83 		   				  1号循环前进2
extern const short b1ActionArr[50] __attribute__((at(0x0801CABC)));//0x0801CABC~0x0801CB1F 		   					2号循环后退1
extern const short b2ActionArr[50] __attribute__((at(0x0801CA58)));//0x0801CA58~0x0801CABB 		   					2号循环后退2
extern const short turnLActionArr[100] __attribute__((at(0x0801C990)));//0x0801C990~0x0801CA57 		   		  3号左转
extern const short turnRActionArr[100] __attribute__((at(0x0801C8C8)));//0x0801C8C8~0x0801C98F 		   			4号右转
extern const short rollForwardActionArr[250] __attribute__((at(0x0801C6D4)));//0x0801C6D4~0x0801C8C7 			5号前滚翻
extern const short backflipActionArr[275] __attribute__((at(0x0801C4AE)));//0x0801C4AE~0x0801C6D3 	    	6号后滚翻
extern const short FWCActionArr[450] __attribute__((at(0x0801C12A)));//0x0801C12A~0x0801C4AD 		   				7号俯卧撑
extern const short abdominalCurActionArr[450] __attribute__((at(0x0801BDA6)));//0x0801BDA6~0x0801C129		  8号仰卧起坐
extern const short waveActionArr[275] __attribute__((at(0x0801BB80)));//0x0801BB80~0x0801BDA5		 					9号挥手
extern const short bowActionArr[175] __attribute__((at(0x0801BA22)));//0x0801BA22~0x0801BB7F							10号鞠躬
extern const short sideslipLActionArr[150] __attribute__((at(0x0801B8F6)));//0x0801B8F6~0x0801BA21				11号左侧滑
extern const short sideslipRActionArr[150] __attribute__((at(0x0801B7CA)));//0x0801B7CA~0x0801B8F5				12号右侧滑
extern const short DPZCActionArr[725] __attribute__((at(0x0801B220)));//0x0801B220~0x0801B7C9							13号大鹏展翅
extern const short sitDownActionArr[25] __attribute__((at(0x0801B1EE)));//0x0801B1EE~0x0801B21F						14号下蹲
extern const short KHDXActionArr[550] __attribute__((at(0x0801ADA2)));//0x0801ADA2~0x0801B1ED							15号开怀大大笑
extern const short hipHopActionArr[3400] __attribute__((at(0x08019312)));//0x08019312~0x0801ADA1					16号街舞
extern const short JNStyleActionArr[5675] __attribute__((at(0x080166BC)));//0x080166BC~0x08019311					17号江南style舞蹈
extern const short XPGActionArr[4550] __attribute__((at(0x08014330)));//0x08014330~0x080166BB							18号小苹果舞蹈
extern const short laSongActionArr[3450] __attribute__((at(0x0801283C)));//0x0801283C~0x0801432F					19号La Song舞蹈
extern const short BESActionArr[5150] __attribute__((at(0x08010000)));//0x08010000~0x0801283B 						20号倍儿爽舞蹈


#endif

