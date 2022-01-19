/*
 * life.c
 *
 * Created: 28/11/2021 22:40:46
 *  Author: Scott Porter
 *
 * Plays Conway's game of life. SPIN button resets, FAST_DROP exits
 */ 

#include "funcs.h"
#include "life.h"

const uint8_t surround[8][2]={
	{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}
};

void randomizeGrid(void){
	uint8_t numDots=50+rand()%100;
	clearBuffer();

	for(uint8_t n=0;n<numDots;n++){
		plot(rand()%10,rand()%18,1);
	}
	
}

uint8_t getNumNeighbours(int8_t x,int8_t y){
	uint8_t ret=0;
	for(uint8_t n=0;n<8;n++){
		ret+=getActive(x+surround[n][0],y+surround[n][1]);
	}
	return ret;
}

void updateGrid(void){
	clearBuffer();
	for(int8_t y=0;y<18;y++){
		for(int8_t x=0;x<10;x++){
			uint8_t isLive=getActive(x,y);
			uint8_t numNeighbours=getNumNeighbours(x,y);
			if(
				(!isLive && numNeighbours==3)
				||
				(isLive && (numNeighbours==2 || numNeighbours==3))
			){
				plot(x,y,1);
			}
		}
	}
}

void playGameOfLife(void){
	setVideoMode(VIDEO_MODE_FAST);
	randomizeGrid();
	uint8_t bState;
	uint8_t waitButUp=0;
	while(1){
		flipBuffers();
		updateGrid();
	
		bState=getButtonState();
		if(bState && !waitButUp){
			waitButUp=1;
			if(butPressed(BUTTON_SPIN)){
				randomizeGrid();
			}
			if(butPressed(BUTTON_FAST_DROP)){
				break;
			}
		}else{
			if(!bState)
				waitButUp=0;
		}
		
	}
	setVideoMode(VIDEO_MODE_SLOW);
}