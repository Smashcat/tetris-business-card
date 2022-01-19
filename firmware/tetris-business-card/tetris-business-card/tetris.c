/*
 * game.c
 *
 * Created: 12/09/2021 14:54:45
 *  Author: Scott Porter
 *
 * Play a nice game of Tetris :-)
 */ 

#include "funcs.h"
#include "tetris.h"

uint8_t gameState;
int8_t gameSpeed;
int8_t moveCnt;
uint16_t score;
uint16_t hiScore;
uint8_t blockNum=0;
uint8_t subGameSpeed=0;
uint8_t nextPiece=0;
uint8_t nextPieceX=0;
uint8_t fCnt=0;
uint8_t npCnt=NP_SPEED;
uint8_t playField[MAX_COL*2];	// 18 vertical rows of 2 bytes. Only 5 low bits used in each byte - designed to be loaded directly into display buffer
int8_t fullRow[4];

Tetri currentPiece;

// 2 byte versions of shapes to display in the "next piece" panel
const uint8_t pieceByte[TOTAL_SHAPES][2]={
	{0,0b11110},{0b01000,0b01110},{0b00010,0b01110},{0b01100,0b01100},{0b00110,0b01100},{0b00100,0b01110},{0b01100,0b00110}
};

// Definition of the 7 game tetrii in each of their 4 angles of rotation.
// Each tetrii comprises 4 blocks, each block having x,y offset coordinates.
const int8_t pieceDef[TOTAL_SHAPES][TOTAL_ROTATIONS][TOTAL_COORDINATES][2]={
	{

		// [][][][]
		{{-2,0},{-1,0},{0,0},{1,0}},
		{{0,-1},{0,0},{0,1},{0,2}},
		{{-1,0},{0,0},{1,0},{2,0}},
		{{0,-2},{0,-1},{0,0},{0,1}},
		
	},{

		// []
		// [][][]
		{{-1,-1},{-1,0},{0,0},{1,0}},
		{{1,-1},{0,-1},{0,0},{0,1}},
		{{-1,0},{0,0},{1,0},{1,1}},
		{{-1,1},{0,1,},{0,0},{0,-1}},
	},{

		//     []
		// [][][]
		{{-1,0},{0,0},{1,0},{1,-1}},
		{{0,-1},{0,0},{0,1},{1,1}},
		{{-1,1},{-1,0},{0,0},{1,0}},
		{{-1,-1},{0,-1},{0,0},{0,1}},
	},{

		// [][]
		// [][]
		{{0,-1},{1,-1},{0,0},{1,0}},
		{{0,-1},{1,-1},{0,0},{1,0}},
		{{0,-1},{1,-1},{0,0},{1,0}},
		{{0,-1},{1,-1},{0,0},{1,0}},
	},{

		//   [][]
		// [][]
		{{-1,0},{0,0},{0,-1},{1,-1}},
		{{0,-1},{0,0},{1,0},{1,1}},
		{{0,0},{1,0},{0,1},{-1,1}},
		{{-1,-1},{-1,0},{0,0},{0,1}},
	},{

		//   []
		// [][][]
		{{-1,0},{0,0},{1,0},{0,-1}},
		{{0,-1},{0,0},{0,1},{1,0}},
		{{-1,0},{0,0},{1,0},{0,1}},
		{{-1,0},{0,0},{0,-1},{0,1}},
	},{

		// [][]
		//   [][]
		{{-1,-1},{0,-1},{0,0},{1,0}},
		{{0,0},{0,1},{1,0},{1,-1}},
		{{-1,0},{0,0},{0,1},{1,1}},
		{{-1,0},{-1,1},{0,0},{0,-1}},
	}
};

