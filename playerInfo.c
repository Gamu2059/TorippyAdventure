#include "define.h"

int playerInfo[1][PLAYER_LINES][PLAYER_COLS]=
{
{
{ 0, 0, 0, 0, 0, 0, 0, 0,15,15,15,15, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0,15,15,15,15,15,15, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,15,15,15,15,15,15,15,15, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,15,15,15,15,15,15,15,15, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0,15,15,15,15,15,15, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0,15,15,15,15, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0,15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,15,15,15,15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0,15, 0, 0, 0,15,15,15, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0,15, 0, 0, 0, 0,15,15, 0,15, 0, 0, 0,15, 0, 0, 0},
{ 0, 0, 0,15, 0, 0, 0, 0, 0,15,15, 0, 0,15,15,15, 0, 0, 0, 0},
{ 0, 0, 0,15, 0, 0, 0, 0, 0,15,15, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0,15,25,15, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0,15,15, 0,25,15, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0,15,25, 0, 0,25,15, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0,15,25, 0, 0, 0, 0,25,15, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,15,25, 0, 0, 0, 0, 0, 0,15,15, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0,15,25, 0, 0, 0, 0, 0, 0, 0,15,15, 0, 0, 0, 0},
{ 0, 0, 0, 0,15,15, 0, 0, 0, 0, 0, 0, 0, 0,15,15, 0, 0, 0, 0},
{ 0, 0, 0, 0,25,25,25,25,25, 0, 0, 0, 0, 0,25,25,25,25,25, 0}
}
};

void initializePlayer(PD* pd,int number,int direction);
void createPlayer(PD* pd,int number,int direction);

void initializePlayer(PD* pd,int number,int direction){
	createPlayer(pd,number,direction);
	pd->aX=4.0;
	pd->maxHitPoint=100;
	pd->hitPoint=100;
	pd->attackPower=50;
	pd->maxStamina=100;
	pd->stamina=100;
	pd->chargeGauge=0;
}
void createPlayer(PD* pd,int number,int direction){
	int y,x;
	for(y=0;y<PLAYER_LINES;y++){
		for(x=0;x<PLAYER_COLS;x++){
			if(direction>0)
				pd->look[y][x]=playerInfo[number][y][x];
			else if(direction<0)
				pd->look[y][x]=playerInfo[number][y][PLAYER_COLS-x-1];
		}
	}
	pd->number=number;
	pd->direction=direction;
}

