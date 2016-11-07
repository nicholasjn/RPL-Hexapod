#include "menu.h"

#define MENU_LIMIT 39

char menu[MENU_LIMIT][16] = {"MAIN", "SONAR", "ADC", "COLOR", "TPA",
							"UVTRON", "PROXIMITY", "WF KIRI", "WF KANAN", "RATA KIRI",
							"RATA KANAN", "GESER KIRI", "GESER KANAN", "JALAN", "HALLto3",
							"ROOM3to2", "ROOM2to1", "ROOM1to4", "APPR FIRE", "SEARCH FIRE",
							"KILL FIRE", "TES KIPAS", "TES SOUNDACT", "FIRE LED", "WHT THRES",
							"BLCK THRES", "CEK THRES", "PTRKN90", "PTRKR90", "PTR BALIK",
							"CHECK ROOMIN", "CHECK ROOMOUT", "ROOM START", "KANAN MUNDUR", "KIRI MUNDUR",
							"APPR KILL", "TEZT", "ROBOT NAV", "STICK MODE"};

void menuLCD(void){
	uint8_t i = 0, j = 1, option = 0, iPrev = 1;
	uint8_t ok;
	char lcdNow[20], lcdNext[20];
	tpaSetServo(26);
	uint16_t time = 60;

	while(1) {
		ok = 0;
		if(startButtonPressed() || soundDetected1(10)) {
			if(startButtonPressed()) {
//				tes(time);
				kiri(60, BLACK);
				while(1);
			}
			else if(soundDetected1(10)) {
				if(soundDetected1(10)) {
					navigasiAllArbitrary(time);
					while(1);
				}
			}
		}
		else if(button1Pressed()){
			delayMs(50);
			if(i > 0) {
				i--;
				j = i + 1;
			}
			else {
				i = MENU_LIMIT - 1;
				j = 0;
			}
		}
		if(button2Pressed()){
			delayMs(50);
			if(i < MENU_LIMIT - 2) {
				i++;
				j = i + 1;
			}
			else if(i == MENU_LIMIT - 1) {
				i = 0;
				j = 1;
			}
			else {
				i = MENU_LIMIT - 1;
				j = 0;
			}
		}
		else if(button4Pressed()){
			ok = 1;
		}
		sprintf(lcdNow,">%2d %s", i, menu[i]);
		sprintf(lcdNext,"%2d %s", j, menu[j]);
		if(iPrev != i) {
			lcd_clear();
			lcd_putsf(0,0, lcdNow);
			lcd_putsf(0,1, lcdNext);
			iPrev = i;
		}
		option = i;
		if(ok) {
			delayMs(200);
			iPrev = i - 1;
			switch(option){
				case 0:
					navigasiAll(time);
					while(1);
					break;
				case 1:
					tesSonar();
					break;
				case 2:
					tesAdc();
					break;
				case 3:
					tesColorSensor();
					break;
				case 4:
					while(!button3Pressed()) {
//						nungging(2, 100);
						tpaValue tpaValues = tpaScanGetPosition();
//						normal(100);
						char temp[16];
						sprintf(temp, "Val=%3d Pos=%2d", tpaValues.tpaTemp, tpaValues.tpaPos);
						lcd_clear();
						lcd_putsf(0, 0, temp);
						while(!button4Pressed()) {
							if(button3Pressed()) break;
						}
					}
					while(button3Pressed());
					tpaSetServo(26);
					break;
				case 5:
					lcd_clear();
					while(!button3Pressed()) {
						char temp[16];
						uint8_t photon = uvtronCheck(1000);
						sprintf(temp, "PHOTON = %3d", photon);
						lcd_putsf(0, 1, temp);
					}
					while(button3Pressed());
					break;
				case 6:
					while(!button3Pressed()) {
						char temp[16];
						uint8_t proximity = proximityCheck();
						uint8_t doll = dollCheck();
						sprintf(temp, "PROX=%1d  DOLL=%1d", proximity, doll);
						lcd_putsf(0, 0, temp);
						delayMs(25);
					}
					while(button3Pressed());
					break;
				case 7:
					kiri(time, LINE);
					break;
				case 8:
					kanan(time, LINE);
					break;
				case 9:
					rataKiri(time);
					break;
				case 10:
					rataKanan(time);
					break;
				case 11:
					geserKiri(time);
					break;
				case 12:
					geserKanan(time);
					break;
				case 13:
					while(!button3Pressed()) {
						tpaSetServo(26);
						moveRobot(0, 1, 0, 150);
					}
					while(button3Pressed());
					break;
				case 14:
					robotNavigation(HALL, ROOM3, time);
					break;
				case 15:
					robotNavigation(ROOM3, ROOM2, time);
					break;
				case 16:
					robotNavigation(ROOM2, ROOM1, time);
					break;
				case 17:
					robotNavigation(ROOM1, ROOM4, time);
					break;
				case 18:
					approachFire(50);
					break;
				case 19:
					searchFire();
					break;
				case 20:
					killFire();
					break;
				case 21:
					kipasStart();
					while(button4Pressed());
					kipasStop();
					break;
				case 22:
					lcd_clear();
					while(!button3Pressed()) {
						tesSoundAct(10);
					}
					while(button3Pressed());
					lcd_clear();
					break;
				case 23:
					fireLedStartBlink();
					lcd_clear();
					lcd_putsf(0, 0, "BLINK");
					while(!button3Pressed());
					fireLedStopBlink();
					break;
				case 24:
					getWhiteThreshold();
					break;
				case 25:
					getBlackThreshold();
					break;
				case 26:
					checkThreshold();
					break;
				case 27:
					putarKanan90(time);
					break;
				case 28:
					putarKiri90(time);
					break;
				case 29:
					putarBalik(time);
					break;
				case 30:
					while(!button3Pressed()) {
						checkRoomIn();
						while(!button4Pressed() && !button3Pressed());
					}
					while(button3Pressed());
					break;
				case 31:
					while(!button3Pressed()) {
						checkRoomOut();
						while(!button4Pressed() && !button3Pressed());
					}
					while(button3Pressed());
					break;
				case 32:
					roomStart(time);
					break;
				case 33:
					kananMundur(time, LINE);
					break;
				case 34:
					kiriMundur(time, LINE);
					break;
				case 35:
					approachKillFire(50);
					break;
				case 36:
					tes(time);
					break;
				case 37:
					robotNav(time);
					break;
				case 38:
					ps2Play();
					break;

				default : break;
			}
			iPrev = 0;
		}
	}
}