// Slides next piece in/out of little box beside main grid
void updateNextPieceFrame(void){
	uint8_t b0=pieceByte[nextPiece][0];
	uint8_t b1=pieceByte[nextPiece][1];
	if(nextPieceX<5){
		uint8_t sft=4-nextPieceX;
		b0>>=sft;
		b1>>=sft;
	}else if(nextPieceX<10){
		uint8_t sft=nextPieceX-5;
		b0<<=sft;
		b1<<=sft;
	}
	playField[MAX_COL-2]=b0;
	playField[MAX_COL-1]=b1;
	playField[(MAX_COL*2)-2]=b0;
	playField[(MAX_COL*2)-1]=b1;
}

// Moves the next piece onto the board
void getNewPiece(void){
	while(nextPieceX<10){
		updateNextPieceFrame();
		loadBuffer(playField);
		flipBuffers();
		if(--npCnt==0){
			npCnt=NP_SPEED;
			++nextPieceX;
		}
	}
	currentPiece.x=5;					// Center of row
	currentPiece.y=-1;					// Position 2 is actually the top visible row on the display - this make collision detection consistent.
	currentPiece.rot=0;					// Default rotation
	currentPiece.type=nextPiece;
	nextPiece=(rand()+fCnt)%TOTAL_SHAPES;	// type of piece
	nextPieceX=0;
	moveCnt=gameSpeed;
}

// Resets the game
void resetPlayfield(void){
	clearBuffer();
	loadHi();	 
	loadFcnt();
	srand(fCnt);
	score=0;
	getNewPiece();
	nextPieceX=0;
	npCnt=NP_SPEED;
	gameSpeed=INITIAL_GAME_SPEED;
	subGameSpeed=0;
	gameState=STATE_IDLE;
	for(uint8_t n=0;n<MAX_COL*2;n++){
		playField[n]=0;
	}
}

// Does exactly what it says!
void drawPlayerPiece(void){
	const int8_t (*p)[2]=pieceDef[currentPiece.type][currentPiece.rot];
	for(uint8_t n=0;n<TOTAL_COORDINATES;n++){
		plot(
			(currentPiece.x)+p[n][0],
			(currentPiece.y)+p[n][1],
			2
		);
	}
}

// Sets a pixel on/off on the playfield
void setPlayfield(int8_t x, int8_t y, uint8_t onOff){
	if(x>9 || x<0 || y>17 || y<0)
		return;
	y<<=1;
	if(x<5){
		if(onOff){
			playField[y]|=(1<<x);
		}else{
			playField[y]&=~(1<<x);
		}
	}else{
		if(onOff){
			playField[y+1]|=(1<<(x-5));
		}else{
			playField[y+1]&=~(1<<(x-5));
		}
	}
}

// Tests a pixel in the playfield
uint8_t checkPlayfield(int8_t x, int8_t y){
	if(x>9 || x<0 || y>17)
		return 1;
	if(y<0)
		return 0;
	y<<=1;
	if(x<5){
		return ( playField[y]&(1<<x) ? 1 : 0 );
	}else{
		return ( playField[y+1]&(1<<(x-5)) ? 1 : 0 );
	}
}

// Adds the current player piece to the playfield
void placeCurrentPiece(void){
	const int8_t (*p)[2]=pieceDef[currentPiece.type][currentPiece.rot];
	for(uint8_t n=0;n<TOTAL_COORDINATES;n++){
		setPlayfield(
			(currentPiece.x)+p[n][0],
			(currentPiece.y)+p[n][1],
			1
		);
	}
}

// Check if position is clear to move into. If not, returns zero
uint8_t isBlocked(int8_t x,int8_t y,uint8_t r){
	const int8_t (*p)[2]=pieceDef[currentPiece.type][(currentPiece.rot+r)%4];
	for(uint8_t n=0;n<TOTAL_COORDINATES;n++){
		if(
			checkPlayfield(
				(currentPiece.x)+p[n][0]+x,
				(currentPiece.y)+p[n][1]+y 
			)
		){
			return 1;
		}
	}
	return 0;
}

