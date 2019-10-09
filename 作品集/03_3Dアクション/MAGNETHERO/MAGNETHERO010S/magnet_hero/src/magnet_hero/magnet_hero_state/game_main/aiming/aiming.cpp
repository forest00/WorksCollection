//�e�w�b�_�[
#include "aiming.h"
///���̑��̃w�b�_�[
#include <d3d9.h>
#include <d3dx9.h>

//����̈ʒu�x�N�g��
static D3DXVECTOR3 default_vector()
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(default_aiming_cursor_pos(), default_aiming_cursor_pos(), default_aiming_cursor_pos());
	return pos;

}

//�����Ȉʒu�x�N�g��
static D3DXVECTOR3 invalid_vector()
{
	static D3DXVECTOR3 pos = D3DXVECTOR3(invalid_aiming_cursor_pos(), invalid_aiming_cursor_pos(), invalid_aiming_cursor_pos());
	return pos;
}


//�O���[�o���ϐ�
struct global_t
{
	D3DXVECTOR3 _pos;				//�ʒu
	D3DXVECTOR3 _target;			//�^�[�Q�b�g�̈ʒu
	D3DXVECTOR3 _true_target;		//�ڎw���ׂ��ʒu
	bool _prev_invalid;				//1�t���[���O���������������ǂ���
};
static global_t g;

extern float default_aiming_cursor_pos()
{
	return 0.0f;
}

extern float invalid_aiming_cursor_pos()
{
	return 1024.0f;
}

extern void aiming_cursor_pos_init()
{
	g._pos = default_vector();
	g._target = default_vector();
	g._true_target = default_vector();

}

extern void aiming_cursor_pos_update(float target_x, float target_y, float target_z)
{
	g._target = D3DXVECTOR3(target_x, target_y, target_z);
	if (g._target == invalid_vector())
	{
		g._true_target = default_vector();
		g._prev_invalid = true;
	}
	else
	{
		if (g._prev_invalid)
		{
			//�G���[
		}
		g._true_target = g._target;
		g._prev_invalid = false;
	}
	D3DXVec3Lerp(&g._pos, &g._pos, &g._true_target, 0.095);
}

extern void aiming_cursor_get_pos(float *x, float *y, float *z)
{
	if (x)
	{
		*x = g._pos.x;
	}
	if (y)
	{
		*y = g._pos.y;
	}
	if (z)
	{
		*z = g._pos.z;
	}
}

extern void aiming_cursor_get_target_pos(float *x, float *y, float *z)
{
	if (x)
	{
		*x = g._target.x;
	}
	if (y)
	{
		*y = g._target.y;
	}
	if (z)
	{
		*z = g._target.z;
	}
}