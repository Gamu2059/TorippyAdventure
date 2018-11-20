#include "looks.h"

#include <ncurses.h>
#include <unistd.h>
#define ACCEL 9.8
#define V0 -2.0
#define VA -3.0
#define TIME 0.05
#define GROUND 40
#ifndef __PLAYER_SIZE__
	#define PL_Y 20
	#define PL_X 20
#endif

typedef struct{
	float x,y;			//プレイヤーの座標
	float v0,vA,vX;		//初速度、初速度補正、Ｘ軸速度
	float fall_time;	//落下時の経過時間
	char  fall_flag;	//落下フラグ
} CD;

int pl_data[PL_Y][PL_X];

void pl_out(char ope){
	int i,j;
	for(i=0;i<PL_Y;i++){
		for(j=0;j<PL_X;j++){
			if(ope<0)	pl_data[i][j]=looks[i][PL_X-1-j];
			else		pl_data[i][j]=looks[i][j];
		}
	}
}

void jump(CD *cd){
	cd->fall_time=0;
	cd->fall_flag=true;
	cd->v0=V0;
	cd->vA=VA;
}

void control(CD *cd){
	int i,j;
	switch(getch()){
		case KEY_LEFT:
			if(cd->fall_flag==false){
				cd->x-=4.0;
				cd->vX-=1.0;
			}
			else{
				if(cd->vX>0){
					cd->vX*=-1;
					//cd->vX/=1.25;
				}
			}
			if(cd->vX<-4.0)	cd->vX=-4.0;
			pl_out(-1);
			break;
		case KEY_RIGHT:
			if(cd->fall_flag==false){
				cd->x+=4.0;
				cd->vX+=1.0;
			}
			else{
				if(cd->vX<0){
					cd->vX*=-1;
					//cd->vX/=1.25;
				}
			}
			if(cd->vX>4.0) 	cd->vX=4.0;
			pl_out(1);
			break;
		case ' ':
			jump(cd);
			break;
		default:
			if(cd->fall_flag==false)	cd->vX=0.0;
			break;
	}
	cd->x+=cd->vX;
	cd->y+=cd->v0*cd->fall_time+(ACCEL/2)*cd->fall_time*cd->fall_time+cd->vA;
	if(cd->fall_flag==true&&cd->y<=GROUND){
		cd->fall_time+=TIME;
		cd->vA+=0.2;
		if(cd->vA>=0)	cd->vA=0.0;
	}
	else{
		cd->y=GROUND;
		cd->v0=0.0;
		cd->fall_time=0.0;
		cd->fall_flag=false;
	}
	for(i=0;i<PL_Y;i++){
		for(j=0;j<PL_X;j++){
			if(pl_data[i][j]!=0){
				if(cd->x+j<0)				cd->x=0;
				else if(cd->x+j>COLS-2)	cd->x=COLS-2;
			}
		}
	}
}

void main(){
	int i,j;
	initscr();
	noecho();
	timeout(0);
	cbreak();
	curs_set(0);
	keypad(stdscr,true);
	start_color();
	init_pair(1,COLOR_RED,COLOR_RED);
	CD player={0.0,0.0,0.0,0.0,0.0,0.0,false};
	pl_out(1);
	while(1){
		control(&player);
		erase();
		for(i=0;i<PL_Y;i++){
			for(j=0;j<PL_X;j++){
				if(pl_data[i][j]!=0){
					attron(COLOR_PAIR(pl_data[i][j]));
					mvaddch(player.y+i,player.x+j,'A');
				}
				attroff(COLOR_PAIR(pl_data[i][j]));
			}
		}
		mvprintw(10,20,"Y : %4f",player.y);
		mvprintw(11,20,"X : %4f",player.x);
		refresh();
		usleep(33333);
	}
	endwin();
}
