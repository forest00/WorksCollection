#pragma once

#include "..\..\..\angle\public\angle.h"
#include "..\..\public\test_enemy\test_enemy_initializer.h"
#include "..\..\public\test_enemy\test_enemy_object_declaration.h"
#include "test_enemy_database.h"

//�e�X�g�p�G�L������̕��̃������m�ۂƏ����������ă|�C���^��Ԃ�
//���s������null��Ԃ�
extern test_enemy_object_t *test_enemy_object_t_init(const test_enemy_database_t *database, const test_enemy_initializer_t *initializer);

//�e�X�g�p�G�L������̕��̍X�V
extern void test_enemy_object_t_update(test_enemy_object_t *object);

//�e�X�g�p�G�L������̕��̕`��
extern void test_enemy_object_t_draw(test_enemy_object_t *object, IDirect3DDevice9 *device);

//�e�X�g�p�G�L������̕��̏I���ƃ��������
//�����Ƃ��ēn�����|�C���^�ɂ�null���������܂�
extern void test_enemy_object_t_end(test_enemy_object_t **object);

//�e�X�g�p�G�L�����̈ʒu�𓾂�
extern const vector3_t *test_enemy_object_t_get_position(const test_enemy_object_t *object);

//�e�X�g�p�G�L�����̊p�x�𓾂�
extern const angle_t *test_enemy_object_t_get_direction(const test_enemy_object_t *object);
