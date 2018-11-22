/***** ヘッダファイルのインクルード *****/
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include "playerInfo.h"
#include "enemyInfo.h"
#include "itemInfo.h"
#include "stageInfo.h"
#include "motionArithmetic.h"
#include "etc.h"
#include "openning.h"
#include "ending.h"
#include "define.h"

/***** プロトタイプ宣言 *****/
void changeStage(SD*,CD*,CD*,OD*,int);
void changeEnemy(CD*,int);
void changeItem(OD*,int);
void decidePlayer(OD*,SD*,CD*,CD*,OD*);
void decideEnemy(OD*,SD*,CD*,CD*,OD*);
void decideItem(OD*,SD*,CD*,CD*,OD*);
void outputObject(SD*,OD*);
void deleteObject(SD*,OD*);
void outputStage(SD*);
void outputStatus(CD*,int);
void scrollDisplay(SD*,OD*);
void confirmExistObject(SD*,OD*,OD*,int,int);
void gameover(SD*,CD*,CD*,OD*);
void gamePause(SD*);
char confirmAchieve(SD*,CD*,CD*,OD*,int);
void objectFlow(OD*,SD*,CD*,CD*,OD*,int);
void objectDamage(OD*,SD*,CD*,int);
void objectReoutput(OD*,SD*,CD*);

/***** ステージやキャラクタ、アイテムの宣言 *****/
/* (スタック領域の圧迫を避けるため、グローバル宣言しています) */
SD stage;
CD player;
CD enemy[MAX_ENEMY];
CD* cdTmp;
OD item[MAX_ITEM];
OD* odTmp;
int restLife=REST_LIFE;
int stageNumber=0;
int achieveFlag=FALSE;
int pauseFlag=FALSE;
clock_t start,end;

char heartLabel[5][5]=
{
{99, 1,99, 1,99},
{ 1, 1, 1, 1, 1},
{99, 1, 1, 1,99},
{99,99, 1,99,99},
{99,99,99,99,99}
};
char lifeTelop[5][23]=
{
{ 7,99,99,99,99,99,99, 7, 7, 7,99,99, 7, 7, 7, 7, 7,99, 7, 7, 7, 7, 7},
{ 7,99,99,99,99,99,99,99, 7,99,99,99, 7,99,99,99,99,99, 7,99,99,99,99},
{ 7,99,99,99,99,99,99,99, 7,99,99,99, 7, 7, 7, 7,99,99, 7, 7, 7, 7,99},
{ 7,99,99,99,99,99,99,99, 7,99,99,99, 7,99,99,99,99,99, 7,99,99,99,99},
{ 7, 7, 7, 7, 7,99,99, 7, 7, 7,99,99, 7,99,99,99,99,99, 7, 7, 7, 7, 7}
};
char hpTelop[5][11]=
{
{ 7,99,99,99, 7,99, 7, 7, 7, 7,99},
{ 7,99,99,99, 7,99, 7,99,99,99, 7},
{ 7, 7, 7, 7, 7,99, 7, 7, 7, 7,99},
{ 7,99,99,99, 7,99, 7,99,99,99,99},
{ 7,99,99,99, 7,99, 7,99,99,99,99}
};
char staminaTelop[5][41]=
{
{99, 7, 7, 7, 7,99, 7, 7, 7, 7, 7,99,99, 7, 7, 7,99,99, 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7,99,99,99, 7,99,99, 7, 7, 7,99},
{ 7,99,99,99,99,99,99,99, 7,99,99,99, 7,99,99,99, 7,99, 7, 7,99, 7, 7,99,99,99, 7,99,99,99, 7, 7,99,99, 7,99, 7,99,99,99, 7},
{99, 7, 7, 7,99,99,99,99, 7,99,99,99, 7, 7, 7, 7, 7,99, 7,99, 7,99, 7,99,99,99, 7,99,99,99, 7,99, 7,99, 7,99, 7, 7, 7, 7, 7},
{99,99,99,99, 7,99,99,99, 7,99,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99,99,99, 7,99,99,99, 7,99,99, 7, 7,99, 7,99,99,99, 7},
{ 7, 7, 7, 7,99,99,99,99, 7,99,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7}
};
char chargeTelop[5][35]=
{
{99, 7, 7, 7, 7,99, 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7,99,99,99, 7, 7, 7,99,99, 7, 7, 7, 7, 7},
{ 7,99,99,99,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99,99,99, 7,99,99,99,99},
{ 7,99,99,99,99,99, 7, 7, 7, 7, 7,99, 7, 7, 7, 7, 7,99, 7, 7, 7, 7,99,99, 7,99,99, 7, 7,99, 7, 7, 7, 7,99},
{ 7,99,99,99,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99, 7,99,99, 7,99,99,99, 7,99, 7,99,99,99,99},
{99, 7, 7, 7, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7, 7}
};
char gameoverTelop[5][47]=
{
{99, 1, 1, 1,99,99,99, 1, 1, 1,99,99, 1,99,99,99, 1,99, 1, 1, 1, 1, 1,99,99, 1, 1, 1,99,99, 1,99,99,99, 1,99, 1, 1, 1, 1, 1,99, 1, 1, 1, 1,99},
{ 1,99,99,99,99,99, 1,99,99,99, 1,99, 1, 1,99, 1, 1,99, 1,99,99,99,99,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99, 1},
{ 1,99,99, 1, 1,99, 1, 1, 1, 1, 1,99, 1,99, 1,99, 1,99, 1, 1, 1, 1,99,99, 1,99,99,99, 1,99,99, 1,99, 1,99,99, 1, 1, 1, 1,99,99, 1, 1, 1,1 ,99},
{ 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99, 1,99,99, 1,99, 1,99,99, 1,99,99,99,99,99, 1,99,99, 1,99},
{99, 1, 1, 1,99,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1, 1, 1, 1, 1,99,99, 1, 1, 1,99,99,99,99, 1,99,99,99, 1, 1, 1, 1, 1,99, 1,99,99,99, 1}
};
char clearTelop[5][29]=
{
{99, 6, 6, 6, 6,99, 6,99,99,99,99,99, 6, 6, 6, 6, 6,99,99, 6, 6, 6,99,99, 6, 6, 6, 6,99},
{ 6,99,99,99,99,99, 6,99,99,99,99,99, 6,99,99,99,99,99, 6,99,99,99, 6,99, 6,99,99,99, 6},
{ 6,99,99,99,99,99, 6,99,99,99,99,99, 6, 6, 6, 6,99,99, 6, 6, 6, 6, 6,99, 6, 6, 6, 6,99},
{ 6,99,99,99,99,99, 6,99,99,99,99,99, 6,99,99,99,99,99, 6,99,99,99, 6,99, 6,99,99, 6,99},
{99, 6, 6, 6, 6,99, 6, 6, 6, 6, 6,99, 6, 6, 6, 6, 6,99, 6,99,99,99, 6,99, 6,99,99,99, 6}
};
char pauseTelop[5][29]=
{
{ 7, 7, 7, 7,99,99,99, 7, 7, 7,99,99, 7,99,99,99, 7,99,99, 7, 7, 7, 7,99, 7, 7, 7, 7, 7},
{ 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99,99,99, 7,99,99,99,99},
{ 7, 7, 7, 7,99,99, 7, 7, 7, 7, 7,99, 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7,99},
{ 7,99,99,99,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99,99,99,99,99, 7,99, 7,99,99,99,99},
{ 7,99,99,99,99,99, 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7,99,99, 7, 7, 7, 7, 7}
};

