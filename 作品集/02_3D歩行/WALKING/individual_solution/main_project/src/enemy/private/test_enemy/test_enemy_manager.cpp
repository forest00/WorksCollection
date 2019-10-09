#pragma once

//�e�w�b�_�[
#include "..\..\public\test_enemy\test_enemy_manager.h"
//���̑��̃w�b�_�[
#include <assert.h>
#include "test_enemy_database.h"
#include "test_enemy_object.h"
#include "..\..\..\angle\public\angle.h"
#include "..\..\..\x_file\public\x_file_type.h"
#include "..\..\..\my_3d_lib\public\my_3d_lib.h"



/*------------------------------*/
enum test_enemy_manager_t_capacity_t
{
	test_enemy_manager_capacity = 64,
};

//�e�X�g�p�G�L�����}�l�[�W���[
struct test_enemy_manager_t
{
	test_enemy_object_t *_objects[test_enemy_manager_capacity];
	size_t _num_of_active;
	test_enemy_database_t _database;
};
/*------------------------------*/











/*------------------------------*/
//�e�X�g�p�G�L�����}�l�[�W���[���m�ۂ��ď��������ĕԂ�
extern test_enemy_manager_t *test_enemy_manager_t_init()
{
	void *creature = malloc(sizeof(test_enemy_manager_t));
	assert(creature != nullptr);
	test_enemy_manager_t *manager = (test_enemy_manager_t*)(creature);
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		manager->_objects[i] = nullptr;
	}
	manager->_num_of_active = 0;
	test_enemy_database_default(&manager->_database);
	return manager;
}

//�e�X�g�p�G�L�����}�l�[�W���[�̍X�V
//�Ǘ����ɂ���S�G�L�����ɑ΂��āA�X�V����������
extern void test_enemy_manager_t_update(test_enemy_manager_t *manager)
{
	if(manager){
		for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
		{
			if (manager->_objects[i])
			{
				test_enemy_object_t_update(manager->_objects[i]);
			}
		}
	}
}

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���e�X�g�p�G�L���������ׂĕ`��
extern void test_enemy_manager_t_draw(const test_enemy_manager_t *manager, IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			test_enemy_object_t_draw(manager->_objects[i], device);
		}
	}
}


//�e�X�g�p�G�L�����}�l�[�W���[�̏I������ & �������������
//�����Ƃ��ēn�����|�C���^�ɂ�null���������܂�
extern void test_enemy_manager_t_end(test_enemy_manager_t **manager)
{
	if (*manager)
	{
		for (size_t i = 0; i < test_enemy_manager_t_get_capacity(*manager); i++)
		{
			test_enemy_object_t_end(&(*manager)->_objects[i]);
		}
		safe_free(*manager);
	}
}

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���e�X�g�p�G�L�����̐���Ԃ�
extern size_t test_enemy_manager_t_get_num_of_active(const test_enemy_manager_t *manager)
{
	return manager->_num_of_active;
}

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ��o����e�X�g�p�G�L�����̍ő吔��Ԃ�
extern size_t test_enemy_manager_t_get_capacity(const test_enemy_manager_t *)
{
	return (size_t)(test_enemy_manager_capacity);
}

//�e�X�g�p�G�L������ǉ�����
//�ǉ������e�X�g�p�G�L�����ւ̃|�C���^��Ԃ�
//���炩�̌����ɂ�莸�s������Anull��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_add(test_enemy_manager_t *manager, const test_enemy_initializer_t *initializer)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i] == nullptr)
		{
			manager->_objects[i] = test_enemy_object_t_init(&manager->_database, initializer);
			manager->_num_of_active++;
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//�e�X�g�p�G�L�������폜����
extern void test_enemy_manager_t_delete(test_enemy_manager_t *manager, test_enemy_object_t **object)
{
	if (object == nullptr)
	{
		return;
	}
	if (*object == nullptr)
	{
		return;
	}
	test_enemy_object_t_end(object);
	manager->_num_of_active--;
}

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���I�u�W�F�N�g�̒��ŁA�ł��擾���₷���I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_search_active(test_enemy_manager_t *manager)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���I�u�W�F�N�g�̒��ŁAAABB�ƏՓ˂��Ă���I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_aabb(test_enemy_manager_t *manager, const aabb_t *aabb)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���I�u�W�F�N�g�̒��ŁA���̂ƏՓ˂��Ă���I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_sphere(test_enemy_manager_t *manager, const sphere_t *sphere)
{
	for (size_t i = 0; i < test_enemy_manager_t_get_capacity(manager); i++)
	{
		if (manager->_objects[i])
		{
			return &manager->_objects[i];
		}
	}
	return nullptr;
}

//�e�X�g�p�G�L�����̈ʒu�𓾂�
extern const vector3_t *test_enemy_manager_t_get_position(const test_enemy_manager_t *manager, const test_enemy_object_t *object)
{
	if (object == nullptr)
	{
		return nullptr;
	}
	return test_enemy_object_t_get_position(object);
}

//�e�X�g�p�G�L�����̊p�x�𓾂�
extern const angle_t *test_enemy_manager_t_get_direction(const test_enemy_manager_t *manager, const test_enemy_object_t *object)
{
	if (object == nullptr)
	{
		return nullptr;
	}
	return test_enemy_object_t_get_direction(object);
}
/*------------------------------*/


