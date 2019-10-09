//親ヘッダー
#include "easing_type.h"
//その他のヘッダー
#include "easing.h"
#include <stdlib.h>


/*
*@fn et_EaseInRandom
*@brief ランダムなイーズイン
*@param includion et_Linearを含めるか否か
*/
extern easing_type_t et_EaseInRandom(include_or_exclude_linear_t includion)
{
	static easing_type_t data[] = {
		et_EaseInQuad,
		et_EaseInCubic,
		et_EaseInQuart,
		et_EaseInQuint,
		et_EaseInSine,
		et_EaseInExpo,
		et_EaseInCirc,
		et_Linear,
	};
	bool exclude_linear = includion == ioel_exclude_linear;
	size_t index_max = _countof(data) - (int)(exclude_linear);
	size_t index = rand() % index_max;
	return data[index];
}

/*
*@fn et_EaseOutRandom
*@brief ランダムなイーズアウト
*@param includion et_Linearを含めるか否か
*/
extern easing_type_t et_EaseOutRandom(include_or_exclude_linear_t includion)
{
	static easing_type_t data[] = {
		et_EaseOutQuad,
		et_EaseOutCubic,
		et_EaseOutQuart,
		et_EaseOutQuint,
		et_EaseOutSine,
		et_EaseOutExpo,
		et_EaseOutCirc,
		et_Linear,
	};
	bool exclude_linear = includion == ioel_exclude_linear;
	size_t index_max = _countof(data) - (int)(exclude_linear);
	size_t index = rand() % index_max;
	return data[index];
}

/*
*@fn et_EaseInOutRandom
*@brief ランダムなイーズインアウト
*@param includion et_Linearを含めるか否か
*/
extern easing_type_t et_EaseInOutRandom(include_or_exclude_linear_t includion)
{
	static easing_type_t data[] = {
		et_EaseInOutQuad,
		et_EaseInOutCubic,
		et_EaseInOutQuart,
		et_EaseInOutQuint,
		et_EaseInOutSine,
		et_EaseInOutExpo,
		et_EaseInOutCirc,
		et_Linear,
	};
	bool exclude_linear = includion == ioel_exclude_linear;
	size_t index_max = _countof(data) - (int)(exclude_linear);
	size_t index = rand() % index_max;
	return data[index];
}

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
extern double call_easing(easing_type_t type, double t, double b, double c, double d)
{
	switch (type)
	{
	case et_None:
		return 0.0;
		break;
	case et_Linear:
		return Linear(t, b, c, d);
		break;
	case et_EaseInQuad:
		return EaseInQuad(t, b, c, d);
		break;
	case et_EaseOutQuad:
		return EaseOutQuad(t, b, c, d);
		break;
	case et_EaseInOutQuad:
		return EaseInOutQuad(t, b, c, d);
		break;
	case et_EaseInCubic:
		return EaseInCubic(t, b, c, d);
		break;
	case et_EaseOutCubic:
		return EaseOutCubic(t, b, c, d);
		break;
	case et_EaseInOutCubic:
		return EaseInOutCubic(t, b, c, d);
		break;
	case et_EaseInQuart:
		return EaseInQuart(t, b, c, d);
		break;
	case et_EaseOutQuart:
		return EaseOutQuart(t, b, c, d);
		break;
	case et_EaseInOutQuart:
		return EaseInOutQuart(t, b, c, d);
		break;
	case et_EaseInQuint:
		return EaseInQuint(t, b, c, d);
		break;
	case et_EaseOutQuint:
		return EaseOutQuint(t, b, c, d);
		break;
	case et_EaseInOutQuint:
		return EaseInOutQuint(t, b, c, d);
		break;
	case et_EaseInSine:
		return EaseInSine(t, b, c, d);
		break;
	case et_EaseOutSine:
		return EaseOutSine(t, b, c, d);
		break;
	case et_EaseInOutSine:
		return EaseInOutSine(t, b, c, d);
		break;
	case et_EaseInExpo:
		return EaseInExpo(t, b, c, d);
		break;
	case et_EaseOutExpo:
		return EaseOutExpo(t, b, c, d);
		break;
	case et_EaseInOutExpo:
		return EaseInOutExpo(t, b, c, d);
		break;
	case et_EaseInCirc:
		return EaseInCirc(t, b, c, d);
		break;
	case et_EaseOutCirc:
		return EaseOutCirc(t, b, c, d);
		break;
	case et_EaseInOutCirc:
		return EaseInOutCirc(t, b, c, d);
		break;
	case et_Max:
		return 0.0;
		break;
	default:
		return 0.0;
		break;
	}
}

