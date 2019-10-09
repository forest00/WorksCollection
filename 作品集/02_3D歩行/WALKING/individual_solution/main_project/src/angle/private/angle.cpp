
//親ヘッダー
#include "..\public\angle.h"
//その他のヘッダー
#include "..\..\math_with_defines\math_with_defines.h"


//マイナスπからプラスπの数値にしたものを返します
extern angle_t angle_pi_to_pi(angle_t angle)
{
	angle_t answer = angle;
	const double c_2_pi = M_PI * 2.0;
	const double fl = floor((angle + M_PI) / c_2_pi);
	answer -= c_2_pi * fl;
	return answer;
}

//目標の角度に近づけたものを返します
extern angle_t angle_follow(angle_t angle, angle_t target, angle_t turning_force)
{
	angle_t sub = target - angle;
	sub = angle_pi_to_pi(sub);
	if (fabs(sub) <= turning_force)
	{
		return angle_pi_to_pi(target);
	}
	else
	{
		if (sub >= 0.0)
		{
			return angle_pi_to_pi(angle + turning_force);
		}
		else
		{
			return angle_pi_to_pi(angle - turning_force);
		}
	}
}


