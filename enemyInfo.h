/*******************************************************************************
 * ***** enemyInfo *****
 *
 * 定義しているもの
 * ・敵キャラクタのドット情報。
 * ・敵キャラクタの情報作成用関数。
 * 備考
 * ・特になし
 * *******************************************************************************/
#include "define.h"

extern void initializeEnemy(CD* cd,int variety,int state,int direction);
extern void createEnemy(CD* cd,int variety,int number,int direction);
extern int getEnemyMoveNumber(CD* cd,int variety,int state);
extern void motionEnemy(CD* cd,SD* sd,CD* pd,CD* ed,ID* id);

