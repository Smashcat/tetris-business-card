/*
 * defs.h
 *
 * Created: 10/09/2021 13:44:23
 *  Author: Scott Porter
 */ 


#ifndef DEFS_H_
#define DEFS_H_

// Version 1 boards used a wake button, version 2 uses a slide switch for power-on/off
#define VER					2

// If in test mode, preload game grid ready for a tetris
#define TEST_MODE			0

// Defines used to switch display between fast and slow modes
#define VIDEO_MODE_FAST		1
#define VIDEO_MODE_SLOW		2

// Number of bytes per video frame (3 brightness levels per pixel)
#define BYTES_PER_FRAME		45

// Speed at which to update the "next piece" display. Lower=faster
#define NP_SPEED			2

// The game speeds up every 2 rows cleared
// Initial game speed - higher = slower
#define INITIAL_GAME_SPEED	30

// Highest game speed - lower = faster - 5 is about the limit of playability
#define HIGHEST_GAME_SPEED	5

// Speed of the timer interrupt that runs the LEDs - lower = faster, but this will slow the rest of the code
#define TIMER_SPEED_SLOW	28

// Speed for "fast" mode, where the LEDs are only 1 brightness, so can spend
// less time in the display update interrupt, leaving more cycles for other processing
#define TIMER_SPEED_FAST	100

// Delay per frame used when scrolling score/hiscore
#define SHOW_SCORE_DELAY	7

// Delay per frame used when scrolling intro
#define INTRO_DELAY			1

// Delay per frame when clearing blocks
#define CLEAR_DELAY			4

// Delay per frame when scrolling blocks down screen at game over
#define GAME_OVER_DELAY		3

// Number of columns in LED array (including "next piece" area)
#define MAX_COL				38

// Number of columns in main LED array (display is 36 columns of 5 LEDs)
#define MAIN_MAX_COL		36

// Pin definitions for button inputs - pin 2 is a hardware interrupt to wake/sleep, the other 2 are on the ADC, to handle multiple buttons
#define BUT_ON_OFF	2
#define BUT_DIR		4
#define BUT_SPIN	5

// Positions for button states in bitfield
#define BUTTON_FAST_DROP	0
#define BUTTON_SPIN			1
#define BUTTON_LEFT			2
#define BUTTON_RIGHT		3
#define BUTTON_DOWN			4
#define BUTTON_ON_OFF		5

// 7 displayed shapes in Tetris game
#define TOTAL_SHAPES		7
#define TOTAL_ROTATIONS		4
#define TOTAL_COORDINATES	4

// Game states
#define STATE_IDLE			0
#define STATE_PLAYING		1
#define STATE_FAST_DROP		2
#define STATE_LANDED		3
#define STATE_CLEARING_ROW	4
#define STATE_GAMEOVER		5

// Signature byte compared to this to check if EEPROM initialized
#define EEPROM_SIG			10

#if VER==1
#define FAST_DROP_ADC		4
#define SPIN_ADC			10
#define DOWN_ADC			10
#define LEFT_ADC			14
#define RIGHT_ADC			15
#elif VER==2
#define FAST_DROP_ADC		4
#define SPIN_ADC			10
#define DOWN_ADC			10
#define LEFT_ADC			7
#define RIGHT_ADC			3
#endif

#endif /* DEFS_H_ */