
//�e�w�b�_�[
#include "..\public\angle.h"
//���̑��̃w�b�_�[
#include "..\..\math_with_defines\math_with_defines.h"


//�}�C�i�X�΂���v���X�΂̐��l�ɂ������̂�Ԃ��܂�
extern angle_t angle_pi_to_pi(angle_t angle)
{
	angle_t answer = angle;
	const double c_2_pi = M_PI * 2.0;
	const double fl = floor((angle + M_PI) / c_2_pi);
	answer -= c_2_pi * fl;
	return answer;
}

//�ڕW�̊p�x�ɋ߂Â������̂�Ԃ��܂�
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


