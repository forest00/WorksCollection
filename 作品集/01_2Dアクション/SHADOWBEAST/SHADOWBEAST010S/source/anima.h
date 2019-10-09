#pragma once
/**
 * @file  anima.h
 * @brief アニメーション制御
 *
 * @date 2016-06-16
 * @version 3.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */
#include "image_manager.h"

 ///< アニメーション用構造体
typedef struct {
	unsigned int time;			///< 現在時刻
	unsigned int started;		///< 開始時刻
	unsigned int duration;		///< 持続時間
	unsigned int limit;			///< コマ数制限
} Anima;

typedef struct {
	unsigned int duration;		///< 持続時間
	unsigned int limit;			///< コマ数制限
} AnimaData;


/**
アニメデータの作成
*/
int createAnimaData(AnimaData *outPut, ImageFileId file_id);
/**
 *  アニメ構造体の設定
 *
 *  @param anima アニメ構造体へのポインタ
 *  @param started 開始時刻
 *  @param anima_data アニメデータ構造体へのポインタ
 */
void animaSetup(Anima *anim, unsigned int started, const AnimaData* anima_data);
/**
*  アニメーションコマ数の取得
*
*  @param anima アニメ構造体へのポインタ
*  @retval frame 現在のコマ数
*/
unsigned int animaGetFrame(const Anima *anim);
/**
 *  アニメーション回数の取得
 *
 *  @param anima アニメ構造体へのポインタ
 *  @retval count アニメーションを繰り返した回数
 */
unsigned int animaGetCount(const Anima *anim);
/**
 *  時刻に対応した値の取得
 *  @param anima アニメーション構造体へのポインタ
 *  @retval value 時刻に対応した値
 */
unsigned int animaGetValueStep(const Anima *anim);
