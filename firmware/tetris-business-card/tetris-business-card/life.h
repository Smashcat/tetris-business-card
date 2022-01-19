/*
 * life.h
 *
 * Created: 28/11/2021 22:40:32
 *  Author: Scott Porter
 */ 


#ifndef LIFE_H_
#define LIFE_H_

void randomizeGrid(void);
uint8_t getNumNeighbours(int8_t x,int8_t y);
void updateGrid(void);
void playGameOfLife(void);

#endif /* LIFE_H_ */