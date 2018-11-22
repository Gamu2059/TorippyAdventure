/*******************************************************************************
 * ***** itemInfo *****
 *
 * 定義しているもの
 * ・アイテムのドット情報。
 * ・アイテムの情報作成用関数。
 * 備考
 * ・特になし
 * *******************************************************************************/
#include "define.h"

extern void initializeItem(ID* id,int variety,int state,int direction);
extern void createItem(ID* id,int variety,int number,int direction);
extern int getItemMoveNumber(ID* id,int variety,int state);
extern void motionItem(ID* item,SD* sd,CD* pd,CD* ed,ID* id);