void changeStage(SD* sd,CD* pd,CD* ed,OD* id,int stageNumber){
	int i,j;
	/***** ステージ共通設定 *****/
	/* オブジェクトの初期化 */
	for(i=0;i<MAX_ITEM;i++)
		initItemPara(&id[i],EMPTY,NO_ACTION,RIGHT);
	for(i=0;i<MAX_ENEMY+1;i++){
		cdTmp=(i==0)?pd:&ed[i-1];
		initItemPara(&cdTmp->mine,EMPTY,NO_ACTION,RIGHT);
		for(j=0;j<MAX_HAVE;j++)
			initItemPara(&cdTmp->have[i],EMPTY,NO_ACTION,RIGHT);
	}
	/* ステージと背景の生成 */
	createStage(sd,stageNumber);
	/* プレイヤーの構成の生成 */
	initPlayerPara(&pd->mine,NO_ACTION,RIGHT);
	/* 敵キャラクタの構成の生成 */
	changeEnemy(ed,stageNumber);
	/* 単独アイテムの構成の生成 */
	changeItem(id,stageNumber);
	/***** ステージ別設定 *****/
	switch(stageNumber){
		case 0:
		case 1:
			/* プレイヤー初期座標 */
			pd->mine.now.y=pd->mine.accept.y=0;
			pd->mine.now.x=pd->mine.accept.x=-30;
			/* ステージ初期座標 */
			sd->displayY=0;
			sd->displayX=0;
			sd->slideDispY=sd->slideDispX=0;
			break;
			break;
	}
	/* バックカラーの設定 */
	bkgd(COLOR_PAIR(sd->stageColor));
	/***** 設定反映 *****/
	/* キャラクタをステージ描画 */
	outputObject(sd,&pd->mine);
	for(i=0;i<MAX_ENEMY;i++){
		if(ed[i].mine.state.existFlag==TRUE)
			outputObject(sd,&ed[i].mine);
	}
	/* 単独アイテムをステージ描画 */
	for(i=0;i<MAX_ITEM;i++){
		if(id[i].state.existFlag==TRUE)
			outputObject(sd,&id[i]);
	}
}
void changeEnemy(CD* cd,int stageNumber){
	int num=0;
	switch(stageNumber){
		case 0:
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=92;cd[num].mine.now.x=cd[num++].mine.accept.x=119;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=222;cd[num].mine.now.x=cd[num++].mine.accept.x=108;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=134;cd[num].mine.now.x=cd[num++].mine.accept.x=361;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=251;cd[num].mine.now.x=cd[num++].mine.accept.x=377;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=155;cd[num].mine.now.x=cd[num++].mine.accept.x=632;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=161;cd[num].mine.now.x=cd[num++].mine.accept.x=713;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=81;cd[num].mine.now.x=cd[num++].mine.accept.x=783;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=235;cd[num].mine.now.x=cd[num++].mine.accept.x=701;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=156;cd[num].mine.now.x=cd[num++].mine.accept.x=988;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=-5;cd[num].mine.now.x=cd[num++].mine.accept.x=1051;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=15;cd[num].mine.now.x=cd[num++].mine.accept.x=1224;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=111;cd[num].mine.now.x=cd[num++].mine.accept.x=1260;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=263;cd[num].mine.now.x=cd[num++].mine.accept.x=1121;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=259;cd[num].mine.now.x=cd[num++].mine.accept.x=1234;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=215;cd[num].mine.now.x=cd[num++].mine.accept.x=1160;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=260;cd[num].mine.now.x=cd[num++].mine.accept.x=1410;
			for(;num<MAX_ENEMY;num++)
				cd[num].mine.state.existFlag=FALSE;
			break;
		case 1:
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=92;cd[num].mine.now.x=cd[num++].mine.accept.x=119;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=222;cd[num].mine.now.x=cd[num++].mine.accept.x=108;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=134;cd[num].mine.now.x=cd[num++].mine.accept.x=361;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=251;cd[num].mine.now.x=cd[num++].mine.accept.x=377;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=155;cd[num].mine.now.x=cd[num++].mine.accept.x=632;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=161;cd[num].mine.now.x=cd[num++].mine.accept.x=713;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=81;cd[num].mine.now.x=cd[num++].mine.accept.x=783;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=235;cd[num].mine.now.x=cd[num++].mine.accept.x=701;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=156;cd[num].mine.now.x=cd[num++].mine.accept.x=988;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=-5;cd[num].mine.now.x=cd[num++].mine.accept.x=1051;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=15;cd[num].mine.now.x=cd[num++].mine.accept.x=1224;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=111;cd[num].mine.now.x=cd[num++].mine.accept.x=1260;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=263;cd[num].mine.now.x=cd[num++].mine.accept.x=1121;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=259;cd[num].mine.now.x=cd[num++].mine.accept.x=1234;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=215;cd[num].mine.now.x=cd[num++].mine.accept.x=1160;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=260;cd[num].mine.now.x=cd[num++].mine.accept.x=1410;
			for(;num<MAX_ENEMY;num++)
				cd[num].mine.state.existFlag=FALSE;
			break;
	}
}
void changeItem(OD* id,int stageNumber){
	int num=0;
	switch(stageNumber){
		case 0:
			//initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=90;id[num].now.x=id[num++].accept.x=10;
			break;
		case 1:
			/*
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=90;id[num].now.x=id[num++].accept.x=10;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=90;id[num].now.x=id[num++].accept.x=30;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=90;id[num].now.x=id[num++].accept.x=50;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=246;id[num].now.x=id[num++].accept.x=285;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=296;id[num].now.x=id[num++].accept.x=585;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=272;id[num].now.x=id[num++].accept.x=753;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=290;id[num].now.x=id[num++].accept.x=1050;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=290;id[num].now.x=id[num++].accept.x=1070;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=290;id[num].now.x=id[num++].accept.x=1360;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=290;id[num].now.x=id[num++].accept.x=1380;
			*/
			for(;num<MAX_ITEM;num++)
				id[num].state.existFlag=FALSE;
			break;
		default:
			break;
	}
}
void decidePlayer(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	int i,j=0;
	int distant=getSqrtDistant(0,0,DISPLAY_LINES,DISPLAY_COLS);
	switch(getch()){
		case KEY_LEFT:
			if(od->now.motion==NO_ACTION){
				od->now.motion=WALK;
				od->now.direction=LEFT;
			}
			break;
		case KEY_RIGHT:
			if(od->now.motion==NO_ACTION){
				od->now.motion=WALK;
				od->now.direction=RIGHT;
			}
			break;
		case KEY_DOWN:
			if(od->state.fallFlag==TRUE&&od->state.fallTime<getParabolaTopTime(od->state.v0))
				od->state.fallTime+=(getParabolaTopTime(od->state.v0)-od->state.fallTime)*2;
			break;
		case 'z':
			if(od->now.motion!=AVOIDANCE)
				od->now.motion=AVOIDANCE;
			break;
		case 'x':
			od->now.motion=JUMP;
			break;
		case 'c':
			if(od->now.motion!=N_ATTACK1&&od->now.motion!=N_ATTACK2&&od->now.motion!=N_ATTACK3&&od->now.motion!=AVOIDANCE)
				od->now.motion=N_ATTACK1;
			else if(od->now.motion==N_ATTACK1)
				od->now.motion=N_ATTACK2;
			else if(od->now.motion==N_ATTACK2||od->now.motion==AVOIDANCE)
				od->now.motion=N_ATTACK3;
			break;
		case 'C':
			if(od->now.motion!=S_ATTACK)
				od->now.motion=S_ATTACK;
			break;
		case '1':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].mine.state.existFlag==FALSE){
					initEnemyPara(&ed[i].mine,SLIME,NO_ACTION,RIGHT);ed[i].mine.now.y=ed[i].mine.accept.y=od->now.y;ed[i].mine.now.x=ed[i].mine.accept.x=(od->now.x+40<STAGE_COLS-OBJECT_COLS)?od->now.x+40:STAGE_COLS-OBJECT_COLS;
					break;
				}
			}
			break;
		case '2':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].mine.state.existFlag==FALSE){
					initEnemyPara(&ed[i].mine,WISP,NO_ACTION,RIGHT);ed[i].mine.now.y=ed[i].mine.accept.y=od->now.y;ed[i].mine.now.x=ed[i].mine.accept.x=(od->now.x+40<STAGE_COLS-OBJECT_COLS)?od->now.x+40:STAGE_COLS-OBJECT_COLS;
					break;
				}
			}
			break;
		case '3':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].mine.state.existFlag==FALSE){
					initEnemyPara(&ed[i].mine,SOLDIER,NO_ACTION,RIGHT);ed[i].mine.now.y=ed[i].mine.accept.y=od->now.y;ed[i].mine.now.x=ed[i].mine.accept.x=(od->now.x+40<STAGE_COLS-OBJECT_COLS)?od->now.x+40:STAGE_COLS-OBJECT_COLS;
					break;
				}
			}
			break;
		case '4':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].mine.state.existFlag==FALSE){
					initEnemyPara(&ed[i].mine,WIZARD,NO_ACTION,RIGHT);ed[i].mine.now.y=ed[i].mine.accept.y=od->now.y;ed[i].mine.now.x=ed[i].mine.accept.x=(od->now.x+40<STAGE_COLS-OBJECT_COLS)?od->now.x+40:STAGE_COLS-OBJECT_COLS;
					break;
				}
			}
			break;
		case '5':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].mine.state.existFlag==FALSE){
					initEnemyPara(&ed[i].mine,MIRROR,NO_ACTION,RIGHT);ed[i].mine.now.y=ed[i].mine.accept.y=od->now.y;ed[i].mine.now.x=ed[i].mine.accept.x=(od->now.x+40<STAGE_COLS-OBJECT_COLS)?od->now.x+40:STAGE_COLS-OBJECT_COLS;
					break;
				}
			}
			break;
		case 'Q':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].mine.state.existFlag==TRUE){
					if(getSqrtDistant(od->now.y+OBJECT_LINES/2,od->now.x+OBJECT_COLS/2,ed[i].mine.now.y+OBJECT_LINES/2,ed[i].mine.now.x+OBJECT_COLS/2)<distant){
						distant=getSqrtDistant(od->now.y+OBJECT_LINES/2,od->now.x+OBJECT_COLS/2,ed[i].mine.now.y+OBJECT_LINES/2,ed[i].mine.now.x+OBJECT_COLS/2);
						j=i;
					}
				}
			}
			ed[j].mine.state.HP=0;
			break;
		case 'p':
			pauseFlag=TRUE;
			break;
		case 'a':
			od->state.HP=od->state.maxHP;
			break;
		default:
			if(od->accept.motion==NO_ACTION&&od->state.fallFlag==FALSE){
				od->state.vX=0;
				od->state.motionCount=0;
			}
			if(od->accept.motion==WALK||od->accept.motion==JUMP)
				od->now.motion=NO_ACTION;
			break;
	}
}
void decideEnemy(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	int i;
	switch(od->now.variety){
		case SLIME:
			/* 方向処理 */
			if(od->now.motion==NO_ACTION){
				if(od->now.x<pd->mine.now.x)
					od->now.direction=RIGHT;
				else
					od->now.direction=LEFT;
			}
			/* 攻撃処理 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,60,60)){
				if(od->now.motion!=N_ATTACK1&&od->state.stamina-od->state.maxStamina>=0)
					od->now.motion=N_ATTACK1;
			}
			/* 移動処理 */
			if((od->now.y-pd->mine.now.y>-OBJECT_LINES&&od->now.y-pd->mine.now.y<OBJECT_LINES)&&od->now.motion==NO_ACTION){
				if(od->now.direction==RIGHT){
					for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
						if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX+(int)od->state.iX*2].stage)!=0){
							od->now.motion=WALK;
							break;
						}
					}
				}
				else{
					for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
						if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX-(int)od->state.iX*2].stage)!=0){
							od->now.motion=WALK;
							break;
						}
					}
				}
			}
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE)
				od->now.motion=NO_ACTION;
			break;
		case WISP:
			/* 攻撃処理 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,60,60)){
				if(od->now.motion!=N_ATTACK1&&od->state.stamina-od->state.maxStamina>=0)
					od->now.motion=N_ATTACK1;
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION)
				od->now.motion=WALK;
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE)
				od->now.motion=NO_ACTION;
			break;
		case SOLDIER:
			/* 方向処理 */
			if(od->now.motion==NO_ACTION){
				if(od->now.x<pd->mine.now.x)
					od->now.direction=RIGHT;
				else
					od->now.direction=LEFT;
			}
			/* 攻撃処理 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,20,20)){
				if(
					/* 未行動でスタミナ満タンの時または */
					(od->now.motion!=N_ATTACK1&&od->state.stamina-od->state.maxStamina>=0)||
					/* N_ATTACK1でスタミナ半分で攻撃終了間際の時に攻撃する */
					(od->now.motion==N_ATTACK1&&od->state.stamina-od->state.maxStamina/2>=0&&od->state.motionCount==8)
				)
					od->now.motion=N_ATTACK1;
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION){
				/* 一定距離間隔なら自分から近づく */
				if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,100,100)){
					if(od->now.direction==RIGHT){
						for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
							if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX+(int)od->state.iX*2].stage)!=0){
								od->now.motion=WALK;
								break;
							}
						}
					}
					else{
						for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
							if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX-(int)od->state.iX*2].stage)!=0){
								od->now.motion=WALK;
								break;
							}
						}
					}
				}
			}
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE)
				od->now.motion=NO_ACTION;
			break;
		case WIZARD:
			/* 方向処理 */
			if(od->now.motion==NO_ACTION){
				if(od->now.x<pd->mine.now.x)
					od->now.direction=RIGHT;
				else
					od->now.direction=LEFT;
			}
			/* ビーム反撃 */
			if(od->now.motion==N_ATTACK2)
				break;
			/* 回避処理 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,50,50)){
				if(od->now.motion!=AVOIDANCE&&od->state.stamina-od->state.maxStamina/2>=0){
					od->now.motion=AVOIDANCE;
					od->now.direction=(pd->mine.now.x-od->now.x>0)?RIGHT:LEFT;
				}
			}
			/* 落雷攻撃処理 */
			else if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,120,120)){
				if(od->now.motion!=N_ATTACK1&&od->state.stamina-od->state.maxStamina/2>=0){
					od->now.motion=N_ATTACK1;
					od->now.direction=(pd->mine.now.x-od->now.x>0)?RIGHT:LEFT;
				}
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION){
				/* 一定距離間隔なら自分から近づく */
				if(
					(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)>getSqrtDistant(0,0,70,70))&&
					(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,140,140))
				){
					if(od->now.direction==RIGHT){
						for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
							if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX+(int)od->state.iX*2].stage)!=0){
								od->now.motion=WALK;
								break;
							}
						}
					}
					else{
						for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
							if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX-(int)od->state.iX*2].stage)!=0){
								od->now.motion=WALK;
								break;
							}
						}
					}
				}
			}
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE)
				od->now.motion=NO_ACTION;
			break;
		case MIRROR:
			/* 方向処理 */
			if(od->now.motion==NO_ACTION){
				if(od->now.x<pd->mine.now.x)
					od->now.direction=RIGHT;
				else
					od->now.direction=LEFT;
			}
			if(od->state.fallFlag==TRUE&&od->now.motion==AVOIDANCE)
				break;
			/* 固定ジャンプ */
			if(od->state.eventFlag==TRUE){
				od->now.motion=AVOIDANCE;
				od->state.eventFlag=FALSE;
			}
			/* 固定回避処理 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,30,30)){
				if(od->now.motion==N_ATTACK3&&od->state.motionCount==20&&od->state.stamina-10>=0){
					od->state.eventFlag=TRUE;
					od->now.motion=JUMP;
				}
			}
			/* 攻撃処理3 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,40,40)){
				if(od->now.motion==N_ATTACK2&&od->state.motionCount==8&&od->state.stamina-25>=0)
					od->now.motion=N_ATTACK3;
			}
			/* 攻撃処理2 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,25,25)){
				if(od->now.motion==N_ATTACK1&&od->state.motionCount==8&&od->state.stamina-15>=0)
					od->now.motion=N_ATTACK2;
			}
			/* 攻撃処理1 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,20,20)){
				if((od->now.motion<N_ATTACK1||od->now.motion>N_ATTACK3)&&od->state.stamina-100>=0)
					od->now.motion=N_ATTACK1;
			}
			/* 派生攻撃処理3 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,80,80)){
				if(od->now.motion==AVOIDANCE&&!(pd->mine.now.motion>=N_ATTACK1&&pd->mine.now.motion<=N_ATTACK3)&&od->state.stamina-25>=0)
					od->now.motion=N_ATTACK3;
			}
			/* 回避処理 */
			if(pd->mine.now.motion>=N_ATTACK1&&pd->mine.now.motion<=N_ATTACK3){
				if(od->now.motion!=AVOIDANCE&&od->state.stamina-10>=0)
					od->now.motion=AVOIDANCE;
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION){
				/* 一定距離間隔なら自分から近づく */
				if(
					(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)>getSqrtDistant(0,0,30,30))&&
					(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,100,100))
				){
					if(od->now.direction==RIGHT){
						for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
							if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX+(int)od->state.iX*2].stage)!=0){
								od->now.motion=WALK;
								break;
							}
						}
					}
					else{
						for(i=od->now.y+od->now.crashEndY+1;i<STAGE_LINES-1;i++){
							if(getCrashInfo(sd->look[i][od->now.x+od->now.wallDownBaseX-(int)od->state.iX*2].stage)!=0){
								od->now.motion=WALK;
								break;
							}
						}
					}
				}
			}
			/* 必殺 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,140,140)){
				if(od->now.motion!=S_ATTACK&&od->state.stamina-50>=0&&od->state.charge-od->state.maxCharge>=0)
					od->now.motion=S_ATTACK;
			}
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE)
				od->now.motion=NO_ACTION;
			break;
	}
}
void decideItem(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	switch(od->now.variety){
		case EMPTY:
		case BROCK:
		case WISPBALL:
		case PLAYEREFFECT:
		case MIRROREFFECT:
			od->now.motion=NO_ACTION;
			break;
	}
}
void outputObject(SD* sd,OD* od){
	int y,x;
	for(y=od->now.existStartY;y<=od->now.existEndY;y++){
		for(x=(y==od->now.existStartY)?od->now.existStartX:0;x<OBJECT_COLS;x++){
			od->state.back[y][x]=0;
			if(
				/* 範囲外でなくて */
				(od->now.y+y>=0&&od->now.y+y<STAGE_LINES&&od->now.x+x>=0&&od->now.x+x<STAGE_COLS)
			){
				if(
					/* 透明でなくて */
					(getColorInfo(od->state.look[y][x])<64&&getColorInfo(od->state.look[y][x])>0)&&
					/* 表示優先度が下より高い時に表示する */
					(getPriorityInfo(od->state.look[y][x])>getPriorityInfo(sd->look[od->now.y+y][od->now.x+x].stage))
				){
					od->state.back[y][x]=1;
					/* ダメージのある時はオブジェクトは色が赤くなる */
					if(od->state.damageFlag==TRUE)
						sd->look[od->now.y+y][od->now.x+x].stage=(od->state.look[y][x]/100)*100+1;
					else
						sd->look[od->now.y+y][od->now.x+x].stage=od->state.look[y][x];
					if(getHitInfo(od->state.look[y][x])==1)
						sd->look[od->now.y+y][od->now.x+x].damage=od->state.damage;
				}
			}
		}
	}
}
void deleteObject(SD* sd,OD* od){
	int y,x;
	for(y=od->now.existStartY;y<=od->now.existEndY;y++){
		for(x=(y==od->now.existStartY)?od->now.existStartX:0;x<OBJECT_COLS;x++){
			if(
				/* バックアップが1で */
				(od->state.back[y][x]==1)
			){
				if(
					/* 透明でなくて */
					(getColorInfo(od->state.look[y][x])<64&&getColorInfo(od->state.look[y][x])>0)&&
					/* 表示優先度が下の絵以上の時に削除 */
					(getPriorityInfo(od->state.look[y][x])>=getPriorityInfo(sd->look[od->now.y+y][od->now.x+x].stage))
				){
					sd->look[od->now.y+y][od->now.x+x].stage=sd->back[od->now.y+y][od->now.x+x];
					sd->look[od->now.y+y][od->now.x+x].damage=0;
				}
			}
		}
	}
}
void outputStage(SD* sd){
	int y,x,z;
	/* ステージの描画 */
	for(y=sd->displayY;y<DISPLAY_LINES+sd->displayY;y++){
		for(x=sd->displayX;x<DISPLAY_COLS+sd->displayX;x++){
			if(
				/* 範囲外でなくて */
				(sd->displayY>=0&&sd->displayY<STAGE_LINES&&sd->displayX>=0&&sd->displayX<STAGE_COLS)
			){
				/* 内部的に透明でなければ描画(64以上は透過) */
				if(getColorInfo(sd->look[y][x].stage)<64){
					attron(COLOR_PAIR(getColorInfo(sd->look[y][x].stage)));
					for(z=0;z<2;z++)
						mvaddch(y-sd->displayY,(x-sd->displayX)*2+z,' ');
				}
			}
			else{
				attron(COLOR_PAIR(getColorInfo(sd->look[0][0].stage)));
				for(z=0;z<2;z++)
					mvaddch(y-sd->displayY,(x-sd->displayX)*2+z,' ');
			}
		}
	}
}
void outputStatus(CD* cd,int restLife){
	int i,j,k,l,y,x,z;
	/* ステータスバーの背景を描画 */
	attron(COLOR_PAIR(getColorInfo(10)));
	for(y=0;y<LINES;y++){
		for(x=DISPLAY_COLS;x<DISPLAY_COLS+COLS;x++){
			for(z=0;z<2;z++)
				mvaddch(y,x*2+z,' ');
		}
	}
	/* lifeTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<23;x++){
			if(getColorInfo(lifeTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(lifeTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(2+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	/* heartLabelの描画 */
	for(i=0;i<restLife;i++){
		for(y=0;y<5;y++){
			for(x=0;x<5;x++){
				if(getColorInfo(heartLabel[y][x])<64){
					attron(COLOR_PAIR(getColorInfo(heartLabel[y][x])));
					for(z=0;z<2;z++)
						mvaddch(8+y,(DISPLAY_COLS+2+x+i*6)*2+z,' ');
				}
			}
		}
	}
	/* hpTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<11;x++){
			if(getColorInfo(hpTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(hpTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(17+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	/* hpGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->mine.state.HP*100)/cd->mine.state.maxHP)?2:4));
			mvaddch(24+y,(DISPLAY_COLS+2)*2+x,' ');
		}
	}
	/* staminaTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<41;x++){
			if(getColorInfo(staminaTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(staminaTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(30+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	/* staminaGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->mine.state.stamina*100)/cd->mine.state.maxStamina)?3:4));
			mvaddch(37+y,(DISPLAY_COLS+2)*2+x,' ');
		}
	}
	/* chargeTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<35;x++){
			if(getColorInfo(chargeTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(chargeTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(43+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	/* chargeGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->mine.state.charge*100)/cd->mine.state.maxCharge)?5:4));
			mvaddch(50+y,(DISPLAY_COLS+2)*2+x,' ');
		}
	}
}
void scrollDisplay(SD* sd,OD* od){
	sd->displayY=od->now.y+OBJECT_LINES/2-DISPLAY_LINES/2;
	sd->displayX=od->now.x+OBJECT_COLS/2-DISPLAY_COLS/2;
	if(sd->displayY<sd->minDisplayY)
		sd->displayY=sd->minDisplayY;
	if(sd->displayX<sd->minDisplayX)
		sd->displayX=sd->minDisplayX;
	if(sd->displayY+DISPLAY_LINES>=sd->maxDisplayY)
		sd->displayY=sd->maxDisplayY-DISPLAY_LINES;
	if(sd->displayX+DISPLAY_COLS>=sd->maxDisplayX)
		sd->displayX=sd->maxDisplayX-DISPLAY_COLS;
	sd->displayY+=sd->slideDispY;
	sd->displayX+=sd->slideDispX;
}
void confirmExistObject(SD* sd,OD* od,OD* pd,int label,int ope){
	if(
		/* operandがTRUEで */
		(ope==TRUE)&&
		(
			/* 底に到達した時または */
			(od->now.y+od->now.crashEndY+2>=STAGE_LINES)||
			/* 死亡状態でNO_ACTIONになった時に存在を削除 */
			(od->state.deathFlag==TRUE&&od->now.motion==NO_ACTION)
		)
	){
		if(label==PLAYER)
			od->state.existFlag=FALSE;
		else
			initItemPara(od,EMPTY,NO_ACTION,RIGHT);
	}
	/* operandがFALSEでHPが0なら死亡状態とする */
	if(od->state.HP<=0&&ope==FALSE){
		od->state.deathFlag=TRUE;
		if(label!=ITEM){
			if((pd->state.charge+=CHARGE_VALUE)>pd->state.maxCharge)
				pd->state.charge=pd->state.maxCharge;
		}
	}
}
void gameover(SD* sd,CD* pd,CD* ed,OD* id){
	int y,x,z;
	erase();
	/* 背景を黒にする */
	bkgd(COLOR_PAIR(getColorInfo(10)));
	/* 単独アイテムの削除 */
	for(y=0;y<MAX_ITEM;y++)
		deleteObject(sd,&id[y]);
	/* 所持アイテムとキャラクタの削除 */
	for(y=0;y<MAX_ENEMY+1;y++){
		cdTmp=(y==0)?pd:&ed[y-1];
		for(x=0;x<MAX_HAVE;x++)
			deleteObject(sd,&cdTmp->have[x]);
		deleteObject(sd,&cdTmp->mine);
	}
	outputStage(sd);
	/* gameoverTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<47;x++){
			if(getColorInfo(gameoverTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(gameoverTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-47)/2+x)*2+z,' ');
			}
		}
	}
	outputStatus(pd,0);
	refresh();
}
void gamePause(SD* sd){
	int y,x,z;
	/* pauseTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<29;x++){
			if(getColorInfo(pauseTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(pauseTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-29)/2+x)*2+z,' ');
			}
		}
	}
	refresh();
	while(getch()!='p');
	bkgd(COLOR_PAIR(sd->stageColor));
	pauseFlag=FALSE;
}
char confirmAchieve(SD* sd,CD* pd,CD* ed,OD* id,int rest){
	int y,x,z;
	
	/* 達成条件の確認 */
	for(y=0;y<MAX_ENEMY;y++){
		if(ed[y].mine.state.existFlag==TRUE)
			return FALSE;
	}
	if(stageNumber==0)
		changeStage(sd,pd,ed,id,++stageNumber);
	else{
	erase();
	/* 背景を黒にする */
	bkgd(COLOR_PAIR(getColorInfo(10)));
	/* 単独アイテムの削除 */
	for(y=0;y<MAX_ITEM;y++)
		deleteObject(sd,&id[y]);
	/* 所持アイテムとキャラクタの削除 */
	for(y=0;y<MAX_ENEMY+1;y++){
		cdTmp=(y==0)?pd:&ed[y-1];
		for(x=0;x<MAX_HAVE;x++)
			deleteObject(sd,&cdTmp->have[x]);
		deleteObject(sd,&cdTmp->mine);
	}
	outputStage(sd);
	/* clearTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<29;x++){
			if(getColorInfo(clearTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(clearTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-29)/2+x)*2+z,' ');
			}
		}
	}
	outputStatus(pd,rest);
	refresh();
	usleep(2000000);
	return TRUE;
	}
}
void objectFlow(OD* od,SD* sd,CD* pd,CD* ed,OD* id,int label){
	if(
		/* プレイヤー自身の時または */
		/* オブジェクトが存在して */
		(od->state.existFlag==TRUE)&&
		/* プレイヤーとの距離が表示域の対角距離以下の時に処理する */
		(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,DISPLAY_LINES,DISPLAY_COLS))
	){
		/* オブジェクトの時間経過 */
		od->state.fallTime+=ELAPSED_TIME;
		/* オブジェクトの削除 */
		deleteObject(sd,od);
		/* オブジェクトの行動決定 */
		if(label==PLAYER){
			decidePlayer(od,sd,pd,ed,id);
			motionPlayer(od,sd,pd,ed,id);
		}
		else if(label==ENEMY){
			decideEnemy(od,sd,pd,ed,id);
			motionEnemy(od,sd,pd,ed,id);
		}
		else if(label==ITEM){
			decideItem(od,sd,pd,ed,id);
			motionItem(od,sd,pd,ed,id);
		}
		/* オブジェクトの行動修正 */
		modifyObject(od,sd,label);
		/* 壁の取得 */
		getWallLineObject(sd,od);
		/* オブジェクトの自由落下運動 */
		if(od->state.fallFlag==TRUE){
			if((od->state.vA+=0.2)>0)
				od->state.vA=0;
			od->now.x+=(int)od->state.vX;
			od->now.y+=getGravityMove(od->state.v0,od->state.vA,od->state.fallTime);
		}
		/* オブジェクトの自由落下運動修正 */
		modifyCrashObject(sd,od,label);
		/* オブジェクトの受理 */
		od->accept=od->now;
		/* オブジェクトの存在判定 */
		confirmExistObject(sd,od,&pd->mine,label,TRUE);
		/* オブジェクトの反映 */
		if(od->state.existFlag==TRUE)
			outputObject(sd,od);
	}
}
void objectDamage(OD* od,SD* sd,CD* pd,int label){
	int y,x;
	if(
		(od->state.existFlag==TRUE)&&
		(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,DISPLAY_LINES,DISPLAY_COLS))
	){
		od->state.damageFlag=FALSE;
		if(confirmDamageObject(sd,od,&y,&x)==TRUE){
			if(sd->look[od->now.y+y][od->now.x+x].damage-od->state.DEF>0){
				if(label!=ITEM)
					od->state.damageFlag=TRUE;
				if((od->state.HP-=(sd->look[od->now.y+y][od->now.x+x].damage-od->state.DEF))<0)
					od->state.HP=0;
			}
		}
		confirmExistObject(sd,od,&pd->mine,label,FALSE);
	}
}
void objectReoutput(OD* od,SD* sd,CD* pd){
	if(
		(od->state.existFlag==TRUE)&&
		(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,DISPLAY_LINES,DISPLAY_COLS))
	){
		deleteObject(sd,od);
		outputObject(sd,od);
	}
}
void main(void){
	/***** プリセット *****/
	/* ncursesの設定 */
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr,true);
	/* 使用色の設定 */
	setColorPair();
	/* オブジェクトチップの初期情報の設定 */
	initPlayerInfo();
	initEnemyInfo();
	initItemInfo();
	/* タイトルの描画 */
	//openning();
	int i,j;
	/***** メインフロー *****/
	while(restLife>0){
		/* ステージ構成をリセット */
		changeStage(&stage,&player,enemy,item,stageNumber);
		timeout(0);
		/* プレイヤーの存在している間 */
		while(player.mine.state.existFlag==TRUE){
			start=clock();
			/***** オブジェクト処理 *****/
			/***** 単独アイテム処理 *****/
			for(i=0;i<MAX_ITEM;i++){
				odTmp=&item[i];
				objectFlow(odTmp,&stage,&player,enemy,item,ITEM);
			}
			/***** キャラクタ処理 *****/
			for(i=0;i<MAX_ENEMY+1;i++){
				cdTmp=(i==0)?&player:&enemy[i-1];
				/***** 所持アイテム処理 *****/
				for(j=0;j<MAX_HAVE;j++){
					odTmp=&cdTmp->have[j];
					objectFlow(odTmp,&stage,&player,enemy,item,ITEM);
				}
				odTmp=&cdTmp->mine;
				objectFlow(odTmp,&stage,&player,enemy,item,(i==0)?PLAYER:ENEMY);
			}
			/***** ダメージ処理 *****/
			/***** 単独アイテムジ処理 *****/
			for(i=0;i<MAX_ITEM;i++){
				odTmp=&item[i];
				objectDamage(odTmp,&stage,&player,ITEM);
			}
			/***** キャラクタ処理 *****/
			for(i=0;i<MAX_ENEMY+1;i++){
				cdTmp=(i==0)?&player:&enemy[i-1];
				/***** 所持アイテムジ処理 *****/
				for(j=0;j<MAX_HAVE;j++){
					odTmp=&cdTmp->have[j];
					objectDamage(odTmp,&stage,&player,ITEM);
				}
				odTmp=&cdTmp->mine;
				objectDamage(odTmp,&stage,&player,(i==0)?PLAYER:ENEMY);
			}
			/***** 描画処理 *****/
			/***** 単独アイテム処理 *****/
			for(i=0;i<MAX_ITEM;i++){
				odTmp=&item[i];
				objectReoutput(odTmp,&stage,&player);
			}
			/***** キャラクタ処理 *****/
			for(i=0;i<MAX_ENEMY+1;i++){
				cdTmp=(i==0)?&player:&enemy[i-1];
				/***** 所持アイテム処理 *****/
				for(j=0;j<MAX_HAVE;j++){
					odTmp=&cdTmp->have[j];
					objectReoutput(odTmp,&stage,&player);
				}
				odTmp=&cdTmp->mine;
				objectReoutput(odTmp,&stage,&player);
			}
			/***** ゲームクリア条件 *****/
			achieveFlag=confirmAchieve(&stage,&player,enemy,item,restLife);
			/***** 出力処理 *****/
			if(achieveFlag==TRUE)
				break;
			erase();
			scrollDisplay(&stage,&player.mine);
			if(pauseFlag==TRUE)
				bkgd(COLOR_PAIR(8));
			outputStage(&stage);
			if(player.mine.state.deathFlag==TRUE)
				player.mine.state.charge=0;
			outputStatus(&player,restLife);
			if(pauseFlag==TRUE)
				gamePause(&stage);
			refresh();
			end=clock();
			usleep(4000-((int)((int)(end*1000-start*1000)/CLOCKS_PER_SEC)>4000)?4000:(int)((int)(end*1000-start*1000)/CLOCKS_PER_SEC));
		}
		/***** 死亡時処理 *****/
		if(achieveFlag==TRUE)
			break;
		restLife--;
		if(restLife!=0){
			bkgd(COLOR_PAIR(stage.stageColor));
			erase();
			refresh();
			timeout(1000);
			getch();
		}
	}
	/***** ゲームオーバー処理 *****/
	if(achieveFlag!=TRUE){
		gameover(&stage,&player,enemy,item);
		timeout(-1);
		getch();
	}
	else{
		ending();
	}
	endwin();
}

