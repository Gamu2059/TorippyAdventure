#include <ncurses.h>
#include "define.h"
#include "etc.h"
#include "playerInfo.h"
#include "enemyInfo.h"

char confirmCrash(SD* sd,PD* pd);
void getWallLine(SD* sd,PD* pd);
void modifyCrash(SD* sd,PD* pd);
void jump(SD* sd,PD *pd,char operand);
void scrollDisplay(SD* sd,PD* pd);
void moveArithmetic(SD* sd,PD* pd,int direction,char operand);

char confirmCrash(SD* sd,PD* pd){
	int y,x;
	for(y=0;y<PLAYER_LINES;y++){
		for(x=0;x<PLAYER_COLS;x++){
			if(getCrashInfo(pd->look[y][x])!=0&&getCrashInfo(sd->stage[pd->y+y][pd->x+x])!=0)
				return TRUE;
		}
	}
	return FALSE;
}
void getWallLine(SD* sd,PD* pd){
	int y,x;
	int distant;		// プレイヤーから壁までの距離
	int absDistant;	// 壁の絶対距離
	int minDistant;
	int minAbsDistant,maxAbsDistant;
	// 下方向にチェック
	minDistant=STAGE_LINES-1;
	maxAbsDistant=0;
	for(x=0;x<PLAYER_LINES;x++){
		for(y=0;y<PLAYER_COLS;y++){
			if(getCrashInfo(pd->look[y][x])!=0){
				for(distant=pd->y+y;getCrashInfo(sd->stage[distant][pd->x+x])==0&&distant<STAGE_LINES;distant++);
				if(distant<STAGE_LINES){
					absDistant=distant;
					distant-=(pd->y+y);
					if(distant<=minDistant){
						if(distant==minDistant){
							if(absDistant<maxAbsDistant)
								continue;
						}
						minDistant=distant;
						maxAbsDistant=absDistant;
						pd->downWallY=y;
						pd->downWallX=x;
					}
				}
			}
		}
	}
	pd->downWallLine=maxAbsDistant;
	
	// 上方向にチェック
	minDistant=STAGE_LINES-1;
	minAbsDistant=STAGE_LINES-1;
	for(x=0;x<PLAYER_LINES;x++){
		for(y=PLAYER_COLS-1;y>=0;y--){
			if(getCrashInfo(pd->look[y][x])!=0){
				for(distant=pd->y+y;getCrashInfo(sd->stage[distant][pd->x+x])==0&&distant>=0;distant--);
				if(distant>=0){
					absDistant=distant;
					distant=pd->y+y-distant;
					if(distant<=minDistant){
						if(distant==minDistant){
							if(absDistant>minAbsDistant)
								continue;
						}
						minDistant=distant;
						minAbsDistant=absDistant;
						pd->upWallY=y;
						pd->upWallX=x;
					}
				}
			}
		}
	}
	pd->upWallLine=minAbsDistant;

	// 右方向にチェック
	minDistant=STAGE_COLS-1;
	maxAbsDistant=0;
	for(y=0;y<PLAYER_LINES;y++){
		for(x=0;x<PLAYER_COLS;x++){
			if(getCrashInfo(pd->look[y][x])!=0){
				for(distant=pd->x+x;getCrashInfo(sd->stage[pd->y+y][distant])==0&&distant<STAGE_COLS;distant++);
				if(distant<STAGE_COLS){
					absDistant=distant;
					distant-=(pd->x+x);
					if(distant<=minDistant){
						if(distant==minDistant){
							if(absDistant<maxAbsDistant)
								continue;
						}
						minDistant=distant;
						maxAbsDistant=absDistant;
						pd->rightWallY=y;
						pd->rightWallX=x;
					}
				}
			}
		}
	}
	pd->rightWallLine=maxAbsDistant;
	
	// 左方向にチェック
	minDistant=STAGE_COLS-1;
	minAbsDistant=STAGE_COLS-1;
	for(y=0;y<PLAYER_LINES;y++){
		for(x=PLAYER_COLS-1;x>=0;x--){
			if(getCrashInfo(pd->look[y][x])!=0){
				for(distant=pd->x+x;getCrashInfo(sd->stage[pd->y+y][distant])==0&&distant>=0;distant--);
				if(distant>=0){
					absDistant=distant;
					distant=pd->x+x-distant;
					if(distant<=minDistant){
						if(distant==minDistant){
							if(absDistant>minAbsDistant)
								continue;
						}
						minDistant=distant;
						minAbsDistant=absDistant;
						pd->leftWallY=y;
						pd->leftWallX=x;
					}
				}
			}
		}
	}
	pd->leftWallLine=minAbsDistant;
}
void modifyCrash(SD* sd,PD* pd){
	int tmp;
	int count;
	pd->wallKickFlag=FALSE;
	// 下方向に修正
	while(pd->y+pd->downWallY>=pd->downWallLine||pd->y+pd->downWallY>=STAGE_LINES)
		pd->y--;
	if(pd->y+pd->downWallY+1==pd->downWallLine){	// 床に接触している状態
		pd->jumpCount=0;
		pd->fallFlag=FALSE;
		pd->fallTime=0;
		pd->v0=0;
		pd->vA=0;
	}
	else{
		pd->fallFlag=TRUE;
		if(pd->v0==0){
			pd->v0=-V0;
			pd->jumpCount=1;
			pd->fallTime=ELAPSED_TIME*10;
		}
	}
	// 上方向に修正
	while(pd->y+pd->upWallY<=pd->upWallLine||pd->y+pd->upWallY<0)
		pd->y++;
	if(pd->y+pd->upWallY-1==pd->upWallLine){		// 天井に接触している状態
		if(pd->fallTime<getParabolaTopTime(pd))
			pd->fallTime+=(getParabolaTopTime(pd)-pd->fallTime)*2;
	}
	// 右方向に修正
	while(pd->x+pd->rightWallX>=pd->rightWallLine||pd->x+pd->rightWallX>=STAGE_COLS)
		pd->x--;
	if(pd->x+pd->rightWallX+1==pd->rightWallLine&&pd->vX!=0&&pd->direction>0){	// 壁に接触している状態
		pd->wallKickFlag=TRUE;
		pd->jumpCount=0;
		if(pd->vX>0){
			pd->y--;
			pd->x++;
			if(confirmCrash(sd,pd)==TRUE){
				pd->y++;
				pd->x--;
			}
		}
	}
	// 左方向に修正
	while(pd->x+pd->leftWallX<=pd->leftWallLine||pd->x+pd->leftWallX<0)
		pd->x++;
	if(pd->x+pd->leftWallX-1==pd->leftWallLine&&pd->vX!=0&&pd->direction<0){	// 壁に接触している状態
		pd->wallKickFlag=TRUE;
		pd->jumpCount=0;
		if(pd->vX<0){
			pd->y--;
			pd->x--;
			if(confirmCrash(sd,pd)==TRUE){
				pd->y++;
				pd->x++;
			}
		}
	}

	// 斜め跳び時の壁すり抜け対策
	tmp=pd->x;
	count=0;
	while(confirmCrash(sd,pd)==TRUE){
		if(pd->vX<0)
			pd->x++;
		else if(pd->vX>0)
			pd->x--;
		count++;
		if(count>PLAYER_COLS){
			pd->x=tmp;
			break;
		}
	}
	// 足以外の場所で着地した時の滑り処理
	if(getCrashInfo(pd->look[pd->downWallY][pd->downWallX])!=2&&pd->y+pd->downWallY+1==pd->downWallLine){
		tmp=pd->x;
		if(pd->downWallX-PLAYER_COLS/2<0)
			pd->x++;
		else
			pd->x--;
		if(confirmCrash(sd,pd)==TRUE)
			pd->x=tmp;
	}
}
void jump(SD* sd,PD *pd,char operand){
	int count;
	int tmpX;
	if(pd->jumpCount<2){
		pd->jumpCount++;
		pd->fallTime=0;
		pd->fallFlag=TRUE;
		pd->v0=V0;
		pd->vA=VA;
		if(pd->wallKickFlag==TRUE){
			pd->wallKickFlag=FALSE;
			pd->vX*=-1;
			if(pd->vX>0){
				if(operand==TRUE)
					createPlayer(pd,pd->number,1);
				else
					createEnemy(pd,pd->variety,pd->number,1);
			}
			else if(pd->vX<0){
				if(operand==TRUE)
					createPlayer(pd,pd->number,-1);
				else
					createEnemy(pd,pd->variety,pd->number,-1);
			}
			tmpX=pd->x;
			count=0;
			while(confirmCrash(sd,pd)==TRUE){
				if(pd->direction>0){
					if(pd->rightWallX-PLAYER_COLS/2>0)
						pd->x--;
					else
						pd->x++;
				}
				else if(pd->direction<0){
					if(pd->leftWallX-PLAYER_COLS/2>0)
						pd->x--;
					else
						pd->x++;
				}
				count++;
				if(count>PLAYER_COLS/2){
					pd->x=tmpX;
					getWallLine(sd,pd);
					break;
				}
			}
			if(confirmCrash(sd,pd)==TRUE){
				if(operand==TRUE)
					createPlayer(pd,pd->number,-1);
				else
					createEnemy(pd,pd->variety,pd->number,-1);
			}
			getWallLine(sd,pd);
		}
	}
}
void scrollDisplay(SD* sd,PD* pd){
	// 上方向にスクロール
	if(getGravityMove(pd)<0&&pd->y<=sd->displayY+SCROLL_LINES){
		sd->displayY+=getGravityMove(pd);
		if(sd->displayY<0)
			sd->displayY=0;
	}
	// 下方向にスクロール
	if(getGravityMove(pd)>0&&pd->y+PLAYER_LINES>=sd->displayY+DISPLAY_LINES-SCROLL_LINES){
		sd->displayY+=getGravityMove(pd);
		if(sd->displayY+DISPLAY_LINES>=SCROLL_LIMIT_LINES)
			sd->displayY=SCROLL_LIMIT_LINES-DISPLAY_LINES;
	}
	// 左方向にスクロール
	if(pd->x<=sd->displayX+SCROLL_COLS){
		sd->displayX+=-4;
		if(sd->displayX<0)
			sd->displayX=0;
	}
	// 右方向にスクロール
	if(pd->x+PLAYER_COLS>=sd->displayX+DISPLAY_COLS-SCROLL_COLS){
		sd->displayX+=4;
		if(sd->displayX+DISPLAY_COLS>=STAGE_COLS)
			sd->displayX=STAGE_COLS-DISPLAY_COLS;
	}
}
void moveArithmetic(SD* sd,PD* pd,int direction,char operand){
	int count;
	int tmpX;
	if(operand==TRUE)
		createPlayer(pd,pd->number,direction);
	else
		createEnemy(pd,pd->variety,pd->number,direction);
	tmpX=pd->x;
	count=0;
	while(confirmCrash(sd,pd)==TRUE){
		if(pd->direction>0){
			if(pd->rightWallX-PLAYER_COLS/2>0)
				pd->x--;
			else
				pd->x++;
		}
		else if(pd->direction<0){
			if(pd->leftWallX-PLAYER_COLS/2>0)
				pd->x--;
			else
				pd->x++;
		}
		count++;
		if(count>PLAYER_COLS/2){
			pd->x=tmpX;
			break;
		}
	}
	if(confirmCrash(sd,pd)==TRUE){
		if(operand==TRUE)
			createPlayer(pd,pd->number,-direction);
		else
			createEnemy(pd,pd->variety,pd->number,-direction);
	}
	getWallLine(sd,pd);
	if(pd->fallFlag==FALSE){
		if(direction>0){
			pd->x+=pd->aX;
			pd->vX+=(int)(pd->aX/4);
			if(pd->vX>(int)(pd->aX/4)*3)
				pd->vX=(int)(pd->aX/4)*3;
		}
		else{
			pd->x-=pd->aX;
			pd->vX-=(int)(pd->aX/4);
			if(pd->vX<-(int)(pd->aX/4)*3)
				pd->vX=-(int)(pd->aX/4)*3;
		}
	}
	else{
		if(pd->vX*direction<0)
			pd->vX*=-0.5;
	}
}

