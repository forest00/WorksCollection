#pragma once
//���z�̓��͂��󂯕t����ꏊ
//�L�[�{�[�h��}�E�X����̓��͂͂��炩���ߎ擾�ł��Ă��邱�Ƃ������ɂȂ�܂�
//�쐬�ҁ@�X�@�m�u

//���z����
struct virtual_input_t
{
	float _lstick_x;	//���X�e�B�b�N�̉��̌X��
	float _lstick_y;	//���X�e�B�b�N�̏c�̌X��
	float _rstick_x;	//�E�X�e�B�b�N�̉��̌X��
	float _rstick_y;	//�E�X�e�B�b�N�̏c�̌X��
	bool _lkey_left;	//WASD��
	bool _lkey_up;		//WASD��
	bool _lkey_right;	//WASD�E
	bool _lkey_down;	//WASD��
	bool _rkey_left;	//���L�[��
	bool _rkey_up;		//���L�[��
	bool _rkey_right;	//���L�[�E
	bool _rkey_down;	//���L�[��
	bool _a;			//A(����L�[)
	bool _b;			//B(�߂�L�[)
	bool _pause;		//�|�[�Y�L�[
	char _padding;		//�p�f�B���O
};

//���z�̓��͂��󂯕t���܂�
//repeat�ɂ͍��t���[���̓��͂�����܂�
//trigger�ɂ͉������u�Ԃ̓��͂�����܂�
//release�ɂ͗������u�Ԃ̓��͂�����܂�
extern void virtual_input_update(virtual_input_t *repeat, virtual_input_t *trigger, virtual_input_t *release);


