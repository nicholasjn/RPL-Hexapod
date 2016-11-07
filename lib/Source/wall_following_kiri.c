#include "wall_following_kiri.h"

#define PI 3.1415926535897

#define MIN_DIST 12
#define MAX_DIST 30
#define MIN_DIST_FR 17
#define MIN_DIST_DG 15
#define MAX_DIST_DG 21
#define MID_DIST 15

#define Y_MAX (double)5.3
#define X_MAX (double)2.2

#define ANGLE_LIMIT_DOUBLE (double)60
#define ANGLE_LIMIT (double)30
#define SRF_1_TO_2_DIST (double)9


uint8_t i;

void tesTimerSonar(void){
	char srf[4][4];
	timer3Start(links);
	while(1){
		sprintf(srf[0], "%3i", srfFront);
		sprintf(srf[1], "%3i", srfLDg);
		sprintf(srf[2], "%3i", srfL1);
		sprintf(srf[3], "%3i", srfL2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);
		lcd_putsf(12,1,srf[3]);
	}
	timer3Stop();
}

void kiri(uint16_t time, colorStatus color) {
	uint8_t colorCorrect;
	char srf[4][4];
	colorMatch = 0;
	timer5Start(color);
	timer3Start(links);
	wf = 1;
	tpaSetServo(27);

	lcd_clear();
	if(color == BLACK)lcd_putsf(0,0,"WFKIRI BLACK");
	else if(color == GRAY) lcd_putsf(0,0,"WFKIRI GRAY");
	else if(color == WHITE) lcd_putsf(0,0,"WFKIRI HOME");
	else if(color == LINE) lcd_putsf(0,0,"WFKIRI LINE");
	else lcd_putsf(0,0,"WFKIRI NOTLINE");

	double x, y, angle;
	while(1) {
		x = 0, y = 0, angle = 0;

//		if(colorMatch) {
//			if(color == NOTLINE) {
//				if(checkColorWF(color, -3, time)) {
//					colorCorrect++;
//				}
//				if(colorCorrect > 3) {
//					timer3Stop();
//					return;
//				}
//				else {
//					colorMatch = 0;
//					timer5Start(color);
//					wf = 1;
//				}
//			}
//			else {
//				if(checkColorWF(color, -3, time)) {
//					timer3Stop();
//					return;
//				}
//			}
//		}

//		for(i = 0; i < 5; i++) {
//			sonarRange(pingKiri[i]);
//			delayMs(10);
//		}
//		srfL2 = sonarGet(pingKiri[0]);
//		srfLDg = sonarGet(pingKiri[1]);
//		srfFront = sonarGet(pingKiri[2]);
//		srfL1 = sonarGet(pingKiri[3]);

		sprintf(srf[0], "%3i", srfFront);
		sprintf(srf[1], "%3i", srfLDg);
		sprintf(srf[2], "%3i", srfL1);
		sprintf(srf[3], "%3i", srfL2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);
		lcd_putsf(12,1,srf[3]);

//		srfRDg = sonarGet(pingKiri[4]);

		if(srfFront <= MIN_DIST_FR) {
			if(srfRDg <= MIN_DIST_DG && srfLDg > MIN_DIST_DG) {
				angle = -15;
			}
			else {
				angle = ANGLE_LIMIT;
			}
//			putarKanan90(time);
		}
		else if(srfRDg <= MIN_DIST_DG) {
			y = 1;
			x = -1;
			angle = -15;
		}
		else if(srfLDg <= MIN_DIST_DG) {
			y = 3;
			x = 1;
			angle = 15;
		}
		else if(srfL1 > MAX_DIST) {
			y = 3;
			x = -1;
			angle = -17;
		}
		else if(srfL2 > MAX_DIST) {
			y = 6;
		}
		else {
			if (srfL1 < srfL2) {
				x = MIN_DIST - srfL1;
			}
			else {
				x = MIN_DIST - srfL2;
			}
			y = 6;
			angle = (atan((srfL2 - srfL1) / SRF_1_TO_2_DIST) * 180 / PI);
			if(angle > 10) {
				angle = 10;
			}
			else if(angle < -10) {
				angle = -10;
			}
		}

		if (x > X_MAX) {
			x = X_MAX;
		}
		else if (x < -X_MAX) {
			x = -X_MAX;
		}
		if (y > Y_MAX) {
			y = Y_MAX;
		}
		else if (y < -Y_MAX) {
			y = -Y_MAX;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}

		moveRobot(x, y, angle, time);
	}
}

