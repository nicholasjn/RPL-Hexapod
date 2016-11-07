//--------------------------------------------------------------
// File     : eeprom_flash.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __EEPROMFLASH_H
#define __EEPROMFLASH_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"


//--------------------------------------------------------------
// Grosse vom virtuellen EEprom
// = Anzahl der 16bit Werte die gespeichert werden können
//--------------------------------------------------------------
#define  EE_FLASH_MAX_ADR   40



//--------------------------------------------------------------
// Spannung der CPU muss zwischen 2.7V und 3.6V liegen
//--------------------------------------------------------------
#define VOLTAGE_RANGE           (uint8_t)VoltageRange_3


//--------------------------------------------------------------
// Adressen der beiden Pages
// benutzt werden zwei Sektoren im Flash
// die beiden Sektoren muessen gleich gross sein
// -> Adressen und Groesse siehe (Referenz-Manual Seite 61)
//--------------------------------------------------------------
#define FLASH_SEKTOR_SIZE    ((uint32_t)0x20000)     // 128kByte
#define FLASH_SEKTOR10_START  ((uint32_t)0x080C0000) // Sektor_10
#define FLASH_SEKTOR11_START  ((uint32_t)0x080E0000) // Sektor_11

//----------------
// Page-0
//----------------
#define PAGE0_BASE_ADDRESS    ((uint32_t)(FLASH_SEKTOR10_START))
#define PAGE0_END_ADDRESS     ((uint32_t)(PAGE0_BASE_ADDRESS + (FLASH_SEKTOR_SIZE - 1)))
#define PAGE0_ID               FLASH_Sector_10

//----------------
// Page-1
//----------------
#define PAGE1_BASE_ADDRESS    ((uint32_t)(FLASH_SEKTOR11_START))
#define PAGE1_END_ADDRESS     ((uint32_t)(PAGE1_BASE_ADDRESS + (FLASH_SEKTOR_SIZE - 1)))
#define PAGE1_ID               FLASH_Sector_11



//--------------------------------------------------------------
// defines
//--------------------------------------------------------------
#define EE_PAGE0                 ((uint16_t)0x0000)
#define EE_PAGE1                 ((uint16_t)0x0001)

#define EE_ERASED        0xFFFF
#define EE_RECEIVE_DATA  0xEEEE
#define EE_VALID_PAGE    0x0000

#define EE_NO_ERROR           ((uint16_t)0x0000)
#define EE_PAGE_FULL          ((uint16_t)0x0080)
#define NO_VALID_PAGE         ((uint16_t)0x00AB)

#define READ_FROM_VALID_PAGE  ((uint8_t)0x00)
#define WRITE_IN_VALID_PAGE   ((uint8_t)0x01)



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
ErrorStatus eepromInit(void);
int32_t eepromRead(uint16_t VirtAddress);
int32_t eepromWrite(uint16_t VirtAddress, uint16_t Data);



//--------------------------------------------------------------
#endif // __EEPROMFLASH_H
