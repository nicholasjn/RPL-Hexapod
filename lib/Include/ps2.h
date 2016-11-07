#ifndef PS2_H_
#define PS2_H_

#include "tm_stm32f4_usart.h"
#include "jalan_dasar.h"
#include "kipas.h"
#include <stdbool.h>

void ps2_init();
void ps2Play(void);
void ps2_read();
int ps2_lx();
int ps2_ly();
int ps2_rx();
int ps2_ry();
bool ps2_segitiga();
bool ps2_lingkaran();
bool ps2_silang();
bool ps2_kotak();
bool ps2_atas();
bool ps2_kanan();
bool ps2_bawah();
bool ps2_kiri();
bool ps2_r1();
bool ps2_r2();
bool ps2_l1();
bool ps2_l2();
bool ps2_r3();
bool ps2_l3();
bool ps2_start();
bool ps2_select();

#endif
