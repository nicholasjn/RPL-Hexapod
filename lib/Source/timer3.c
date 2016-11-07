//--------------------------------------------------------------
// File     : timer3.c
// Datum    : 06.02.2014
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : TIM, MISC
// Funktion : Timer-Funktionen per Timer5
//            (mit Callback-Funktion fÃ¼r externe ISR)
//
// Hinweis  : beim Timerevent wird die Funktion :
//            "timer7ISR_CallBack()" aufgerufen
//            diese Funktion muss vom User programmiert werden
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "timer3.h"
#include "srf_tpa.h"

sonarSector side;

//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_TIM3_TIMER(uint16_t prescaler, uint16_t periode);
void P_TIM3_NVIC(void);
//--------------------------------------------------------------
uint32_t tim3_enable_flag=0;



//--------------------------------------------------------------
// Init und Stop vom Timer mit Vorteiler und Counterwert
// prescaler : [0...65535]
// periode   : [0...65535]
//
// F_TIM = 84 MHz / (prescaler+1) / (periode+1)
//--------------------------------------------------------------
void timer3Init(uint16_t prescaler, uint16_t periode)
{
  // Timer flag lÙ�schen
  tim3_enable_flag=0;
  // Timer einstellen
  P_TIM3_TIMER(prescaler, periode);
  P_TIM3_NVIC();
}


//--------------------------------------------------------------
// Init und Stop vom Timer mit einem FRQ-Wert (in Hz)
// frq_hz : [1...42000000]
//
// Hinweis : die tatsÙ†chliche Frq weicht wegen Rundungsfehlern
//           etwas vom Sollwert ab (Bitte nachrechnen falls Wichtig)
//--------------------------------------------------------------
void timer3InitFreq(float frq_hz)
{
  RCC_ClocksTypeDef RCC_Clocks;
  uint32_t clk_frq;
  uint16_t prescaler, periode;
  uint32_t u_temp;
  float teiler,f_temp;

  // Clock-Frequenzen (PCLK1) auslesen
  RCC_GetClocksFreq(&RCC_Clocks);
  clk_frq = RCC_Clocks.PCLK1_Frequency;

  // check der werte
  if(frq_hz==0) frq_hz=1;
  if(frq_hz>clk_frq) frq_hz=clk_frq;

  // berechne teiler
  teiler=(float)(clk_frq<<1)/(frq_hz);

  // berechne prescaler
  u_temp=(uint32_t)(teiler);
  prescaler=(u_temp>>16);

  // berechne periode
  f_temp=(float)(teiler)/(float)(prescaler+1);
  periode=(uint16_t)(f_temp-1);

  // werte einstellen
  timer3Init(prescaler, periode);
}


//--------------------------------------------------------------
// Timer starten
//--------------------------------------------------------------
void timer3Start(sonarSector sector)
{
	side = sector;
  // Timer enable
  TIM_Cmd(TIM3, ENABLE);
  // Timer flag setzen
  tim3_enable_flag=1;
}


//--------------------------------------------------------------
// Timer anhalten
//--------------------------------------------------------------
void timer3Stop(void)
{
  // Timer flag lÙ�schen
  tim3_enable_flag=0;
  // Timer disable
  TIM_Cmd(TIM3, DISABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Timer
//--------------------------------------------------------------
void P_TIM3_TIMER(uint16_t prescaler, uint16_t periode)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  // Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // Timer disable
  TIM_Cmd(TIM3, DISABLE);

  // Timer init
  TIM_TimeBaseStructure.TIM_Period =  periode;
  TIM_TimeBaseStructure.TIM_Prescaler = prescaler;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  // Timer preload enable
  TIM_ARRPreloadConfig(TIM3, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom Interrupt
//--------------------------------------------------------------
void P_TIM3_NVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // Update Interrupt enable
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}



//--------------------------------------------------------------
// Timer-Interrupt
// wird beim erreichen vom Counterwert aufgerufen
// die Callback funktion muss extern benutzt werden
//--------------------------------------------------------------
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    // wenn Interrupt aufgetreten
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    // Callback Funktion aufrufen
    // Diese Funktion muss extern vom User benutzt werden !!
    if(tim3_enable_flag!=0) {
      // nur wenn Timer aktiviert ist
      timer3ISRCallBack();
    }
  }
}



