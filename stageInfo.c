#include "define.h"

int stageInfo[1][STAGE_LINES][STAGE_COLS];

void initializeStageInfo(void);
void createStage(SD* sd,int number);

void initializeStageInfo(void){
	int number,y,x;
	// ステージ情報の初期化
	for(number=0;number<1;number++){
		for(y=0;y<STAGE_LINES;y++){
			for(x=0;x<STAGE_COLS;x++)
				stageInfo[number][y][x]=0;
		}
	}
	for(number=0;number<1;number++){
		for(y=0;y<STAGE_LINES;y++){
			for(x=0;x<STAGE_COLS;x++){
				switch(number){
					case 0:
						// ノーマル(白)
						if(
							//床：ブロック
							(y>340 && x<250)||
							(y>310 && x>140&&x<250)||
							(y>280 && x>200&&x<250)||
							(y>320&&y<330 && x>=250&&x<450)||
							(y>280 && x>=450&&x<511)||
							(y>280 && x>=550&&x<630)||
							(y>170 && x>=649&&x<690)||
							(y>(x-690)+170 && x>=690&&x<720)||
							(y>300 && x>=720&&x<807)||
							(y>310 && x>=910&&x<960)||
							//床：プレート
							(y==130 && x>=400&&x<520)||
							(y==161 && x>=822&&x<860)||
							(y==290 && x>=822&&x<860)||
							//壁
							(y<250 && x>=782&&x<787)||
							(y>160 && x>=817&&x<822)||
							(y<260 && x>=900&&x<905)
						)stageInfo[number][y][x]=16;
						// ガイド(黄)
						if(
							(y>280&&y<285 && x>=470&&x<511)||
							(y>280&&y<285 && x>=560&&x<585)
						)stageInfo[number][y][x]=17;
						// アイテム(青)
						if(
							(y>=130&&y<135 && x>=410&&x<440)
						)stageInfo[number][y][x]=3;
						// 作成中(赤)
						if(
							(y>310 && x>=910&&x<960)
						)stageInfo[number][y][x]=12;
						if(stageInfo[number][y][x]==0&&y>100&&y<=400&&y>=-2*x)stageInfo[number][y][x]=3;
						// リミットウォール
						if(
							(y==0 || y==STAGE_LINES-1 || x==0 || x==STAGE_COLS-1)
						)stageInfo[number][y][x]=11;
						break;
					default:
						break;
				}
			}
		}
	}
	// ステージ情報のない部分の定義
	for(number=0;number<1;number++){
		for(y=0;y<STAGE_LINES;y++){
			for(x=0;x<STAGE_COLS;x++){
				if(stageInfo[number][y][x]==0)
					stageInfo[number][y][x]=8;
			}
		}
	}
}
void createStage(SD* sd,int number){
	int y,x;
	for(y=0;y<STAGE_LINES;y++){
		for(x=0;x<STAGE_COLS;x++)
			sd->stage[y][x]=stageInfo[number][y][x];
	}
}

