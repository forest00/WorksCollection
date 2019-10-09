/** 
 * @file  effect_manager.cpp
 * エフェクト管理
 *
 * @date 2017-06-22
 * @version 1.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */
#include "out_game_effect.h"
#include "game_state.h"
#include "amgame.h"

#define EFFECT_MAX 200000  ///< エフェクトの最大数

/**
 *  エフェクトリスト
 */
static Effect g_effect_list[EFFECT_MAX];

/**
 *  エフェクトの更新
 *  @param エフェクトへのポインタ
 */
static void EffectUpdate(Effect* ptc);
/**
 *  エフェクトの描画
 *  @param エフェクトへのポインタ
 */
static void EffectRender(Effect* ptc);

/**
 *  エフェクト管理の初期化
 */
void EffectMngInit() {
  Effect zero = {0};
  for (unsigned int i = 0; i < EFFECT_MAX; i++) {
    g_effect_list[i] = zero;
  }
}
/**
 *  エフェクト管理の更新
 */
void EffectMngUpdate() {
  for (unsigned int i = 0; i < EFFECT_MAX; i++) {
    Effect* ptc = &g_effect_list[i];
    if (ptc->active) {
      EffectUpdate(ptc);
    }
  }
}
/**
 *  エフェクト管理の描画
 */
void EffectMngRender() {
  // (3-1)バイリニアフィルタリング設定
    SetDrawMode(AMDRAW_BILINEAR, 0);
  for (unsigned int i = 0; i < EFFECT_MAX; i++) {
    Effect* ptc = &g_effect_list[i];
    if (ptc->active) {
      EffectRender(ptc);
    }
  }
  // (2-2.)設定戻す
  SetDrawMode(AMDRAW_NOBLEND, 0);
  // (3-2.)設定戻す
  SetDrawMode(AMDRAW_NEAREST, 0);
}
/**
 *  エフェクトの取得
 *  g_effect_list上の空き要素へのポインタを返す。
 */
Effect* GetEffect() {
  for (unsigned int i = 0; i < EFFECT_MAX; i++) {
    if (!g_effect_list[i].active) {
      return &g_effect_list[i];
    }
  }
  return nullptr;
}

/**
 *  エフェクトの更新
 */
void EffectUpdate(Effect* ptc) {
  unsigned int current = getGameFrame();
  if ((current - ptc->start_time) < ptc->delay) {
    // ディレイ中
    return;
  }
  // 経過時間＝現在時刻－(ディレイ＋開始時刻)
  current -= (ptc->delay + ptc->start_time);

  // (4-2)経過時間 ＜ 生存期間
  if(current < ptc->lifetime){
      ParameterUpdate(&ptc->x, current, ptc->lifetime);
      ParameterUpdate(&ptc->y, current, ptc->lifetime);
      ParameterUpdate(&ptc->angle, current, ptc->lifetime);
      ParameterUpdate(&ptc->alpha, current, ptc->lifetime);
      ParameterUpdate(&ptc->scale, current, ptc->lifetime);




    return;
  }
  // (4.-3.)生存期間終了
  switch (ptc->behavior_after) {
    case LOOP:
        ptc->start_time = getGameFrame();
        ptc->x.current = ptc->x.start;
        ptc->y.current = ptc->y.start;
        ptc->alpha.current = ptc->alpha.start;
        ptc->angle.current = ptc->angle.start;
        ptc->scale.current = ptc->scale.start;
      break;
    case REMOVE:
        ptc->active = false;
        break;
    case KEEP:
        ptc->x.current = ptc->x.stop;
        ptc->y.current = ptc->y.stop;
        ptc->alpha.current = ptc->alpha.stop;
        ptc->angle.current = ptc->angle.stop;
        ptc->scale.current = ptc->scale.stop;
      break;
    default:
      break;
  }
}
/**
 *  エフェクトの描画
 */
void EffectRender(Effect* ptc) {
  // (2-1)アルファブレンド設定
    SetDrawMode(ptc->blend, (unsigned int)ptc->alpha.current);
  // (1.)回転＋拡縮描画
    DrawMemRot(ptc->texture_w / 2,
               ptc->texture_h / 2,
               (int)ptc->x.current,
               (int)ptc->y.current,
               ptc->angle.current,
               ptc->scale.current,
               ptc->handle);
}

/**
 *  パラメータの設定
 */
void ParameterSet(Parameter* prm,  EasingType easing,
                  double current, double start, double stop) {
  prm->easing = easing;
  prm->current = current;
  prm->start = start;
  prm->stop = stop;
}
