/*
 * tetris-business-card.c
 *
 * Created: 10/09/2021 13:37:38
 * Author : Scott Porter
 */ 

#include "funcs.h"
#include "tetris.h"
#include "maze.h"
#include "life.h"

int main(void)
{
	while(1){
		
		// Version 1 boards had hardware interrupt to wake from sleep. New boards use slide switch.
		#if VER==1
		goToSleep();
		#endif

		setup();
		//testButtons();	// Enable this to just test the input buttons
		#if VER==2
		getButtonState();
		#endif
		//testDisplay();	// enable this to just test the LED matrix
		if(butPressed(BUTTON_LEFT)){
			waitBut();
			spinCog();
			playGameOfLife();
		}
		doMenu();
		playTetris();
		/* Initially was going to have 2 games. But ran out of time!
		uint8_t option=doMenu();
		switch(option){
			case 0:
				playTetris();
				break;
			case 1:
				playMaze();
				break;
			default:
				break;
		}
		*/
		shutdown();
		spinLogo();
	}
}

