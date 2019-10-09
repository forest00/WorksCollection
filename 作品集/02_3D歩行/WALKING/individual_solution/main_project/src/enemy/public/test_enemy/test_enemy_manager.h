#pragma once

#include "..\..\public\test_enemy\test_enemy_initializer.h"
#include "..\..\public\test_enemy\test_enemy_object_declaration.h"
#include "..\..\..\collision\public\collision.h"


struct test_enemy_manager_t;

/*---------------------------------------*/
//�e�X�g�p�G�L�����}�l�[�W���[���m�ۂ��ď��������ĕԂ�
extern test_enemy_manager_t *test_enemy_manager_t_init();

//�e�X�g�p�G�L�����}�l�[�W���[�̍X�V
//�Ǘ����ɂ���S�G�L�����ɑ΂��āA�X�V����������
extern void test_enemy_manager_t_update(test_enemy_manager_t *manager);

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���e�X�g�p�G�L���������ׂĕ`��
extern void test_enemy_manager_t_draw(const test_enemy_manager_t *manager, IDirect3DDevice9 *device);

//�e�X�g�p�G�L�����}�l�[�W���[�̏I������ & �������������
//�����Ƃ��ēn�����|�C���^�ɂ�null���������܂�
extern void test_enemy_manager_t_end(test_enemy_manager_t **manager);

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���e�X�g�p�G�L�����̐���Ԃ�
//�G�L��������̒ǉ����邽�тɁA�����Ă���
extern size_t test_enemy_manager_t_get_num_of_active(const test_enemy_manager_t *manager);

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ��o����e�X�g�p�G�L�����̍ő吔��Ԃ�
extern size_t test_enemy_manager_t_get_capacity(const test_enemy_manager_t *manager);

//�e�X�g�p�G�L������ǉ�����
//�ǉ������e�X�g�p�G�L�����ւ̃|�C���^��Ԃ�
//���炩�̌����ɂ�莸�s������Anull��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_add(test_enemy_manager_t *manager, const test_enemy_initializer_t *initializer);

//�e�X�g�p�G�L�������폜����
//������Ăяo������A�|�C���^��null���������܂�
extern void test_enemy_manager_t_delete(test_enemy_manager_t *manager, test_enemy_object_t **object);

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���I�u�W�F�N�g�̒��ŁA�ł��擾���₷���I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_search_active(test_enemy_manager_t *manager);

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���I�u�W�F�N�g�̒��ŁAAABB�ƏՓ˂��Ă���I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_aabb(test_enemy_manager_t *manager, const aabb_t *aabb);

//�e�X�g�p�G�L�����}�l�[�W���[���Ǘ����Ă���I�u�W�F�N�g�̒��ŁA���̂ƏՓ˂��Ă���I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
extern test_enemy_object_t **test_enemy_manager_t_search_hitting_sphere(test_enemy_manager_t *manager, const sphere_t *sphere);

//�e�X�g�p�G�L�����̈ʒu�𓾂�
extern const vector3_t *test_enemy_manager_t_get_position(const test_enemy_manager_t *manager, const test_enemy_object_t *object);

//�e�X�g�p�G�L�����̊p�x�𓾂�
extern const angle_t *test_enemy_manager_t_get_direction(const test_enemy_manager_t *manager, const test_enemy_object_t *object);
/*---------------------------------------*/




