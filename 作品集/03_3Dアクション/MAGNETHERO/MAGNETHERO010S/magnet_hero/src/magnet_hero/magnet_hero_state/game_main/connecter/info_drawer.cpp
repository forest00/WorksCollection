//�e�w�b�_�[
#include "info_drawer.h"
//���̑��̃w�b�_�[
#include "info_holder.h"
#include "..\fbx_object\wrench.h"
#include "..\camera\camera_manager.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"


//�v���[���[�̉e��`�悷��
extern void drawer_player_shadow_draw()
{
	//�e�̈ʒu�擾
	D3DXVECTOR3 position;
	holder_player_shadow_position_get(&position);
	//������Ƃ��炷
	position.y += 0.05f;
	//�e�̖@���擾
	D3DXVECTOR3 normal;
	holder_player_shadow_normal_get(&normal);
	if (normal == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		normal.y = 1.0f;
	}
	//���[���h�s��쐬
	D3DXMATRIX matrix_scale;			//�X�P�[�����O�s��
	D3DXMATRIX matrix_rotation;			//��]�s��
	D3DXMATRIX matrix_translation;		//���s�ړ��s��
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&matrix_translation, position.x, position.y, position.z);
	//D3DXMatrixLookAtLH(&matrix_rotation, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &normal, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixRotationX(&matrix_rotation, (float)(M_PI_2));
	D3DXMatrixScaling(&matrix_scale, 8.0f, 8.0f, 0.0f);
	D3DXMatrixMultiply(&world, &world, &matrix_scale);
	D3DXMatrixMultiply(&world, &world, &matrix_rotation);
	D3DXMatrixMultiply(&world, &world, &matrix_translation);
	//UV���W�p��
	D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//�`����o�^
	mylib_draw_board_3d(
		&world,
		&uv[0], 0xffffffff,
		&uv[1], 0xffffffff,
		&uv[2], 0xffffffff,
		&uv[3], 0xffffffff,
		texture_manager_get(ti_shade), mbm_alpha
	);
}

extern void drawer_player_magnet_hand()
{
	bool do_draw;
	time_t frame;
	D3DXVECTOR3 position;
	holder_player_magnet_hand_position_get(&do_draw, &frame, &position);
	if (do_draw)
	{
#if 0
		wrench_draw(
			&position,
			1.0f,
			frame,
			camera_manager_get_theta(),
			camera_manager_get_phi()
		);
#else
		ball_draw(
			&position,
			50.0f
		);
#endif
	}
}
