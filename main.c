#include <ncurses.h>
#include <unistd.h>
#include "playerInfo.h"
#include "enemyInfo.h"
#include "itemInfo.h"
#include "stageInfo.h"
#include "motionArithmetic.h"
#include "motion.h"
#include "etc.h"
#include "define.h"

void changeStage(SD*,CD*,CD*,ID*,int);
void changeEnemy(CD*,int);
void changeItem(ID*,int);
void resetHaveItem(CD*);
void decidePlayer(CD*,SD*);
void decideEnemy(CD*,SD*);
void decideItem(ID*,SD*);
void outputCharacter(SD*,CD*);
void outputItem(SD*,ID*);
void deleteCharacter(SD*,CD*);
void deleteItem(SD*,ID*);
void outputStage(SD*);
void outputStatus(CD*,int);
void scrollDisplay(SD*,CD*);
void confirmExistCharacter(SD*,CD*,CD*,char);
void confirmExistItem(SD*,ID*);
void gameover(SD*,CD*,CD*,ID*);

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

void changeStage(SD* stage,CD* player,CD* enemy,ID* item,int stageNumber){
	int i;
	/***** ステージ共通設定 *****/
	/* ステージと背景の生成 */
	createStage(stage,stageNumber);
	/* プレイヤーの構成の生成 */
	initializePlayer(player,NO_ACTION,RIGHT);
	/* 敵キャラクタの構成の生成 */
	changeEnemy(enemy,stageNumber);
	/* 単独アイテムの構成の生成 */
	changeItem(item,stageNumber);
	/* 所持アイテムの削除 */
	resetHaveItem(player);
	for(i=0;i<MAX_ENEMY;i++){
		if(enemy[i].existFlag==TRUE)
			resetHaveItem(&enemy[i]);
	}
	
	/***** ステージ別設定 *****/
	switch(stageNumber){
		case 0:
			/* プレイヤー初期座標 */
			player->y=player->acceptY=250;
			player->x=player->acceptX=200;
			/* ステージ初期座標 */
			stage->displayY=150;
			stage->displayX=0;
			break;
		default:
			player->y=300;
			player->x=50;
			break;
	}
	
	/***** 設定反映 *****/
	/* キャラクタをステージ描画 */
	outputCharacter(stage,player);
	for(i=0;i<MAX_ENEMY;i++){
		if(enemy[i].existFlag==TRUE)
			outputCharacter(stage,&enemy[i]);
	}
	/* 単独アイテムをステージ描画 */
	for(i=0;i<MAX_ITEM;i++){
		if(item[i].existFlag==TRUE)
			outputItem(stage,&item[i]);
	}
}
void changeEnemy(CD* cd,int stageNumber){
	int num=0;
	int i;
	switch(stageNumber){
		case 0:
			initializeEnemy(&cd[num],0,NO_ACTION,RIGHT);cd[num].y=100;cd[num++].x=20;
			initializeEnemy(&cd[num],0,NO_ACTION,LEFT);cd[num].y=100;cd[num++].x=40;
			initializeEnemy(&cd[num],0,NO_ACTION,LEFT);cd[num].y=100;cd[num++].x=60;
			for(;num<MAX_ENEMY;num++)
				cd[num].existFlag=FALSE;
			break;
		default:
			break;
	}
}
void changeItem(ID* id,int stageNumber){
	int num=0;
	switch(stageNumber){
		case 0:
			initializeItem(&id[num],0,NO_ACTION,RIGHT);id[num].y=180;id[num++].x=120;
			initializeItem(&id[num],0,NO_ACTION,RIGHT);id[num].y=180;id[num++].x=140;
			initializeItem(&id[num],0,NO_ACTION,RIGHT);id[num].y=180;id[num++].x=160;
			initializeItem(&id[num],0,NO_ACTION,RIGHT);id[num].y=180;id[num++].x=180;
			initializeItem(&id[num],0,NO_ACTION,RIGHT);id[num].y=180;id[num++].x=200;
			for(;num<MAX_ITEM;num++)
				id[num].existFlag=FALSE;
			break;
		default:
			break;
	}
}
void resetHaveItem(CD* cd){
	int i;
	for(i=0;i<MAX_HAVE;i++){
		/* 空にする */
		initializeItem(&cd->have[i],0,NO_ACTION,RIGHT);
		/* 存在させない */
		cd->have[i].existFlag=FALSE;
	}
}
void decidePlayer(CD* cd,SD* sd){
	switch(getch()){
		case KEY_LEFT:
			if(cd->motionFlag==NO_ACTION){
				cd->motionFlag=WALK;
				cd->direction=LEFT;
			}
			break;
		case KEY_RIGHT:
			if(cd->motionFlag==NO_ACTION){
				cd->motionFlag=WALK;
				cd->direction=RIGHT;
			}
			break;
		case ' ':
			cd->motionFlag=JUMP;
			break;
		case 'a':
			cd->motionCount=0;
			if(cd->motionFlag!=N_ATTACK1)
				cd->motionFlag=N_ATTACK1;
			else
				cd->motionFlag=N_ATTACK2;
			break;
		case 'A':
			if(cd->motionFlag!=S_ATTACK){
				cd->motionCount=0;
				cd->motionFlag=S_ATTACK;
			}
			break;
		case 'B':
			cd->hitPoint=0;
			break;
		case 'C':
			cd->chargeGauge=100;
			break;
		case 'S':
			cd->stamina=cd->maxStamina;
			break;
		case KEY_DOWN:
			if(cd->fallFlag==TRUE&&cd->fallTime<getParabolaTopTime(cd->v0))
				cd->fallTime+=(getParabolaTopTime(cd->v0)-cd->fallTime)*2;
			break;
		default:
			if(cd->fallFlag==FALSE)
			cd->vX=0;
			break;
	}
}
void decideEnemy(CD* cd,SD* sd){
	/*
	if(
		// 左右に壁と隣接していた場合に跳ね返る
		(getCrashInfo(sd->stage[cd->y+cd->leftWallY][cd->x+cd->leftWallX-1])!=0)||
		(getCrashInfo(sd->stage[cd->y+cd->rightWallY][cd->x+cd->rightWallX+1])!=0)
	){
		cd->direction=(cd->direction==RIGHT)?LEFT:RIGHT;
		//cd->motionFlag=JUMP;
	}
	else
		cd->motionFlag=WALK;
	if(cd->deathFlag==TRUE)*/
		cd->motionFlag=NO_ACTION;
}
void decideItem(ID* id,SD* sd){
	id->motionFlag=NO_ACTION;
}
void outputCharacter(SD* sd,CD* cd){
	int y,x;
	for(y=0;y<CHARACTER_LINES;y++){
		for(x=0;x<CHARACTER_COLS;x++){
			if(
				// 範囲外でなくて
				(cd->y+y>=0&&cd->y+y<STAGE_LINES&&cd->x+x>=0&&cd->x+x<STAGE_COLS)&&
				// 透明でなくて
				(getColorInfo(cd->look[y][x])<64)&&
				// 表示優先度が下の絵以上の時に出力
				(getPriorityInfo(cd->look[y][x])>=getPriorityInfo(sd->stage[cd->y+y][cd->x+x]))
			){
				cd->back[y][x]=1;
				sd->stage[cd->y+y][cd->x+x]=cd->look[y][x];
			}
			else
				cd->back[y][x]=0;
		}
	}
}
void outputItem(SD* sd,ID* id){
	int y,x;
	for(y=0;y<ITEM_LINES;y++){
		for(x=0;x<ITEM_COLS;x++){
			if(
				// 範囲外でなくて
				(id->y+y>=0&&id->y+y<STAGE_LINES&&id->x+x>=0&&id->x+x<STAGE_COLS)&&
				/* 透明でなくて */
				(getColorInfo(id->item[y][x])<64)&&
				/* 表示優先度が下の絵以上の時に出力 */
				(getPriorityInfo(id->item[y][x])>=getPriorityInfo(sd->stage[id->y+y][id->x+x]))
			){
				/* 優先度1以下の時のみ背景をバックアップする */
				if(getPriorityInfo(sd->stage[id->y+y][id->x+x])<=1)
					id->back[y][x]=sd->stage[id->y+y][id->x+x];
				else
					id->back[y][x]=9099;
				sd->stage[id->y+y][id->x+x]=id->item[y][x];
			}
			else
				id->back[y][x]=9099;
		}
	}
}
void deleteCharacter(SD* sd,CD* cd){
	int y,x;
	for(y=0;y<CHARACTER_LINES;y++){
		for(x=0;x<CHARACTER_COLS;x++){
			if(
				/* 透明でなくて */
				(getColorInfo(cd->look[y][x])<64)&&
				/* 表示優先度が下の絵以上で */
				(getPriorityInfo(cd->look[y][x])>=getPriorityInfo(sd->stage[cd->y+y][cd->x+x]))&&
				/* バックアップが1の時のみ削除 */
				(cd->back[y][x]==1)
			)
				sd->stage[cd->y+y][cd->x+x]=getStageInfo(cd->y+y,cd->x+x,sd->number);
		}
	}
}
void deleteItem(SD* sd,ID* id){
	int y,x;
	for(y=0;y<ITEM_LINES;y++){
		for(x=0;x<ITEM_COLS;x++){
			if(
				/* 透明でなくて */
				(getColorInfo(id->item[y][x])<64)&&
				/* バックアップの優先度が1以下の時のみ削除 */
				(getPriorityInfo(id->back[y][x])<=1)
			)
				sd->stage[id->y+y][id->x+x]=id->back[y][x];
		}
	}
}
void outputStage(SD* sd){
	int y,x,z;
	/* ステージの描画 */
	for(y=sd->displayY;y<DISPLAY_LINES+sd->displayY;y++){
		for(x=sd->displayX;x<DISPLAY_COLS+sd->displayX;x++){
			/* 内部的に透明でなければ描画(64以上は透過) */
			if(getColorInfo(sd->stage[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(sd->stage[y][x])));
				for(z=0;z<2;z++)
					mvaddch(y-sd->displayY,(x-sd->displayX)*2+z,getGradationInfo(sd->stage[y][x]));
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
	// lifeTelopの描画
	for(y=0;y<5;y++){
		for(x=0;x<23;x++){
			if(getColorInfo(lifeTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(lifeTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(2+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	// heartLabelの描画
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
	// hpTelopの描画
	for(y=0;y<5;y++){
		for(x=0;x<11;x++){
			if(getColorInfo(hpTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(hpTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(17+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	// hpGaugeの描画
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->hitPoint*100)/cd->maxHitPoint)?2:4));
			mvaddch(24+y,(DISPLAY_COLS+2)*2+x,' ');
		}
	}
	// staminaTelopの描画
	for(y=0;y<5;y++){
		for(x=0;x<41;x++){
			if(getColorInfo(staminaTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(staminaTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(30+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	// staminaGaugeの描画
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->stamina*100)/cd->maxStamina)?3:4));
			mvaddch(37+y,(DISPLAY_COLS+2)*2+x,' ');
		}
	}
	// chargeTelopの描画
	for(y=0;y<5;y++){
		for(x=0;x<35;x++){
			if(getColorInfo(chargeTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(chargeTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(43+y,(DISPLAY_COLS+2+x)*2+z,' ');
			}
		}
	}
	// chargeGaugeの描画
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->chargeGauge*100)/cd->maxChargeGauge)?5:4));
			mvaddch(50+y,(DISPLAY_COLS+2)*2+x,' ');
		}
	}
}
void scrollDisplay(SD* sd,CD* cd){
	/* 上方向にスクロール */
	if(getGravityMove(cd->v0,cd->vA,cd->fallTime)<0&&cd->y<=sd->displayY+SCROLL_LINES){
		sd->displayY+=getGravityMove(cd->v0,cd->vA,cd->fallTime);
		if(sd->displayY<sd->minDisplayY)
			sd->displayY=sd->minDisplayY;
	}
	/* 下方向にスクロール */
	if(getGravityMove(cd->v0,cd->vA,cd->fallTime)>0&&cd->y+CHARACTER_LINES>=sd->displayY+DISPLAY_LINES-SCROLL_LINES){
		sd->displayY+=getGravityMove(cd->v0,cd->vA,cd->fallTime);
		if(sd->displayY+DISPLAY_LINES>sd->maxDisplayY)
			sd->displayY=sd->maxDisplayY-DISPLAY_LINES;
	}
	/* 左方向にスクロール */
	if(cd->x<=sd->displayX+SCROLL_COLS){
		sd->displayX+=-cd->aX;
		if(sd->displayX<sd->minDisplayX)
			sd->displayX=sd->minDisplayX;
	}
	/* 右方向にスクロール */
	if(cd->x+CHARACTER_COLS>=sd->displayX+DISPLAY_COLS-SCROLL_COLS){
		sd->displayX+=cd->aX;
		if(sd->displayX+DISPLAY_COLS>sd->maxDisplayX)
			sd->displayX=sd->maxDisplayX-DISPLAY_COLS;
	}
}
void confirmExistCharacter(SD* sd,CD* cd,CD* pd,char operand){
	if(
		/* 底に到達した時は即死 */
		(cd->y+cd->downWallY+2==STAGE_LINES)&&operand==TRUE
	)
		//cd->hitPoint=0;
		cd->existFlag=FALSE;
	if(
		/* 死亡状態でNO_ACTIONになった時に存在を削除 */
		(cd->deathFlag==TRUE&&cd->motionFlag==NO_ACTION)&&operand==TRUE
	)
		cd->existFlag=FALSE;
	/* HPが0なら死亡状態とする */
	if(cd->hitPoint<=0&&operand==FALSE){
		cd->deathFlag=TRUE;
		if((pd->chargeGauge+=10)>pd->maxChargeGauge)
			pd->chargeGauge=pd->maxChargeGauge;
	}
}
void confirmExistItem(SD* sd,ID* id){
	/*
	 * アイテムごとに存在を消すべきタイミングが異なるため、
	 * ここでは底に到達した場合のみ定義しています
	 * */
	if(id->y+id->downWallY+2==STAGE_LINES)
		id->existFlag=FALSE;
}
void gameover(SD* sd,CD* pd,CD* ed,ID* id){
	int y,x,z;
	CD *cdTmp;
	ID *idTmp;
	erase();
	/* 背景を黒にする */
	bkgd(COLOR_PAIR(getColorInfo(10)));
	// 単独アイテムの削除
	for(y=0;y<MAX_ITEM;y++)
		deleteItem(sd,&id[y]);
	// 所持アイテムとキャラクタの削除
	for(y=0;y<MAX_ENEMY+1;y++){
		cdTmp=(y==0)?pd:&ed[y-1];
		for(x=0;x<MAX_HAVE;x++)
			deleteItem(sd,&cdTmp->have[x]);
		deleteCharacter(sd,cdTmp);
	}
	outputStage(sd);
	// gameoverTelopの描画
	for(y=0;y<5;y++){
		for(x=0;x<47;x++){
			if(getColorInfo(gameoverTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(gameoverTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-47)/2+x)*2+z,getGradationInfo(gameoverTelop[y][x]));
			}
		}
	}
	outputStatus(pd,0);
	refresh();
}
void main(){
	/***** プリセット *****/
	/* ncursesの設定 */
	initscr();
	noecho();
	timeout(0);
	cbreak();
	curs_set(0);
	keypad(stdscr,true);
	/* 使用色の設定 */
	setColorPair();
	/* バックカラーの設定 */
	bkgd(COLOR_PAIR(getColorInfo(6)));
	
	/* ステージ、プレイヤーの設定 */
	SD stage;
	CD player;
	CD enemy[MAX_ENEMY];
	CD* cdTmp;
	ID item[MAX_ITEM];
	ID* idTmp;
	
	/* その他情報 */
	char restLife=3;
	int stageNumber=0;
	
	int i,j;
	
	/***** プレイヤーの残機ある間 *****/
	while(restLife>0){
		/* ステージ構成を最初の状態でリセット */
		changeStage(&stage,&player,enemy,item,stageNumber);
		/***** プレイヤーの存在している間 *****/
		while(player.existFlag==TRUE){
			/***** 単独アイテム処理 *****/
			for(i=0;i<MAX_ITEM;i++){
				/* 参照の省略 */
				idTmp=&item[i];
				/* メインフロー */
				if(
					/* 単独アイテムが存在して */
					(idTmp->existFlag==TRUE)&&
					/* プレイヤーとの距離が表示域の対角距離以下の時に処理する */
					(
					getSqrtDistant(
						player.y+CHARACTER_LINES/2,
						player.x+CHARACTER_COLS/2,
						idTmp->y+ITEM_LINES/2,
						idTmp->x+ITEM_COLS/2
					)<=
					getSqrtDistant(
						0,
						0,
						DISPLAY_LINES*0.75,
						DISPLAY_COLS*0.75
					)
					)
				){
					/* 単独アイテムの時間経過 */
					idTmp->fallTime+=ELAPSED_TIME;
					/* 単独アイテムの削除 */
					deleteItem(&stage,idTmp);
					/* 単独アイテムの行動決定 */
					decideItem(idTmp,&stage);
					/* 単独アイテムの行動 */
					motionItem(idTmp,&stage,&player,enemy,item);
					/* 単独アイテムの行動修正 */
					modifyItem(idTmp,&stage);
					/* 壁の取得 */
					getWallLineItem(&stage,idTmp);
					/* 単独アイテムの自由落下運動 */
					if(idTmp->fallFlag==TRUE){
						if((idTmp->vA+=0.2)>0)
							idTmp->vA=0;
						idTmp->x+=idTmp->vX;
						idTmp->y+=getGravityMove(idTmp->v0,idTmp->vA,idTmp->fallTime);
					}
					/* 単独アイテムの自由落下運動修正 */
					modifyCrashItem(&stage,idTmp);
					/* 単独アイテムの座標受理 */
					idTmp->acceptY=idTmp->y;
					idTmp->acceptX=idTmp->x;
					idTmp->acceptDirection=idTmp->direction;
					/* 単独アイテムの存在判定 */
					confirmExistItem(&stage,idTmp);
					/* 単独アイテムの反映 */
					if(idTmp->existFlag==TRUE)
						outputItem(&stage,idTmp);
				}
			}
			/***** キャラクタ処理 *****/
			for(i=0;i<MAX_ENEMY+1;i++){
				/* 参照の省略 */
				cdTmp=(i==0)?&player:&enemy[i-1];
				/***** 所持アイテム処理 *****/
				for(j=0;j<MAX_HAVE;j++){
					/* 参照の省略 */
					idTmp=&cdTmp->have[j];
					/* メインフロー */
					if(
						/* 所持アイテムが存在して */
						(idTmp->existFlag==TRUE)&&
						/* プレイヤーとの距離が表示域の台閣距離以下の時に処理する */
						(
						getSqrtDistant(
							player.y+CHARACTER_LINES/2,
							player.x+CHARACTER_COLS/2,
							idTmp->y+ITEM_LINES/2,
							idTmp->x+ITEM_COLS/2
						)<=
						getSqrtDistant(
							0,
							0,
							DISPLAY_LINES*0.75,
							DISPLAY_COLS*0.75
						)
						)
					){
						/* 所持アイテムの時間経過 */
						idTmp->fallTime+=ELAPSED_TIME;
						/* 所持アイテムの削除 */
						deleteItem(&stage,idTmp);
						/* 所持アイテムの行動決定 */
						decideItem(idTmp,&stage);
						/* 所持アイテムの行動 */
						motionItem(idTmp,&stage,&player,enemy,item);
						/* 所持アイテムの行動修正 */
						modifyItem(idTmp,&stage);
						/* 壁の取得 */
						getWallLineItem(&stage,idTmp);
						/* 所持アイテムの自由落下運動 */
						if(idTmp->fallFlag==TRUE){
							if((idTmp->vA+=0.2)>0)
								idTmp->vA=0;
							idTmp->x+=idTmp->vX;
							idTmp->y+=getGravityMove(idTmp->v0,idTmp->vA,idTmp->fallTime);
						}
						/* 所持アイテムの自由落下運動修正 */
						modifyCrashItem(&stage,idTmp);
						/* 所持アイテムの座標受理 */
						idTmp->acceptY=idTmp->y;
						idTmp->acceptX=idTmp->x;
						idTmp->acceptDirection=idTmp->direction;
						/* 所持アイテムの存在判定 */
						confirmExistItem(&stage,idTmp);
						/* 所持アイテムの反映 */
						if(idTmp->existFlag==TRUE)
							outputItem(&stage,idTmp);
					}
				}
				/***** キャラクタ処理 *****/
				/* メインフロー */
				if(
					/* キャラクタが存在して */
					(cdTmp->existFlag==TRUE)&&
					/* プレイヤーとの距離が表示域の台閣距離以下の時に処理する */
					(
					getSqrtDistant(
						player.y+CHARACTER_LINES/2,
						player.x+CHARACTER_COLS/2,
						cdTmp->y+CHARACTER_LINES/2,
						cdTmp->x+CHARACTER_COLS/2
					)<=
					getSqrtDistant(
						0,
						0,
						DISPLAY_LINES*0.75,
						DISPLAY_COLS*0.75
					)
					)
				){
					/* キャラクタの時間経過 */
					cdTmp->fallTime+=ELAPSED_TIME;
					/* キャラクタの削除 */
					deleteCharacter(&stage,cdTmp);
					/* キャラクタの行動決定と行動と行動修正 */
					if(i==0){
						decidePlayer(cdTmp,&stage);
						motionPlayer(cdTmp,&stage,enemy,item);
						modifyCharacter(cdTmp,&stage,TRUE);
					}
					else{
						decideEnemy(cdTmp,&stage);
						motionEnemy(cdTmp,&stage,&player,enemy,item);
						modifyCharacter(cdTmp,&stage,FALSE);
					}
					/* 壁の取得 */
					getWallLineCharacter(&stage,cdTmp);
					/* キャラクタの自由落下運動 */
					if(cdTmp->fallFlag==TRUE){
						if((cdTmp->vA+=0.2)>0)
							cdTmp->vA=0;
						cdTmp->x+=cdTmp->vX;
						cdTmp->y+=getGravityMove(cdTmp->v0,cdTmp->vA,cdTmp->fallTime);
					}
					/* キャラクタの自由落下運動修正 */
					modifyCrashCharacter(&stage,cdTmp);
					/* キャラクタの座標受理 */
					cdTmp->acceptY=cdTmp->y;
					cdTmp->acceptX=cdTmp->x;
					cdTmp->acceptDirection=cdTmp->direction;
					/* キャラクタの存在判定 */
					confirmExistCharacter(&stage,cdTmp,&player,TRUE);
					/* キャラクタの反映 */
					if(cdTmp->existFlag==TRUE)
						outputCharacter(&stage,cdTmp);
				}
			}
			/***** ダメージ処理 *****/
			for(i=0;i<MAX_ENEMY+1;i++){
				// 参照の省略
				cdTmp=(i==0)?&player:&enemy[i-1];
				if(
					// キャラクタが存在する時のみ処理する
					(cdTmp->existFlag==TRUE)
				){
					// キャラクタのダメージ判定
					if(confirmDamageCharacter(&stage,cdTmp)==TRUE){
						cdTmp->hitPoint-=(i!=0&&player.motionFlag==S_ATTACK)?DAMAGE*2:DAMAGE;
					}
					/* キャラクタの存在判定 */
					confirmExistCharacter(&stage,cdTmp,&player,FALSE);
				}
			}
			/***** スクロール処理 *****/
			scrollDisplay(&stage,&player);
			/***** 出力処理 *****/
			erase();
			/* ステージの表示 */
			outputStage(&stage);
			/* ステータスの表示 */
			if(player.deathFlag==TRUE)
				player.chargeGauge=0;
			outputStatus(&player,restLife);
			refresh();
			usleep(10000);
		}
		/***** 死亡時処理 *****/
		restLife--;
		if(restLife!=0){
			erase();
			refresh();
			timeout(1000);
			getch();
			timeout(0);
		}
	}
	/***** ゲームオーバー処理 *****/
	gameover(&stage,&player,enemy,item);
	/* 待機 */
	timeout(-1);
	getch();
	endwin();
}