// Draws the current player piece to the active buffer in bright
void drawCurrentPieceToActive(uint8_t onOff){
	const int8_t (*p)[2]=pieceDef[currentPiece.type][currentPiece.rot];
	for(uint8_t n=0;n<TOTAL_COORDINATES;n++){
		if(onOff){
			plotActive(
				(currentPiece.x)+p[n][0],
				(currentPiece.y)+p[n][1],
				2
			);
		}else{
			plotActive(
				(currentPiece.x)+p[n][0],
				(currentPiece.y)+p[n][1],
				0
			);
		}
	}
}

// Check for full rows
uint8_t anyRowsFull(void){
	for(uint8_t n=0;n<4;n++){
		fullRow[n]=-1;
	}
	uint8_t ix=0;
	uint8_t y;
	for(uint8_t n=0;n<18;n++){
		y=n<<1;
		if(playField[y]==31 && playField[y+1]==31){
			if(ix<4){
				fullRow[ix++]=n;
				score+=(ix*2);
				if(++subGameSpeed==2){
					subGameSpeed=0;
					if(gameSpeed>HIGHEST_GAME_SPEED){
						--gameSpeed;
					}
				}
			}
		}
	}
	return ix;
}

uint8_t playTetris(void){
	resetPlayfield();

// Test mode pre-draws pixels so it's easy to get a 4 row clear
#if TEST_MODE
	for(uint8_t n=14;n<18;n++){
		setPlayfield(0,n,1);
		setPlayfield(1,n,1);
		setPlayfield(2,n,1);
		setPlayfield(3,n,1);
		setPlayfield(5,n,1);
		setPlayfield(6,n,1);
		setPlayfield(7,n,1);
		setPlayfield(8,n,1);
		setPlayfield(9,n,1);
	}
#endif

	uint8_t waitButUp=0,bState=0;
	while(gameState!=STATE_GAMEOVER){
		loadBuffer(playField);	// Load current state of playfield into display buffer before adding the player piece
		switch(gameState){
			
			case STATE_IDLE:
				getNewPiece();
				gameState=STATE_PLAYING;
			break;
			
			case STATE_PLAYING:
				bState=getButtonState();
				++fCnt;
				if(bState && !waitButUp){
					waitButUp=1;
					if(butPressed(BUTTON_DOWN)){
						if(isBlocked(0,1,0)){
							gameState=STATE_LANDED;
						}else{
							++currentPiece.y;
							if(moveCnt<2){
								moveCnt=2;
							}
						}
						fCnt+=1;
					}else if(butPressed(BUTTON_LEFT)){
						if(!isBlocked(-1,0,0)){
							--currentPiece.x;
							if(moveCnt<2){
								moveCnt=2;
							}
						}
						fCnt+=2;
					}else if(butPressed(BUTTON_RIGHT)){
						if(!isBlocked(1,0,0)){
							++currentPiece.x;
							if(moveCnt<2){
								moveCnt=2;
							}
						}
						fCnt+=3;
					}else{
						rand();
					}

					if(butPressed(BUTTON_SPIN)){
						if(!isBlocked(0,0,1)){
							currentPiece.rot=(currentPiece.rot+1)%TOTAL_ROTATIONS;
						}
						fCnt+=5;
					}else if(butPressed(BUTTON_FAST_DROP)){
						gameState=STATE_FAST_DROP;
						fCnt+=7;
					}
					
					if(butPressed(BUTTON_ON_OFF)){
						waitBut();
						return 0;
					}
				}else{
					if(!bState)
						waitButUp=0;
					if(--moveCnt<=0){
						if(isBlocked(0,1,0)){
							gameState=STATE_LANDED;
						}else{
							moveCnt=gameSpeed;
							currentPiece.y++;
						}
					}
				}
				if(--npCnt==0){
					npCnt=NP_SPEED;
					if(nextPieceX<5){
						++nextPieceX;
					}
				}
				updateNextPieceFrame();
				drawPlayerPiece();
			break;
			
			case STATE_FAST_DROP:
				// Do fast drop animation here  by drawing directly to the active buffer (the one the LEDs are displaying)
				{
					int8_t cY=currentPiece.y;
					while(1){
						drawCurrentPieceToActive(1);
						if(isBlocked(0,1,0))
							break;
						++currentPiece.y;
						_delay_ms(1);
					}
					currentPiece.y=cY;
					while(1){
						drawCurrentPieceToActive(0);
						if(isBlocked(0,1,0))
							break;
						++currentPiece.y;
						_delay_ms(3);
					}
					gameState=STATE_LANDED;
				}
			break;
			
			case STATE_LANDED:
				drawPlayerPiece();
				placeCurrentPiece();
				if(currentPiece.y==-1){
					gameState=STATE_GAMEOVER;
				}else{
					if(anyRowsFull())
						gameState=STATE_CLEARING_ROW;
					else
						gameState=STATE_IDLE;
				}
			break;
			
			case STATE_CLEARING_ROW:
			{
			
				// Slide full rows off display	
				uint8_t y;
				for(uint8_t n=0;n<12;n++){
					for(uint8_t i=0;i<4;i++){
						if(fullRow[i]>-1){
							if((i*2)<n){
								y=fullRow[i]<<1;
								playField[y]>>=1;
								playField[y+1]<<=1;
							}
						}
					}
					loadBuffer(playField);
					flipBuffers();
					_delay_ms(CLEAR_DELAY);
				}
				
				// Move rows above cleared rows down, one cleared row at a time
				for(int8_t n=3;n>-1;n--){
					if(fullRow[n]>-1){
						for(uint8_t i=fullRow[n];i>0;i--){
							y=i<<1;
							playField[y]=playField[y-2];
							playField[y-2]=0;
							playField[y+1]=playField[y+1-2];
							playField[y+1-2]=0;
						}
						for(int8_t i=n;i>-1;i--){
							if(fullRow[i]>-1){
								fullRow[i]++;
							}
						}
						loadBuffer(playField);
						flipBuffers();
						_delay_ms(CLEAR_DELAY);
					}
				}
				
				loadBuffer(playField);
				flipBuffers();
				gameState=STATE_IDLE;
			}
			break;
		}
		flipBuffers();			// Show current state of game
		_delay_ms(1);
	}
	
	for(uint8_t n=0;n<18;n++){
		for(int8_t i=17;i>0;i--){
			uint8_t y=i<<1;
			playField[y]=playField[y-2];
			playField[y-2]=0;
			playField[y+1]=playField[y+1-2];
			playField[y+1-2]=0;
		}
		loadBuffer(playField);
		flipBuffers();
		_delay_ms(GAME_OVER_DELAY);
	}
	
	if(score>hiScore){
		hiScore=score;
		saveHi();
	}
	
	showScore(score,0);
	showScore(hiScore,1);
	
	savefCnt();
	
	return 1;
}

// Load high score from EEPROM
 void loadHi(void){
 	 int8_t sig = eeprom_read_byte((uint8_t*)0);
 	 if(sig!=EEPROM_SIG){
	 	 eeprom_write_byte((uint8_t*) 0, EEPROM_SIG);
	 	 saveHi();
 	 }
	 hiScore=eeprom_read_byte((uint8_t*)1);
	 hiScore=(hiScore<<8)+eeprom_read_byte((uint8_t*)2);
 }
 
 // Save new high score to EEPROM
 void saveHi(void){
	 eeprom_write_byte((uint8_t*) 1, hiScore>>8);
	 eeprom_write_byte((uint8_t*) 2, hiScore&0xff);
 }
 
 // Load an incrementing var from EEPROM, to make new games more "random"
 void loadFcnt(void){
	 fCnt=eeprom_read_byte((uint8_t*)3);
 }
 
 // Store the incrementing random seed var to EEPROM - and no, I don't know why
 // I decided on different capitalization here either :-)
 void savefCnt(void){
	 eeprom_write_byte((uint8_t*) 3, fCnt);
 }