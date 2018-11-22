////////////////////////////////////////////////////////////////////////////////
// ***** define *****
//
// 定義しているもの
// ・全ての定数。
// 備考
// ・このヘッダファイルはマクロ定義のみを行う特殊なものです。
// ・必ず、【すべてのソースファイルおよびヘッダファイル】に
//   このヘッダファイルをインクルードして下さい。
////////////////////////////////////////////////////////////////////////////////

/***** 領域マクロ群 *****/

// PLAYER
// ・プレイヤーの最大サイズです。
// ・LINESは縦幅です。
// ・COLSは横幅です。
#define PLAYER_LINES 20
#define PLAYER_COLS  20

// STAGE
// ・ステージの最大サイズです。
// ・LINESは縦幅です。
// ・COLSは横幅です。
#define STAGE_LINES 400
#define STAGE_COLS  1580

// DISPLAY
// ・ステージを表示する際の最大サイズです。
// ・LINESは縦幅です。
// ・COLSは横幅です。
#define DISPLAY_LINES 170
#define DISPLAY_COLS  316

// ITEM
// ・アイテムの最大サイズです。
// ・LINESは縦幅です。
// ・COLSは横幅です。
#define ITEM_LINES 40
#define ITEM_COLS  40

// OPENNING
// ・オープニング画面を表示する際の最大サイズです。
// ・LINESは縦幅です。
// ・COLSは横幅です。
#define OPENNING_LINES 200
#define OPENNING_COLS  320

// SCROLL
// ・表示域を移動させるタイミングとなる最大幅です。
// ・LINESは縦幅です。
// ・COLSは横幅です。
#define SCROLL_LINES 20
#define SCROLL_COLS  100

// SCROLL_LIMIT
// ・下方向にスクロールする際の限界となるラインです。
#define SCROLL_LIMIT_LINES 360

/***** 物理演算マクロ群 *****/

// ACCEL
// ・重力加速度です。
#define ACCEL 9.8

// ELAPSED_TIME
// ・落下（ジャンプ含む）際の１サイクルでの経過時間です。
#define ELAPSED_TIME 0.03

/***** その他マクロ群 *****/

// TRUE,FALSE
// ・TRUEは真の値です。
// ・FALSEは偽の値です。
#define TRUE 1
#define FALSE 0
#define V0 -4.5
#define VA -4.5

// MAX_ENEMY
// ・１ステージにおける敵の最大数です。
#define MAX_ENEMY 30

/***** 構造体型定義 *****/

// SD(StageData)
// ・ステージの情を一元管理する構造体型です。
// ・stageはステージの外見を管理します。
// ・numberはステージの外見の番号を管理します。
// ・displayY,Xはステージの表示域の絶対座標を管理します。
#ifndef __TYPED_SD__
	#define __TYPED_SD__
	typedef struct{
		char stage[STAGE_LINES][STAGE_COLS];
		char number;
		int displayY,displayX;
	} SD;
#endif

// PD(PlayerData)
// ・プレイヤーの情報を一元管理する構造体型です。
// ・lookはプレイヤーの外見を管理します。
// ・backはプレイヤーの外見と重なった【ステージの情報】を管理します。
// ・y,xはプレイヤーのステージ内での絶対座標を管理します。
// ・varietyは敵キャラクタ専用で、敵の種類を管理します。
// ・numberはプレイヤーの外見の番号を管理します。
// ・directionはプレイヤーの外見の向きを管理します。
// ・upWallLineはプレイヤーの上側にある壁の縦軸絶対座標を管理します。
// ・upWallY,XはupWallLineを決める基準となったプレイヤーの添え字を管理します。
// ・downWallLineはプレイヤーの下側にある壁の縦軸絶対座標を管理します。
// ・downWallY,XはdownWallLineを決める基準となったプレイヤーの添え字を管理します。
// ・leftWallLineはプレイヤーの左側にある壁の横軸絶対座標を管理します。
// ・leftWallY,XはleftWallLineを決める基準となったプレイヤーの添え字を管理します。
// ・rightWallLineはプレイヤーの右側にある壁の横軸絶対座標を管理します。
// ・rightWallY,XはrightWallLineを決める基準となったプレイヤーの添え字を管理します。
// ・jumpCountはジャンプした回数を管理します。
// ・wallKickFlagはプレイヤーがleftWallLineかrightWallLineに接触した状態かどうかを管理します。
// ・fallFlagはプレイヤーが空中にいる状態かどうかを管理します。
// ・fallTimeはfallFlagがtrueになってから経過した時間を管理します。
// ・v0はジャンプなどの落下運動をした際の初速度を管理します。
// ・vAはジャンプした際の初期補正値を管理します。
// ・vXはジャンプした際の横軸方向の移動係数を管理します。
// ・aXは着地時の移動係数を管理します。
// ・hitPointは残り体力を管理します。
// ・maxHitPointは最大体力を管理します。
// ・attackPowerは基礎攻撃力を管理します。
// ・staminaはプレイヤー専用で、行動力を管理します。
// ・maxStaminaはプレイヤー専用で、最大行動力を管理します。
// ・chargeGaugeはプレイヤー専用で、チャージ量を管理します。
// ・deathFlagは死亡状態もしくは最初から居ない状態を管理します。
#ifndef __TYPED_PD__
	#define __TYPED_PD__
	typedef struct{
		int look[PLAYER_LINES][PLAYER_COLS];
		int back[PLAYER_LINES][PLAYER_COLS];
		int y,x;
		char variety;
		char number;
		char direction;
		int upWallLine,upWallY,upWallX;
		int downWallLine,downWallY,downWallX;
		int leftWallLine,leftWallY,leftWallX;
		int rightWallLine,rightWallY,rightWallX;
		char jumpCount;
		char wallKickFlag;
		char fallFlag;
		float fallTime;
		float v0,vA,vX,aX;
		int hitPoint,maxHitPoint;
		int attackPower;
		int stamina,maxStamina;
		int chargeGauge;
		char deathFlag;
	} PD;
#endif

// ID(ItemData)
// ・ステージ上のあらゆるイベント（アイテム）の情報を一元管理する構造体型です。
// ・itemはアイテムの外見を管理します。
// ・backはアイテムの外見と重なった【ステージの情報】を管理します。
// ・y,xはプレイヤーのステージ内での絶対座標を管理します。
// ・varietyはアイテムの種類を管理します。
// ・numberはアイテムの外見の番号を管理します。
#ifndef __TYPED_ID__
	#define __TYPED_ID__
	typedef struct{
		int item[ITEM_LINES][ITEM_COLS];
		int back[ITEM_LINES][ITEM_COLS];
		int y,x;
		char variety;
		char number;
	} ID;
#endif

