#include "object.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
//for debug
#include "primitive/debug.h"

//�I�u�W�F�N�g
static Object *all_obj = nullptr;

//�I�u�W�F�N�g�̐�
static size_t object_kind_num[(int)(ObjectKind_Max)];
static size_t object_num = 0;

//���ׂẴI�u�W�F�N�g�̃������m��
static int AllocateObjectMemory()
{
	if (all_obj != nullptr)
	{
		return -1;
	}
	all_obj = (Object *)malloc(sizeof(Object) * ObjectCapacity);
	if (all_obj == nullptr)
	{
		return -1;
	}
	for (int i = 0; i < ObjectCapacity; i++)
	{
		all_obj[i].kind = ObjectKind_None;
	}
	return 0;
}

//���ׂẴI�u�W�F�N�g�̃��������
static int DeallocateObjectMemory()
{
	if (all_obj == nullptr)
	{
		return -1;
	}
	free(all_obj);
	all_obj = nullptr;
	return 0;
}

//�I�u�W�F�N�g�̐������Z�b�g
static void CountReset()
{
	for (int i = 0 ; i < (int)(ObjectKind_Max) ; i++)
	{
		object_kind_num[i] = 0;
	}
	object_num = 0;
}

//�I�u�W�F�N�g���󂩔���
static bool IsNoneObject(const Object *obj)
{
	return obj->kind == ObjectKind_None;
}

//��̃I�u�W�F�N�g��T��
static Object *FindNoneObject()
{
	for (int i = 0; i < ObjectCapacity; i++)
	{
		Object *obj = GetObject(i);
		if (IsNoneObject(obj))
		{
			return obj;
		}
	}
	return nullptr;
}

extern int InitObject()
{
	//�������m��&�J�E���g���Z�b�g
	if (AllocateObjectMemory() != 0)
	{
		return -1;
	}
	CountReset();
	return 0;
}

extern int EndObject()
{
	//����������̂�
	if (DeallocateObjectMemory() != 0)
	{
		return -1;
	}
	return 0;
}

extern Object *GetObject(int index)
{
	//�C���f�b�N�X�ɑΉ������I�u�W�F�N�g��Ԃ�
	return &all_obj[index];
}

extern Object *BeginObject()
{
	//�擪�̃I�u�W�F�N�g��Ԃ�
	return GetObject(0);
}

extern Object *OutObject()
{
	return GetObject(ObjectCapacity);
}

extern Object *NextObject(Object *obj)
{
	return obj + 1;
}

extern Object *SetObject(ObjectKind kind, Vector2D pos, LooksID looks_id, void *kind_info)
{
	//���E�ʂ𒴂��Ă����牽�����Ȃ�
	if (object_kind_num[(int)(kind)] >= GetCapacity(kind))
	{
		return nullptr;
	}
	//�󂫂�T��
	Object *obj = FindNoneObject();
	//�󂫂��Ȃ���Ή������Ȃ�
	if (obj == nullptr)
	{
		return nullptr;
	}
	//�Z�b�g
	obj->kind = kind;
	obj->pos = pos;
	obj->looks_id = looks_id;
	obj->kind_info = kind_info;
	//�J�E���g�A�b�v
	object_num++;
	object_kind_num[(int)(kind)]++;
	return obj;
}

extern int ClearObject(Object *obj)
{
	//�k���`�F�b�N
	if (obj == nullptr)
	{
		return -1;
	}
	//��̃I�u�W�F�N�g�`�F�b�N
	if (IsNoneObject(obj))
	{
		return -1;
	}
	//�J�E���g�_�E��
	object_kind_num[(int)(obj->kind)]--;
	object_num--;
	//��ނ��ƂɌ㏈������
	switch (obj->kind)
	{
	case ObjectKind_None:
		break;
	case ObjectKind_Player:
		PlayerCleared(obj->kind_info);
		break;
	case ObjectKind_Bullet:
		BulletCleared(obj->kind_info);
		break;
	case ObjectKind_Enemy:
		EnemyCleared(obj->kind_info);
		break;
	case ObjectKind_Max:
		break;
	default:
		break;
	}
	//��̃I�u�W�F�N�g�ɂ���
	obj->kind = ObjectKind_None;
	return 0;
}

extern void ClearAllObject()
{
	//�S���̃I�u�W�F�N�g������
	for (int i = 0; i < ObjectCapacity; i++)
	{
		Object *obj = GetObject(i);
		ClearObject(obj);
	}
}

extern void UpdateObject(const Keyboard *key)
{
	//�S���̃I�u�W�F�N�g�ɑ΂���
	for (int i = 0; i < ObjectCapacity; i++)
	{
		//��ނ��ƂɍX�V����
		switch (all_obj[i].kind)
		{
		case ObjectKind_None:
			break;
		case ObjectKind_Player:
			UpdatePlayer(i, key);
			break;
		case ObjectKind_Bullet:
			UpdateBullet(i);
			break;
		case ObjectKind_Enemy:
			UpdateEnemy(i);
			break;
		case ObjectKind_Max:
			break;
		default:
			break;
		}
	}
}

extern void DrawObject()
{
	//for debug
	//�f�o�b�O���폜
	ClearDebug();
	//�f�o�b�O���o�^
	RegisterDebug("Object", GetObjectNum());
	RegisterDebug("Player", GetPlayerNum());
	RegisterDebug("Bullet", GetBulletNum());
	RegisterDebug("Enemy ", GetEnemyNum());
	//�S���̃I�u�W�F�N�g�ɑ΂���
	for (int i = 0; i < ObjectCapacity; i++)
	{
		//�����ڂ��擾
		Object *obj = GetObject(i);
		if (IsNoneObject(obj) == false)
		{
			const Looks *looks = GetLooks(obj->looks_id);
			//�`��
			DrawLooks(looks, obj->pos);
		}
	}
}

extern bool IsHitObjectRect(const Object *obj, Rect hit_rect)
{
	//��̃I�u�W�F�N�g�Ȃ玸�s
	if (IsNoneObject(obj))
	{
		return false;
	}
	//��`�擾
	const Looks *looks = GetLooks(obj->looks_id);
	Rect obj_rect = CalcLooksRect(looks, obj->pos);
	//����
	return IsHitRect(obj_rect, hit_rect);
}

extern Object *HitObject(const Object *obj, Object *offset, bool ignore_self)
{
	//�I�t�Z�b�g��K�p
	Object *current = offset;
	//�k���`�F�b�N
	if (current == nullptr)
	{
		current = BeginObject();
	}
	//��`�擾
	const Looks *looks = GetLooks(obj->looks_id);
	Rect obj_rect = CalcLooksRect(looks, obj->pos);
	//�������B�`�F�b�N
	while (current != OutObject())
	{
		//���g�𖳎�����
		if (ignore_self && (current == obj))
		{
			current = NextObject(current);
		}
		//�Փ˃`�F�b�N
		if (IsHitObjectRect(current, obj_rect))
		{
			return current;
		}
		//���̑����
		current = NextObject(current);
	}
	return current;
}

extern size_t GetObjectKindNum(ObjectKind kind)
{
	//��ނ��Ƃ̑�����Ԃ�
	return object_kind_num[(int)(kind)];
}

extern size_t GetObjectNum()
{
	//������Ԃ�
	return object_num;
}
