#pragma once

#include "chara__common.h"

/*
守るべき拠点をセットする
@param x ワールドx座標
@param y ワールドy座標
@return 0...成功 -1...失敗
*/
int setProtectTarget(double x, double y);

/*
お城のx座標を得る関数
@return お城のワールドx座標
*/
int getCastleX();

/*
お城にダメージを与える関数
@param target お城へのポインタ
@param damage ダメージ量
@return 0...成功 -1...城は無敵状態、ダメージは通らない
*/
int castleHitDamage(pCharacter target, int damage);

/*
アイテムを出す
@param vx		アイテムを吹っ飛ばすx方向への力
@param vy		アイテムを吹っ飛ばすy方向への力
@return 0...成功 -1...失敗
*/
int setItemOnCastle(double vx, double vy);

/*
城のHPを得る
@param hp		城の現在HPを格納する変数へのポインタ
@param maxHp	城の最大HPを格納する変数へのポインタ
*/
void getCastleHp(int *hp, int *maxHp);
