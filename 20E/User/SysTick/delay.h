#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(__IO u32 nTime);
#define Delay_ms(x) Delay_us(100*x)	 //µ¥Î»ms

void delay_us( __IO uint32_t us);
void delay_ms( __IO uint32_t ms);


#endif /* __DELAY_H */
