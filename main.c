#include <ncurses.h>
#include <unistd.h>
#include <math.h>
#include "playerInfo.h"
#include "enemyInfo.h"
#include "stageInfo.h"
#include "motionArithmetic.h"
#include "etc.h"
#include "define.h"

void initializeCharacter(PD* tmp);
void initializeEnemyToStage(PD* tmp,int stageNumber);
void inputKey(SD* sd,PD* pd,PD *ed);
void AI(SD* sd,PD* ed);
void outputStage(SD* sd);
void outputCharacter(SD* sd,PD* pd);
void deleteCharacter(SD* sd,PD* pd);
void confirmDeath(SD* sd,PD* pd);
void gameover(SD* sd);
void debug(SD* sd){
	outputStage(sd);
	refresh();
	while(getch()!=' ');
}

int gameoverTelop[5][47]=
{
{0,1,1,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0},
{1,0,0,0,0,0,1,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1},
{1,0,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0},
{1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0},
{0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1}
};

void initializeCharacter(PD* tmp){
	// キャラクタの情報初期化
	tmp->v0=0;
	tmp->vA=0;
	tmp->vX=0;
	tmp->wallKickFlag=FALSE;
	tmp->fallTime=0;
	tmp->fallFlag=TRUE;
	tmp->jumpCount=0;
}
void initializeEnemyToStage(PD* tmp,int stageNumber){
	int num=0;
	switch(stageNumber){
		case 0:
			if((tmp[num].deathFlag=FALSE)==FALSE){initializeEnemy(&tmp[num],0,0,-1);tmp[num].y=1;tmp[num++].x=40;}
			if((tmp[num].deathFlag=FALSE)==FALSE){initializeEnemy(&tmp[num],0,0,-1);tmp[num].y=1;tmp[num++].x=80;}
			//if((tmp[num].deathFlag=FALSE)==FALSE){initializeEnemy(&tmp[num],0,0,-1);tmp[num].y=1;tmp[num++].x=120;}
			//if((tmp[num].deathFlag=FALSE)==FALSE){initializeEnemy(&tmp[num],0,0,-1);tmp[num].y=1;tmp[num++].x=160;}
			//if((tmp[num].deathFlag=FALSE)==FALSE){initializeEnemy(&tmp[num],0,0,-1);tmp[num].y=1;tmp[num++].x=200;}
			//if((tmp[num].deathFlag=FALSE)==FALSE){initializeEnemy(&tmp[num],0,0,-1);tmp[num].y=1;tmp[num++].x=240;}
			for(;num<MAX_ENEMY;num++)
				tmp[num].deathFlag=TRUE;
			break;
		default:break;
	}
}
void inputKey(SD* sd,PD* pd,PD *ed){
	int i,keep=0;
	float distant,minDistant=sqrt(DISPLAY_LINES*DISPLAY_LINES+DISPLAY_COLS*DISPLAY_COLS);
	switch(getch()){
		case KEY_LEFT:
			moveArithmetic(sd,pd,-1,TRUE);
			break;
		case KEY_RIGHT:
			moveArithmetic(sd,pd,1,TRUE);
			break;
		case KEY_DOWN:
			if(pd->fallFlag==TRUE&&pd->fallTime<getParabolaTopTime(pd))
				pd->fallTime=getParabolaTopTime(pd);
			break;
		case ' ':
			jump(sd,pd,TRUE);
			break;
		case 'A':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].deathFlag==TRUE)
					continue;
				distant=sqrt((pd->y-ed[i].y)*(pd->y-ed[i].y)+(pd->x-ed[i].x)*(pd->x-ed[i].x));
				if(distant<minDistant){
					minDistant=distant;
					keep=i;
				}
			}
			ed[keep].hitPoint=0;
			break;
		default:
			if(pd->fallFlag==FALSE)
				pd->vX=0;
			break;
	}
}
void AI(SD* sd,PD* ed){
	if(ed->fallFlag==FALSE){
		if(ed->wallKickFlag==TRUE){
			ed->direction*=-1;
			jump(sd,ed,FALSE);
		}
		moveArithmetic(sd,ed,ed->direction,FALSE);
	}
	else{
		if(ed->wallKickFlag==TRUE)
			jump(sd,ed,FALSE);
	}
}
void outputStage(SD* sd){
	int y,x,z;
	for(y=sd->displayY;y<DISPLAY_LINES+sd->displayY;y++){
		for(x=sd->displayX;x<DISPLAY_COLS+sd->displayX;x++){
			attron(COLOR_PAIR(getColorInfo(sd->stage[y][x])));
			for(z=0;z<2;z++)
				mvaddch(y-sd->displayY,(x-sd->displayX)*2+z,' ');
		}
	}
}
void outputCharacter(SD* sd,PD* pd){
	int y,x;
	for(y=0;y<PLAYER_LINES;y++){
		for(x=0;x<PLAYER_COLS;x++){
			if(getCrashInfo(pd->look[y][x])!=0||getColorInfo(pd->look[y][x])!=0){
				if(getCrashInfo(sd->stage[pd->y+y][pd->x+x])==0)
					pd->back[y][x]=sd->stage[pd->y+y][pd->x+x];
				sd->stage[pd->y+y][pd->x+x]=pd->look[y][x];
			}
		}
	}
}
void deleteCharacter(SD* sd,PD* pd){
	int y,x;
	for(y=0;y<PLAYER_LINES;y++){
		for(x=0;x<PLAYER_COLS;x++){
			if(getCrashInfo(pd->look[y][x])!=0||getColorInfo(pd->look[y][x])!=0)
				sd->stage[pd->y+y][pd->x+x]=pd->back[y][x];
		}
	}
}
void confirmDeath(SD* sd,PD* pd){
	if(pd->y+pd->downWallY+2==STAGE_LINES||pd->hitPoint<=0)
		pd->deathFlag=TRUE;
	else
		pd->deathFlag=FALSE;
	if(pd->deathFlag==TRUE){
		deleteCharacter(sd,pd);
		int y,x;
		for(y=0;y<PLAYER_LINES;y++){
			for(x=0;x<PLAYER_COLS;x++)
				pd->look[y][x]=pd->back[y][x]=0;
		}
	}
}
void gameover(SD* sd){
	int y,x;
	for(y=0;y<5;y++){
		for(x=0;x<47;x++){
			if(gameoverTelop[y][x]!=0)
				sd->stage
				[sd->displayY+(int)((DISPLAY_LINES-5)/2)+y]
				[sd->displayX+(int)((DISPLAY_COLS-47)/2)+x]
				=gameoverTelop[y][x];
		}
	}
}
void main(){
	// ncursesの設定
	initscr();
	noecho();
	timeout(0);
	cbreak();
	curs_set(0);
	keypad(stdscr,true);
	start_color();
	// 使用色の設定
	init_pair(1,COLOR_BLACK,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_RED);
	init_pair(3,COLOR_GREEN,COLOR_GREEN);
	init_pair(4,COLOR_YELLOW,COLOR_YELLOW);
	init_pair(5,COLOR_BLUE,COLOR_BLUE);
	init_pair(6,COLOR_MAGENTA,COLOR_MAGENTA);
	init_pair(7,COLOR_CYAN,COLOR_CYAN);
	init_pair(8,COLOR_WHITE,COLOR_WHITE);
	// ステージ情報の初期化
	initializeStageInfo();
	// ステージ、プレイヤーの設定
	SD stage;				// ステージ情報
	PD player;				// プレイヤー情報
	PD enemy[MAX_ENEMY];	// 敵キャラクタ情報
	PD* tmp;				// 汎用
	// その他情報
	int i;
	char retryCount=2;	// リトライできる回数
	int stageNumber=0;	// ステージの階層
	// メインフロー
	while(retryCount>0){
		// ステージの生成
		createStage(&stage,stageNumber);
		// ステージの描画位置を初期化
		stage.displayY=stage.displayX=0;
		// プレイヤーの生成
		initializePlayer(&player,0,1);
		player.y=1;
		player.x=-1;
		// 敵キャラクタの生成
		initializeEnemyToStage(enemy,stageNumber);
		// キャラクタをステージ描画
		initializeCharacter(&player);
		outputCharacter(&stage,&player);
		for(i=0;i<MAX_ENEMY;i++){
			if(enemy[i].deathFlag==FALSE){
				initializeCharacter(&enemy[i]);
				outputCharacter(&stage,&enemy[i]);
			}
		}
		outputStage(&stage);
		player.deathFlag=FALSE;
		while(player.deathFlag==FALSE){
			// メインフロー
			for(i=0;i<MAX_ENEMY+1;i++){
				if(i==0)
					tmp=&player;
				else
					tmp=&enemy[i-1];
				if(tmp->deathFlag==FALSE){
					// キャラクタの削除
					deleteCharacter(&stage,tmp);
					// 壁の取得
					getWallLine(&stage,tmp);
					// 入力処理
					if(i==0)
						inputKey(&stage,&player,enemy);
					else
						AI(&stage,tmp);
					// 経過時間の計算
					tmp->fallTime+=ELAPSED_TIME;
					tmp->vA+=0.2;
					if(tmp->vA>0)
						tmp->vA=0;
					// x,y座標の計算
					if(tmp->fallFlag==TRUE)
						tmp->x+=tmp->vX;
					tmp->y+=getGravityMove(tmp);
					// 位置修正
					modifyCrash(&stage,tmp);
					// 死亡判定
					confirmDeath(&stage,tmp);
					// 反映処理
					if(tmp->deathFlag==FALSE)
						outputCharacter(&stage,tmp);
				}
			}
			// 画面スクロールの計算
			scrollDisplay(&stage,&player);
			// 出力処理
			erase();
			outputStage(&stage);
			refresh();
			usleep(10000);
		}
		retryCount--;
		if(retryCount!=0){
			erase();
			refresh();
			timeout(1000);
			getch();
			timeout(0);
		}
	}
	// ゲームオーバー処理
	gameover(&stage);
	outputStage(&stage);
	refresh();
	while(1);
	endwin();
}