void rataKiri(uint16_t time) {
	int vs = 0, dist = 0, a;
	wf = 1;
	tpaSetServo(27);
	lcd_clear();
	lcd_putsf(0,0,"RATA KIRI");
	double x, y, angle;
	while(1) {
		x = 0, y = 0, angle = 0;

		delayMs(30);
		sonarRange(pingKiri[0]);
		delayMs(10);
		sonarRange(pingKiri[3]);
		delayMs(10);
		srfL2 = sonarGet(pingKiri[0]);
		srfL1 = sonarGet(pingKiri[3]);

		vs = srfL1 - srfL2;

		if(vs < -1 || vs > 1) {
			angle = -(atan((srfL1 - srfL2) / SRF_1_TO_2_DIST) * 180 / PI);
		}
		else {
			wf = 0;
			moveNormal2(time);
			break;
		}

		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}
		moveRobot(0, 0, angle, time);
	}
}

void geserKiri(uint16_t time){
	const uint8_t ping[2] = {0xEA, 0xEC};
	char temp[16];
	int vs = 0, dist;
	double x = 0, y = 0, angle = 0;
	tpaSetServo(27);
	wf = 1;
	lcd_clear();
	lcd_putsf(0,0, "GESER KIRI");
	while(1){
		y = 0;

		delayMs(30);
		for(i = 0; i < 2; i++) {
			sonarRange(ping[i]);
			delayMs(10);
		}
		srfL2 = sonarGet(ping[0]);
		srfL1 = sonarGet(ping[1]);
		sprintf(temp, "L1=%3d L2=%3d", srfL1, srfL2);
		lcd_putsf(0, 1, temp);

		vs = srfL1 - srfL2;
		dist = srfL1 - (MID_DIST);
		if(dist > 2 || dist < -2){
			if(vs > 1 || vs < -1 ){
				angle = -(atan((srfL1 - srfL2) / SRF_1_TO_2_DIST) * 180 / PI);
				if(angle > 10) {
					angle = 10;
				}
				else if(angle < -10) {
					angle = -10;
				}
			}
			if(srfL1 > MID_DIST){
				x = -4;
			}
			else{
				x = -(srfL1 - MID_DIST);
			}

		}
		else{
			wf = 0;
			if(vs > 1 || vs < -1 ) {
				rataKiri(time);
			}
			else {
				moveNormal2(time);
			}
			break;
		}
		moveRobot(x, y, angle, time);
	}
}

void putarKiri90(uint16_t time){
	uint8_t temp = wf;
	wf = 0;
	lcd_clear();
	uint8_t a;
	lcd_putsf(0,0,"PUTAR KIRI 90");
	normal(time);
	//Untuk sepatu tpe pake -33
	for(a = 0; a < 2; a++) {
		moveRobot(0, 0, -31, time);
		moveRobot(0, 0, -31, time);
	}
	moveNormal(time);
	wf = temp;
}

