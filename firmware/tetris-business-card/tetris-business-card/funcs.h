/*
 * funcs.h
 *
 * Created: 10/09/2021 13:40:22
 *  Author: Scott Porter
 */ 
#define F_CPU	1000000L

#include "defs.h"
#include <avr/io.h>
#include <inttypes.h>
#include <stddef.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#ifndef FUNCS_H_
#define FUNCS_H_

void setVideoMode(uint8_t mode);
void goToSleep(void);
void selectCol(void);
void setup(void);
void clearBuffer(void);
void flipBuffers(void);
void copyToBackBuffer(void);
void loadBuffer(uint8_t *data);
void displayVideoFrame(const uint8_t *data, uint16_t frameIX);
uint8_t getActive(int8_t x,int8_t y);
uint8_t getDraw(int8_t x,int8_t y);

void plotActive(int8_t x, int8_t y, uint8_t c);
void plot(int8_t x, int8_t y, uint8_t c);

void drawSpinner(uint8_t dotOff,uint8_t amp);
void drawLine(int8_t x, int8_t y, int8_t w, int8_t h, uint8_t c);
void drawBlock(int8_t x, int8_t y, int8_t w, int8_t h, uint8_t c);
void drawChar(uint8_t charIX,int16_t yPos, uint8_t c);
void drawText(char *s,int16_t yPos, uint8_t c);
void fastHLine(int8_t yPos, uint8_t c);

void showScore(uint16_t s, uint8_t type);
uint8_t doMenu(void);
uint8_t butPressed(uint8_t which);
uint8_t getButtonState(void);
void waitBut(void);
void showButtonStates(void);
void testButtons(void);
void shutdown(void);
void testDisplay(void);
void spinCog(void);
void spinLogo(void);

#endif /* FUNCS_H_ */