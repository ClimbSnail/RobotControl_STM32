#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

#define TIM1_PWM_1 TIM1->CCR1
#define TIM1_PWM_2 TIM1->CCR2
#define TIM1_PWM_3 TIM1->CCR3
#define TIM1_PWM_4 TIM1->CCR4

#define TIM2_PWM_5 TIM2->CCR1
#define TIM2_PWM_6 TIM2->CCR2
#define TIM2_PWM_7 TIM2->CCR3
#define TIM2_PWM_8 TIM2->CCR4

#define TIM3_PWM_9 TIM3->CCR1
#define TIM3_PWM_10 TIM3->CCR2
#define TIM3_PWM_11 TIM3->CCR3
#define TIM3_PWM_12 TIM3->CCR4

#define TIM4_PWM_13 TIM4->CCR1
#define TIM4_PWM_14 TIM4->CCR2
#define TIM4_PWM_15 TIM4->CCR3
#define TIM4_PWM_16 TIM4->CCR4

#define TIM8_PWM_17 TIM8->CCR1
#define TIM8_PWM_18 TIM8->CCR2
#define TIM8_PWM_19 TIM8->CCR3
#define TIM8_PWM_20 TIM8->CCR4

#define TIM5_PWM_21 TIM5->CCR1
#define TIM5_PWM_22 TIM5->CCR2
#define TIM5_PWM_23 TIM5->CCR3
#define TIM5_PWM_24 TIM5->CCR4

#define  L3 TIM1_PWM_1
#define  L4 TIM1_PWM_2
#define  L5 TIM1_PWM_3 
#define  L6 TIM1_PWM_4

#define  L7 TIM2_PWM_5
#define  L8 TIM2_PWM_6 
#define  R4 TIM2_PWM_7 
#define  R3 TIM2_PWM_8 

#define  R8 TIM3_PWM_9 
#define  R7 TIM3_PWM_10
#define  R6 TIM3_PWM_11
#define  R5 TIM3_PWM_12 

#define  L9 TIM4_PWM_13 
#define  L10 TIM4_PWM_14
#define  L11 TIM4_PWM_15
#define  L12 TIM4_PWM_16

#define  R2 TIM8_PWM_17
#define  R1 TIM8_PWM_18
#define  L1 TIM8_PWM_19
#define  L2 TIM8_PWM_20

#define  R12 TIM5_PWM_21
#define  R11 TIM5_PWM_22
#define  R10 TIM5_PWM_23
#define  R9 TIM5_PWM_24

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIM5_PWM_Init(u16 arr,u16 psc);
void TIM8_PWM_Init(u16 arr,u16 psc);
//void TIM2_Int_Init(u16 arr,u16 psc);
void TIM6_Int_Init(u16 arr,u16 psc);
#endif

