#pragma once
#include <stdlib.h>
#include "object_kind.h"
#include "primitive/vector2d.h"
#include "primitive/keyboard.h"
#include "looks.h"

#define ObjectCapacity	100

struct Object
{
	ObjectKind kind;		//���
	Vector2D pos;			//���W
	LooksID looks_id;		//������
	void *kind_info;		//��ނ��Ƃ̒ǉ����
};

//�I�u�W�F�N�g������
extern int InitObject();

//�I�u�W�F�N�g�I��
extern int EndObject();

//�I�u�W�F�N�g���擾
extern Object *GetObject(int index);

//�I�u�W�F�N�g���X�g�̐擪���擾
extern Object *BeginObject();

//�I�u�W�F�N�g���X�g�̖����̎��̗v�f���擾
extern Object *OutObject();

//�I�u�W�F�N�g���X�g�̎��̗v�f���擾
//��������ɂ�OutObject���g��
extern Object *NextObject(Object *obj);

//�I�u�W�F�N�g���o��
extern Object *SetObject(ObjectKind kind, Vector2D pos, LooksID looks_id, void *kind_info);

//�I�u�W�F�N�g������
extern int ClearObject(Object *obj);

//�I�u�W�F�N�g�S����
extern void ClearAllObject();

//�I�u�W�F�N�g�X�V
extern void UpdateObject(const Keyboard *key);

//�I�u�W�F�N�g�`��
extern void DrawObject();

//�I�u�W�F�N�g�Ƌ�`���Փ˂��Ă��邩�`�F�b�N
extern bool IsHitObjectRect(const Object *obj, Rect hit_rect);

//�����ƏՓ˂��Ă���I�u�W�F�N�g���擾
extern Object *HitObject(const Object *obj, Object *offset, bool ignore_self);

//�I�u�W�F�N�g�̎�ނ��Ƃ̐��𓾂�
extern size_t GetObjectKindNum(ObjectKind kind);

//�I�u�W�F�N�g�̐��𓾂�
extern size_t GetObjectNum();
