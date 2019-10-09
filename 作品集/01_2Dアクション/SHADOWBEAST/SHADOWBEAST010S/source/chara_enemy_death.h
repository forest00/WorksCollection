#pragma once
//敵が死亡したときの塵エフェクトを出します
#include "chara__common.h"
#include "chara_point.h"



/*
敵が死んだときのエフェクトを出す関数
@param	type	敵の識別ID
@param	x		敵のx座標
@param	y		敵のy座標
@return			0...成功 -1...失敗
*/
int enemyDeadEffectSet(EnemyType type, double x, double y, Direction dir);

/*
敵死亡時のエフェクトを出すための準備をする関数
この関数を呼び出さないと他の関数は無意味になります
@return			0...成功 -1...失敗
*/
int enemyDeadEffectInit();

/*
敵死亡時のエフェクトを更新する関数
呼び出すたびにエフェクトが1フレーム分だけ動きます
@return			0...成功 -1...失敗
*/
int enemyDeadEffectUpdate();

/*
敵死亡時のエフェクトを描画する関数
@return			0...成功 -1...失敗
*/
int enemyDeadEffectDraw();

/*
敵死亡時のエフェクトの使用をやめる関数
@return			0...成功 -1...失敗
*/
int enemyDeadEffectEnd();









