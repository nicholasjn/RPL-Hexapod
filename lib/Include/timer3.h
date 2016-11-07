//--------------------------------------------------------------
// File     : timer3.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_TIM3_H
#define __STM32F4_TIM3_H

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "srf_tpa.h"

sonarSector side;

//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void timer3Init(uint16_t prescaler, uint16_t periode);
void timer3InitFreq(float frq_hz);
void timer3Start(sonarSector sector);
void timer3Stop(void);
void timer3ISRCallBack(void);  // Diese Funktion muss extern benutzt werden !!


//--------------------------------------------------------------
#endif // __STM32F4_TIM3_H
