#pragma once

enum easing_type_t
{
	et_None,
	et_Linear,
	et_EaseInQuad,
	et_EaseOutQuad,
	et_EaseInOutQuad,
	et_EaseInCubic,
	et_EaseOutCubic,
	et_EaseInOutCubic,
	et_EaseInQuart,
	et_EaseOutQuart,
	et_EaseInOutQuart,
	et_EaseInQuint,
	et_EaseOutQuint,
	et_EaseInOutQuint,
	et_EaseInSine,
	et_EaseOutSine,
	et_EaseInOutSine,
	et_EaseInExpo,
	et_EaseOutExpo,
	et_EaseInOutExpo,
	et_EaseInCirc,
	et_EaseOutCirc,
	et_EaseInOutCirc,
	et_Max,
};

enum include_or_exclude_linear_t
{
	ioel_include_linear,
	ioel_exclude_linear,
};

/*
*@fn et_EaseInRandom
*@brief ランダムなイーズイン
*@param includion et_Linearを含めるか否か
*/
extern easing_type_t et_EaseInRandom(include_or_exclude_linear_t);

/*
*@fn et_EaseOutRandom
*@brief ランダムなイーズアウト
*@param includion et_Linearを含めるか否か
*/
extern easing_type_t et_EaseOutRandom(include_or_exclude_linear_t);

/*
*@fn et_EaseInOutRandom
*@brief ランダムなイーズインアウト
*@param includion et_Linearを含めるか否か
*/
extern easing_type_t et_EaseInOutRandom(include_or_exclude_linear_t);

/*
*@fn call_easing
*@brief イージングタイプに応じてイージング関数を呼び出す
*@param type イージングタイプ
*@param t カレントタイム
*@param b 開始値
*@param c 全体の変化量
*@param d デュレーション
*@return  計算結果
*/
extern double call_easing(easing_type_t type, double t, double b, double c, double d);
