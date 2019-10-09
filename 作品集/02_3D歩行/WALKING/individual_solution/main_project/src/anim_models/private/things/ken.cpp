//�e�w�b�_�[
#include "ken.h"
//���̑��̃w�b�_�[
#include <stdlib.h>
#include "..\..\..\calc\public\calc.h"
#include "..\..\public\anim_model_base.h"
#include "..\..\public\anim_data_loader.h"
#include "..\..\..\font\public\font.h"
#include "..\..\..\input\public\input.h"
#include "..\..\..\loop\public\loop.h"
#include "..\..\..\world_transformation\public\world_transformation.h"
//�f�o�b�O�p�w�b�_�[
#include "..\..\..\game_debug\public\message_box.h"



#define KEN_CAPACITY	16


//ken�\����(ken�Ƃ�...?)
struct ken_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//ken_t�̏���������
static void ken_t_init(ken_t *ken)
{
	ken->_is_active = false;
	ken->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	ken->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	ken->_base._scale = vector3_t(1.0f, 1.0f, 1.0f);
	anim_init(&ken->_anim);
}

//ken_t�̏I������
static void ken_t_end(ken_t *ken)
{
	anim_end(&ken->_anim);
}

//ken_t���A�N�e�B�u�ɂ���
static HRESULT ken_t_to_active(ken_t *ken, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// �S�����o�[�ɏ����l����
	ken_t_init(ken);
	//�A�N�e�B�u�ɂ���
	ken->_is_active = true;
	//�����ʒu�ɓ���
	ken->_base._position = *pvPosition;
	//�A�j���[�V�������擾
	anim_setup(&ken->_anim, anim_kind);
	anim_play(&ken->_anim, 0, anim_repeat_off);
	ken->_anim._playing = true;

	return S_OK;
}


//ken_t��1�t���[�����̍X�V
static void ken_t_update(ken_t *ken)
{
	//�A�N�e�B�u���`�F�b�N
	if (ken->_is_active == false)
	{
		return;
	}
	//���삵�₷���悤�Ƀ|�C���^���擾���Ă���
	thing_parameter_t *base = &ken->_base;
	anim_parameter_t *anim = &ken->_anim;
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



//ken_t��`��
static void ken_t_draw(ken_t *ken, IDirect3DDevice9 *device)
{
	if (ken->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&ken->_base._position,
		&ken->_base._angle,
		&ken->_base._scale
	);
	anim_thing_draw(device, &world, &ken->_anim);
}





//ken�̃n���h���Ƃ��ċ@�\����\����
struct ken_handle_t
{
	void *_ptr;
};

//�n���h���𐶐�����
void ken_handle_t_create(ken_handle_t *handle, ken_t *object)
{
	handle->_ptr = (void*)(object);
}

//�n���h��������̂ւ̃|�C���^���擾
ken_t *ken_handle_t_get(ken_handle_t handle)
{
	return (ken_t*)(handle._ptr);
}


//ken�̏W�����Ǘ�����\����
struct ken_manager_t
{
	//�Ƃ肠�����z��(�����͊�{�Œ�)
	ken_t *_array_head;
	size_t _capacity;
};




//ken_manager_t�̏���������
void ken_manager_t_init(ken_manager_t *manager, size_t capacity)
{
	manager->_array_head = (ken_t*)malloc(sizeof(ken_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"�������m�ۂɎ��s���܂���\n�������T�C�Y : %ld",
			sizeof(ken_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		ken_t_init(&manager->_array_head[i]);
	}
}

//ken_manager_t�̏I������
void ken_manager_t_end(ken_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		ken_t_end(&manager->_array_head[i]);
	}
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//ken_manager_t��ʂ���ken_t��1�̃A�N�e�B�u�ɂ���
ken_handle_t ken_manager_t_add_ken(ken_manager_t *manager, D3DXVECTOR3 *position)
{
	ken_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			ken_t_to_active(&manager->_array_head[i], anim_model_kind_ken, position);
			ken_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	ken_handle_t_create(&creature, nullptr);
	return creature;
}

//ken_manager_t���Ǘ����Ă��邷�ׂĂ�ken_t���X�V
void ken_manager_t_update(ken_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		ken_t_update(&manager->_array_head[i]);
	}
}

//ken_manager_t���Ǘ����Ă��邷�ׂĂ�ken_t��`��
void ken_manager_t_draw(ken_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		ken_t_draw(&manager->_array_head[i], device);
	}
}













static ken_manager_t *g_ken_manager;






//ken������ & �o��
extern void ken_init()
{
	g_ken_manager = (ken_manager_t*)malloc(sizeof(ken_manager_t));
	ken_manager_t_init(g_ken_manager, KEN_CAPACITY);
	//ken����̏o��
	ken_manager_t_add_ken(g_ken_manager,
		&D3DXVECTOR3(
			uniform_rand(0.0, 12.0),
			uniform_rand(0.0, 1.0),
			uniform_rand(0.0, 8.0)
		)
	);
}

//ken�I��
extern void ken_end()
{
	ken_manager_t_end(g_ken_manager);
	safe_free(g_ken_manager);
}

//ken�X�V
extern void ken_update()
{
	//�����_����ken����̏o��
	if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	{
		ken_manager_t_add_ken(g_ken_manager,
			&D3DXVECTOR3(
				uniform_rand(0.0, 12.0),
				uniform_rand(0.0, 1.0),
				uniform_rand(0.0, 8.0)
			)
		);
	}
	ken_manager_t_update(g_ken_manager);
}

//ken�`��
extern void ken_draw(IDirect3DDevice9 *device)
{
	ken_manager_t_draw(g_ken_manager, device);
}


