//�e�w�b�_�[
#include "golem.h"
//���̑��̃w�b�_�[
#include <stdlib.h>
#include "..\..\..\calc\public\calc.h"
#include "..\..\public\anim_model_base.h"
#include "..\..\public\anim_data_loader.h"
#include "..\..\..\font\public\font.h"
#include "..\..\..\input\public\input.h"
#include "..\..\..\loop\public\loop.h"
#include "..\..\..\world_transformation\public\world_transformation.h"
#include "..\..\..\math_with_defines\math_with_defines.h"
#include "..\..\..\camera\public\camera.h"
#include "..\..\..\sound\public\sound.h"
//�f�o�b�O�p�w�b�_�[
#include "..\..\..\game_debug\public\message_box.h"



#define GOLEM_CAPACITY	16


//golem�\����
struct golem_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//golem_t�̏���������
static void golem_t_init(golem_t *golem)
{
	golem->_is_active = false;
	golem->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	golem->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	golem->_base._scale = vector3_t(0.01f, 0.01f, 0.01f);
	anim_init(&golem->_anim);
}

//golem_t�̏I������
static void golem_t_end(golem_t *golem)
{
	anim_end(&golem->_anim);
}

//golem_t���A�N�e�B�u�ɂ���
static HRESULT golem_t_to_active(golem_t *golem, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// �S�����o�[�ɏ����l����
	golem_t_init(golem);
	//�A�N�e�B�u�ɂ���
	golem->_is_active = true;
	//�����ʒu�ɓ���
	golem->_base._position = *pvPosition;
	//�A�j���[�V�������擾
	anim_setup(&golem->_anim, anim_kind);
	anim_play(&golem->_anim, 0, anim_repeat_off);
	golem->_anim._playing = true;

	return S_OK;
}


//golem_t��1�t���[�����̍X�V
static void golem_t_update(golem_t *golem)
{
	//�A�N�e�B�u���`�F�b�N
	if (golem->_is_active == false)
	{
		return;
	}
	//���삵�₷���悤�Ƀ|�C���^���擾���Ă���
	thing_parameter_t *base = &golem->_base;
	anim_parameter_t *anim = &golem->_anim;
	//�A�j���[�V�����؂�ւ�����
	{
		if (input_keyboard_trigger(DIK_F1))
		{
			anim_play(anim, 0, anim_repeat_off);
		}
		if (input_keyboard_trigger(DIK_F2))
		{
			anim_play(anim, 1, anim_repeat_off);
		}
		if (input_keyboard_trigger(DIK_F3))
		{
			anim_play(anim, 2, anim_repeat_off);
		}
		if (input_keyboard_trigger(DIK_F4))
		{
			anim_play(anim, 3, anim_repeat_on);
		}
		if (input_keyboard_trigger(DIK_F5))
		{
			anim_play(anim, 4, anim_repeat_on);
		}
	}
	//�A�j���[�V�����Đ��ƒ�~�𐧌�
	{
		if (input_keyboard_trigger(DIK_NUMPAD1))
		{
			anim->_playing = true;
		}
		if (input_keyboard_trigger(DIK_NUMPAD2))
		{
			anim->_playing = false;
		}
		if (input_keyboard_trigger(DIK_NUMPAD3))
		{
			anim->_playing = bool_invers(anim->_playing);
		}
	}
	//�A�j���[�V������i�߂�
	{
		anim_advance(
			anim,
			loop_time(),
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		);
	}
}



//golem_t��1�t���[�����̍X�V(�v���C���[�p)
static void golem_t_update_as_player(golem_t *golem)
{
	//�A�N�e�B�u���`�F�b�N
	if (golem->_is_active == false)
	{
		return;
	}
	//���삵�₷���悤�Ƀ|�C���^���擾���Ă���
	thing_parameter_t *base = &golem->_base;
	anim_parameter_t *anim = &golem->_anim;
	//���͂ɂ�蓮��
	{
		double input_x = input_controller_left_x() + input_keyboard_repeat(DIK_RIGHT) - input_keyboard_repeat(DIK_LEFT);
		double input_y = input_controller_left_y() + input_keyboard_repeat(DIK_DOWN) - input_keyboard_repeat(DIK_UP);
		bool input_fire = 
			input_controller_trigger(input_controller_code_a) ||
			input_controller_trigger(input_controller_code_b) || 
			input_keyboard_trigger(DIK_Z);
		bool input_on = (fabs(input_x) > 0.1) || (fabs(input_y) > 0.1);
		double input_angle = atan2(input_y, input_x);
		angle_t camera_theta = 0.0;
		camera_get_theta(&camera_theta);
		double go_angle = input_angle - camera_theta;
		if (anim->_playing_index != 2)
		{
			if (input_on)
			{
				base->_position.x -= cos(go_angle + M_PI_2) / 8.0;
				base->_position.z += sin(go_angle + M_PI_2) / 8.0;
				base->_angle.y = go_angle;
				if (anim->_playing_index != 3)
				{
					sound_play(sound_index_se_3);
					anim_play(anim, 3, anim_repeat_on);
				}
			}
			else
			{
				if (anim->_playing_index != 4)
				{
					anim_play(anim, 4, anim_repeat_on);
				}
			}
			if (input_fire)
			{
				sound_play(sound_index_se_4);
				anim_play(anim, 2, anim_repeat_off);
			}
		}
	}
	//�A�j���[�V������i�߂�
	{
		bool ended = false;
		anim_advance(
			anim,
			loop_time() * 2.0,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			&ended
		);
		if (anim->_playing_index == 2 && ended)
		{
			anim_play(anim, 3, anim_repeat_on);
		}
	}
}



