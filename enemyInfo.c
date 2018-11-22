#include "define.h"

int enemyInfo[1][1][PLAYER_LINES][PLAYER_COLS]=
{
{
{
{ 0, 0, 0, 0, 0, 0, 0, 0,12,12,12,12, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0,12,12,12,12,12,12, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,12,12,12,12,12,12,12,12, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,12,12,12,12,12,12,12,12, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0,12,12,12,12,12,12, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0,12,12,12,12, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0,12,12, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,12,12,12,12,12, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0,12, 0, 0, 0,12,12,12, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0,12, 0, 0, 0, 0,12,12, 0,12, 0, 0, 0,12, 0, 0, 0},
{ 0, 0, 0,12, 0, 0, 0, 0, 0,12,12, 0, 0,12,12,12, 0, 0, 0, 0},
{ 0, 0, 0,12, 0, 0, 0, 0, 0,12,12, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0, 0,12,22,12, 0, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0,12,12, 0,22,12, 0, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0, 0,12,22, 0, 0,22,12, 0, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0, 0,12,22, 0, 0, 0, 0,22,12, 0, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0, 0,12,22, 0, 0, 0, 0, 0, 0,12,12, 0, 0, 0, 0},
{ 0, 0, 0, 0, 0,12,22, 0, 0, 0, 0, 0, 0, 0,12,12, 0, 0, 0, 0},
{ 0, 0, 0, 0,12,12, 0, 0, 0, 0, 0, 0, 0, 0,12,12, 0, 0, 0, 0},
{ 0, 0, 0, 0,22,22,22,22,22, 0, 0, 0, 0, 0,22,22,22,22,22, 0}
}
}
};

void initializeEnemy(PD* pd,int variety,int number,int direction);
void createEnemy(PD* pd,int variety,int number,int direction);

void initializeEnemy(PD* pd,int variety,int number,int direction){
	createEnemy(pd,variety,number,direction);
	switch(variety){
		case 0:
			pd->aX=4.0;
			pd->maxHitPoint=100;
			pd->hitPoint=100;
			pd->attackPower=20;
			break;
		default:
			pd->aX=4.0;
		break;
	}
	pd->v0=0;
	pd->vA=0;
	pd->vX=0;
}
void createEnemy(PD* pd,int variety,int number,int direction){
	int y,x;
	for(y=0;y<PLAYER_LINES;y++){
		for(x=0;x<PLAYER_COLS;x++){
			if(direction>0)
				pd->look[y][x]=enemyInfo[variety][number][y][x];
			else if(direction<0)
				pd->look[y][x]=enemyInfo[variety][number][y][PLAYER_COLS-x-1];
		}
	}
	pd->variety=variety;
	pd->number=number;
	pd->direction=direction;
}
																																																							
