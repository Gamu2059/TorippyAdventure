#include <ncurses.h>
#include <unistd.h>
#include "playerInfo.h"
#include "enemyInfo.h"
#include "itemInfo.h"
#include "stageInfo.h"
#include "motionArithmetic.h"
#include "etc.h"
#include "openning.h"
#include "define.h"

void changeStage(SD*,CD*,CD*,OD*,int);
void changeEnemy(CD*,int);
void changeItem(OD*,int);
void resetHaveItem(CD*);
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
void acceptObject(OD*);
void objectFlow(OD*,SD*,CD*,CD*,OD*,int);
void objectDamage(OD*,SD*,CD*,int);

/* ステージやキャラクタ、アイテムの宣言 */
/* (スタック領域の圧迫を避けるため、グローバル宣言しています) */
SD stage;
CD player;
CD enemy[MAX_ENEMY];
CD* cdTmp;
OD item[MAX_ITEM];
OD* odTmp;
int restLife=REST_LIFE;
int stageNumber=0;

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

void changeStage(SD* sd,CD* pd,CD* ed,OD* id,int stageNumber){
	int i;
	/***** ステージ共通設定 *****/
	/* ステージと背景の生成 */
	createStage(sd,stageNumber);
	/* プレイヤーの構成の生成 */
	initPlayerPara(&pd->mine,NO_ACTION,RIGHT);
	/* 敵キャラクタの構成の生成 */
	changeEnemy(ed,stageNumber);
	/* 単独アイテムの構成の生成 */
	changeItem(id,stageNumber);
	/* 所持アイテムの削除 */
	resetHaveItem(pd);
	for(i=0;i<MAX_ENEMY;i++){
		if(ed[i].mine.state.existFlag==TRUE)
			resetHaveItem(&ed[i]);
	}
	/***** ステージ別設定 *****/
	switch(stageNumber){
		case 0:
			/* プレイヤー初期座標 */
			pd->mine.now.y=pd->mine.accept.y=50;
			pd->mine.now.x=pd->mine.accept.x=200;
			/* ステージ初期座標 */
			sd->displayY=0;
			sd->displayX=0;
			break;
		default:
			pd->mine.now.y=pd->mine.accept.y=300;
			pd->mine.now.x=pd->mine.accept.x=50;
			break;
	}
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
			/*initEnemyPara(&cd[num].mine,0,NO_ACTION,RIGHT);cd[num].mine.now.y=cd[num].mine.accept.y=100;cd[num].mine.now.x=cd[num++].mine.accept.x=40;*/
			for(;num<MAX_ENEMY;num++)
				cd[num].mine.state.existFlag=FALSE;
			break;
		default:
			break;
	}
}
void changeItem(OD* id,int stageNumber){
	int num=0;
	switch(stageNumber){
		case 0:
			//initItemPara(&id[num],1,NO_ACTION,RIGHT);id[num].now.y=id[num].accept.y=100;id[num].now.x=id[num++].accept.x=345;
			for(;num<MAX_ITEM;num++)
				id[num].state.existFlag=FALSE;
			break;
		default:
			break;
	}
}
void resetHaveItem(CD* cd){
	int i;
	for(i=0;i<MAX_HAVE;i++){
		/* 空にする */
		initItemPara(&cd->have[i],0,NO_ACTION,RIGHT);
		/* 存在させない */
		cd->have[i].state.existFlag=FALSE;
	}
}
void decidePlayer(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	int i;
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
		case ' ':
			od->now.motion=JUMP;
			break;
		case 'a':
			od->state.motionCount=0;
			if(od->now.motion!=N_ATTACK1)
				od->now.motion=N_ATTACK1;
			else
				od->now.motion=N_ATTACK2;
			break;
		case 'A':
			if(od->now.motion!=S_ATTACK){
				od->state.motionCount=0;
				od->now.motion=S_ATTACK;
			}
			break;
		case 'B':
			od->state.HP=0;
			break;
		case 'C':
			od->state.charge=od->state.maxCharge;
			break;
		case 'E':
			for(i=0;i<MAX_ENEMY;i++){
				if(ed[i].mine.state.existFlag==FALSE){
					initEnemyPara(&ed[i].mine,0,NO_ACTION,RIGHT);ed[i].mine.now.y=ed[i].mine.accept.y=od->now.y;ed[i].mine.now.x=ed[i].mine.accept.x=(od->now.x+40<STAGE_COLS-OBJECT_COLS)?od->now.x+40:STAGE_COLS-OBJECT_COLS;
					break;
				}
			}
			break;
		case 'I':
			for(i=0;i<MAX_ITEM;i++){
				if(id[i].state.existFlag==FALSE){
					initItemPara(&id[i],1,NO_ACTION,RIGHT);id[i].now.y=id[i].accept.y=(od->now.y+40>=1)?od->now.y+40:1;id[i].now.x=id[i].accept.x=(od->now.x+40<STAGE_COLS-OBJECT_COLS)?od->now.x+40:STAGE_COLS-OBJECT_COLS;
					break;
				}
			}
			break;
		case 'S':
			od->state.stamina=od->state.maxStamina;
			break;
		case KEY_DOWN:
			if(od->state.fallFlag==TRUE&&od->state.fallTime<getParabolaTopTime(od->state.v0))
				od->state.fallTime+=(getParabolaTopTime(od->state.v0)-od->state.fallTime)*2;
			break;
		default:
			if(od->state.fallFlag==FALSE)
				od->state.vX=0;
			break;
	}
}
void decideEnemy(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	od->now.motion=NO_ACTION;
}
void decideItem(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	od->now.motion=NO_ACTION;
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
					(getColorInfo(od->state.look[y][x])<64)&&
					/* 表示優先度が下の絵以上の時に出力 */
					(getPriorityInfo(od->state.look[y][x])>getPriorityInfo(sd->look[od->now.y+y][od->now.x+x].stage))
				){
					od->state.back[y][x]=1;
					sd->look[od->now.y+y][od->now.x+x].stage=od->state.look[y][x];
					if(getHitInfo(od->state.back[y][x])==1)
						sd->look[od->now.y+y][od->now.x+x].damage=od->state.ATK;
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
					(getColorInfo(od->state.look[y][x])<64)&&
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
			/* 内部的に透明でなければ描画(64以上は透過) */
			if(getColorInfo(sd->look[y][x].stage)<64){
				attron(COLOR_PAIR(getColorInfo(sd->look[y][x].stage)));
				for(z=0;z<2;z++)
					mvaddch(y-sd->displayY,(x-sd->displayX)*2+z,getGradationInfo(sd->look[y][x].stage));
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
}
void confirmExistObject(SD* sd,OD* od,OD* pd,int label,int ope){
	if(
		/* operandがTRUEで */
		(ope==TRUE)&&
		(
			/* 底に到達した時または */
			(od->now.y+od->now.crashEndY+2==STAGE_LINES)||
			/* 死亡状態でNO_ACTIONになった時に存在を削除 */
			(od->state.deathFlag==TRUE&&od->now.motion==NO_ACTION)
		)
	)
		od->state.existFlag=FALSE;
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
					mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-47)/2+x)*2+z,getGradationInfo(gameoverTelop[y][x]));
			}
		}
	}
	outputStatus(pd,0);
	refresh();
}
void acceptObject(OD* od){
	od->accept.y=od->now.y;
	od->accept.x=od->now.x;
	od->accept.variety=od->now.variety;
	od->accept.number=od->now.number;
	od->accept.direction=od->now.direction;
	od->accept.existStartY=od->now.existStartY;
	od->accept.existEndX=od->now.existEndX;
	od->accept.existStartY=od->now.existStartY;
	od->accept.crashEndX=od->now.crashEndX;
	od->accept.wallUpLine=od->now.wallUpLine;
	od->accept.wallDownLine=od->now.wallDownLine;
	od->accept.wallLeftLine=od->now.wallLeftLine;
	od->accept.wallRightLine=od->now.wallRightLine;
	od->accept.wallUpBaseX=od->now.wallUpBaseX;
	od->accept.wallDownBaseX=od->now.wallDownBaseX;
	od->accept.wallLeftBaseY=od->now.wallLeftBaseY;
	od->accept.wallRightBaseY=od->now.wallRightBaseY;
	od->accept.motion=od->now.motion;
}
void objectFlow(OD* od,SD* sd,CD* pd,CD* ed,OD* id,int label){
	if(
		/* オブジェクトが存在して */
		(od->state.existFlag==TRUE)&&
		/* プレイヤーとの距離が表示域の対角距離以下の時に処理する */
		(
			getSqrtDistant(
				pd->mine.now.y+OBJECT_LINES/2,
				pd->mine.now.x+OBJECT_COLS/2,
				od->now.y+OBJECT_LINES/2,
				od->now.x+OBJECT_COLS/2
			)<=
			getSqrtDistant(
				0,
				0,
				DISPLAY_LINES*0.75,
				DISPLAY_COLS*0.75
			)
		)
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
			od->now.x+=od->state.vX;
			od->now.y+=getGravityMove(od->state.v0,od->state.vA,od->state.fallTime);
		}
		/* オブジェクトの自由落下運動修正 */
		modifyCrashObject(sd,od,label);
		/* オブジェクトの受理 */
		acceptObject(od);
		/* オブジェクトの存在判定 */
		confirmExistObject(sd,od,&pd->mine,label,TRUE);
		/* オブジェクトの反映 */
		if(od->state.existFlag==TRUE)
			outputObject(sd,od);
	}
}
void objectDamage(OD* od,SD* sd,CD* pd,int label){
	if(
		(od->state.existFlag==TRUE)&&
		(
			getSqrtDistant(
				pd->mine.now.y+OBJECT_LINES/2,
				pd->mine.now.x+OBJECT_COLS/2,
				od->now.y+OBJECT_LINES/2,
				od->now.x+OBJECT_COLS/2
			)<=
			getSqrtDistant(
				0,
				0,
				DISPLAY_LINES*0.75,
				DISPLAY_COLS*0.75
			)
		)
	){
		if(confirmDamageObject(sd,od)==TRUE){
			if((od->state.HP-=(pd->mine.now.motion==S_ATTACK)?DAMAGE_VALUE*2:DAMAGE_VALUE)<0)
				od->state.HP=0;
		}
		confirmExistObject(sd,od,&pd->mine,label,FALSE);
	}
}
void main(void){
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
	/* オブジェクトチップの初期情報の設定 */
	initPlayerInfo();
	initEnemyInfo();
	initItemInfo();
	int i,j;
	/* タイトル描画 */
	openning();
	/***** メインフロー *****/
	while(restLife>0){
		/* ステージ構成をリセット */
		changeStage(&stage,&player,enemy,item,stageNumber);
		/* プレイヤーの存在している間 */
		while(player.mine.state.existFlag==TRUE){
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
			/***** 出力処理 *****/
			erase();
			scrollDisplay(&stage,&player.mine);
			outputStage(&stage);
			if(player.mine.state.deathFlag==TRUE)
				player.mine.state.charge=0;
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
	timeout(-1);
	getch();
	endwin();
}

