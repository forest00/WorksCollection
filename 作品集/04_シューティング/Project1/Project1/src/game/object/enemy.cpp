#include "enemy.h"
#include "object.h"
#include "primitive/screen.h"

extern int SetEnemy(Vector2D pos)
{
	Object *obj = SetObject(ObjectKind_Enemy, pos, LooksID_Enemy, nullptr);
	if (obj == nullptr)
	{
		return -1;
	}
	//���ɑ��݂��Ă��鑼�̓G�Ƃ̏d�Ȃ������
	int hit_count = 0;
	int hit_count_max = 32;
	Vector2D dir = Vector2D_UniformNormal();
	if (dir.x < 0)
	{
		dir.x *= -1.0;
	}
	while (true)
	{
		//�Փ˃`�F�b�N
		Object *hit = HitObject(obj, BeginObject(), true);
		if (hit == OutObject())
		{
			break;
		}
		//�Փˑ��肪�����Ɠ������(�G�L����)�Ȃ�
		if (hit->kind == obj->kind)
		{
			//�ʒu���C��
			obj->pos = Add(obj->pos, dir);
			//�J�E���g�A�b�v
			hit_count++;
			//�J�E���g�����ȏ�ɂȂ�����I���
			if (hit_count >= hit_count_max)
			{
				break;
			}
		}
	}
	return 0;
}

extern void EnemyCleared(void *kind_info)
{
	(void)(kind_info);
}

extern void UpdateEnemy(int object_index)
{
	Object *obj = GetObject(object_index);
	const Looks *looks = GetLooks(obj->looks_id);
	obj->pos.x -= EnemyMoveSpeed;
	Rect looks_rect = CalcLooksRect(looks, obj->pos);
	//���[�ɍs�����������
	if (OutOfScreen(looks_rect))
	{
		if (obj->pos.x < ScreenWidth())
		{
			ClearObject(obj);
			return;
		}
	}
	Object *current = BeginObject();
	while (true)
	{
		//�Փ˃`�F�b�N
		Object *hit = HitObject(obj, current, true);
		if (hit == OutObject())
		{
			break;
		}
		//�Փˑ��肪�e�Ȃ�
		if (hit->kind == ObjectKind_Bullet)
		{
			//������
			ClearObject(obj);
			//���������
			ClearObject(hit);
			return;
		}
		//���̑����
		current = NextObject(hit);
	}
}

extern size_t GetEnemyNum()
{
	return GetObjectKindNum(ObjectKind_Enemy);
}
