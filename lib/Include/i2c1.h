//--------------------------------------------------------------
// File     : i2c1.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_I2C1_H
#define __STM32F4_I2C1_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"

//--------------------------------------------------------------
// MultiByte defines
//--------------------------------------------------------------
#define    I2C1_MULTIBYTE_ANZ   10        // anzahl der Bytes
uint8_t    I2C1_DATA[I2C1_MULTIBYTE_ANZ]; // Array



//--------------------------------------------------------------
// I2C-Clock
// Grundfrequenz (I2C1)= APB1 (APB1=42MHz)
// M�gliche Einstellungen = 100000 = 100 kHz
//--------------------------------------------------------------
#define   I2C1_CLOCK_FRQ   100000  // I2C-Frq in Hz (100 kHz) 


//--------------------------------------------------------------
// Defines
//-------------------------------------------------------------- 
#define   I2C1_TIMEOUT     0x3000  // Timeout Zeit



//--------------------------------------------------------------
// Struktur eines I2C-Pins
//--------------------------------------------------------------
typedef struct {
  GPIO_TypeDef* PORT;     // Port
  const uint16_t PIN;     // Pin
  const uint32_t CLK;     // Clock
  const uint8_t SOURCE;   // Source
}I2C1_PIN_t; 


//--------------------------------------------------------------
// Struktur vom I2C-Device
//--------------------------------------------------------------
typedef struct {
  I2C1_PIN_t  SCL;       // Clock-Pin
  I2C1_PIN_t  SDA;       // Data-Pin
}I2C1_DEV_t;




//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void i2c1Init(void);
int16_t i2c1ReadByte(uint8_t slave_adr, uint8_t adr);
int16_t i2c1WriteByte(uint8_t slave_adr, uint8_t adr, uint8_t wert);
int16_t i2c1ReadMultiByte(uint8_t slave_adr, uint8_t adr, uint8_t cnt);
int16_t i2c1WriteMultiByte(uint8_t slave_adr, uint8_t adr, uint8_t cnt);
int16_t i2c1WriteCMD(uint8_t slave_adr, uint8_t cmd);
int16_t i2c1ReadByte16(uint8_t slave_adr, uint16_t adr);
int16_t i2c1WriteByte16(uint8_t slave_adr, uint16_t adr, uint8_t wert);
void i2c1Delay(volatile uint32_t nCount);

//--------------------------------------------------------------
#endif // __STM32F4_I2C1_H
