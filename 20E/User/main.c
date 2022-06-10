#include "stm32f10x.h"   // 相当于51单片机中的  #include <reg51.h>
#include "delay.h"
#include "sys.h"
#include "bsp_AdvanceTim.h"
#include "bsp_adc.h"
#include "fft_calculate.h"
#include "bsp_usart.h"
#include "stm32f10x_gpio.h"
#include "gpio.h"
/*20-20k*/
/*(1822-1758)/2=1800*/
#define N 256
// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[ADC_LEN];
// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal;  
uint8_t string[20];
float   Mag[N/2];
extern float magmax;
unsigned char  buf[64];
u32 Freq;
u16 Row;
u16 Max_Val=0;
float a=1;
int b;
float TTT[5]={0};		//存放THD的数组
int th=0;

 void HMISends(char *buf1)		  //???????
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART1,buf1[i]);  //??????
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//??????
		 	i++;
		}
	 else 
	 return ;

		}
	}


void GetTHD()
{
	float  Uo2,Uo3,Uo4,Uo5;
	double thd_fz=0,thd_fm=0;
    float THD; 
 
	//Uo1=MagBufArray[10];
	Uo2=MagBufArray[20];
	Uo3=MagBufArray[30];
	Uo4=MagBufArray[40];
	Uo5=MagBufArray[10];

	thd_fm=magmax;
	thd_fz=Uo2*Uo2 + Uo3*Uo3 + Uo4*Uo4 + Uo5*Uo5;
	thd_fz=sqrt(thd_fz);
	THD=thd_fz/thd_fm;
    TTT[th]=THD;
//    delay_ms(100);
    th++;
	//求THD的平均值
	if(th==5)
	{
        float THDEND;
		THDEND=(TTT[0]+TTT[1]+TTT[2]+TTT[3]+TTT[4])/5;
		THDEND=THDEND/1000;
    delay_ms(100);
    sprintf((char *)buf,"t0.txt=\"%2.1f\"",THDEND); 
    HMISends((char *)buf);
//    printf("x0.val=%d",THD);
    printf("\xff");printf("\xff");printf("\xff");
        th=0;
    }

}

int main(void)
{
//	u16 i;
	USART_Config();
	// ADC 初始化
	ADCx_Init();
	ADVANCE_TIM_Init();
	TIM_Cmd(ADVANCE_TIM, ENABLE);
	  lunzi_Init();
		while (1)
	{		
//			for(i=2;i<NPT/2;i++)
//			{
              //  Freq=(72000000)/(NPT*(ADVANCE_TIM_PERIOD+1)*(ADVANCE_TIM_PSC+1)/i);//频率计算公式
             //      GetPowerMag();
              //  	GetTHD();

				//printf("i:%d \n",i);
				//printf("Freq:%d   Max_Val:%ld\n",Freq,MagBufArray[i]);//输出频率及幅值
                
                
//			}

	}
}

/**************************************************
DMA中断  主要完成了数据转移并进行FFT快速傅里叶变换操作
***************************************************/

void DMA1_Channel1_IRQHandler(void)
{
  u16 i = 0;
  DMA_Cmd(DMA1_Channel1, DISABLE);

	for(i=0;i<NPT;i++)
		InBufArray[i] = ((signed short)(ADC_ConvertedValue[i])) << 16;		//获取adc的值输入
																																	
	//cr4_fft_1024_stm32(OutBufArray, InBufArray, NPT);
	cr4_fft_256_stm32(OutBufArray, InBufArray, NPT);//执行fft转换
	GetPowerMag();	//将运算结果求模得到幅值
	GetTHD();//将各次谐波投入计算出thd值并打印
	
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
  DMA_ClearITPendingBit(DMA_IT_HT);
  DMA_ClearITPendingBit(DMA1_IT_TC1);
}

// 串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
	ucTemp = USART_ReceiveData(DEBUG_USARTx);
        if(ucTemp==0x01)//交越失真
        {
            GPIO_ResetBits(GPIOA,GPIO_Pin_7);
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);
            GPIO_ResetBits(GPIOA,GPIO_Pin_4);
            GPIO_ResetBits(GPIOA,GPIO_Pin_5);
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);
            GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
                              
            GPIO_SetBits(GPIOA,GPIO_Pin_2);
            
            GPIO_SetBits(GPIOA,GPIO_Pin_5);

            GPIO_SetBits(GPIOA,GPIO_Pin_1); 
        }   
        else if(ucTemp==0x02)//双向失真
        {  
            GPIO_ResetBits(GPIOA,GPIO_Pin_7);
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);
            GPIO_ResetBits(GPIOA,GPIO_Pin_4);
            GPIO_ResetBits(GPIOA,GPIO_Pin_5);
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);
            GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
            
            GPIO_SetBits(GPIOA,GPIO_Pin_4);
            GPIO_SetBits(GPIOA,GPIO_Pin_7);
        }
        else if(ucTemp==0x03)//顶部失真
         { 
            GPIO_ResetBits(GPIOA,GPIO_Pin_7);
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);
            GPIO_ResetBits(GPIOA,GPIO_Pin_4);
            GPIO_ResetBits(GPIOA,GPIO_Pin_5);
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);
            GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
                                             
            GPIO_SetBits(GPIOA,GPIO_Pin_5);
            GPIO_SetBits(GPIOA,GPIO_Pin_7);
        }
        else if(ucTemp==0x04)//正常波形
                    {  
            printf("n0.val=88"); printf("\xff");printf("\xff");printf("\xff");
                        
            GPIO_ResetBits(GPIOA,GPIO_Pin_5);           
            GPIO_ResetBits(GPIOA,GPIO_Pin_1);
            GPIO_ResetBits(GPIOA,GPIO_Pin_7);
            GPIO_ResetBits(GPIOA,GPIO_Pin_4);
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);
                        
            GPIO_SetBits(GPIOA,GPIO_Pin_5);
        }
        else if(ucTemp==0x05)//底部失真
         {  
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);              
            GPIO_ResetBits(GPIOA,GPIO_Pin_1);
            GPIO_ResetBits(GPIOA,GPIO_Pin_7);
            GPIO_ResetBits(GPIOA,GPIO_Pin_4);
            GPIO_ResetBits(GPIOA,GPIO_Pin_5);
            GPIO_ResetBits(GPIOA,GPIO_Pin_2);
                        
            GPIO_SetBits(GPIOA,GPIO_Pin_6);
        }
    USART_SendData(DEBUG_USARTx,ucTemp);    
	}	 
}

