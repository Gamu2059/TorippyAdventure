/***** ヘッダファイルのインクルード *****/
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include "playerInfo.h"
#include "enemyInfo.h"
#include "itemInfo.h"
#include "stageInfo.h"
#include "motionArithmetic.h"
#include "etc.h"
#include "openning.h"
#include "ending.h"
#include "autoPlay.h"
#include "record.h"
#include "define.h"

/***** プロトタイプ宣言 *****/
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
void outputBoss(OD*);
void outputNumber(SD*,CD*,int,int);
void scrollDisplay(SD*,OD*);
void confirmExistObject(SD*,OD*,OD*,int,int);
void gameover(SD*,CD*,CD*,OD*,int);
char confirmClear(void);
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
int ATKTmp=0,DEFTmp=0;
int SCORETmp=0;
int startIndex,goalIndex;
int restLife=5;
int stageNumber=0;
int eventTimer,maxEventTime=1000,event;
int achieveFlag=FALSE;
int pauseFlag=FALSE;
int autoFlag;
time_t start,finish;
char outPut=' ';

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
char eventTelop[5][29]=
{
{ 7, 7, 7, 7, 7,99, 7,99,99,99, 7,99, 7, 7, 7, 7, 7,99, 7,99,99,99, 7,99, 7, 7, 7, 7, 7},
{ 7,99,99,99,99,99, 7,99,99,99, 7,99, 7,99,99,99,99,99, 7, 7,99,99, 7,99,99,99, 7,99,99},
{ 7, 7, 7, 7,99,99,99, 7,99, 7,99,99, 7, 7, 7, 7,99,99, 7,99, 7,99, 7,99,99,99, 7,99,99},
{ 7,99,99,99,99,99,99, 7,99, 7,99,99, 7,99,99,99,99,99, 7,99,99, 7, 7,99,99,99, 7,99,99},
{ 7, 7, 7, 7, 7,99,99,99, 7,99,99,99, 7, 7, 7, 7, 7,99, 7,99,99,99, 7,99,99,99, 7,99,99}
};
char bossTelop[5][23]=
{
{ 1, 1, 1, 1,99,99,99, 1, 1, 1,99,99,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1},
{ 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99,99},
{ 1, 1, 1, 1,99,99, 1,99,99,99, 1,99,99, 1, 1, 1,99,99,99, 1, 1, 1,99},
{ 1,99,99,99, 1,99, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99,99,99, 1},
{ 1, 1, 1, 1,99,99,99, 1, 1, 1,99,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1,99}
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
char bestTelop[5][65]=
{
{ 1, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99,99, 1, 1, 1, 1,99, 1, 1, 1, 1, 1,99,99,99,99,99,99,99,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1,99,99, 1, 1, 1,99,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99,99,99, 1, 1,99},
{ 1,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99,99,99,99,99, 1,99,99,99,99,99,99,99,99,99, 1,99,99,99,99,99, 1,99,99,99,99,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99,99,99,99,99, 1, 1,99},
{ 1, 1, 1, 1,99,99, 1, 1, 1, 1,99,99,99, 1, 1, 1,99,99,99,99, 1,99,99,99,99,99,99,99,99,99,99, 1, 1, 1,99,99, 1,99,99,99,99,99, 1,99,99,99, 1,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1,99,99,99, 1,99,99,99},
{ 1,99,99,99, 1,99, 1,99,99,99,99,99,99,99,99,99, 1,99,99,99, 1,99,99,99,99,99,99,99,99,99,99,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99, 1,99, 1,99,99, 1,99,99, 1,99,99,99,99,99,99,99,99,99,99},
{ 1, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99, 1, 1, 1, 1,99,99,99,99, 1,99,99,99,99,99,99,99,99,99, 1, 1, 1, 1,99,99,99, 1, 1, 1, 1,99,99, 1, 1, 1,99,99, 1,99,99,99, 1,99, 1, 1, 1, 1, 1,99, 1,99,99,99,99}
};
char stage1Telop[5][35]=
{
{99, 1, 1, 1, 1,99, 1, 1, 1, 1, 1,99,99, 1, 1, 1,99,99,99, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99,99,99, 1,99,99},
{ 1,99,99,99,99,99,99,99, 1,99,99,99, 1,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99,99,99,99, 1, 1,99,99},
{99, 1, 1, 1,99,99,99,99, 1,99,99,99, 1, 1, 1, 1, 1,99, 1,99,99, 1, 1,99, 1, 1, 1, 1,99,99,99,99, 1,99,99},
{99,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99,99,99,99,99, 1,99,99},
{ 1, 1, 1, 1,99,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99,99,99, 1,99,99}
};
char stage2Telop[5][35]=
{
{99, 1, 1, 1, 1,99, 1, 1, 1, 1, 1,99,99, 1, 1, 1,99,99,99, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99,99, 1, 1, 1,99},
{ 1,99,99,99,99,99,99,99, 1,99,99,99, 1,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99,99,99, 1,99,99,99, 1},
{99, 1, 1, 1,99,99,99,99, 1,99,99,99, 1, 1, 1, 1, 1,99, 1,99,99, 1, 1,99, 1, 1, 1, 1,99,99,99,99,99, 1,99},
{99,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99,99,99,99,99, 1,99,99},
{ 1, 1, 1, 1,99,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99, 1, 1, 1, 1, 1}
};
char bossbattleTelop[5][65]=
{
{ 1, 1, 1, 1,99,99,99, 1, 1, 1,99,99,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1,99,99,99,99,99,99,99, 1, 1, 1, 1,99,99,99, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99, 1, 1, 1, 1, 1,99, 1,99,99,99,99,99, 1, 1, 1, 1, 1},
{ 1,99,99,99, 1,99, 1,99,99,99, 1,99, 1,99,99,99,99,99, 1,99,99,99,99,99,99,99,99,99,99,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99,99},
{ 1, 1, 1, 1,99,99, 1,99,99,99, 1,99,99, 1, 1, 1,99,99,99, 1, 1, 1,99,99,99,99,99,99,99,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99, 1,99,99,99,99,99, 1, 1, 1, 1,99},
{ 1,99,99,99, 1,99, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99,99,99, 1,99,99,99,99,99,99,99, 1,99,99,99, 1,99, 1,99,99,99, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99,99},
{ 1, 1, 1, 1,99,99,99, 1, 1, 1,99,99, 1, 1, 1, 1,99,99, 1, 1, 1, 1,99,99,99,99,99,99,99,99, 1, 1, 1, 1,99,99, 1,99,99,99, 1,99,99,99, 1,99,99,99,99,99, 1,99,99,99, 1, 1, 1, 1, 1,99, 1, 1, 1, 1, 1}
};
char atkTelop[5][23]=
{
{99, 1, 1, 1,99,99, 1, 1, 1, 1, 1,99, 1,99,99, 1,99,99,99,99,99,99,99},
{ 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99, 1,99,99,99,99, 7, 7,99,99},
{ 1, 1, 1, 1, 1,99,99,99, 1,99,99,99, 1, 1,99,99,99,99,99,99,99,99,99},
{ 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99, 1,99,99,99,99, 7, 7,99,99},
{ 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99, 1,99,99,99,99,99,99,99}
};
char defTelop[5][23]=
{
{ 4, 4, 4, 4,99,99, 4, 4, 4, 4, 4,99, 4, 4, 4, 4, 4,99,99,99,99,99,99},
{ 4,99,99,99, 4,99, 4,99,99,99,99,99, 4,99,99,99,99,99,99, 7, 7,99,99},
{ 4,99,99,99, 4,99, 4, 4, 4, 4,99,99, 4, 4, 4, 4,99,99,99,99,99,99,99},
{ 4,99,99,99, 4,99, 4,99,99,99,99,99, 4,99,99,99,99,99,99, 7, 7,99,99},
{ 4, 4, 4, 4,99,99, 4, 4, 4, 4, 4,99, 4,99,99,99,99,99,99,99,99,99,99}
};
char scoreTelop[5][35]=
{
{99, 7, 7, 7, 7,99,99, 7, 7, 7, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7,99,99, 7, 7, 7, 7, 7,99,99,99,99,99,99},
{ 7,99,99,99,99,99, 7,99,99,99,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99,99,99,99, 7, 7,99,99},
{99, 7, 7, 7,99,99, 7,99,99,99,99,99, 7,99,99,99, 7,99, 7, 7, 7, 7,99,99, 7, 7, 7, 7,99,99,99,99,99,99,99},
{99,99,99,99, 7,99, 7,99,99,99,99,99, 7,99,99,99, 7,99, 7,99,99, 7,99,99, 7,99,99,99,99,99,99, 7, 7,99,99},
{ 7, 7, 7, 7,99,99,99, 7, 7, 7, 7,99,99, 7, 7, 7,99,99, 7,99,99,99, 7,99, 7, 7, 7, 7, 7,99,99,99,99,99,99}
};
char modeTelop[5][29]=
{
{ 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7,99,99, 7, 7, 7, 7, 7,99,99,99,99,99,99},
{ 7, 7,99, 7, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99,99,99,99, 7, 7,99,99},
{ 7,99, 7,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7, 7, 7, 7,99,99,99,99,99,99,99},
{ 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99,99,99,99, 7, 7,99,99},
{ 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7,99,99, 7, 7, 7, 7, 7,99,99,99,99,99,99}
};
char normalTelop[5][35]=
{
{ 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7, 7, 7, 7,99,99, 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7,99,99,99,99},
{ 7, 7,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7, 7,99, 7, 7,99, 7,99,99,99, 7,99, 7,99,99,99,99},
{ 7,99, 7,99, 7,99, 7,99,99,99, 7,99, 7, 7, 7, 7,99,99, 7,99, 7,99, 7,99, 7, 7, 7, 7, 7,99, 7,99,99,99,99},
{ 7,99,99, 7, 7,99, 7,99,99,99, 7,99, 7,99,99, 7,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99,99},
{ 7,99,99,99, 7,99,99, 7, 7, 7,99,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7,99,99,99, 7,99, 7, 7, 7, 7, 7}
};
char recordTelop[5][35]=
{
{ 6, 6, 6, 6,99,99, 6, 6, 6, 6, 6,99,99, 6, 6, 6, 6,99,99, 6, 6, 6,99,99, 6, 6, 6, 6,99,99, 6, 6, 6, 6,99},
{ 6,99,99,99, 6,99, 6,99,99,99,99,99, 6,99,99,99,99,99, 6,99,99,99, 6,99, 6,99,99,99, 6,99, 6,99,99,99, 6},
{ 6, 6, 6, 6,99,99, 6, 6, 6, 6,99,99, 6,99,99,99,99,99, 6,99,99,99, 6,99, 6, 6, 6, 6,99,99, 6,99,99,99, 6},
{ 6,99,99, 6,99,99, 6,99,99,99,99,99, 6,99,99,99,99,99, 6,99,99,99, 6,99, 6,99,99, 6,99,99, 6,99,99,99, 6},
{ 6,99,99,99, 6,99, 6, 6, 6, 6, 6,99,99, 6, 6, 6, 6,99,99, 6, 6, 6,99,99, 6,99,99,99, 6,99, 6, 6, 6, 6,99}
};
char autoTelop[5][23]=
{
{99, 1, 1, 1,99,99, 1,99,99,99, 1,99, 1, 1, 1, 1, 1,99,99, 1, 1, 1,99},
{ 1,99,99,99, 1,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1},
{ 1, 1, 1,1 , 1,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1},
{ 1,99,99,99, 1,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1,99,99,99, 1},
{ 1,99,99,99, 1,99,99, 1, 1, 1,99,99,99,99, 1,99,99,99,99, 1, 1, 1,99}
};
char value[10][5][5]=
{
{
{99, 7, 7, 7,99},
{ 7,99,99, 7, 7},
{ 7,99, 7,99, 7},
{ 7, 7,99,99, 7},
{99, 7, 7, 7,99}
},
{
{99,99, 7,99,99},
{99, 7, 7,99,99},
{99,99, 7,99,99},
{99,99, 7,99,99},
{99,99, 7,99,99}
},
{
{99, 7, 7, 7,99},
{ 7,99,99,99, 7},
{99,99,99, 7,99},
{99,99, 7,99,99},
{ 7, 7, 7, 7, 7}
},
{
{ 7, 7, 7, 7,99},
{99,99,99,99, 7},
{99, 7, 7, 7,99},
{99,99,99,99, 7},
{ 7, 7, 7, 7,99}
},
{
{99,99,99, 7,99},
{99,99, 7, 7,99},
{99, 7,99, 7,99},
{ 7, 7, 7, 7, 7},
{99,99,99, 7,99}
},
{
{ 7, 7, 7, 7, 7},
{ 7,99,99,99,99},
{ 7, 7, 7, 7,99},
{99,99,99,99, 7},
{ 7, 7, 7, 7,99}
},
{
{99, 7, 7, 7, 7},
{ 7,99,99,99,99},
{ 7, 7, 7, 7,99},
{ 7,99,99,99, 7},
{99, 7, 7, 7,99}
},
{
{ 7, 7, 7, 7, 7},
{99,99,99,99, 7},
{99,99,99, 7,99},
{99,99, 7,99,99},
{99, 7,99,99,99}
},
{
{99, 7, 7, 7,99},
{ 7,99,99,99, 7},
{99, 7, 7, 7,99},
{ 7,99,99,99, 7},
{99, 7, 7, 7,99}
},
{
{99, 7, 7, 7,99},
{ 7,99,99,99, 7},
{99, 7, 7, 7, 7},
{99,99,99,99, 7},
{ 7, 7, 7, 7,99}
}
};

void changeStage(SD* sd,CD* pd,CD* ed,OD* id,int stageNumber){
	int i;
	/***** ステージ共通設定 *****/
	/* ステージと背景の生成 */
	createStage(sd,stageNumber);
	/* オブジェクトの初期化 */
	initItemPara(&pd->mine,EMPTY,NO_ACTION,RIGHT);
	pd->mine.now.y=pd->mine.now.x=pd->mine.accept.y=pd->mine.accept.x=0;
	for(i=0;i<MAX_ENEMY;i++){
		initItemPara(&ed[i].mine,EMPTY,NO_ACTION,RIGHT);
		ed[i].mine.now.y=ed[i].mine.now.x=ed[i].mine.accept.y=ed[i].mine.accept.x=0;
	}
	for(i=0;i<MAX_ITEM;i++){
		initItemPara(&id[i],EMPTY,NO_ACTION,RIGHT);
		id[i].now.y=id[i].now.x=id[i].accept.y=id[i].accept.x=0;
	}
	/* プレイヤーの構成の生成 */
	initPlayerPara(&pd->mine,NO_ACTION,RIGHT);
	pd->mine.state.ATK+=ATKTmp;
	pd->mine.state.DEF+=DEFTmp;
	pd->mine.state.score=SCORETmp;
	/* 敵キャラクタの構成の生成 */
	changeEnemy(ed,stageNumber);
	/* 単独アイテムの構成の生成 */
	changeItem(id,stageNumber);
	/* 所持アイテムの削除 */
	resetHaveItem(pd);
	for(i=0;i<MAX_ENEMY;i++){
		resetHaveItem(&ed[i]);
	}
	/***** ステージ別設定 *****/
	sd->slideDispY=sd->slideDispX=0;
	switch(stageNumber){
		case 0:
			/* プレイヤー初期座標 */
			pd->mine.now.y=pd->mine.accept.y=350-pd->mine.now.crashStartY;
			pd->mine.now.x=pd->mine.accept.x=20-pd->mine.now.crashStartX;
			break;
		case 1:
			pd->mine.now.y=pd->mine.accept.y=77-pd->mine.now.crashStartY;
			pd->mine.now.x=pd->mine.accept.x=20-pd->mine.now.crashStartX;
			break;
		case 2:
			pd->mine.now.y=pd->mine.accept.y=350-pd->mine.now.crashStartY;
			pd->mine.now.x=pd->mine.accept.x=21-pd->mine.now.crashStartX;
			eventTimer=0;
			event=FALSE;
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
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=366-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=343-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=274-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=199-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=266-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=111-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=245-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=12-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=152-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=-16-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=59-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=75-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=-6-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=92-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=24-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=286-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=-10-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=291-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=98-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=638-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=323-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=590-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=-1-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=735-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=48-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=834-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=163-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=889-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=369-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1073-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=369-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1112-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=358-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1232-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=358-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1272-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=358-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1312-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WISP,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=303-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1130-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=133-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1123-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=194-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1333-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=53-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1346-cd[num++].mine.now.crashStartX;
			break;
		case 1:
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=30-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=197-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=168-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=164-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=168-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=91-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=303-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=179-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=367-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=252-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=367-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=292-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=290-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=280-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=186-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=311-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=68-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=424-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=104-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=478-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=350-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=500-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=350-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=525-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=350-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=550-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=350-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=575-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=210-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=574-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=210-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=599-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=210-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=765-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=121-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=651-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=288-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=725-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=288-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=750-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=288-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=954-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=288-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=979-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=104-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1068-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=82-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1160-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SLIME,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=45-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1276-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=30-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1399-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=30-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1424-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=348-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1384-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,WIZARD,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=328-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1435-cd[num++].mine.now.crashStartX;
			initEnemyPara(&cd[num].mine,SOLDIER,NO_ACTION,RIGHT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=254-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=1430-cd[num++].mine.now.crashStartX;
			break;
		case 2:
			initEnemyPara(&cd[num].mine,MIRROR,NO_ACTION,LEFT);cd[num].mine.state.address=num;cd[num].mine.now.y=cd[num].mine.accept.y=299-cd[num].mine.now.crashStartY;cd[num].mine.now.x=cd[num].mine.accept.x=615-cd[num++].mine.now.crashStartX;
			break;
	}
}
void changeItem(OD* id,int stageNumber){
	int num=0;
	switch(stageNumber){
		case 0:
			initItemPara(&id[num],START,NO_ACTION,RIGHT);id[num].state.address=startIndex=num;id[num].now.y=id[num].accept.y=390-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=72-id[num++].now.crashStartX;
			initItemPara(&id[num],GOAL,NO_ACTION,RIGHT);id[num].state.address=goalIndex=num;id[num].now.y=id[num].accept.y=200-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1497-id[num++].now.crashStartX;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=60-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=455-id[num++].now.crashStartX;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=90-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1275-id[num++].now.crashStartX;
			initItemPara(&id[num],ATK_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=133-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=341-id[num++].now.crashStartX;
			initItemPara(&id[num],ATK_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=370-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1460-id[num++].now.crashStartX;
			initItemPara(&id[num],ATK_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=70-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1315-id[num++].now.crashStartX;
			initItemPara(&id[num],DEF_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=40-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=505-id[num++].now.crashStartX;
			initItemPara(&id[num],DEF_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=370-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1440-id[num++].now.crashStartX;
			initItemPara(&id[num],DEF_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=31-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1090-id[num++].now.crashStartX;
			initItemPara(&id[num],HP_REC,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=39-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=434-id[num++].now.crashStartX;
			break;
		case 1:
			initItemPara(&id[num],START,NO_ACTION,RIGHT);id[num].state.address=startIndex=num;id[num].now.y=id[num].accept.y=110-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=69-id[num++].now.crashStartX;
			initItemPara(&id[num],GOAL,NO_ACTION,RIGHT);id[num].state.address=goalIndex=num;id[num].now.y=id[num].accept.y=195-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1497-id[num++].now.crashStartX;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=330-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=850-id[num++].now.crashStartX;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=250-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=870-id[num++].now.crashStartX;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=70-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1315-id[num++].now.crashStartX;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=70-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1335-id[num++].now.crashStartX;
			initItemPara(&id[num],BROCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=70-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1355-id[num++].now.crashStartX;
			initItemPara(&id[num],SHOCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=370-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1235-id[num++].now.crashStartX;
			initItemPara(&id[num],SHOCK,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=230-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=700-id[num++].now.crashStartX;
			initItemPara(&id[num],HP_REC,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=370-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=10-id[num++].now.crashStartX;
			initItemPara(&id[num],DEF_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=309-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=259-id[num++].now.crashStartX;
			initItemPara(&id[num],ATK_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=369-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=850-id[num++].now.crashStartX;
			initItemPara(&id[num],DEF_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=369-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=870-id[num++].now.crashStartX;
			initItemPara(&id[num],ATK_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=349-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=850-id[num++].now.crashStartX;
			initItemPara(&id[num],HP_REC,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=349-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=870-id[num++].now.crashStartX;
			initItemPara(&id[num],ATK_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=39-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=900-id[num++].now.crashStartX;
			initItemPara(&id[num],DEF_UP,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=49-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1469-id[num++].now.crashStartX;
			break;
		case 2:
			initItemPara(&id[num],START,NO_ACTION,RIGHT);id[num].state.address=startIndex=num;id[num].now.y=id[num].accept.y=390-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=430-id[num++].now.crashStartX;
			initItemPara(&id[num],GOAL,NO_ACTION,RIGHT);id[num].state.address=goalIndex=num;id[num].now.y=id[num].accept.y=390-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1402-id[num++].now.crashStartX;
			initItemPara(&id[num],BROKEN_WALL,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=290-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=335-id[num++].now.crashStartX;
			initItemPara(&id[num],MOVED_WALL,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=194-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=860-id[num++].now.crashStartX;
			initItemPara(&id[num],CLEAR_WALL,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=340-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1295-id[num++].now.crashStartX;
			initItemPara(&id[num],SWITCH,NO_ACTION,RIGHT);id[num].state.address=num;id[num].now.y=id[num].accept.y=224-id[num].now.crashStartY;id[num].now.x=id[num].accept.x=1201-id[num++].now.crashStartX;
			break;
	}
}
void resetHaveItem(CD* cd){
	int i;
	for(i=0;i<MAX_HAVE;i++){
		/* 空にする */
		initItemPara(&cd->have[i],EMPTY,NO_ACTION,RIGHT);
		cd->have[i].now.y=cd->have[i].now.x=cd->have[i].accept.y=cd->have[i].accept.x=0;
	}
}
void decidePlayer(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	int key;
	if(id[startIndex].state.existFlag==TRUE&&id[startIndex].state.eventFlag==FALSE){
		od->now.motion=WALK;
		getch();
		return;
	}
	else if(sd->number==2&&od->state.eventFlag==FALSE&&ed[0].mine.state.walkCount>=1){
		od->now.motion=NO_ACTION;
		od->state.vX=0;
		getch();
		return;
	}
	if(od->state.deathFlag==TRUE){
		od->now.motion=DEATH;
		return;
	}
	key=getch();
	if(od->state.autoFlag==AUTO){
		if(key=='|'||(key=autoPlay())==-99)
			od->state.autoFlag=NORMAL;
	}
	switch(key){
		case KEY_LEFT:
			if(od->now.direction==LEFT&&od->now.number==getPlayerMoveNumber(od,AVOIDANCE))
				od->now.motion=BEHIND;
			else if(od->now.motion==NO_ACTION||od->now.direction==RIGHT){
				od->now.motion=WALK;
				od->now.direction=LEFT;
			}
			break;
		case KEY_RIGHT:
			if(od->now.direction==RIGHT&&od->now.number==getPlayerMoveNumber(od,AVOIDANCE))
				od->now.motion=BEHIND;
			else if(od->now.motion==NO_ACTION||od->now.direction==LEFT){
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
			if(od->now.motion!=N_ATTACK1&&od->now.motion!=N_ATTACK2&&od->now.motion!=N_ATTACK3)
				od->now.motion=N_ATTACK1;
			else if(od->now.motion==N_ATTACK1)
				od->now.motion=N_ATTACK2;
			else if(od->now.motion==N_ATTACK2)
				od->now.motion=N_ATTACK3;
			break;
		case 'C':
			if(od->now.motion!=S_ATTACK)
				od->now.motion=S_ATTACK;
			break;
		case 'p':
			pauseFlag=TRUE;
			break;
		default:
			/* 入力のない状態は-2と定義 */
			key=-2;
			if(od->accept.motion==NO_ACTION&&od->state.fallFlag==FALSE){
				od->state.vX=0;
				od->state.motionCount=0;
			}
			if(od->accept.motion==WALK||od->accept.motion==JUMP)
				od->now.motion=NO_ACTION;
			break;
	}
	if(od->state.autoFlag==RECORD)
		record_log(key);
}
void decideEnemy(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	int i;
	switch(od->now.variety){
		case SLIME:
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE){
				od->now.motion=DEATH;
				break;
			}
			/* 方向処理 */
			if(od->now.motion==NO_ACTION){
				if(od->now.x<pd->mine.now.x)
					od->now.direction=RIGHT;
				else
					od->now.direction=LEFT;
			}
			/* 攻撃処理 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,40,40)){
				if(od->now.motion!=N_ATTACK1&&od->state.stamina-od->state.maxStamina>=0)
					od->now.motion=N_ATTACK1;
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION&&getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,60,60)){
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
			break;
		case WISP:
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE){
				od->now.motion=DEATH;
				break;
			}
			/* 攻撃処理 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,60,60)){
				if(od->now.motion!=N_ATTACK1&&od->state.stamina-od->state.maxStamina>=0)
					od->now.motion=N_ATTACK1;
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION)
				od->now.motion=WALK;
			break;
		case SOLDIER:
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE){
				od->now.motion=DEATH;
				break;
			}
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
				if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,60,60)){
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
			break;
		case WIZARD:
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE){
				od->now.motion=DEATH;
				break;
			}
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
			else if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,80,80)){
				if(od->now.motion!=N_ATTACK1&&od->state.stamina-od->state.maxStamina/2>=0){
					od->now.motion=N_ATTACK1;
					od->now.direction=(pd->mine.now.x-od->now.x>0)?RIGHT:LEFT;
				}
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION){
				/* 一定距離間隔なら自分から近づく */
				if(
					(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)>getSqrtDistant(0,0,60,60))&&
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
			break;
		case MIRROR:
			if(pd->mine.state.eventFlag==FALSE){
				if(id[startIndex].state.existFlag==TRUE&&id[startIndex].state.eventFlag==FALSE){
					if(pd->mine.now.x+pd->mine.now.crashEndX>=id[startIndex].now.x+id[startIndex].now.crashStartX&&od->state.walkCount==0){
						od->state.walkCount++;
						od->state.charge=od->state.maxCharge;
						od->now.motion=S_ATTACK;
					}
					break;
				}
				if(od->state.walkCount==1&&od->now.motion==NO_ACTION&&id[2].state.wallFootFlag==TRUE)
					if(od->now.x+od->now.crashStartX>=460)
						od->now.motion=WALK;
					else
						od->state.walkCount++;
				else if(od->state.walkCount==2){
					pd->mine.state.eventFlag=TRUE;
					od->state.walkCount=0;
					od->state.charge=od->state.maxCharge;
					od->now.motion=S_ATTACK;
				}
				if(od->state.walkCount>0)
					break;
			}
			/* 死亡時処理 */
			if(od->state.deathFlag==TRUE){
				od->now.motion=DEATH;
				break;
			}
			/* 方向処理 */
			if(od->now.motion==NO_ACTION){
				if(od->now.x<pd->mine.now.x)
					od->now.direction=RIGHT;
				else
					od->now.direction=LEFT;
			}
			/* 特殊行動 */
			if(od->state.walkCount>=200){
				if(od->state.eventFlag==FALSE){
					if(pd->mine.now.direction==RIGHT)
						od->now.x=pd->mine.now.x+pd->mine.now.crashEndX+1-od->now.crashStartX;
					else
						od->now.x=pd->mine.now.x+pd->mine.now.crashStartX-1-od->now.crashEndX;
					od->now.y=pd->mine.now.y;
					od->now.direction=(pd->mine.now.direction==RIGHT)?LEFT:RIGHT;
					od->state.eventFlag=TRUE;
					od->now.motion=AVOIDANCE;
				}
				else{
					od->state.eventFlag=FALSE;
					od->state.walkCount=0;
					if(pd->mine.now.y+pd->mine.now.crashEndY<=264&&pd->mine.now.x+pd->mine.now.crashEndX<=1120&&pd->mine.now.direction==RIGHT)
						od->now.motion=NO_ACTION;
					else
						od->now.motion=BEHIND;
				}
				break;
			}
			/* 必殺攻撃 */
			if(event==FALSE){
				if(od->now.motion==S_ATTACK)
					break;
				if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,80,80)){
					if(od->now.motion!=S_ATTACK&&od->state.stamina-50>=0&&od->state.charge-od->state.maxCharge>=0)
						od->now.motion=S_ATTACK;
				}
			}
			/* 確定回避ジャンプ */
			if(event==FALSE){
				if(od->state.eventFlag==TRUE){
					if(od->state.stamina-40>=0){
						od->state.eventFlag=FALSE;
						od->now.motion=AVOIDANCE;
						break;
					}
				}
				if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,40,40)){
					if(od->now.motion==N_ATTACK3&&od->state.motionCount==13&&od->state.stamina-50>=0){
						od->state.eventFlag=TRUE;
						od->now.motion=JUMP;
						break;
					}
				}
			}
			/* 攻撃処理3 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,60,60)){
				if(od->now.motion==N_ATTACK2&&od->state.motionCount==8&&od->state.stamina-25>=0)
					od->now.motion=N_ATTACK3;
			}
			/* 攻撃処理2 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,35,35)){
				if(od->now.motion==N_ATTACK1&&od->state.motionCount==8&&od->state.stamina-15>=0)
					od->now.motion=N_ATTACK2;
			}
			/* 攻撃処理1 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,30,30)){
				if((od->now.motion<N_ATTACK1||od->now.motion>N_ATTACK3)&&od->state.stamina-od->state.maxStamina>=0)
					od->now.motion=N_ATTACK1;
			}
			/* チャージ回避 */
			if(pd->mine.now.motion==S_ATTACK&&od->state.stamina-10>=0){
				if(event==FALSE&&getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,30,30))
					od->now.motion=BEHIND;
				else if(od->state.jumpCount==0||od->state.jumpCount==1&&od->state.fallTime>=getParabolaTopTime(od->state.v0)){
					od->state.eventFlag=TRUE;
					od->now.motion=JUMP;
				}
			}
			/* 反射回避 */
			if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,30,30)){
				if(od->now.motion==AVOIDANCE&&od->state.fallFlag==FALSE)
					od->now.motion=BEHIND;
			}
			/* 回避処理 */
			if(event==FALSE&&(pd->mine.now.motion==N_ATTACK2||pd->mine.now.motion==N_ATTACK3)||event==TRUE&&pd->mine.now.motion==N_ATTACK3){
				if(!(pd->mine.now.y+pd->mine.now.crashEndY<=264&&pd->mine.now.x+pd->mine.now.crashEndX<=1120&&od->now.y+od->now.crashEndY>264)){
					if(od->now.motion!=AVOIDANCE&&od->state.stamina-10>=0)
						od->now.motion=AVOIDANCE;
				}
			}
			/* 移動処理 */
			if(od->now.motion==NO_ACTION){
				/* 上下で離れていたら動かない */
				if(od->now.y>pd->mine.now.y&&od->now.y-pd->mine.now.y>=OBJECT_LINES/2){
					od->now.motion=NO_ACTION;
					od->state.walkCount++;
					break;
				}
				/* 一定距離間隔なら自分から近づく */
				if(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)>getSqrtDistant(0,0,25,25)){
					od->state.walkCount=0;
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
			break;
	}
}
void decideItem(OD* od,SD* sd,CD* pd,CD* ed,OD* id){
	return;
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
					if(getHitInfo(od->state.look[y][x])==1){
						sd->look[od->now.y+y][od->now.x+x].damage=od->state.damage;
						sd->look[od->now.y+y][od->now.x+x].stage=(sd->back[od->now.y+y][od->now.x+x]/100000)*100000+sd->look[od->now.y+y][od->now.x+x].stage%100000;
					}
					sd->look[od->now.y+y][od->now.x+x].label=od->state.label;
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
					sd->look[od->now.y+y][od->now.x+x].label=0;
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
				if(sd->number==2&&event==TRUE&&getColorInfo(sd->look[y][x].stage)==63&&getPriorityInfo(sd->look[y][x].stage)<=1)
					attron(COLOR_PAIR(8));
				else if(sd->number==2&&event==TRUE&&getColorInfo(sd->look[y][x].stage)==8&&getPriorityInfo(sd->look[y][x].stage)<=1)
					attron(COLOR_PAIR(7));
				else
					attron(COLOR_PAIR(getColorInfo(sd->look[y][x].stage)));
				for(z=0;z<2;z++)
					mvaddch(y-sd->displayY,(x-sd->displayX)*2+z,outPut);
			}
		}
	}
}
void outputStatus(CD* cd,int restLife){
	int i,j,k,l,y,x,z;
	char *num;
	/* ステータスバーの背景を描画 */
	attron(COLOR_PAIR(getColorInfo(10)));
	for(y=0;y<LINES;y++){
		for(x=DISPLAY_COLS;x<DISPLAY_COLS+COLS;x++){
			for(z=0;z<2;z++)
				mvaddch(y,x*2+z,outPut);
		}
	}
	/* lifeTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<23;x++){
			if(getColorInfo(lifeTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(lifeTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(2+y,(DISPLAY_COLS+2+x)*2+z,outPut);
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
						mvaddch(8+y,(DISPLAY_COLS+2+x+i*6)*2+z,outPut);
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
					mvaddch(17+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* hpGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->mine.state.HP*100)/cd->mine.state.maxHP)?2:4));
			mvaddch(24+y,(DISPLAY_COLS+2)*2+x,outPut);
		}
	}
	/* staminaTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<41;x++){
			if(getColorInfo(staminaTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(staminaTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(30+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* staminaGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->mine.state.stamina*100)/cd->mine.state.maxStamina)?3:4));
			mvaddch(37+y,(DISPLAY_COLS+2)*2+x,outPut);
		}
	}
	/* chargeTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<35;x++){
			if(getColorInfo(chargeTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(chargeTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(43+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* chargeGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(cd->mine.state.charge*100)/cd->mine.state.maxCharge)?5:4));
			mvaddch(50+y,(DISPLAY_COLS+2)*2+x,outPut);
		}
	}
	/* atkTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<23;x++){
			if(getColorInfo(atkTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(atkTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(((cd->mine.state.eventFlag==TRUE)?84:58)+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* ATKの描画 */
	//num[0]='\0';
	//iToa(cd->mine.state.ATK,num);
	for(i=0,j=cd->mine.state.ATK;(j/=10)>0;i++);
	for(i,l=0;i>=0;i--,l++){
		for(k=i,j=cd->mine.state.ATK;k>0;k--)
			j/=10;
		j%=10;
		for(y=0;y<5;y++){
			for(x=0;x<5;x++){
				if(getColorInfo(value[j][y][x])<64){
					attron(COLOR_PAIR(getColorInfo(value[j][y][x])));
					for(z=0;z<2;z++)
						mvaddch(((cd->mine.state.eventFlag==TRUE)?84:58)+y,(DISPLAY_COLS+2+23+l*6+x)*2+z,outPut);
				}
			}
		}
	}
	/*
	for(i=0;num[i]!='\0';i++){
		j=num[i]-'0';
		for(y=0;y<5;y++){
			for(x=0;x<5;x++){
				if(getColorInfo(value[j][y][x])<64){
					attron(COLOR_PAIR(getColorInfo(value[j][y][x])));
					for(z=0;z<2;z++)
						mvaddch(((cd->mine.state.eventFlag==TRUE)?84:58)+y,(DISPLAY_COLS+2+23+l*6+x)*2+z,' ');
				}
			}
		}
	
	}
	*/
	/* defTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<23;x++){
			if(getColorInfo(defTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(defTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(((cd->mine.state.eventFlag==TRUE)?90:64)+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* DEFの描画 */
	for(i=0,j=cd->mine.state.DEF;(j/=10)>0;i++);
	for(i,l=0;i>=0;i--,l++){
		for(k=i,j=cd->mine.state.DEF;k>0;k--)
			j/=10;
		j%=10;
		for(y=0;y<5;y++){
			for(x=0;x<5;x++){
				if(getColorInfo(value[j][y][x])<64){
					attron(COLOR_PAIR(getColorInfo(value[j][y][x])));
					for(z=0;z<2;z++)
						mvaddch(((cd->mine.state.eventFlag==TRUE)?90:64)+y,(DISPLAY_COLS+2+23+l*6+x)*2+z,outPut);
				}
			}
		}
	}
	/* scoreTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<35;x++){
			if(getColorInfo(scoreTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(scoreTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(((cd->mine.state.eventFlag==TRUE)?101:75)+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* scoreの描画 */
	for(i=0,j=cd->mine.state.score;(j/=10)>0;i++);
	for(i,l=0;i>=0;i--,l++){
		for(k=i,j=cd->mine.state.score;k>0;k--)
			j/=10;
		j%=10;
		for(y=0;y<5;y++){
			for(x=0;x<5;x++){
				if(getColorInfo(value[j][y][x])<64){
					attron(COLOR_PAIR(getColorInfo(value[j][y][x])));
					for(z=0;z<2;z++)
						mvaddch(((cd->mine.state.eventFlag==TRUE)?107:81)+y,(DISPLAY_COLS+2+6+l*6+x)*2+z,outPut);
				}
			}
		}
	}
	/* modeTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<29;x++){
			if(getColorInfo(modeTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(modeTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(DISPLAY_LINES-13+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* modeの描画 */
	if(cd->mine.state.autoFlag==NORMAL){
		for(y=0;y<5;y++){
			for(x=0;x<35;x++){
				if(getColorInfo(normalTelop[y][x])<64){
					attron(COLOR_PAIR(getColorInfo(normalTelop[y][x])));
					for(z=0;z<2;z++)
						mvaddch(DISPLAY_LINES-7+y,(DISPLAY_COLS+2+6+x)*2+z,outPut);
				}
			}
		}
	}
	else if(cd->mine.state.autoFlag==AUTO){
		for(y=0;y<5;y++){
			for(x=0;x<23;x++){
				if(getColorInfo(autoTelop[y][x])<64){
					attron(COLOR_PAIR(getColorInfo(autoTelop[y][x])));
					for(z=0;z<2;z++)
						mvaddch(DISPLAY_LINES-7+y,(DISPLAY_COLS+2+6+x)*2+z,outPut);
				}
			}
		}
	}
	else if(cd->mine.state.autoFlag==RECORD){
		for(y=0;y<5;y++){
			for(x=0;x<35;x++){
				if(getColorInfo(recordTelop[y][x])<64){
					attron(COLOR_PAIR(getColorInfo(recordTelop[y][x])));
					for(z=0;z<2;z++)
						mvaddch(DISPLAY_LINES-7+y,(DISPLAY_COLS+2+6+x)*2+z,outPut);
				}
			}
		}
	}
}
void outputBoss(OD* od){
	int y,x,z;
	/* bossTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<23;x++){
			if(getColorInfo(bossTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(bossTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(56+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* bossHPGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(od->state.HP*100)/od->state.maxHP)?2:4));
			mvaddch(63+y,(DISPLAY_COLS+2)*2+x,outPut);
		}
	}
	/* eventTelopの描画 */
	for(y=0;y<5;y++){
		for(x=0;x<29;x++){
			if(getColorInfo(eventTelop[y][x])<64){
				attron(COLOR_PAIR(getColorInfo(eventTelop[y][x])));
				for(z=0;z<2;z++)
					mvaddch(69+y,(DISPLAY_COLS+2+x)*2+z,outPut);
			}
		}
	}
	/* eventGaugeの描画 */
	for(y=0;y<3;y++){
		for(x=0;x<100;x++){
			attron(COLOR_PAIR((x<(eventTimer*100)/maxEventTime)?7:4));
			mvaddch(76+y,(DISPLAY_COLS+2)*2+x,outPut);
		}
	}
}
void outputNumber(SD* sd,CD* pd,int rest,int number){
	int y,x,z;
	int slide,elapsed;
	for(elapsed=-40;elapsed<=40;elapsed++){
		erase();
		outputStage(sd);
		if(elapsed<=10&&elapsed>=-10)
			slide=elapsed*((elapsed<0)?-1:1);
		else{
			if(elapsed<0)
				slide=(elapsed+4)*(elapsed+4);
			else
				slide=(elapsed-4)*(elapsed-4);
		}
		switch(number){
			case 0:
				for(y=0;y<5;y++){
					for(x=0;x<35;x++){
						if(getColorInfo(stage1Telop[y][x])<64){
							attron(COLOR_PAIR(5));
							for(z=0;z<2;z++){
								if(elapsed<0)
									mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-35)/2+x-slide)*2+z,outPut);
								else
									mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-35)/2+x+slide)*2+z,outPut);
							}
						}
					}
				}
				break;
			case 1:
				for(y=0;y<5;y++){
					for(x=0;x<35;x++){
						if(getColorInfo(stage2Telop[y][x])<64){
							attron(COLOR_PAIR(4));
							for(z=0;z<2;z++){
								if(elapsed<0)
									mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-35)/2+x-slide)*2+z,outPut);
								else
									mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-35)/2+x+slide)*2+z,outPut);
							}
						}
					}
				}
				break;
			case 2:
				for(y=0;y<5;y++){
					for(x=0;x<65;x++){
						if(getColorInfo(bossbattleTelop[y][x])<64){
							attron(COLOR_PAIR(1));
							for(z=0;z<2;z++){
								if(elapsed<0)
									mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-65)/2+x-slide)*2+z,outPut);
								else
									mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-65)/2+x+slide)*2+z,outPut);
							}
						}
					}
				}
				break;
		}
		slide=0;
		if(elapsed<25&&elapsed>-25)
			slide++;
		if(elapsed<23&&elapsed>-23)
			slide++;
		if(elapsed<22&&elapsed>-22)
			slide++;
		for(y=0;y<slide;y++){
			switch(number){
				case 0:attron(COLOR_PAIR(5));break;
				case 1:attron(COLOR_PAIR(4));break;
				case 2:attron(COLOR_PAIR(1));break;
			}
			for(x=0;x<DISPLAY_COLS*2+1;x++){
				mvaddch(DISPLAY_LINES/2-(4+y),x,outPut);
				mvaddch(DISPLAY_LINES/2+(4+y),x,outPut);
			}
		}
		outputStatus(pd,rest);
		getch();
		refresh();
		usleep(40000);
	}
}
void scrollDisplay(SD* sd,OD* od){
	sd->displayY=od->now.y+OBJECT_LINES/2-DISPLAY_LINES/2;
	sd->displayX=od->now.x+OBJECT_COLS/2-DISPLAY_COLS/2;
	sd->displayY+=sd->slideDispY;
	sd->displayX+=sd->slideDispX;
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
		if(label!=ITEM&&od->now.motion!=DEATH){
			if((pd->state.charge+=pd->state.maxCharge/10)>pd->state.maxCharge)
				pd->state.charge=pd->state.maxCharge;
		}
	}
}
void gameover(SD* sd,CD* pd,CD* ed,OD* id,int flag){
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
	if(flag==TRUE)
		createPlayer(&pd->mine,NO_ACTION,pd->mine.now.direction);
	outputObject(sd,&pd->mine);
	outputStage(sd);
	if(flag==TRUE){
		/* 最高得点専用の処理 */
		if(pd->mine.state.score>getScore()&&pd->mine.state.autoFlag==RECORD){
			/* 最高点の上書き */
			setScore(pd->mine.state.score);
			change_best();
			/* bestTelopの描画 */
			for(y=0;y<5;y++){
				for(x=0;x<65;x++){
					if(getColorInfo(bestTelop[y][x])<64){
						attron(COLOR_PAIR(getColorInfo(bestTelop[y][x])));
						for(z=0;z<2;z++)
							mvaddch((DISPLAY_LINES-5)/2+y-11,((DISPLAY_COLS-65)/2+x)*2+z,outPut);
					}
				}
			}
		}
		/* clearTelopの描画 */
		for(y=0;y<5;y++){
			for(x=0;x<29;x++){
				if(getColorInfo(clearTelop[y][x])<64){
					attron(COLOR_PAIR(getColorInfo(clearTelop[y][x])));
					for(z=0;z<2;z++)
						mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-29)/2+x)*2+z,outPut);
				}
			}
		}
	}
	else{
		/* gameoverTelopの描画 */
		for(y=0;y<5;y++){
			for(x=0;x<47;x++){
				if(getColorInfo(gameoverTelop[y][x])<64){
					attron(COLOR_PAIR(getColorInfo(gameoverTelop[y][x])));
					for(z=0;z<2;z++)
						mvaddch((DISPLAY_LINES-5)/2+y,((DISPLAY_COLS-47)/2+x)*2+z,outPut);
				}
			}
		}
	}
	outputStatus(pd,restLife);
	if(sd->number==2)
		outputBoss(&ed[0].mine);
	refresh();
	break_log();
}
char confirmClear(void){
	if(stageNumber<3-1)
		restLife+=2;
	stageNumber++;
	ATKTmp+=player.mine.state.ATKUpper;
	DEFTmp+=player.mine.state.DEFUpper;
	player.mine.state.score+=player.mine.state.HP;
	SCORETmp=player.mine.state.score;
	return TRUE;
}
void objectFlow(OD* od,SD* sd,CD* pd,CD* ed,OD* id,int label){
	if(
		(od->state.existFlag==TRUE)&&
		(
		(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,DISPLAY_LINES,DISPLAY_COLS))||
		(od->now.variety==MIRROR&&od->state.label==ENEMY)||
		(od->now.variety==PLAYEREFFECT&&od->state.label==PLAYER)||
		(od->now.variety==MIRROREFFECT&&od->state.label==ENEMY)||
		(od->now.variety==SWITCH&&od->state.label==ITEM)
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
		(
		(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,DISPLAY_LINES,DISPLAY_COLS))||
		(od->now.variety==MIRROR&&od->state.label==ENEMY)||
		(od->now.variety==PLAYEREFFECT&&od->state.label==PLAYER)||
		(od->now.variety==MIRROREFFECT&&od->state.label==ENEMY)||
		(od->now.variety==SWITCH&&od->state.label==ITEM)
		)
	){
		od->state.damageFlag=FALSE;
		if(confirmDamageObject(sd,od,&y,&x)==TRUE){
			if(sd->look[od->now.y+y][od->now.x+x].label!=od->state.label){
				if(sd->look[od->now.y+y][od->now.x+x].damage-od->state.DEF>0){
					if(label!=ITEM)
						od->state.damageFlag=TRUE;
					if((od->state.HP-=(sd->look[od->now.y+y][od->now.x+x].damage-od->state.DEF))<0)
						od->state.HP=0;
				}
			}
		}
		confirmExistObject(sd,od,&pd->mine,label,FALSE);
	}
}
void objectReoutput(OD* od,SD* sd,CD* pd){
	if(
		(od->state.existFlag==TRUE)&&
		(
		(getSqrtDistant(od->now.y,od->now.x,pd->mine.now.y,pd->mine.now.x)<=getSqrtDistant(0,0,DISPLAY_LINES,DISPLAY_COLS))||
		(od->now.variety==MIRROR&&od->state.label==ENEMY)||
		(od->now.variety==PLAYEREFFECT&&od->state.label==PLAYER)||
		(od->now.variety==MIRROREFFECT&&od->state.label==ENEMY)||
		(od->now.variety==SWITCH&&od->state.label==ITEM)
		)
	){
		deleteObject(sd,od);
		outputObject(sd,od);
	}
}
void catchEnd(){
	restLife=1;
	player.mine.state.HP=0;
}
void main(void){
	/***** プリセット *****/
	/* ncursesの設定 */
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr,true);
	/* Ctrl + C での挙動における準備 */
	struct sigaction sa;
	sa.sa_handler=catchEnd;
	sa.sa_flags |= SA_RESTART;
	/* 使用色の設定 */
	setColorPair();
	/* オブジェクトチップの初期情報の設定 */
	initPlayerInfo();
	initEnemyInfo();
	initItemInfo();
	/* タイトルの描画 */
	if(signal(SIGINT,SIG_IGN)==SIG_ERR)
		perror("sigaction");
	autoFlag=openning();
	/* ログファイルの動的生成 */
	if(autoFlag==RECORD){
		if(gene_log()==FALSE){
			/* プレイできない旨を表示 */
			playStopper();
			endwin();
			exit(0);
		}
	}
	int i,j;
	/***** メインフロー *****/
	if(signal(SIGINT,catchEnd)==SIG_ERR)
		perror("sigaction");
	while(restLife>0&&stageNumber<3){
		/* ステージ構成をリセット */
		changeStage(&stage,&player,enemy,item,stageNumber);
		timeout(0);
		achieveFlag=pauseFlag=FALSE;
		player.mine.state.autoFlag=autoFlag;
		start=time(NULL);
		/* ログファイルを開く */
		if(autoFlag==AUTO)
			init_auto(stage.number);
		else if(autoFlag==RECORD)
			init_log(stage.number);
		/* プレイヤーの存在している間 */
		while(player.mine.state.existFlag==TRUE&&achieveFlag==FALSE&&pauseFlag==FALSE){
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
			/***** 単独アイテム処理 *****/
			for(i=0;i<MAX_ITEM;i++){
				odTmp=&item[i];
				objectDamage(odTmp,&stage,&player,ITEM);
			}
			/***** キャラクタ処理 *****/
			for(i=0;i<MAX_ENEMY+1;i++){
				cdTmp=(i==0)?&player:&enemy[i-1];
				/***** 所持アイテム処理 *****/
				for(j=0;j<MAX_HAVE;j++){
					odTmp=&cdTmp->have[j];
					objectDamage(odTmp,&stage,&player,ITEM);
				}
				odTmp=&cdTmp->mine;
				objectDamage(odTmp,&stage,&player,(i==0)?PLAYER:ENEMY);
			}
			/***** 再描画処理 *****/
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
			/***** 出力処理 *****/
			erase();
			scrollDisplay(&stage,&player.mine);
			outputStage(&stage);
			if(player.mine.state.deathFlag==TRUE)
				player.mine.state.charge=0;
			outputStatus(&player,restLife);
			if(stageNumber==2&&player.mine.state.eventFlag==TRUE){
				if(item[5].now.motion==NO_ACTION){
					event=FALSE;
					if((eventTimer+=1)>maxEventTime)
						eventTimer=maxEventTime;
				}
				else{
					event=TRUE;
					if((eventTimer-=2)<0){
						eventTimer=0;
						item[5].state.eventFlag=FALSE;
					}
				}
				outputBoss(&enemy[0].mine);
			}
			if(stageNumber<2&&item[startIndex].state.eventFlag==TRUE||stageNumber==2&&player.mine.state.eventFlag==FALSE&&enemy[0].mine.state.walkCount==2){
				outputNumber(&stage,&player,restLife,stageNumber);
			}
			else if(item[goalIndex].state.eventFlag==TRUE){
				finish=time(NULL);
				player.mine.state.score-=(finish-start)*5;
				achieveFlag=confirmClear();
			}
			refresh();
			usleep(3000);
		}
		/***** 死亡時処理 *****/
		/* ログファイルを閉じる */
		if(autoFlag==AUTO)
			end_auto();
		else if(autoFlag==RECORD)
			end_log();
		if(achieveFlag==TRUE||pauseFlag==TRUE)
			continue;
		restLife--;
		if((SCORETmp-=SCORETmp/5)<0)
			SCORETmp=0;
		if(restLife!=0){
			bkgd(COLOR_PAIR(stage.stageColor));
			erase();
			refresh();
			timeout(1000);
			getch();
		}
	}
	/***** ゲームオーバー処理 *****/
	gameover(&stage,&player,enemy,item,achieveFlag);
	usleep(2000000);
	if(achieveFlag==TRUE){
		if(signal(SIGINT,SIG_IGN)==SIG_ERR)
			perror("sigaction");
		ending();
	}
	endwin();
}

