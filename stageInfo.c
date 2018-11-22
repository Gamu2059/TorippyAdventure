#include "define.h"

int stageInfo[1][STAGE_LINES][STAGE_COLS];
int stageBackInfo[1][STAGE_LINES][STAGE_COLS];

void initializeStageInfo(void);
void createStage(SD*,SD*,int);

void initializeStageInfo(void){
	int number,y,x;
	/* ステージ情報の初期化 */
	for(number=0;number<1;number++){
		for(y=0;y<STAGE_LINES;y++){
			for(x=0;x<STAGE_COLS;x++)
				stageInfo[number][y][x]=99;
		}
	}
	/* ステージ背景情報の初期化 */
	for(number=0;number<1;number++){
		for(y=0;y<STAGE_LINES;y++){
			for(x=0;x<STAGE_COLS;x++)
				stageBackInfo[number][y][x]=99;
		}
	}
	/* ステージ情報の設定 */
	for(number=0;number<1;number++){
		for(y=0;y<STAGE_LINES;y++){
			for(x=0;x<STAGE_COLS;x++){
				switch(number){
					case 0:
						
						// ノーマル(白)
						if(
							// 床：ブロック
							(y>340-80 && x<250)||
							(y>310-80 && x>140&&x<250)||
							(y>280-80 && x>200&&x<250)||
							(y>320-80&&y<330-80 && x>=250&&x<450)||
							(y>280-80 && x>=450&&x<511)||
							(y>280-80 && x>=550&&x<630)||
							(y>170-80 && x>=649&&x<690)||
							(y>(x-690)+170-80 && x>=690&&x<720)||
							(y>300-80 && x>=720&&x<807)||
							(y>310-80 && x>=910&&x<960)||
							// 床：プレート
							(y==130-80 && x>=400&&x<520)||
							(y==161-80 && x>=822&&x<860)||
							(y==290-80 && x>=822&&x<860)||
							// 壁
							(y<250-80 && x>=782&&x<787)||
							(y>160-80 && x>=817&&x<822)||
							(y<260-80 && x>=900&&x<905)
						)stageInfo[number][y][x]=100077;
						
						//if(y>=STAGE_LINES-10)
						//	stageInfo[number][y][x]=100077;
						if(
							/* リミットウォール */
							(y==0 || y==STAGE_LINES-1 || x==0 || x==STAGE_COLS-1)
						)stageInfo[number][y][x]=100077;
						break;
					default:break;
				}
			}
		}
	}
	/* ステージ背景情報の設定 */
	for(number=0;number<1;number++){
		for(y=0;y<STAGE_LINES;y++){
			for(x=0;x<STAGE_COLS;x++){
				switch(number){
					case 0:
						stageBackInfo[number][y][x]=66;
						if(
							(x%50>=0&&x%50<10)
						)
							stageBackInfo[number][y][x]=310;
						break;
					default:break;
				}
			}
		}
	}
}
void createStage(SD* sd,SD* bd,int number){
	int y,x;
	for(y=0;y<STAGE_LINES;y++){
		for(x=0;x<STAGE_COLS;x++){
			sd->stage[y][x]=stageInfo[number][y][x];
			bd->stage[y][x]=stageBackInfo[number][y][x];
		}
	}
	switch(number){
		case 0:
			sd->maxDisplayY=SCROLL_LIMIT_LINES;
			sd->maxDisplayX=STAGE_COLS;
			bd->maxDisplayY=DISPLAY_LINES+(sd->maxDisplayY-DISPLAY_LINES)/2;
			bd->maxDisplayX=DISPLAY_COLS+(sd->maxDisplayX-DISPLAY_COLS)/2;
			break;
		default:break;
	}
	sd->minDisplayY=sd->minDisplayX=bd->minDisplayY=bd->minDisplayX=0;
	sd->number=bd->number=number;
}

