#pragma once

#include "type_parameter.h"

/**
 *  生存期間後の挙動
 */
typedef enum {
	REMOVE, ///< 外す
	LOOP,   ///< 繰り返す
	KEEP    ///< 保つ
} BehaviorAfter;
/**
 *  エフェクト
 */
typedef struct {
	bool active;	      ///< 活動中
	unsigned int delay;     ///< 遅延
	unsigned int lifetime;  ///< 生存期間
	BehaviorAfter behavior_after; ///< 生存期間後の挙動

	Parameter x, y;   ///< 座標
	Parameter alpha;  ///< アルファ
	Parameter angle;  ///< 回転角
	Parameter scale;  ///< 拡大率

	unsigned int blend;   ///< 合成方法
	int texture_w;///< 画像の幅
	int texture_h;///< 画像の高さ

	int handle;     ///< 画像ハンドル
	unsigned int start_time;///< 開始時刻

} Effect;

/**
 *  エフェクト管理の初期化
 */
void EffectMngInit();
/**
 *  エフェクト管理の更新
 */
void EffectMngUpdate();
/**
 *  エフェクト管理の描画
 */
void EffectMngRender();
/**
 *  エフェクトの取得
 *  @return 空きEffectへのポインタ
 */
Effect* GetEffect();
/**
 *  パラメータの設定
 *  @param prm    設定先パラメータへのポインタ
 *  @param easing イージング種別
 *  @param value  現在の値
 *  @param start  開始の値
 *  @param stop	  終了の値
 */
void ParameterSet(Parameter* prm, EasingType easing, double value, double start, double stop);

