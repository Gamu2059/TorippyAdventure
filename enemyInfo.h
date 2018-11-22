//////////////////////////////////////////////////////////////////////////////
// ***** enemyInfo *****
//
// 定義しているもの
// ・敵キャラクタのドット情報。
// ・敵キャラクタの情報作成用関数。
// 備考
// ・特になし
//////////////////////////////////////////////////////////////////////////////
#include "define.h"

// void initializeEnemy(PD*,int,int,int)
// ・敵キャラクタのドット情報を初期化するための関数です。
// ・引数
//     pd：敵キャラクタのタグアドレスを指定します。
//     variety：作成する敵キャラクタの種類の番号を指定します。
//     number：作成する敵キャラクタの絵の番号を指定します。
//     direction：向きを指定します。
//                1なら右向き、-1なら左向きを指定します。
// ・戻り値
//     なし
extern void initializeEnemy(PD* pd,int variety,int number,int direction);

// void createEnemy(PD*,int,int,int)
// ・敵キャラクタのドット情報を作成するための関数です。
// ・引数
//     pd：敵キャラクタのタグアドレスを指定します。
//     variety：作成する敵キャラクタの種類の番号を指定します。
//     number：作成する敵キャラクタの絵の番号を指定します。
//     direction：向きを指定します。
//                1なら右向き、-1なら左向きを指定します。
// ・戻り値
//     なし
extern void createEnemy(PD* pd,int variety,int number,int direction);

