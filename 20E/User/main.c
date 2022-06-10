#include "stm32f10x.h"   // �൱��51��Ƭ���е�  #include <reg51.h>
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
// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[ADC_LEN];
// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
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
float TTT[5]={0};		//���THD������
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
	//��THD��ƽ��ֵ
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
	// ADC ��ʼ��
	ADCx_Init();
	ADVANCE_TIM_Init();
	TIM_Cmd(ADVANCE_TIM, ENABLE);
	  lunzi_Init();
		while (1)
	{		
//			for(i=2;i<NPT/2;i++)
//			{
              //  Freq=(72000000)/(NPT*(ADVANCE_TIM_PERIOD+1)*(ADVANCE_TIM_PSC+1)/i);//Ƶ�ʼ��㹫ʽ
             //      GetPowerMag();
              //  	GetTHD();

				//printf("i:%d \n",i);
				//printf("Freq:%d   Max_Val:%ld\n",Freq,MagBufArray[i]);//���Ƶ�ʼ���ֵ
                
                
//			}

	}
}

/**************************************************
DMA�ж�  ��Ҫ���������ת�Ʋ�����FFT���ٸ���Ҷ�任����
***************************************************/

void DMA1_Channel1_IRQHandler(void)
{
  u16 i = 0;
  DMA_Cmd(DMA1_Channel1, DISABLE);

	for(i=0;i<NPT;i++)
		InBufArray[i] = ((signed short)(ADC_ConvertedValue[i])) << 16;		//��ȡadc��ֵ����
																																	
	//cr4_fft_1024_stm32(OutBufArray, InBufArray, NPT);
	cr4_fft_256_stm32(OutBufArray, InBufArray, NPT);//ִ��fftת��
	GetPowerMag();	//����������ģ�õ���ֵ
	GetTHD();//������г��Ͷ������thdֵ����ӡ
	
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
  DMA_ClearITPendingBit(DMA_IT_HT);
  DMA_ClearITPendingBit(DMA1_IT_TC1);
}

// �����жϷ�����
void DEBUG_USART_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
	ucTemp = USART_ReceiveData(DEBUG_USARTx);
        if(ucTemp==0x01)//��Խʧ��
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
        else if(ucTemp==0x02)//˫��ʧ��
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
        else if(ucTemp==0x03)//����ʧ��
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
        else if(ucTemp==0x04)//��������
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
        else if(ucTemp==0x05)//�ײ�ʧ��
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

