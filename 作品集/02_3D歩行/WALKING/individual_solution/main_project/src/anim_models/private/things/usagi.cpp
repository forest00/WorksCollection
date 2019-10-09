//�e�w�b�_�[
#include "usagi.h"
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



#define USAGI_CAPACITY	16


//usagi�\����
struct usagi_t
{
	bool _is_active;
	thing_parameter_t _base;
	anim_parameter_t _anim;
};

//usagi_t�̏���������
static void usagi_t_init(usagi_t *usagi)
{
	usagi->_is_active = false;
	usagi->_base._position = vector3_t(0.0f, 0.0f, 0.0f);
	usagi->_base._angle = vector3_t(0.0f, 0.0f, 0.0f);
	usagi->_base._scale = vector3_t(1.0f, 1.0f, 1.0f);
	anim_init(&usagi->_anim);
}

//usagi_t�̏I������
static void usagi_t_end(usagi_t *usagi)
{
	anim_end(&usagi->_anim);
}

//usagi_t���A�N�e�B�u�ɂ���
static HRESULT usagi_t_to_active(usagi_t *usagi, anim_model_kind_t anim_kind, D3DXVECTOR3 *pvPosition)
{
	// �S�����o�[�ɏ����l����
	usagi_t_init(usagi);
	//�A�N�e�B�u�ɂ���
	usagi->_is_active = true;
	//�����ʒu�ɓ���
	usagi->_base._position = *pvPosition;
	//�A�j���[�V�������擾
	anim_setup(&usagi->_anim, anim_kind);
	anim_play(&usagi->_anim, 0, anim_repeat_off);
	usagi->_anim._playing = true;

	return S_OK;
}


//usagi_t��1�t���[�����̍X�V
static void usagi_t_update(usagi_t *usagi)
{
	//�A�N�e�B�u���`�F�b�N
	if (usagi->_is_active == false)
	{
		return;
	}
	//���삵�₷���悤�Ƀ|�C���^���擾���Ă���
	thing_parameter_t *base = &usagi->_base;
	anim_parameter_t *anim = &usagi->_anim;
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



//usagi_t��`��
static void usagi_t_draw(usagi_t *usagi, IDirect3DDevice9 *device)
{
	if (usagi->_is_active == false)
	{
		return;
	}
	D3DXMATRIX world;
	world_transformation(
		&world,
		&usagi->_base._position,
		&usagi->_base._angle,
		&usagi->_base._scale
	);
	anim_thing_draw(device, &world, &usagi->_anim);
}





//usagi�̃n���h���Ƃ��ċ@�\����\����
struct usagi_handle_t
{
	void *_ptr;
};

//�n���h���𐶐�����
void usagi_handle_t_create(usagi_handle_t *handle, usagi_t *object)
{
	handle->_ptr = (void*)(object);
}

//�n���h��������̂ւ̃|�C���^���擾
usagi_t *usagi_handle_t_get(usagi_handle_t handle)
{
	return (usagi_t*)(handle._ptr);
}


//usagi�̏W�����Ǘ�����\����
struct usagi_manager_t
{
	//�Ƃ肠�����z��(�����͊�{�Œ�)
	usagi_t *_array_head;
	size_t _capacity;
};




//usagi_manager_t�̏���������
void usagi_manager_t_init(usagi_manager_t *manager, size_t capacity)
{
	manager->_array_head = (usagi_t*)malloc(sizeof(usagi_t) * capacity);
	if (manager->_array_head == nullptr)
	{
		message_box(nullptr, MB_OK, MB_ICONERROR,
			"�������m�ۂɎ��s���܂���\n�������T�C�Y : %ld",
			sizeof(usagi_t) * capacity
		);
	}
	manager->_capacity = capacity;
	for (size_t i = 0; i < capacity; i++)
	{
		usagi_t_init(&manager->_array_head[i]);
	}
}

//usagi_manager_t�̏I������
void usagi_manager_t_end(usagi_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		usagi_t_end(&manager->_array_head[i]);
	}
	safe_free(manager->_array_head);
	manager->_capacity = 0;
}

//usagi_manager_t��ʂ���usagi_t��1�̃A�N�e�B�u�ɂ���
usagi_handle_t usagi_manager_t_add_usagi(usagi_manager_t *manager, D3DXVECTOR3 *position)
{
	usagi_handle_t creature = {};
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		if (manager->_array_head[i]._is_active == false)
		{
			usagi_t_to_active(&manager->_array_head[i], anim_model_kind_usagi, position);
			usagi_handle_t_create(&creature, &manager->_array_head[i]);
			return creature;
		}
	}
	usagi_handle_t_create(&creature, nullptr);
	return creature;
}

//usagi_manager_t���Ǘ����Ă��邷�ׂĂ�usagi_t���X�V
void usagi_manager_t_update(usagi_manager_t *manager)
{
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		usagi_t_update(&manager->_array_head[i]);
	}
}

//usagi_manager_t���Ǘ����Ă��邷�ׂĂ�usagi_t��`��
void usagi_manager_t_draw(usagi_manager_t *manager, IDirect3DDevice9 *device)
{
	//���C���`��
	for (size_t i = 0; i < manager->_capacity; i++)
	{
		usagi_t_draw(&manager->_array_head[i], device);
	}
}













static usagi_manager_t *g_usagi_manager;






//usagi������ & �o��
extern void usagi_init()
{
	g_usagi_manager = (usagi_manager_t*)malloc(sizeof(usagi_manager_t));
	usagi_manager_t_init(g_usagi_manager, USAGI_CAPACITY);
	//usagi����̏o��
	usagi_manager_t_add_usagi(g_usagi_manager,
		&D3DXVECTOR3(
			uniform_rand(-20.0, 20.0),
			uniform_rand(0.0, 1.0),
			uniform_rand(-8.0, 8.0)
		)
	);
}

//usagi�I��
extern void usagi_end()
{
	usagi_manager_t_end(g_usagi_manager);
	safe_free(g_usagi_manager);
}

//usagi�X�V
extern void usagi_update()
{
	//�����_����usagi����̏o��
	if (input_keyboard_trigger(DIK_F9) || ((rand() % 1000 == 0)))
	{
		usagi_manager_t_add_usagi(g_usagi_manager,
			&D3DXVECTOR3(
				uniform_rand(-20.0, 20.0),
				uniform_rand(0.0, 1.0),
				uniform_rand(-8.0, 8.0)
			)
		);
	}
	usagi_manager_t_update(g_usagi_manager);
}

//usagi�`��
extern void usagi_draw(IDirect3DDevice9 *device)
{
	usagi_manager_t_draw(g_usagi_manager, device);
}


