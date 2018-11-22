//////////////////////////////////////////////////////////////////////////////
// ***** etc *****
//
// 定義しているもの
// ・ドット情報取得用関数。
// ・重力演算関数。
// 備考
// ・特になし
//////////////////////////////////////////////////////////////////////////////
#include "define.h"

// int getColorInfo(int)
// ・ドット情報から色情報を取得するための関数です。
// ・引数
//     object：情報を取得したい要素を指定します。
// ・戻り値
//     色情報
extern int getColorInfo(int object);

// int getCrashInfo(int)
// ・ドット情報から衝突判定情報を取得するための関数です。
// ・引数
//     object：情報を取得したい要素を指定します。
// ・戻り値
//     衝突判定情報
extern int getCrashInfo(int object);

// int getDamageHitInfo(int)
// ・ドット情報からダメージ当たり判定情報を取得するための関数です。
// ・引数
//     object：情報を取得したい要素を指定します。
// ・戻り値
//     ダメージ当たり判定情報
extern int getDamageHitInfo(int object);

// int getAttackHitInfo(int)
// ・ドット情報から攻撃当たり判定情報を取得するための関数です。
// ・引数
//     object：情報を取得したい要素を指定します。
// ・戻り値
//     攻撃当たり判定情報
extern int getAttackHitInfo(int object);

// float getGravityMove(PD*)
// ・重力によってプレイヤーがy軸方向に移動する距離を求めるための関数です。
// ・引数
//     pd：キャラクタのタグアドレスを指定します。
// ・戻り値
//     y軸方向に移動する距離
extern float getGravityMove(PD* pd);

// float getParabolaTopTime(PD*)
// ・ジャンプした際の最高到達点までに掛かる時間を求めるための関数です。
// ・引数
//     pd：キャラクタのタグアドレスを指定します。
// ・戻り値
//     最高到達点までに掛かる時間
extern float getParabolaTopTime(PD* pd);

