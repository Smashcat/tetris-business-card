/*
 * tetris.h
 *
 * Created: 12/09/2021 14:54:34
 *  Author: Scott Porter
 */ 


#ifndef TETRIS_H_
#define TETRIS_H_

// Current piece controlled by player
typedef struct Tetri {
	int8_t x;
	int8_t y;
	int8_t type;
	int8_t rot;
} Tetri;

void updateNextPieceFrame(void);
void getNewPiece(void);
void resetPlayfield(void);
void drawPlayerPiece(void);
void setPlayfield(int8_t x, int8_t y, uint8_t onOff);
uint8_t checkPlayfield(int8_t x, int8_t y);
void placeCurrentPiece(void);
uint8_t isBlocked(int8_t x,int8_t y,uint8_t r);
void drawCurrentPieceToActive(uint8_t onOff);
uint8_t anyRowsFull(void);
uint8_t playTetris(void);
void loadHi(void);
void saveHi(void);
void loadFcnt(void);
void savefCnt(void);


#endif /* TETRIS_H_ */