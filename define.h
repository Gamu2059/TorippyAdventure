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

#define DISPLAY_LINES 202
#define DISPLAY_COLS  260

#define SCROLL_LIMIT_LINES 340

#define NO_ACTION 0
#define WALK 1
#define JUMP 2
#define JUMP_UP 3
#define JUMP_DOWN 4
#define N_ATTACK1 5
#define N_ATTACK2 6
#define S_ATTACK 7

#define ACCEL 9.8

#define ELAPSED_TIME 0.05

#define TRUE 1
#define FALSE 0

#define RIGHT 1
#define LEFT -1

#define PLAYER 1
#define ENEMY 2
#define ITEM 3

#define PLAYER_NUMBER 18
#define ENEMY_VARIETY 1
#define ENEMY_NUMBER 1
#define ITEM_VARIETY 2
#define ITEM_NUMBER 1

#define MAX_ITEM 20
#define MAX_HAVE 10
#define MAX_ENEMY 20

#define REST_LIFE 6

#define DAMAGE_VALUE 10

#define CHARGE_VALUE 10

typedef struct{
	int stage;
	int slideY,slideX;
	int damage;
} ST;

typedef struct{
	ST look[STAGE_LINES][STAGE_COLS];
	int back[STAGE_LINES][STAGE_COLS];
	int number;
	int displayY,displayX;
	int minDisplayY,minDisplayX;
	int maxDisplayY,maxDisplayX;
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
	int look[OBJECT_LINES][OBJECT_COLS];
	int back[OBJECT_LINES][OBJECT_COLS];
	int existFlag,deathFlag,eventFlag,damageFlag;
	int HP,maxHP;
	int stamina,maxStamina;
	int charge,maxCharge;
	int ATK;
	int DEF;
	int wallHeadFlag,wallFootFlag,wallKickFlag;
	float fallTime;
	int fallFlag;
	int jumpCount,maxJumpCount;
	float v0,i0;
	float vA,iA;
	float vX,iX;
	int motionCount;
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

