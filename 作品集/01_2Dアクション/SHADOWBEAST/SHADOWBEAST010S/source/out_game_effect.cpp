/** 
 * @file  effect_manager.cpp
 * �G�t�F�N�g�Ǘ�
 *
 * @date 2017-06-22
 * @version 1.0
 *
 * Copyright (C) Amusement Media Academy All rights Resved.
 */
#include "out_game_effect.h"
#include "game_state.h"
#include "amgame.h"

#define EFFECT_MAX 200000  ///< �G�t�F�N�g�̍ő吔

/**
 *  �G�t�F�N�g���X�g
 */
static Effect g_effect_list[EFFECT_MAX];

/**
 *  �G�t�F�N�g�̍X�V
 *  @param �G�t�F�N�g�ւ̃|�C���^
 */
static void EffectUpdate(Effect* ptc);
/**
 *  �G�t�F�N�g�̕`��
 *  @param �G�t�F�N�g�ւ̃|�C���^
 */
static void EffectRender(Effect* ptc);

/**
 *  �G�t�F�N�g�Ǘ��̏�����
 */
void EffectMngInit() {
  Effect zero = {0};
  for (unsigned int i = 0; i < EFFECT_MAX; i++) {
    g_effect_list[i] = zero;
  }
}
/**
 *  �G�t�F�N�g�Ǘ��̍X�V
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
 *  �G�t�F�N�g�Ǘ��̕`��
 */
void EffectMngRender() {
  // (3-1)�o�C���j�A�t�B���^�����O�ݒ�
    SetDrawMode(AMDRAW_BILINEAR, 0);
  for (unsigned int i = 0; i < EFFECT_MAX; i++) {
    Effect* ptc = &g_effect_list[i];
    if (ptc->active) {
      EffectRender(ptc);
    }
  }
  // (2-2.)�ݒ�߂�
  SetDrawMode(AMDRAW_NOBLEND, 0);
  // (3-2.)�ݒ�߂�
  SetDrawMode(AMDRAW_NEAREST, 0);
}
/**
 *  �G�t�F�N�g�̎擾
 *  g_effect_list��̋󂫗v�f�ւ̃|�C���^��Ԃ��B
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
 *  �G�t�F�N�g�̍X�V
 */
void EffectUpdate(Effect* ptc) {
  unsigned int current = getGameFrame();
  if ((current - ptc->start_time) < ptc->delay) {
    // �f�B���C��
    return;
  }
  // �o�ߎ��ԁ����ݎ����|(�f�B���C�{�J�n����)
  current -= (ptc->delay + ptc->start_time);

  // (4-2)�o�ߎ��� �� ��������
  if(current < ptc->lifetime){
      ParameterUpdate(&ptc->x, current, ptc->lifetime);
      ParameterUpdate(&ptc->y, current, ptc->lifetime);
      ParameterUpdate(&ptc->angle, current, ptc->lifetime);
      ParameterUpdate(&ptc->alpha, current, ptc->lifetime);
      ParameterUpdate(&ptc->scale, current, ptc->lifetime);




    return;
  }
  // (4.-3.)�������ԏI��
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
 *  �G�t�F�N�g�̕`��
 */
void EffectRender(Effect* ptc) {
  // (2-1)�A���t�@�u�����h�ݒ�
    SetDrawMode(ptc->blend, (unsigned int)ptc->alpha.current);
  // (1.)��]�{�g�k�`��
    DrawMemRot(ptc->texture_w / 2,
               ptc->texture_h / 2,
               (int)ptc->x.current,
               (int)ptc->y.current,
               ptc->angle.current,
               ptc->scale.current,
               ptc->handle);
}

/**
 *  �p�����[�^�̐ݒ�
 */
void ParameterSet(Parameter* prm,  EasingType easing,
                  double current, double start, double stop) {
  prm->easing = easing;
  prm->current = current;
  prm->start = start;
  prm->stop = stop;
}
