#include <ncurses.h>
#include <math.h>
#include "define.h"
#include "etc.h"
#include "playerInfo.h"
#include "enemyInfo.h"
#include "itemInfo.h"

char confirmDamageObject(SD*,OD*);
char confirmCrashObject(SD*,OD*,int*,int*);
char reviseObject(OD*,SD*);
void modifyObject(OD*,SD*,int);
void getWallLineObject(SD*,OD*);
void modifyCrashObject(SD*,OD*,int);

char confirmDamageObject(SD* sd,OD* od){
	int y,x;
	for(y=od->now.existStartY;y<=od->now.existEndY;y++){
		for(x=(y==od->now.existStartY)?od->now.existStartX:0;x<OBJECT_COLS;x++){
			if(getHitInfo(od->state.look[y][x])==2&&getHitInfo(sd->look[od->now.y+y][od->now.x+x].stage)==1)
				return TRUE;
		}
	}
	return FALSE;
}
char confirmCrashObject(SD* sd,OD* od,int* crashY,int* crashX){
	int y,x;
	for(y=od->now.existStartY;y<=od->now.existEndY;y++){
		for(x=(y==od->now.existStartY)?od->now.existStartX:0;x<OBJECT_COLS;x++){
			if(getColorInfo(od->state.look[y][x])<64){
				if( !(od->now.y+y>=0&&od->now.y+y<STAGE_LINES&&od->now.x+x>=0&&od->now.x+x<STAGE_COLS)||
					(getCrashInfo(od->state.look[y][x])!=0&&getCrashInfo(sd->look[od->now.y+y][od->now.x+x].stage)!=0)
				){
					*crashY=y;
					*crashX=x;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
char reviseObject(OD* od,SD* sd){
	int tmp;
	int count;
	int cY,cX;
	tmp=od->now.x;
	count=0;
	/* 横方向に補正 */
	while(confirmCrashObject(sd,od,&cY,&cX)==TRUE){
		if(od->now.x+cX>od->accept.x+OBJECT_COLS/2)
			od->now.x--;
		else if(od->now.x+cX<od->accept.x+OBJECT_COLS/2)
			od->now.x++;
		count++;
		if(count>OBJECT_COLS/4){
			od->now.x=tmp;
			count=0;
			break;
		}
	}
	tmp=od->now.y;
	/* 縦方向に補正 */
	while(confirmCrashObject(sd,od,&cY,&cX)==TRUE){
		if(od->now.y+cY>od->accept.y+OBJECT_LINES/2)
			od->now.y--;
		else if(od->now.y+cY<od->accept.y+OBJECT_LINES/2)
			od->now.y++;
		count++;
		if(count>OBJECT_LINES/4){
			od->now.y=tmp;
			count=0;
			break;
		}
	}
	if(confirmCrashObject(sd,od,&cY,&cX)==TRUE)
		return FALSE;
	else
		return TRUE;
}
void modifyObject(OD* od,SD* sd,int label){
	if(reviseObject(od,sd)==FALSE){
		if(label==PLAYER)
			createPlayer(od,od->accept.motion,od->now.direction);
		else if(label==ENEMY)
			createEnemy(od,od->now.variety,od->accept.motion,od->now.direction);
		else if(label==ITEM)
			createItem(od,od->now.variety,od->accept.motion,od->now.direction);
		if(reviseObject(od,sd)==FALSE){
			od->now.y=od->accept.y;
			od->now.x=od->accept.x;
		}
		od->now.motion=NO_ACTION;
	}
}
void getWallLineObject(SD* sd,OD* od){
	int y,x;
	int distant;
	int absDistant;
	int minDistant;
	int minAbsDistant,maxAbsDistant;
	/* 下方向にチェック */
	minDistant=od->now.crashEndY;
	maxAbsDistant=od->now.y+OBJECT_LINES*1.5;
	y=od->now.crashEndY;
	for(x=od->now.crashStartX;x<=od->now.crashEndX;x++){
		for(distant=od->now.y+y;getCrashInfo(sd->look[distant][od->now.x+x].stage)==0&&distant<=od->now.y+OBJECT_LINES*1.5&&distant<STAGE_LINES;distant++);
		if(distant<=od->now.y+OBJECT_LINES*1.5&&distant<STAGE_LINES){
			absDistant=distant;
			distant-=(od->now.y+y);
			if(distant<=minDistant){
				if(distant==minDistant&&absDistant<maxAbsDistant)
					continue;
				minDistant=distant;
				maxAbsDistant=absDistant;
				od->now.wallDownBaseX=x;
			}
		}
	}
	od->now.wallDownLine=maxAbsDistant;
	/* 上方向にチェック */
	minDistant=od->now.crashStartY;
	minAbsDistant=od->now.y-OBJECT_LINES*0.5;
	y=od->now.crashStartY;
	for(x=od->now.crashStartX;x<=od->now.crashEndX;x++){
		for(distant=od->now.y+y;getCrashInfo(sd->look[distant][od->now.x+x].stage)==0&&distant>=od->now.y-OBJECT_LINES*0.5&&distant>=0;distant--);
		if(distant>=od->now.y-OBJECT_LINES*0.5&&distant>=0){
			absDistant=distant;
			distant=od->now.y+y-distant;
			if(distant<=minDistant){
				if(distant==minDistant&&absDistant>minAbsDistant)
					continue;
				minDistant=distant;
				minAbsDistant=absDistant;
				od->now.wallUpBaseX=x;
			}
		}
	}
	od->now.wallUpLine=minAbsDistant;
	/* 右方向にチェック */
	minDistant=od->now.crashEndX;
	maxAbsDistant=od->now.x+OBJECT_COLS*1.5;
	x=od->now.crashEndX;
	for(y=od->now.crashStartY;y<=od->now.crashEndY;y++){
		for(distant=od->now.x+x;getCrashInfo(sd->look[od->now.y+y][distant].stage)==0&&distant<=od->now.x+OBJECT_COLS*1.5&&distant<STAGE_COLS;distant++);
		if(distant<=od->now.x+OBJECT_COLS*1.5&&distant<STAGE_COLS){
			absDistant=distant;
			distant-=(od->now.x+x);
			if(distant<=minDistant){
				if(distant==minDistant&&absDistant<maxAbsDistant)
					continue;
				minDistant=distant;
				maxAbsDistant=absDistant;
				od->now.wallRightBaseY=y;
			}
		}
	}
	od->now.wallRightLine=maxAbsDistant;
	/* 左方向にチェック */
	minDistant=od->now.crashStartX;
	minAbsDistant=od->now.x-OBJECT_COLS*0.5;
	x=od->now.crashStartX;
	for(y=od->now.crashStartY;y<=od->now.crashStartY;y++){
		for(distant=od->now.x+x;getCrashInfo(sd->look[od->now.y+y][distant].stage)==0&&distant>=od->now.x-OBJECT_COLS*0.5&&distant>=0;distant--);
		if(distant>=od->now.x-OBJECT_COLS*0.5&&distant>=0){
			absDistant=distant;
			distant=od->now.x+x-distant;
			if(distant<=minDistant){
				if(distant==minDistant&&absDistant>minAbsDistant)
					continue;
				minDistant=distant;
				minAbsDistant=absDistant;
				od->now.wallLeftBaseY=y;
			}
		}
	}
	od->now.wallLeftLine=minAbsDistant;
}
void modifyCrashObject(SD* sd,OD* od,int label){
	int i,j;
	int y,x;
	int tmp;
	int count;
	od->state.wallHeadFlag=od->state.wallFootFlag=od->state.wallKickFlag=FALSE;
	/* 下方向に修正 */
	while(od->now.y+od->now.crashEndY>=od->now.wallDownLine||od->now.y+od->now.crashEndY>=STAGE_LINES)
		od->now.y--;
	if(od->now.y+od->now.crashEndY+1==od->now.wallDownLine){
		od->state.jumpCount=0;
		od->state.fallFlag=FALSE;
		od->state.wallFootFlag=TRUE;
		od->state.fallTime=0;
		od->state.v0=0;
		od->state.vA=0;
	}
	else{
		od->state.fallFlag=TRUE;
		if(od->state.v0==0){
			od->state.v0=od->state.i0;
			od->state.jumpCount=1;
			od->state.fallTime=getParabolaTopTime(od->state.v0)+ELAPSED_TIME*5;
		}
	}
	/* 上方向に修正 */
	while(od->now.y+od->now.crashStartY<=od->now.wallUpLine||od->now.y+od->now.crashStartY<0)
		od->now.y++;
	if(od->now.y+od->now.crashStartY-1==od->now.wallUpLine){
		od->state.wallHeadFlag=TRUE;
		if(od->state.fallTime<getParabolaTopTime(od->state.v0))
			od->state.fallTime+=(getParabolaTopTime(od->state.v0)-od->state.fallTime)*2;
	}
	/* 右方向に修正 */
	while(od->now.x+od->now.crashEndX>=od->now.wallRightLine||od->now.x+od->now.crashEndX>=STAGE_COLS)
		od->now.x--;
	if(od->now.x+od->now.crashEndX+1==od->now.wallRightLine&&od->state.vX!=0&&od->now.direction==RIGHT){
		/*if(od->now.motion==JUMP_DOWN)*/
		if(od->state.fallTime>getParabolaTopTime(od->state.v0)){
			od->state.wallKickFlag=TRUE;
			od->state.jumpCount=0;
		}
		if(od->state.vX>0){
			od->now.y--;
			od->now.x++;
			if(confirmCrashObject(sd,od,&y,&x)==TRUE){
				od->now.y++;
				od->now.x--;
			}
		}
	}
	/* 左方向に修正 */
	while(od->now.x+od->now.crashStartX<=od->now.wallLeftLine||od->now.x+od->now.crashStartX<0)
		od->now.x++;
	if(od->now.x+od->now.crashStartX-1==od->now.wallLeftLine&&od->state.vX!=0&&od->now.direction==LEFT){
		/*if(od->now.motion==JUMP_DOWN)*/
		if(od->state.fallTime>getParabolaTopTime(od->state.v0)){
			od->state.wallKickFlag=TRUE;
			od->state.jumpCount=0;
		}
		if(od->state.vX<0){
			od->now.y--;
			od->now.x--;
			if(confirmCrashObject(sd,od,&y,&x)==TRUE){
				od->now.y++;
				od->now.x++;
			}
		}
	}
	/* 斜め跳び時の壁すり抜け対策 */
	modifyObject(od,sd,label);
}

