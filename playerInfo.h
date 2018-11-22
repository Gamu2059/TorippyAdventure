/*******************************************************************************
 * ***** enemyInfo *****
 *
 * 定義しているもの
 * ・プレイヤーのドット情報。
 * ・プレイヤーの情報作成用関数。
 * 備考
 * ・特になし
 * *******************************************************************************/
#include "define.h"

extern void initializePlayer(CD* cd,int state,int direction);
extern void createPlayer(CD* cd,int number,int direction);
extern int getPlayerMoveNumber(CD* cd,int state);
extern void motionPlayer(CD* cd,SD* sd,CD* ed,ID* id);

