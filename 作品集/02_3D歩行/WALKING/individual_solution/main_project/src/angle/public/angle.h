#pragma once

//�p�x�Ɋւ���v�Z������֐���񋟂��܂�


//angle_t�^�ł�
//double�^�Ɠ����ł����A�p�x�������Ƃ��͂��������g�����Ƃɂ��Ă�������
typedef double angle_t;


//�}�C�i�X�΂���v���X�΂̐��l�ɂ������̂�Ԃ��܂�
extern angle_t angle_pi_to_pi(angle_t angle);
//�ڕW�̊p�x�ɋ߂Â������̂�Ԃ��܂�
extern angle_t angle_follow(angle_t angle, angle_t target, angle_t turning_force);
