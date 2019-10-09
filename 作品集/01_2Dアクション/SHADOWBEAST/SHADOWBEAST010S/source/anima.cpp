/**
 * @file  anima.cpp
 * アニメーション制御
 *
 * @date 2016-06-16
 * @version 3.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */

 // インクルード
#include "anima.h"
#include "image_manager.h"



/**
アニメデータの作成
*/
int createAnimaData(AnimaData *outPut, ImageFileId file_id){
	const BlockImage *f = getImageFile(file_id);
	*outPut = {(unsigned int)f->duration,
				(unsigned int)f->a_num};
	return 0;
}

/**
 *  アニメ構造体の設定
 */
void animaSetup(Anima *anim, unsigned int started, const AnimaData* anima_data) {
	anim->started = started;
	anim->duration = anima_data->duration;
	anim->limit = anima_data->limit;
}
/**
*  アニメーションコマ数の取得
*/
unsigned int animaGetFrame(const Anima *anim){
	double d = anim->duration / anim->limit;
	unsigned int frame = (unsigned int)((anim->time - anim->started) / d);
	return frame;
}
/**
 *  アニメーション回数の取得
 */
unsigned int animaGetCount(const Anima *anim) {
	unsigned int count = (anim->time - anim->started) / anim->duration;
	return count;
}
/**
 *  時刻に対応した値の取得
 */
unsigned int animaGetValueStep(const Anima *anim) {
	// 現在の値を求める
	unsigned int index = (anim->time - anim->started) * anim->limit;

	index /= anim->duration;//int型同士の割り算　小数点以下が切り捨てられることを想定している
	// 値をループさせる
	index %= anim->limit;

	return index;
}
