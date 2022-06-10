#include "gpio.h"
#include "stm32f10x.h"

extern int b;

void lunzi_Init(void)//对相应的IO口进行初始化,PA0-3
{

	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA的时钟
  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//输出模式为推挽输出，电平可高可低
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_2|GPIO_Pin_7;//P管脚IO口
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//管脚传输速度
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
}

void forward(void)
{
    if(b==1)
    GPIO_SetBits(GPIOA,GPIO_Pin_1);//P高电平     1
if(b==2)
    GPIO_SetBits(GPIOA,GPIO_Pin_2);//P高电平     1
if(b==3)
    GPIO_SetBits(GPIOA,GPIO_Pin_4);//P高电平     1
if(b==4)
    GPIO_SetBits(GPIOA,GPIO_Pin_5);//P高电平     1
if(b==5)
    GPIO_SetBits(GPIOA,GPIO_Pin_6);//P高电平     1
}
