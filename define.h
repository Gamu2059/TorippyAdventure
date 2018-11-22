/*******************************************************************************
 * ***** define *****
 *
 * 定義しているもの
 * ・全ての定数。
 * 備考
 * ・このヘッダファイルはマクロ定義のみを行う特殊なものです。
 * ・必ず、【すべてのソースファイルおよびヘッダファイル】に
 *   このヘッダファイルをインクルードして下さい。
 *   *******************************************************************************/

/*
 * void setColorPair(void)
 * ・カラーペアを設定するための関数です。
 * ・メインプログラムを始める前に、一回以上呼び出して下さい。
 * ・引数
 * なし
 * ・戻り値
 * なし
 * */
extern void setColorPair(void);
extern void debug(void);

/***** 領域マクロ群 *****/

/*
 * CHARACTER
 * ・キャラクタの最大サイズです。
 * ・LINESは縦幅です。
 * ・COLSは横幅です。
 * */
#define CHARACTER_LINES 100
#define CHARACTER_COLS  100

/*
 * ITEM
 * ・アイテムの最大サイズです。
 * ・LINESは縦幅です。
 * ・COLSは横幅です。
 * */
#define ITEM_LINES 100
#define ITEM_COLS  100

/*
 * STAGE
 * ・ステージの最大サイズです。
 * ・LINESは縦幅です。
 * ・COLSは横幅です。
 * */
#define STAGE_LINES 400
#define STAGE_COLS  1500

/*
 * OPENNING
 * ・オープニング画面を表示する際の最大サイズです。
 * ・LINESは縦幅です。
 * ・COLSは横幅です。
 * */
#define OPENNING_LINES 200
#define OPENNING_COLS  320

/*
 * DISPLAY
 * ・ステージを表示する際の最大サイズです。
 * ・LINESは縦幅です。
 * ・COLSは横幅です。
 * */
#define DISPLAY_LINES 202
#define DISPLAY_COLS  260

/*
 * SCROLL
 * ・表示域を移動させるタイミングとなる最大幅です。
 * ・LINESは縦幅です。
 * ・COLSは横幅です。
 * */
#define SCROLL_LINES 20
#define SCROLL_COLS  100

/*
 * SCROLL_LIMIT
 * ・下方向にスクロールする際の限界となるラインです。
 * */
#define SCROLL_LIMIT_LINES 350

/***** 行動状態マクロ群 *****/

#define NO_ACTION 0
#define WALK 1
#define JUMP 2
#define JUMP_UP 3
#define JUMP_DOWN 4
#define N_ATTACK1 5
#define N_ATTACK2 6
#define S_ATTACK 7

/***** 物理演算マクロ群 *****/

/*
 * ACCEL
 * ・重力加速度です。
 * */
#define ACCEL 9.8

/*
 * ELAPSED_TIME
 * ・落下（ジャンプ含む）際の１サイクルでの経過時間です。
 * */
#define ELAPSED_TIME 0.05

/***** その他マクロ群 *****/

/*
 * TRUE,FALSE
 * ・TRUEは真の値です。
 * ・FALSEは偽の値です。
 * */
#define TRUE 1
#define FALSE 0

/*
 * RIGHT,LEFT
 * ・RIGHTは右向きの時の値です。
 * ・LEFTは右向きの時の値です。
 * */
#define RIGHT 1
#define LEFT -1

/*
 * MAX
 * ・MAX_ITEMは１ステージにおける【単独で存在する】アイテムの最大数です。
 * ・MAX_HAVEは１キャラクタにおける【キャラクタが所持する】アイテムの最大数です。
 * ・MAX_ENEMYは１ステージにおける敵の最大数です。
 * */
#define MAX_ITEM 5
#define MAX_HAVE 1
#define MAX_ENEMY 10

/*
 * DAMAGE
 * ・DAMAGEは一回のヒットで食らうダメージ数です。
 * */
#define DAMAGE 10

