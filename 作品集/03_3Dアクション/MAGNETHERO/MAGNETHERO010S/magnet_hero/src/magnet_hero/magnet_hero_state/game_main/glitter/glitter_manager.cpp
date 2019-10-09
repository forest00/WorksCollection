//�e�w�b�_�[
#include "glitter_manager.h"
//���̑��̃w�b�_�[
#include "glitter.h"

//�L���p
#define _manage_capacity		(0x200)

//�Ǘ��p�\����
struct manage_t
{
	bool _is_active;
	glitter_t *_object;
};

//�O���[�o���ϐ�
struct global_t
{
	manage_t *_manage;
};

static global_t g;

//���->�e�N�X�`��ID�ϊ�
static texture_id_t to_texture_id(glitter_kind_t kind)
{
	switch (kind)
	{
	case gk_none:
		return ti_none;
		break;
	case gk_orange:
		return ti_flare01;
		break;
	case gk_purple:
		return ti_flare07;
		break;
	case gk_ring:
		return ti_flare08;
		break;
	case gk_green:
		return ti_flare09;
		break;
	case gk_blue:
		return ti_flare10;
		break;
	case gk_white:
		return ti_flare12;
		break;
	case gk_red:
		return ti_flare13;
		break;
	case gk_max:
		return ti_none;
		break;
	default:
		return ti_none;
		break;
	}
}

//�m��
static void manage_alloc(manage_t **p, size_t n)
{
	*p = (manage_t*)malloc(sizeof(manage_t) * n);
}

//�[������
static void fill_inactive(manage_t *p, size_t n)
{
#if 0
	for (size_t i = 0; i < n; i++)
	{
		manage_t *manage = &p[i];
		manage->_is_active = false;
		manage->_object = nullptr;
	}
#else
	ZeroMemory(p, sizeof(*p) * n);
#endif
}

//�[���T��
static void find_inactive(manage_t **found, global_t *global)
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &global->_manage[i];
		if (manage->_is_active == false)
		{
			*found = manage;
			return;
		}
	}
	*found = nullptr;
}

extern void glitter_manager_init()
{
	//�������m��
	manage_alloc(&g._manage, _manage_capacity);
	//�[������
	fill_inactive(g._manage, _manage_capacity);
}

extern void glitter_manager_end()
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &g._manage[i];
		//�����`�F�b�N
		if (manage->_is_active)
		{
			//�I������
			glitter_t_end_with_free(&manage->_object);
		}
	}
	//���������
	free(g._manage);
	g._manage = nullptr;
}

extern void glitter_manager_update()
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &g._manage[i];
		//�����`�F�b�N
		if (manage->_is_active)
		{
			bool should_end = false;
			//�X�V
			glitter_t_update(manage->_object, &should_end);
			if (should_end)
			{
				//�t���O�؂�ւ�
				manage->_is_active = false;
				//�I�����ă��������
				glitter_t_end_with_free(&manage->_object);
			}
		}
	}
}

extern void glitter_manager_draw()
{
	for (size_t i = 0; i < _manage_capacity; i++)
	{
		manage_t *manage = &g._manage[i];
		//�����`�F�b�N
		if (manage->_is_active)
		{
			//�`��
			glitter_t_draw(manage->_object);
		}
	}
}

extern void glitter_manager_add(const glitter_add_t *info)
{
	manage_t *manage;
	//�󂫂�����
	find_inactive(&manage, &g);
	//������Ȃ�
	if (manage == nullptr)
	{
		return;
	}
	//�t���O�؂�ւ�
	manage->_is_active = true;
#if 0
	glitter_initializer_t initialzer;
	initialzer._life_sec = info->_life_sec;						//����(�b)
	initialzer._scale_base = info->_scale_base;					//�g�嗦
	initialzer._easing_type = info->_easing_type;				//�C�[�W���O�^�C�v
	initialzer._texture_id = to_texture_id(info->_kind);		//�e�N�X�`��ID
	initialzer._start_position = info->_start_position;			//�J�n�ʒu
	initialzer._end_position = info->_end_position;				//�I���ʒu
	initialzer._red = info->_red;								//�Ԑ���
	initialzer._green = info->_green;							//�ΐ���
	initialzer._blue = info->_blue;								//����
#else
	const glitter_initializer_t initialzer = {
		info->_life_sec,				//����(�b)
		info->_scale_base,				//�g�嗦
		info->_easing_type,				//�C�[�W���O�^�C�v
		to_texture_id(info->_kind),		//�e�N�X�`��ID
		info->_start_position,			//�J�n�ʒu
		info->_end_position,			//�I���ʒu
		info->_red,						//�Ԑ���
		info->_green,					//�ΐ���
		info->_blue						//����
	};
#endif
	//�������m��&������
	glitter_t_init_with_alloc(&manage->_object, &initialzer);
}

extern glitter_kind_t gk_random()
{
	return (glitter_kind_t)(rand() % (int)(gk_max - 1) + 1);
}
