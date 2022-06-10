#include "fft_calculate.h"
#include "math.h"

long InBufArray[NPT]={0};//fft输入数组
long OutBufArray[NPT/2];//fft输出数组
long MagBufArray[NPT/2];//幅值

float magmax;
void GetPowerMag()
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (OutBufArray[i] << 16) >> 16;
        lY  = (OutBufArray[i] >> 16);
        
        //除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y) / NPT;//取模
        if(i == 0)
            MagBufArray[i] = (unsigned long)(Mag * 32768);
        else
            MagBufArray[i] = (unsigned long)(Mag * 65536);
        if(Mag > magmax)
		{
			magmax = Mag;
		}
    }
}