/***** 構造体型定義 *****/
/*
 * SD(StageData)
 * ・ステージの情報を一元管理する構造体型です。
 * ・stageはステージの外見を管理します。
 * ・numberはステージの外見の番号を管理します。
 * ・displayY,Xはステージの表示域の絶対座標を管理します。
 * ・minDisplayY,Xはステージの表示域において、上側と左側の限界スクロール域を管理します。
 * ・maxDisplayY,Xはステージの表示域において、下側と右側の限界スクロール域を管理します。
 * */
#ifndef __TYPED_SD__
	#define __TYPED_SD__
	typedef struct{
		int stage[STAGE_LINES][STAGE_COLS];
		char number;
		int displayY,displayX;
		int minDisplayY,minDisplayX;
		int maxDisplayY,maxDisplayX;
	} SD;
#endif

/*
 * ID(ItemData)
 * ・ステージ上のあらゆるイベント（アイテム）の情報を一元管理する構造体型です。
 * ・itemはアイテムの外見を管理します。
 * ・backはアイテムの外見と重なった【ステージの情報】を管理します。
 * ・y,xはアイテムのステージ内での絶対座標を管理します。
 * ・varietyはアイテムの種類を管理します。
 * ・numberはアイテムの外見の番号を管理します。
 * ・directionはアイテムの向きを管理します。
 * ・upWallLineはアイテムの上側にある壁の縦軸絶対座標を管理します。
 * ・upWallY,XはupWallLineを決める基準となったアイテムの添え字を管理します。
 * ・downWallLineはアイテムの下側にある壁の縦軸絶対座標を管理します。
 * ・downWallY,XはdownWallLineを決める基準となったアイテムの添え字を管理します。
 * ・leftWallLineはアイテムの左側にある壁の横軸絶対座標を管理します。
 * ・leftWallY,XはleftWallLineを決める基準となったアイテムの添え字を管理します。
 * ・rightWallLineはアイテムの右側にある壁の横軸絶対座標を管理します。
 * ・rightWallY,XはrightWallLineを決める基準となったアイテムの添え字を管理します。
 * ・fallFlagはアイテムが空中にいる状態かどうかを管理します。
 * ・fallTimeはfallFlagがtrueになってから経過した時間を管理します。
 * ・v0はジャンプなどの落下運動をした際の初速度を管理します。
 * ・vAはジャンプした際の初期補正値を管理します。
 * ・vXはジャンプした際の横軸方向の移動係数を管理します。
 * ・a0はジャンプなどの落下運動をした際の初速度のアイテムごとの定数を管理します。
 * ・aAはジャンプした際の初期補正値のアイテムごとの定数を管理します。
 * ・aXは着地時のアイテムごとの【移動数】を管理します。
 * ・motionCountはアイテムのエフェクトが続く時間を管理します。
 * ・motionFlagはアイテムの行動状態を管理します。
 * ・motionAcceptはキャラクタの直前に受理された行動状態を管理します。
 * ・eventFlagはアイテムが【イベントとして処理されているかどうか】を管理します。
 * ・existFlagはアイテムが【ステージ上に存在しているかどうか】を管理します。
 * */
#ifndef __TYPED_ID__
	#define __TYPED_ID__
	typedef struct{
		int item[ITEM_LINES][ITEM_COLS];
		int back[ITEM_LINES][ITEM_COLS];
		int y,x;
		int acceptY,acceptX;
		char variety,number,direction,acceptDirection;
		int upWallLine,upWallY,upWallX;
		int downWallLine,downWallY,downWallX;
		int leftWallLine,leftWallY,leftWallX;
		int rightWallLine,rightWallY,rightWallX;
		char wallKickFlag,wallHeadFlag;
		char jumpCount;
		char fallFlag;
		float fallTime;
		float v0,vA,vX;
		float a0,aA,aX;
		int motionCount,motionFlag,motionAccept;
		char eventFlag,existFlag;
	} ID;
#endif

