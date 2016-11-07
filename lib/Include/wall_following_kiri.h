#ifndef __WALL_FOLLOWING_KIRI_H
#define __WALL_FOLLOWING_KIRI_H

#include "srf_tpa.h"
#include "jalan_dasar.h"
#include "delay.h"
#include "color_sensor.h"
#include <math.h>
#include "lcd.h"
#include "timer5.h"
#include "wall_following_kanan.h"


void tesTimerSonar(void);
void kiri(uint16_t time, colorStatus color);
void kiriMundur(uint16_t time, colorStatus color);
uint8_t kiriProx(uint16_t time, colorStatus color);
void rataKiri(uint16_t time);
void geserKiri(uint16_t time);
void putarKiri90(uint16_t time);

#endif