void testing() {
	while(!button4Pressed()){
		moveRobot(0, 4, 0, time);
	}
}

void robotNav(uint16_t time){
	char menu1[15][16] = {"ROOM START", "ROOM4START", "ROOM1to2", "ROOM1to3", "ROOM1to4 UNC",
						  "ROOM1to4 C", "ROOM2to1", "ROOM2to3", "ROOM2to4", "ROOM3to1",
						  "ROOM3to2", "ROOM13o4", "ROOM4to1", "ROOM4to2", "ROOM4to3"};
	char lcd_now[20], lcd_next[20];
	int a = 0, b = 1 , pilih = 0;
	int aPrev;
	while(!button3Pressed()){
		pilih = 0;
		if(button1Pressed()){
			delayMs(50);
			if(a > 0) {
				a--;
				b = a + 1;
			}
			else {
				a = 15 - 1;
				b = 0;
			}
		}
		else if(button2Pressed()){
			delayMs(50);
			if(a < 15 - 2) {
				a++;
				b = a + 1;
			}
			else if(a == 15 - 1) {
				a = 0;
				b = 1;
			}
			else {
				a = 0;
				b = 1;
			}
		}
		else if(button4Pressed()){
			pilih = 1;
		}

		sprintf(lcd_now,">%2d %s", a, menu1[a]);
		sprintf(lcd_next,"%2d %s", b, menu1[b]);
		if(aPrev != a) {
			lcd_clear();
			lcd_putsf(0,0, lcd_now);
			lcd_putsf(0,1, lcd_next);
			aPrev = a;
		}
		delayMs(50);
		if(pilih){
			delayMs(200);
			switch (a){
				case 0 : roomStart(time); break;
				case 1 : room4Start(time); break;
				case 2 : room1toRoom2(time); break;
				case 3 : room1toRoom3(time); break;
				case 4 : room1toRoom4Unchecked(time); break;
				case 5 : room1toRoom4Checked(time); break;
				case 6 : room2toRoom1(time); break;
				case 7 : room2toRoom3(time); break;
				case 8 : room2toRoom4(time); break;
				case 9 : room3toRoom1(time); break;
				case 10 : room3toRoom2(time); break;
				case 11 : room3toRoom4(time); break;
				case 12 : room4toRoom1(time); break;
				case 13 : room4toRoom2(time); break;
				case 14 : room4toRoom3(time); break;
			}
			aPrev = 0;
		}
	}
}