/*
 * CD(CharacterData)
 * ・キャラクタの情報を一元管理する構造体型です。
 * ・lookはキャラクタの外見を管理します。
 * ・backはキャラクタの外見と重なった【ステージの情報】を管理します。
 * ・y,xはキャラクタのステージ内での絶対座標を管理します。
 * ・varietyはキャラクタの種類を管理します。
 * ・numberはキャラクタの外見の番号を管理します。
 * ・directionはキャラクタの向きを管理します。
 * ・upWallLineはキャラクタの上側にある壁の縦軸絶対座標を管理します。
 * ・upWallY,XはupWallLineを決める基準となったキャラクタの添え字を管理します。
 * ・downWallLineはキャラクタの下側にある壁の縦軸絶対座標を管理します。
 * ・downWallY,XはdownWallLineを決める基準となったキャラクタの添え字を管理します。
 * ・leftWallLineはキャラクタの左側にある壁の横軸絶対座標を管理します。
 * ・leftWallY,XはleftWallLineを決める基準となったキャラクタの添え字を管理します。
 * ・rightWallLineはキャラクタの右側にある壁の横軸絶対座標を管理します。
 * ・rightWallY,XはrightWallLineを決める基準となったキャラクタの添え字を管理します。
 * ・jumpCountはジャンプした回数を管理します。
 * ・wallKickFlagはキャラクタがleftWallLineかrightWallLineに接触した状態かどうかを管理します。
 * ・wallHeadFlagはキャラクタがupWallLineかdownWallLineに接触した状態かどうかを管理します。
 * ・fallFlagはキャラクタが空中にいる状態かどうかを管理します。
 * ・fallTimeはfallFlagがtrueになってから経過した時間を管理します。
 * ・v0はジャンプなどの落下運動をした際の初速度を管理します。
 * ・vAはジャンプした際の初期補正値を管理します。
 * ・vXはジャンプした際の横軸方向の移動係数を管理します。
 * ・a0はジャンプなどの落下運動をした際の初速度のキャラクタごとの定数を管理します。
 * ・aAはジャンプした際の初期補正値のキャラクタごとの定数を管理します。
 * ・aXは着地時のキャラクタごとの【移動数】を管理します。
 * ・motionCountはキャラクタのエフェクトが続く時間を管理します。
 * ・motionFlagはキャラクタの行動状態を管理します。
 * ・motionAcceptはキャラクタの直前に受理された行動状態を管理します。
 * ・hitPointは残り体力を管理します。
 * ・maxHitPointは最大体力を管理します。
 * ・staminaは行動力を管理します。
 * ・maxStaminaは最大行動力を管理します。
 * ・chargeGaugeはチャージ量を管理します。
 * ・maxChargeGaugeは最大チャージ量を管理します。
 * ・deathFlagはキャラクタが【生存しているかどうか】を管理します。
 * ・existFlagはキャラクタが【ステージ上に存在しているかどうか】を管理します。
 * ・haveItemはキャラクタが各々保有できるアイテムを管理します。
 * */
#ifndef __TYPED_CD__
	#define __TYPED_CD__
	typedef struct{
		int look[CHARACTER_LINES][CHARACTER_COLS];
		int back[CHARACTER_LINES][CHARACTER_COLS];
		int y,x;
		int acceptY,acceptX;
		char variety,number,direction,acceptDirection;
		int upWallLine,upWallY,upWallX;
		int downWallLine,downWallY,downWallX;
		int leftWallLine,leftWallY,leftWallX;
		int rightWallLine,rightWallY,rightWallX;
		char jumpCount;
		char wallKickFlag,wallHeadFlag;
		char fallFlag;
		float fallTime;
		float v0,vA,vX;
		float a0,aA,aX;
		int motionCount, motionFlag,motionAccept;
		int hitPoint,maxHitPoint;
		int stamina,maxStamina;
		int chargeGauge,maxChargeGauge;
		char deathFlag,existFlag;
		ID have[MAX_HAVE];
	} CD;
#endif

