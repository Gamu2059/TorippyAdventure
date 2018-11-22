/*******************************************************************************
 * ***** etc *****
 *
 * 定義しているもの
 * ・ドット情報取得用関数。
 * ・数学演算関数。
 * 備考
 * ・特になし
 * *******************************************************************************/
#include "define.h"

extern int getColorInfo(int object);
extern char getGradationInfo(int object);
extern int getPriorityInfo(int object);
extern int getHitInfo(int object);
extern int getCrashInfo(int object);
extern float getGravityMove(float v0,float vA,float fallTime);
extern float getParabolaTopTime(float v0);
extern float getSqrtDistant(int y1,int x1,int y2,int x2);

