#include "ps2.h"

int PS2_LX = 0; /*Left analog X-axis*/
int PS2_LY = 0; /*Left analog Y-axis*/
int PS2_RX = 0; /*Right analog X-axis*/
int PS2_RY = 0; /*Right analog Y-axis*/
/*Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle*/
int buttonset1 = 0;
/*Button Set 2: select, start, L3, R3, L2, L1, R2, R1*/
int buttonset2 = 0;

void ps2Play(void){
	double x = 0, y = 0, angle = 0, a = 0, b = 0;
	uint8_t time = 100, servoDir = 0;
	wf = 0;
	while(!button3Pressed()){
		ps2_read();

		if(ps2_atas()) y = 3;
		else if(ps2_bawah()) y = -3;
		if(ps2_kiri()) x = -3;
		else if(ps2_kanan()) x = 3;

		if(ps2_segitiga()) killFire();
		if(ps2_kotak()) kipasStart();
		else kipasStop();
		if(ps2_silang()) time = 60;
		else time = time;
		if(ps2_lingkaran()) time = 100;
		else time = time;

		if(ps2_r1()) angle = 15;
		else if(ps2_l1()) angle = -15;
		else angle = 0;

		if(ps2_r2()) tpaSetServo(4);
		else if(ps2_l2()) tpaSetServo(27);
		else tpaSetServo(0);


		//Analog kiri
		if(ps2_ly() > 185){
			if(ps2_ly() < 215 && ps2_ly() > 185) y = -1;
			else if (ps2_ly() >= 215 && ps2_ly() <= 245) y = -2;
			else if(ps2_ly() > 245) y = -3;
		}
		else if (ps2_ly() < 70){
			if(ps2_ly() < 70 && ps2_ly() > 40) y = 1;
			else if (ps2_ly() <= 40 && ps2_ly() > 10) y = 2;
			else if(ps2_ly() <= 10) y = 3;
		}

		if(ps2_lx() > 185){
			if(ps2_lx() < 215 && ps2_lx() > 185) x = 1;
			else if (ps2_lx() >= 215 && ps2_lx() <= 245) x = 2;
			else if(ps2_lx() > 245) x = 3;
//			else moveNormal(time);
		}
		else if (ps2_lx() < 70){
			if(ps2_lx() < 70 && ps2_lx() > 40) x = -1;
			else if (ps2_lx() <= 40 && ps2_lx() > 10) x = -2;
			else if(ps2_lx() <= 10) x = -3;
//			else moveNormal(time);
		}
		//Analog kanan
		if(ps2_ry() > 185){
			a -= 3.5;
		}
		else if (ps2_ry() < 70){
			a += 3.5;
		}

		if(ps2_rx() > 185){
			b += 4;
		}
		else if (ps2_rx() < 70){
			b -= 4;
		}
		if(ps2_start()) moveNormal(time);

		if(x > 2 && y > 2){
			x = 2;
			y = 2;
		}
		else if(x < -2 && y < - 2){
			x = -2;
			y = -2;
		}
		if(a > 4) a = 4;
		else if(a < -4) a = -4;

		if(b > 1.5) b = 1.5;
		else if(b < -1.5) b = -1.5;

		if(time < 60) time = 60;

		miring(b, time);
		nungging(a, time);
		moveRobot(x, y, angle, time);
		x=0;
		y=0;
		angle=0;
		a = 0;
		b = 0;
//		delayMs(100);
	}
}
void ps2_init() {
	/* Initialize USART1 at 9600 baud, TX: PB6, RX: PB7 */
	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
}

void ps2_read() {
	TM_USART_ClearBuffer(USART1);
	while (TM_USART_BufferEmpty(USART1)) {}
	while (TM_USART_Getc(USART1) != '<') {}
	while (TM_USART_BufferEmpty(USART1)) {}
	PS2_LX = TM_USART_Getc(USART1);
	while (TM_USART_BufferEmpty(USART1)) {}
	PS2_LY = TM_USART_Getc(USART1);
	while (TM_USART_BufferEmpty(USART1)) {}
	PS2_RX = TM_USART_Getc(USART1);
	while (TM_USART_BufferEmpty(USART1)) {}
	PS2_RY = TM_USART_Getc(USART1);
	while (TM_USART_BufferEmpty(USART1)) {}
	buttonset1 = TM_USART_Getc(USART1);
	while (TM_USART_BufferEmpty(USART1)) {}
	buttonset2 = TM_USART_Getc(USART1);
}

int ps2_lx() {
	return PS2_LX;
}

int ps2_ly() {
	return PS2_LY;
}

int ps2_rx() {
	return PS2_RX;
}

int ps2_ry() {
	return PS2_RY;
}

/*Button Set 1: dpad_left, dpad_down, dpad_right, dpad_up, squareps, cross, circle, triangle*/
bool ps2_segitiga() {
	return (bool)((buttonset1 >> 0) & 0x1);
}

bool ps2_lingkaran() {
	return (bool)((buttonset1 >> 1) & 0x1);
}

bool ps2_silang() {
	return (bool)((buttonset1 >> 2) & 0x1);
}

bool ps2_kotak() {
	return (bool)((buttonset1 >> 3) & 0x1);
}

bool ps2_atas() {
	return (bool)((buttonset1 >> 4) & 0x1);
}

bool ps2_kanan() {
	return (bool)((buttonset1 >> 5) & 0x1);
}

bool ps2_bawah() {
	return (bool)((buttonset1 >> 6) & 0x1);
}

bool ps2_kiri() {
	return (bool)((buttonset1 >> 7) & 0x1);
}

/*Button Set 2: select, start, L3, R3, L2, L1, R2, R1*/
bool ps2_r1() {
	return (bool)((buttonset2 >> 0) & 0x1);
}

bool ps2_r2() {
	return (bool)((buttonset2 >> 1) & 0x1);
}

bool ps2_l1() {
	return (bool)((buttonset2 >> 2) & 0x1);
}

bool ps2_l2() {
	return (bool)((buttonset2 >> 3) & 0x1);
}

bool ps2_r3() {
	return (bool)((buttonset2 >> 4) & 0x1);
}

bool ps2_l3() {
	return (bool)((buttonset2 >> 5) & 0x1);
}

bool ps2_start() {
	return (bool)((buttonset2 >> 6) & 0x1);
}

bool ps2_select() {
	return (bool)((buttonset2 >> 7) & 0x1);
}
