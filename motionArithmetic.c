#include <ncurses.h>
#include <math.h>
#include "define.h"
#include "etc.h"
#include "playerInfo.h"
#include "enemyInfo.h"
#include "itemInfo.h"

char confirmCrashCharacter(SD*,CD*);
char confirmCrashItem(SD*,ID*);
char reviseCharacter(CD*,SD*);
char reviseItem(ID*,SD*);
void modifyCharacter(CD*,SD*,char);
void modifyItem(ID*,SD*);
void getWallLineCharacter(SD*,CD*);
void getWallLineItem(SD*,ID*);
void modifyCrashCharacter(SD*,CD*);
void modifyCrashItem(SD*,ID*);

char confirmCrashCharacter(SD* sd,CD* cd){
	int y,x;
	for(y=0;y<CHARACTER_LINES;y++){
		for(x=0;x<CHARACTER_COLS;x++){
			if(getColorInfo(cd->look[y][x])<64){
				if(	!(cd->y+y>=0&&cd->y+y<STAGE_LINES&&cd->x+x>=0&&cd->x+x<STAGE_COLS)||
					(getCrashInfo(cd->look[y][x])!=0&&getCrashInfo(sd->stage[cd->y+y][cd->x+x])!=0)
				)
					return TRUE;
			}
		}
	}
	return FALSE;
}
char confirmCrashItem(SD* sd,ID* id){
	int y,x;
	for(y=0;y<ITEM_LINES;y++){
		for(x=0;x<ITEM_COLS;x++){
			if(getCrashInfo(id->item[y][x])!=0&&getCrashInfo(sd->stage[id->y+y][id->x+x])!=0)
				return TRUE;
		}
	}
	return FALSE;
}
char reviseCharacter(CD* cd,SD* sd){
	int tmp;
	int count,maxCount;
	tmp=cd->x;
	count=0;
	maxCount=(cd->turnFlag==TRUE)?CHARACTER_COLS/4:abs(cd->acceptX-tmp);
	// 逆方向に補正を試みる
	while(confirmCrashCharacter(sd,cd)==TRUE){
		if(cd->direction==RIGHT){
			if(cd->wallKickFlag==TRUE)
				cd->x++;
			else
				cd->x--;
		}
		else if(cd->direction==LEFT){
			if(cd->wallKickFlag==TRUE)
				cd->x--;
			else
				cd->x++;
		}
		count++;
		if(count>maxCount){
			cd->x=tmp;
			count=0;
			break;
		}
	}
	// 順方向に補正を試みる
	while(confirmCrashCharacter(sd,cd)==TRUE){
		if(cd->direction==RIGHT){
			if(cd->wallKickFlag==TRUE)
				cd->x--;
			else
				cd->x++;
		}
		else if(cd->direction==LEFT){
			if(cd->wallKickFlag==TRUE)
				cd->x++;
			else
				cd->x--;
		}
		count++;
		if(count>maxCount){
			cd->x=tmp;
			count=0;
			break;
		}
	}
	tmp=cd->y;
	maxCount=(cd->turnFlag==TRUE)?CHARACTER_LINES/4:abs(cd->acceptY-tmp);
	// 逆方向に補正を試みる
	while(confirmCrashCharacter(sd,cd)==TRUE){
		if(getGravityMove(cd->v0,cd->vA,cd->fallTime)>0){
			if(cd->wallHeadFlag==TRUE)
				cd->y++;
			else
				cd->y--;
		}
		else{
			if(cd->wallHeadFlag==TRUE)
				cd->y--;
			else
				cd->y++;
		}
		count++;
		if(count>maxCount){
			cd->y=tmp;
			count=0;
			break;
		}
	}
	// 順方向に補正を試みる
	while(confirmCrashCharacter(sd,cd)==TRUE){
		if(getGravityMove(cd->v0,cd->vA,cd->fallTime)>0){
			if(cd->wallHeadFlag==TRUE)
				cd->y--;
			else
				cd->y++;
		}
		else{
			if(cd->wallHeadFlag==TRUE)
				cd->y++;
			else
				cd->y--;
		}
		count++;
		if(count>maxCount){
			cd->y=tmp;
			count=0;
			break;
		}
	}
	if(confirmCrashCharacter(sd,cd)==TRUE)
		return FALSE;
	else
		return TRUE;
}
char reviseItem(ID* id,SD* sd){
	int tmp;
	int count;
	tmp=id->x;
	count=0;
	/* 順方向に補正を試みる */
	while(confirmCrashItem(sd,id)==TRUE){
		if(id->direction==RIGHT)
			id->x++;
		else if(id->direction==LEFT)
			id->x--;
		count++;
		if(count>ITEM_COLS/2){
			id->x=tmp;
			count=0;
			break;
		}
	}
	/* 逆方向に補正を試みる */
	while(confirmCrashItem(sd,id)==TRUE){
		if(id->direction==RIGHT)
			id->x--;
		else if(id->direction==LEFT)
			id->x++;
		count++;
		if(count>ITEM_COLS/2){
			id->x=tmp;
			count=0;
			break;
		}
	}
	tmp=id->y;
	/* 順方向に補正を試みる */
	while(confirmCrashItem(sd,id)==TRUE){
		if(getGravityMove(id->v0,id->vA,id->fallTime)>0)
			id->y++;
		else
			id->y--;
		count++;
		if(count>ITEM_LINES/2){
			id->y=tmp;
			count=0;
			break;
		}
	}
	/* 逆方向に補正を試みる */
	while(confirmCrashItem(sd,id)==TRUE){
		if(getGravityMove(id->v0,id->vA,id->fallTime)>0)
			id->y--;
		else
			id->y++;
		count++;
		if(count>ITEM_LINES/2){
			id->y=tmp;
			count=0;
			break;
		}
	}
	if(confirmCrashItem(sd,id)==TRUE)
		return FALSE;
	else
		return TRUE;
}
void modifyCharacter(CD* cd,SD* sd,char operand){
	if(reviseCharacter(cd,sd)==FALSE){
		if(operand==TRUE)
			createPlayer(cd,getPlayerMoveNumber(cd,NO_ACTION),cd->direction);
		else
			createEnemy(cd,cd->variety,getEnemyMoveNumber(cd,cd->variety,NO_ACTION),cd->direction);
		if(reviseCharacter(cd,sd)==FALSE){
			if(operand==TRUE)
				createPlayer(cd,cd->motionAccept,cd->direction);
			else
				createEnemy(cd,cd->variety,cd->motionAccept,cd->direction);
			if(reviseCharacter(cd,sd)==FALSE){
				cd->y=cd->acceptY;
				cd->x=cd->acceptX;
			}
		}
		// 行動の強制受理
		cd->motionAccept=cd->number;
		cd->motionFlag=NO_ACTION;
	}
	else
		// 行動の受理
		cd->motionAccept=cd->number;
}
void modifyItem(ID* id,SD* sd){
	if(reviseItem(id,sd)==FALSE){
		createItem(id,id->variety,getItemMoveNumber(id,id->variety,NO_ACTION),id->direction);
		if(reviseItem(id,sd)==FALSE)
			createItem(id,id->variety,id->motionAccept,id->direction);	
		/* 行動の強制受理 */
		id->motionAccept=id->number;
		id->motionFlag=NO_ACTION;
	}
	else
		/* 行動の受理 */
		id->motionAccept=id->number;
}
void getWallLineCharacter(SD* sd,CD* cd){
	int y,x;
	int distant;
	int absDistant;
	int minDistant;
	int minAbsDistant,maxAbsDistant;
	
	/* 下方向にチェック */
	minDistant=STAGE_LINES-1;
	maxAbsDistant=0;
	for(x=0;x<CHARACTER_LINES;x++){
		for(y=0;y<CHARACTER_COLS;y++){
			if(getCrashInfo(cd->look[y][x])!=0){
				for(distant=cd->y+y;getCrashInfo(sd->stage[distant][cd->x+x])==0&&distant<STAGE_LINES;distant++);
				if(distant<STAGE_LINES){
					absDistant=distant;
					distant-=(cd->y+y);
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant<maxAbsDistant)
							continue;
						minDistant=distant;
						maxAbsDistant=absDistant;
						cd->downWallY=y;
						cd->downWallX=x;
					}
				}
			}
		}
	}
	cd->downWallLine=maxAbsDistant;
	
	/* 上方向にチェック */
	minDistant=STAGE_LINES-1;
	minAbsDistant=STAGE_LINES-1;
	for(x=0;x<CHARACTER_LINES;x++){
		for(y=CHARACTER_COLS-1;y>=0;y--){
			if(getCrashInfo(cd->look[y][x])!=0){
				for(distant=cd->y+y;getCrashInfo(sd->stage[distant][cd->x+x])==0&&distant>=0;distant--);
				if(distant>=0){
					absDistant=distant;
					distant=cd->y+y-distant;
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant>minAbsDistant)
							continue;
						minDistant=distant;
						minAbsDistant=absDistant;
						cd->upWallY=y;
						cd->upWallX=x;
					}
				}
			}
		}
	}
	cd->upWallLine=minAbsDistant;
	
	/* 右方向にチェック */
	minDistant=STAGE_COLS-1;
	maxAbsDistant=0;
	for(y=0;y<CHARACTER_LINES;y++){
		for(x=0;x<CHARACTER_COLS;x++){
			if(getCrashInfo(cd->look[y][x])!=0){
				for(distant=cd->x+x;getCrashInfo(sd->stage[cd->y+y][distant])==0&&distant<STAGE_COLS;distant++);
				if(distant<STAGE_COLS){
					absDistant=distant;
					distant-=(cd->x+x);
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant<maxAbsDistant)
							continue;
						minDistant=distant;
						maxAbsDistant=absDistant;
						cd->rightWallY=y;
						cd->rightWallX=x;
					}
				}
			}
		}
	}
	cd->rightWallLine=maxAbsDistant;
	
	/* 左方向にチェック */
	minDistant=STAGE_COLS-1;
	minAbsDistant=STAGE_COLS-1;
	for(y=0;y<CHARACTER_LINES;y++){
		for(x=CHARACTER_COLS-1;x>=0;x--){
			if(getCrashInfo(cd->look[y][x])!=0){
				for(distant=cd->x+x;getCrashInfo(sd->stage[cd->y+y][distant])==0&&distant>=0;distant--);
				if(distant>=0){
					absDistant=distant;
					distant=cd->x+x-distant;
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant>minAbsDistant)
							continue;
						minDistant=distant;
						minAbsDistant=absDistant;
						cd->leftWallY=y;
						cd->leftWallX=x;
					}
				}
			}
		}
	}
	cd->leftWallLine=minAbsDistant;
}
void getWallLineItem(SD* sd,ID* id){
	int y,x;
	int distant;
	int absDistant;
	int minDistant;
	int minAbsDistant,maxAbsDistant;
	
	/* 下方向にチェック */
	minDistant=STAGE_LINES-1;
	maxAbsDistant=0;
	for(x=0;x<ITEM_LINES;x++){
		for(y=0;y<ITEM_COLS;y++){
			if(getCrashInfo(id->item[y][x])!=0){
				for(distant=id->y+y;getCrashInfo(sd->stage[distant][id->x+x])==0&&distant<STAGE_LINES;distant++);
				if(distant<STAGE_LINES){
					absDistant=distant;
					distant-=(id->y+y);
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant<maxAbsDistant)
							continue;
						minDistant=distant;
						maxAbsDistant=absDistant;
						id->downWallY=y;
						id->downWallX=x;
					}
				}
			}
		}
	}
	id->downWallLine=maxAbsDistant;
	
	/* 上方向にチェック */
	minDistant=STAGE_LINES-1;
	minAbsDistant=STAGE_LINES-1;
	for(x=0;x<ITEM_LINES;x++){
		for(y=ITEM_COLS-1;y>=0;y--){
			if(getCrashInfo(id->item[y][x])!=0){
				for(distant=id->y+y;getCrashInfo(sd->stage[distant][id->x+x])==0&&distant>=0;distant--);
				if(distant>=0){
					absDistant=distant;
					distant=id->y+y-distant;
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant>minAbsDistant)
							continue;
						minDistant=distant;
						minAbsDistant=absDistant;
						id->upWallY=y;
						id->upWallX=x;
					}
				}
			}
		}
	}
	id->upWallLine=minAbsDistant;
	
	/* 右方向にチェック */
	minDistant=STAGE_COLS-1;
	maxAbsDistant=0;
	for(y=0;y<ITEM_LINES;y++){
		for(x=0;x<ITEM_COLS;x++){
			if(getCrashInfo(id->item[y][x])!=0){
				for(distant=id->x+x;getCrashInfo(sd->stage[id->y+y][distant])==0&&distant<STAGE_COLS;distant++);
				if(distant<STAGE_COLS){
					absDistant=distant;
					distant-=(id->x+x);
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant<maxAbsDistant)
							continue;
						minDistant=distant;
						maxAbsDistant=absDistant;
						id->rightWallY=y;
						id->rightWallX=x;
					}
				}
			}
		}
	}
	id->rightWallLine=maxAbsDistant;
	
	/* 左方向にチェック */
	minDistant=STAGE_COLS-1;
	minAbsDistant=STAGE_COLS-1;
	for(y=0;y<ITEM_LINES;y++){
		for(x=ITEM_COLS-1;x>=0;x--){
			if(getCrashInfo(id->item[y][x])!=0){
				for(distant=id->x+x;getCrashInfo(sd->stage[id->y+y][distant])==0&&distant>=0;distant--);
				if(distant>=0){
					absDistant=distant;
					distant=id->x+x-distant;
					if(distant<=minDistant){
						if(distant==minDistant&&absDistant>minAbsDistant)
							continue;
						minDistant=distant;
						minAbsDistant=absDistant;
						id->leftWallY=y;
						id->leftWallX=x;
					}
				}
			}
		}
	}
	id->leftWallLine=minAbsDistant;
}
void modifyCrashCharacter(SD* sd,CD* cd){
	int i,j;
	int tmp;
	int count;
	int center;
	cd->wallKickFlag=cd->wallHeadFlag=FALSE;
	/* 下方向に修正 */
	while(cd->y+cd->downWallY>=cd->downWallLine||cd->y+cd->downWallY>=STAGE_LINES)
		cd->y--;
	if(cd->y+cd->downWallY+1==cd->downWallLine){
		cd->jumpCount=0;
		cd->fallFlag=FALSE;
		cd->wallHeadFlag=TRUE;
		cd->fallTime=0;
		cd->v0=0;
		cd->vA=0;
	}
	else{
		cd->fallFlag=TRUE;
		if(cd->v0==0){
			cd->v0=cd->a0;
			cd->jumpCount=1;
			cd->fallTime=getParabolaTopTime(cd->v0)+ELAPSED_TIME*5;
		}
	}
	
	/* 上方向に修正 */
	while(cd->y+cd->upWallY<=cd->upWallLine||cd->y+cd->upWallY<0)
		cd->y++;
	if(cd->y+cd->upWallY-1==cd->upWallLine){
		cd->wallHeadFlag=TRUE;
		if(cd->fallTime<getParabolaTopTime(cd->v0))
			cd->fallTime+=(getParabolaTopTime(cd->v0)-cd->fallTime)*2;
	}
	
	/* 右方向に修正 */
	while(cd->x+cd->rightWallX>=cd->rightWallLine||cd->x+cd->rightWallX>=STAGE_COLS)
		cd->x--;
	if(cd->x+cd->rightWallX+1==cd->rightWallLine&&cd->vX!=0&&cd->direction==RIGHT){
		if(cd->fallTime>=getParabolaTopTime(cd->v0))
			cd->wallKickFlag=TRUE;
		cd->jumpCount=0;
		if(cd->vX>0){
			cd->y--;
			cd->x++;
			if(confirmCrashCharacter(sd,cd)==TRUE){
				cd->y++;
				cd->x--;
			}
		}
	}
	
	/* 左方向に修正 */
	while(cd->x+cd->leftWallX<=cd->leftWallLine||cd->x+cd->leftWallX<0)
		cd->x++;
	if(cd->x+cd->leftWallX-1==cd->leftWallLine&&cd->vX!=0&&cd->direction==LEFT){
		if(cd->fallTime>=getParabolaTopTime(cd->v0))
			cd->wallKickFlag=TRUE;
		cd->jumpCount=0;
		if(cd->vX<0){
			cd->y--;
			cd->x--;
			if(confirmCrashCharacter(sd,cd)==TRUE){
				cd->y++;
				cd->x++;
			}
		}
	}
	
	/* 斜め跳び時の壁すり抜け対策 */
	tmp=cd->x;
	count=0;
	while(confirmCrashCharacter(sd,cd)==TRUE){
		if(cd->vX<0)
			cd->x++;
		else if(cd->vX>0)
			cd->x--;
		count++;
		if(count>cd->aX){
			cd->x=tmp;
			break;
		}
	}
	
	/* 足以外の場所で着地した時の滑り処理 */
	for(i=0,center=-1;i<CHARACTER_LINES;i++){
		for(j=0;j<CHARACTER_COLS;j++){
			if(getCrashInfo(cd->look[i][j])==3){
				center=j;
				break;
			}
		}
		if(center!=-1)
		break;
	}
	if(getCrashInfo(cd->look[cd->downWallY][cd->downWallX])==2&&cd->y+cd->downWallY+1==cd->downWallLine){
		tmp=cd->x;
		if(cd->downWallX-center<0)
			cd->x++;
		else if(cd->downWallX-center>0)
			cd->x--;
		if(confirmCrashCharacter(sd,cd)==TRUE)
			cd->x=tmp;
	}
}
void modifyCrashItem(SD* sd,ID* id){
	int i,j;
	int tmp;
	int count;
	int center;
	id->wallKickFlag=id->wallHeadFlag=FALSE;
	/* 下方向に修正 */
	while(id->y+id->downWallY>=id->downWallLine||id->y+id->downWallY>=STAGE_LINES)
		id->y--;
	if(id->y+id->downWallY+1==id->downWallLine){
		id->fallFlag=FALSE;
		id->fallTime=0;
		id->v0=0;
		id->vA=0;
	}
	else{
		id->fallFlag=TRUE;
		if(id->v0==0){
			id->v0=id->a0;
			id->fallTime=getParabolaTopTime(id->v0)+ELAPSED_TIME;
		}
	}
	
	/* 上方向に修正 */
	while(id->y+id->upWallY<=id->upWallLine||id->y+id->upWallY<0)
		id->y++;
	if(id->y+id->upWallY-1==id->upWallLine){
		id->wallHeadFlag=TRUE;
		if(id->fallTime<getParabolaTopTime(id->v0))
			id->fallTime+=(getParabolaTopTime(id->v0)-id->fallTime)*2;
	}
	
	/* 右方向に修正 */
	while(id->x+id->rightWallX>=id->rightWallLine||id->x+id->rightWallX>=STAGE_COLS)
		id->x--;
	if(id->x+id->rightWallX+1==id->rightWallLine&&id->vX!=0&&id->direction==RIGHT){
		if(id->fallTime>=getParabolaTopTime(id->v0))
			id->wallKickFlag=TRUE;
		if(id->vX>0){
			id->y--;
			id->x++;
			if(confirmCrashItem(sd,id)==TRUE){
				id->y++;
				id->x--;
			}
		}
	}
	
	/* 左方向に修正 */
	while(id->x+id->leftWallX<=id->leftWallLine||id->x+id->leftWallX<0)
		id->x++;
	if(id->x+id->leftWallX-1==id->leftWallLine&&id->vX!=0&&id->direction==LEFT){
		if(id->fallTime>=getParabolaTopTime(id->v0))
			id->wallKickFlag=TRUE;
		if(id->vX<0){
			id->y--;
			id->x--;
			if(confirmCrashItem(sd,id)==TRUE){
				id->y++;
				id->x++;
			}
		}
	}
	
	/* 斜め跳び時の壁すり抜け対策 */
	tmp=id->x;
	count=0;
	while(confirmCrashItem(sd,id)==TRUE){
		if(id->vX<0)
			id->x++;
		else if(id->vX>0)
			id->x--;
		count++;
		if(count>id->aX){
			id->x=tmp;
			break;
		}
	}
	
	/* 足以外の場所で着地した時の滑り処理 */
	for(i=0,center=-1;i<ITEM_LINES;i++){
		for(j=0;j<ITEM_COLS;j++){
			if(getCrashInfo(id->item[i][j])==3){
				center=j;
				break;
			}
		}
		if(center!=-1)
			break;
	}
	if(getCrashInfo(id->item[id->downWallY][id->downWallX])==2&&id->y+id->downWallY+1==id->downWallLine){
		tmp=id->x;
		if(id->downWallX-center<0)
			id->x++;
		else if(id->downWallX-center>0)
			id->x--;
		if(confirmCrashItem(sd,id)==TRUE)
			id->x=tmp;
	}
}

