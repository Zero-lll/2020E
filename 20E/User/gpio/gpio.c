#include "gpio.h"
#include "stm32f10x.h"

extern int b;

void lunzi_Init(void)//����Ӧ��IO�ڽ��г�ʼ��,PA0-3
{

	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��GPIOA��ʱ��
  
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;//���ģʽΪ�����������ƽ�ɸ߿ɵ�
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_2|GPIO_Pin_7;//P�ܽ�IO��
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽŴ����ٶ�
	  GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
}

void forward(void)
{
    if(b==1)
    GPIO_SetBits(GPIOA,GPIO_Pin_1);//P�ߵ�ƽ     1
if(b==2)
    GPIO_SetBits(GPIOA,GPIO_Pin_2);//P�ߵ�ƽ     1
if(b==3)
    GPIO_SetBits(GPIOA,GPIO_Pin_4);//P�ߵ�ƽ     1
if(b==4)
    GPIO_SetBits(GPIOA,GPIO_Pin_5);//P�ߵ�ƽ     1
if(b==5)
    GPIO_SetBits(GPIOA,GPIO_Pin_6);//P�ߵ�ƽ     1
}
