#ifndef _FFT_CALCULATE_H
#define _FFT_CALCULATE_H

#include "stm32f10x.h"
#include "stm32_dsp.h"
#include "bsp_adc.h"

#define NPT 256

extern long InBufArray[NPT];
extern long OutBufArray[NPT/2];
extern long MagBufArray[NPT/2];


void FFT_Compute(void);
void GetPowerMag(void);
#endif
