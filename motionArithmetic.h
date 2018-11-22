/*******************************************************************************
 * ***** motionArithmetic *****
 *
 * 定義しているもの
 * ・衝突判定用関数。
 * ・最短にある障害物の座標を求める関数。
 * ・位置修正用関数。
 * ・ジャンプ演算関数。
 * 備考
 * ・特になし
 * *******************************************************************************/
#include "define.h"

extern char confirmDamageCharacter(SD*,CD*);
extern char confirmCrashCharacter(SD*,CD*);
extern char confirmCrashItem(SD*,ID*);
extern char reviseCharacter(CD*,SD*);
extern char reviseItem(ID*,SD*);
extern void modifyCharacter(CD*,SD*,char);
extern void modifyItem(ID*,SD*);
extern void getWallLineCharacter(SD*,CD*);
extern void getWallLineItem(SD*,ID*);
extern void modifyCrashCharacter(SD*,CD*);
extern void modifyCrashItem(SD*,ID*);

