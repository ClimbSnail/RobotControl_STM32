#ifndef __DMA_H
#define __DMA_H	
#include "sys.h"

#define ADC1_DR_Address    ((uint32_t)0x4001244C) 
	 
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);

#endif 