//golem_t��`��
static void golem_t_draw(golem_t *golem, IDirect3DDevice9 *device)
{
	if (golem->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&golem->_base._position,
		&golem->_base._angle,
		&golem->_base._scale
	);
	anim_thing_draw(device, &world, &golem->_anim);
}





//golem�̃n���h���Ƃ��ċ@�\����\����
struct golem_handle_t
{
	void *_ptr;
};

//�n���h���𐶐�����
void golem_handle_t_create(golem_handle_t *handle, golem_t *object)
{
	handle->_ptr = (void*)(object);
}

//�n���h��������̂ւ̃|�C���^���擾
golem_t *golem_handle_t_get(golem_handle_t handle)
{
	return (golem_t*)(handle._ptr);
}


//golem�̏W�����Ǘ�����\����
struct golem_manager_t
{
	//�Ƃ肠�����z��(�����͊�{�Œ�)
	golem_t *_array_head;
	size_t _capacity;
	//�v���C���[
	golem_t _player_golem;
};




//golem_manager_t�̏���������
void golem_manager_t_init(golem_manager_t *manager, size_t capacity)
{
	manager->_array_head = (golem_t*)malloc(sizeof(golem_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"�������m�ۂɎ��s���܂���\n�������T�C�Y : %ld",
			sizeof(golem_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		golem_t_init(&manager->_array_head[i]);
	}
	//�v���C���[
	golem_t_init(&manager->_player_golem);
	golem_t_to_active(&manager->_player_golem, anim_model_kind_golem, &D3DXVECTOR3(30.0f, 0.0f, -30.0f));
}

//golem_manager_t�̏I������
void golem_manager_t_end(golem_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		golem_t_end(&manager->_array_head[i]);
	}
	//�v���C���[
	golem_t_end(&manager->_player_golem);
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//golem_manager_t��ʂ���golem_t��1�̃A�N�e�B�u�ɂ���
golem_handle_t golem_manager_t_add_golem(golem_manager_t *manager, D3DXVECTOR3 *position)
{
	golem_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			golem_t_to_active(&manager->_array_head[i], anim_model_kind_golem, position);
			golem_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	golem_handle_t_create(&creature, nullptr);
	return creature;
}

//golem_manager_t���Ǘ����Ă��邷�ׂĂ�golem_t���X�V
void golem_manager_t_update(golem_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		golem_t_update(&manager->_array_head[i]);
	}
	//�v���C���[
	golem_t_update_as_player(&manager->_player_golem);
}

//golem_manager_t���Ǘ����Ă��邷�ׂĂ�golem_t��`��
void golem_manager_t_draw(golem_manager_t *manager, IDirect3DDevice9 *device)
{
	///�ݒ��ۑ����Ƃ�
	static DWORD option[7];
	device->GetRenderState(D3DRS_ALPHABLENDENABLE, &option[0]);
	device->GetRenderState(D3DRS_SRCBLEND, &option[1]);
	device->GetRenderState(D3DRS_DESTBLEND, &option[2]);
	device->GetRenderState(D3DRS_ALPHATESTENABLE, &option[3]);
	device->GetTextureStageState(0, D3DTSS_ALPHAOP, &option[4]);
	device->GetTextureStageState(0, D3DTSS_COLORARG1, &option[5]);
	device->GetTextureStageState(0, D3DTSS_COLORARG2, &option[6]);
	///�ݒ��ς���
	//�A���t�@�u�����f�B���O��ݒ肷��
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	// �e�N�X�`���̃u�����f�B���O���@���`����
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	//���C���`��
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		golem_t_draw(&manager->_array_head[i], device);
	}
	//�v���C���[
	golem_t_draw(&manager->_player_golem, device);
	///�ݒ�����ɖ߂�
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, option[0]);
	device->SetRenderState(D3DRS_SRCBLEND, option[1]);
	device->SetRenderState(D3DRS_DESTBLEND, option[2]);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, option[3]);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, option[4]);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, option[5]);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, option[6]);
}













static golem_manager_t *g_golem_manager;






//golem������ & �o��
extern void golem_init()
{
	g_golem_manager = (golem_manager_t*)malloc(sizeof(golem_manager_t));
	golem_manager_t_init(g_golem_manager, GOLEM_CAPACITY);
	//golem����̏o��
	//golem_manager_t_add_golem(g_golem_manager,
	//	&D3DXVECTOR3(
	//		uniform_rand(-20.0, 0.0),
	//		uniform_rand(0.0, 1.0),
	//		uniform_rand(0.0, 8.0)
	//	)
	//);
}

//golem�I��
extern void golem_end()
{
	golem_manager_t_end(g_golem_manager);
	safe_free(g_golem_manager);
}

//golem�X�V
extern void golem_update()
{
	//�����_����golem����̏o��
	//if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	//{
	//	golem_manager_t_add_golem(g_golem_manager,
	//		&D3DXVECTOR3(
	//			uniform_rand(-20.0, 0.0),
	//			uniform_rand(0.0, 1.0),
	//			uniform_rand(0.0, 8.0)
	//		)
	//	);
	//}
	golem_manager_t_update(g_golem_manager);
}

//golem�`��
extern void golem_draw(IDirect3DDevice9 *device)
{
	golem_manager_t_draw(g_golem_manager, device);
}

//���͂ɉ����ē����S�[�����̈ʒu�𓾂�
void golem_get_position_special(D3DXVECTOR3 *out)
{
	*out = g_golem_manager->_player_golem._base._position;
}

