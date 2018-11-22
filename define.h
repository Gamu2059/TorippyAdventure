extern void setColorPair(void);
extern void debug(char);

#ifndef __DEFINE__
#define __DEFINE__

#define OBJECT_LINES 100
#define OBJECT_COLS  100

#define STAGE_LINES 400
#define STAGE_COLS  1500

#define OPENNING_LINES 200
#define OPENNING_COLS  320

#define ENDING_LINES 200
#define ENDING_COLS 320

#define DISPLAY_LINES 203
#define DISPLAY_COLS  260

#define SCROLL_LIMIT_LINES 399

#define ACCEL 9.8

#define ELAPSED_TIME 0.05

#define TRUE 1
#define FALSE 0

#define RIGHT 1
#define LEFT -1

#define MAX_ITEM 20
#define MAX_HAVE 2
#define MAX_ENEMY 40

#define PLAYER 1
#define ENEMY 2
#define ITEM 3

#define PLAYER_NUMBER 23
#define ENEMY_VARIETY 5
#define ENEMY_NUMBER 23
#define ITEM_VARIETY 17
#define ITEM_NUMBER 5

#define SLIME 0
#define WISP 1
#define SOLDIER 2
#define WIZARD 3
#define MIRROR 4

#define EMPTY 0
#define BROCK 1
#define WISPBALL 2
#define PLAYEREFFECT 3
#define MIRROREFFECT 4
#define THUNDER 5
#define FIRE 6
#define ATK_UP 7
#define DEF_UP 8
#define HP_REC 9
#define SHOCK 10
#define START 11
#define GOAL 12
#define BROKEN_WALL 13
#define MOVED_WALL 14
#define CLEAR_WALL 15
#define SWITCH 16

#define PAUSE -1
#define NO_ACTION 0
#define WALK 1
#define JUMP 2
#define JUMP_UP 3
#define JUMP_DOWN 4
#define N_ATTACK1 5
#define N_ATTACK2 6
#define N_ATTACK3 7
#define S_ATTACK 8
#define AVOIDANCE 9
#define DEATH 10

typedef struct{
	int stage;
	int damage;
	int label;
} ST;

typedef struct{
	ST look[STAGE_LINES][STAGE_COLS];
	int back[STAGE_LINES][STAGE_COLS];
	int number;
	int stageColor;
	int displayY,displayX;
	int minDisplayY,minDisplayX;
	int maxDisplayY,maxDisplayX;
	int slideDispY,slideDispX;
} SD;

typedef struct{
	int y,x;
	int variety,number;
	int direction;
	int existStartY,existStartX;
	int existEndY,existEndX;
	int crashStartY,crashStartX;
	int crashEndY,crashEndX;
	int wallUpLine,wallDownLine,wallLeftLine,wallRightLine;
	int wallUpBaseX,wallDownBaseX,wallLeftBaseY,wallRightBaseY;
	int motion;
} BD;

typedef struct{
	int address;
	int parent;
	int label;
	int look[OBJECT_LINES][OBJECT_COLS];
	int back[OBJECT_LINES][OBJECT_COLS];
	int existFlag,deathFlag,eventFlag;
	int HP,maxHP,acceptHP;
	int stamina,maxStamina;
	int charge,maxCharge;
	int ATK,ATKUpper;
	int DEF,DEFUpper;
	int damage;
	int wallHeadFlag,wallFootFlag,wallKickFlag;
	float fallTime;
	int fallFlag;
	int damageFlag;
	int jumpCount,maxJumpCount;
	float v0,i0;
	float vA,iA;
	float vX,iX;
	float vY,iY;
	int motionCount;
	int walkCount;
} PD;

typedef struct{
	BD now,accept;
	PD state;
} OD;

typedef struct{
	OD mine;
	OD have[MAX_HAVE];
} CD;
#endif