uint8_t kiriProx(uint16_t time, colorStatus color) {
	char srf[16][4];
	colorMatch = 0;
	timer5Start(color);
	wf = 1;
	tpaSetServo(27);
	lcd_clear();
	if(color == BLACK)lcd_putsf(0,0,"WFKIRI BLACK");
	else if(color == GRAY) lcd_putsf(0,0,"WFKIRI GRAY");
	else if(color == WHITE) lcd_putsf(0,0,"WFKIRI HOME");
	else lcd_putsf(0,0,"WFKIRI LINE");

	double x, y, angle;
	while(1) {
		x = 0, y = 0, angle = 0;

		if(colorMatch) {
			if(checkColorWF(color, -3, time)) {
				return;
			}
		}

		for(i = 0; i < 5; i++) {
			sonarRange(pingKiri[i]);
			delayMs(10);
		}
		srfL2 = sonarGet(pingKiri[0]);
		srfLDg = sonarGet(pingKiri[1]);
		srfFront = sonarGet(pingKiri[2]);
		srfL1 = sonarGet(pingKiri[3]);

		sprintf(srf[0], "%3i", srfFront);
		sprintf(srf[1], "%3i", srfLDg);
		sprintf(srf[2], "%3i", srfL1);
		sprintf(srf[3], "%3i", srfL2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);
		lcd_putsf(12,1,srf[3]);

		srfRDg = sonarGet(pingKiri[4]);

		if(dollCheck() == 1 && srfL1 <= 36) {
			wf = 0;
			moveRobot(0, -3, 0, time);
			rataKiri(time);
			if(dollCheck() == 1) {
				colorStatus colorCheck;
				uint8_t blackCorrect = 0, grayCorrect = 0;
				for(i = 0; i < 10; i++) {
					colorCheck = checkColorSensor();
					if(colorCheck == BLACK){
						blackCorrect++;
					}
					else if(colorCheck == GRAY) {
						grayCorrect++;
					}
				}
				sonarRange(pingKanan[3]);
				srfR1 = sonarGet(pingKanan[3]);
				if(srfR1 <= 40) {
					moveRobot(0, -5, 0, time);
					moveRobot(0, -5, 0, time);
					moveRobot(0, -5, 0, time);
				}
				if(blackCorrect > grayCorrect) {
					return 1;
				}
				else {
					return 2;
				}
			}
			wf = 1;
		}

		if(srfFront <= MIN_DIST_FR) {
			if(srfRDg <= MIN_DIST_DG && srfLDg > MIN_DIST_DG) {
				angle = -15;
			}
			else {
				angle = ANGLE_LIMIT;
			}
//			putarKanan90(time);
		}
		else if(srfRDg <= MIN_DIST_DG) {
			y = 1;
			x = -1;
			angle = -15;
		}
		else if(srfLDg <= MIN_DIST_DG) {
			y = 3;
			x = 1;
			angle = 15;
		}
		else if(srfL1 > MAX_DIST) {
			y = 3;
			x = -1;
			angle = -17;
		}
		else if(srfL2 > MAX_DIST) {
			y = 6;
		}
		else {
			if (srfL1 < srfL2) {
				x = MIN_DIST - srfL1;
			}
			else {
				x = MIN_DIST - srfL2;
			}
			y = 6;
			angle = (atan((srfL2 - srfL1) / SRF_1_TO_2_DIST) * 180 / PI);
			if(angle > 10) {
				angle = 10;
			}
			else if(angle < -10) {
				angle = -10;
			}
		}

		if (x > X_MAX) {
			x = X_MAX;
		}
		else if (x < -X_MAX) {
			x = -X_MAX;
		}
		if (y > Y_MAX) {
			y = Y_MAX;
		}
		else if (y < -Y_MAX) {
			y = -Y_MAX;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}

		moveRobot(x, y, angle, time);
	}
}

void kiriMundur(uint16_t time, colorStatus color) {
	char srf[16][4];
	colorMatch = 0;
	timer5Start(color);
	wf = 1;
	tpaSetServo(27);

	lcd_clear();
	if(color == BLACK)lcd_putsf(0,0,"WFKIRI BLACK");
	else if(color == GRAY) lcd_putsf(0,0,"WFKIRI GRAY");
	else if(color == WHITE) lcd_putsf(0,0,"WFKIRI WHITE");
	else if(color == LINE) lcd_putsf(0,0,"WFKIRI LINE");
	else lcd_putsf(0,0,"WFKIRI NOTLINE");

	while(1) {
		double x = 0, y = 0, angle = 0;

		if(colorMatch) {
			if(checkColorWF(color, 3, time)) {
				return;
			}
		}

		for(i = 0; i < 5; i++) {
			sonarRange(pingKiri[i]);
			delayMs(10);
		}
		srfL2 = sonarGet(pingKiri[0]);
		srfLDg = sonarGet(pingKiri[1]);
		srfFront = sonarGet(pingKiri[2]);
		srfL1 = sonarGet(pingKiri[3]);

		sprintf(srf[0], "%3i", srfFront);
		sprintf(srf[1], "%3i", srfLDg);
		sprintf(srf[2], "%3i", srfL1);
		sprintf(srf[3], "%3i", srfL2);

		lcd_putsf(0,1,srf[0]);
		lcd_putsf(4,1,srf[1]);
		lcd_putsf(8,1,srf[2]);
		lcd_putsf(12,1,srf[3]);

		srfRDg = sonarGet(pingKiri[4]);

		if (srfL1 < srfL2) {
			x = MIN_DIST - srfL1;
		}
		else {
			x = MIN_DIST - srfL2;
		}
		y = -6;
		angle = (atan((srfL2 - srfL1) / SRF_1_TO_2_DIST) * 180 / PI);
		if(angle > 10) {
			angle = 10;
		}
		else if(angle < -10) {
			angle = -10;
		}

		if (x > X_MAX) {
			x = X_MAX;
		}
		else if (x < -X_MAX) {
			x = -X_MAX;
		}
		if (y > Y_MAX) {
			y = Y_MAX;
		}
		else if (y < -Y_MAX) {
			y = -Y_MAX;
		}
		if(angle > ANGLE_LIMIT) {
			angle = ANGLE_LIMIT;
		}
		else if (angle < -ANGLE_LIMIT) {
			angle = -ANGLE_LIMIT;
		}

		moveRobot(x, y, angle, time);
	}
}
