//�e�w�b�_�[
#include "glitter.h"
//���̑��̃w�b�_�[
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\calc\calc.h"
#include "..\..\..\..\texture_manager\texture_manager.h"

extern void glitter_t_init_with_alloc(glitter_t **obj, const glitter_initializer_t *initializer)
{
	*obj = (glitter_t*)malloc(sizeof(glitter_t));
	glitter_t_init(*obj, initializer);
}

extern void glitter_t_init(glitter_t *obj, const glitter_initializer_t *initializer)
{
	obj->_spawn_sec = mylib_now_sec();								//���܂ꂽ�u��(�b)
	obj->_life_sec = initializer->_life_sec;						//����(�b)
	obj->_scale_base = initializer->_scale_base;					//�g�嗦
	obj->_easing_type = initializer->_easing_type;					//�C�[�W���O�^�C�v
	obj->_texture_id = initializer->_texture_id;					//�e�N�X�`��ID
	obj->_start_position = initializer->_start_position;			//�J�n�ʒu
	obj->_end_position = initializer->_end_position;				//�I���ʒu
	obj->_red = initializer->_red;									//�Ԑ���
	obj->_green = initializer->_green;								//�ΐ���
	obj->_blue = initializer->_blue;								//����
}

extern void glitter_t_end_with_free(glitter_t **obj)
{
	glitter_t_end(*obj);
	free(*obj);
	*obj = nullptr;
}

extern void glitter_t_end(glitter_t *obj)
{
	//���ɉ������Ȃ�
	UNREFERENCED_PARAMETER(obj);
}

extern void glitter_t_update(glitter_t *obj, bool *should_end)
{
	//���Ԃ��擾
	float elapsed_sec = 0.0f;
	glitter_t_elapsed_sec(obj, &elapsed_sec);
	//���Ԃ����ȏ�o�߂��Ă�����I�����ׂ�
	if (elapsed_sec > obj->_life_sec)
	{
		*should_end = true;
		return;
	}
	*should_end = false;
}

extern void glitter_t_draw(const glitter_t *obj)
{
	//UV���W��p��
	static D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//���݈ʒu�ɕ`��
	//todo:���炫��G�t�F�N�g��p�̃V�F�[�_�[������Ďg��
	D3DXVECTOR3 now_position;
	glitter_t_now_position(obj, &now_position);
	const float scale_base = 1.0f;
	const float alpha_max = 255.0f;
	const float alpha_min = 0.0f;
	int now_alpha = 0;
	glitter_t_alpha(obj, &now_alpha);
	float scale = obj->_scale_base * uniform_randf(0.95f, 1.0f);
	float alpha_position = (float)(now_alpha) / 255.0f;
	int alpha = (int)(lerpf(alpha_min, alpha_max, alpha_position));
	int red = (int)(obj->_red * 255.0f);
	int green = (int)(obj->_green * 255.0f);
	int blue = (int)(obj->_blue * 255.0f);
	D3DXVECTOR2 _scale = D3DXVECTOR2(scale, scale);
	mylib_draw_billboard2(
		&now_position,
		0.0f,
		&_scale,
		D3DCOLOR_ARGB(alpha, red, green, blue),
		&uv[0],
		&uv[1],
		&uv[2],
		&uv[3],
		texture_manager_get(obj->_texture_id)
	);
}

extern void glitter_t_elapsed_sec(const glitter_t *obj, float *elapsed_sec)
{
	*elapsed_sec = mylib_now_sec() - obj->_spawn_sec;
}

extern void glitter_t_now_position(const glitter_t *obj, D3DXVECTOR3 *now_position)
{
	//���Ԃ��擾
	float elapsed_sec = 0.0f;
	glitter_t_elapsed_sec(obj, &elapsed_sec);
	//�C�[�W���O����
	float eased = (float)call_easing(obj->_easing_type, elapsed_sec, 0.0f, 1.0f, obj->_life_sec);
	//���W�����߂�
	D3DXVec3Lerp(now_position, &obj->_start_position, &obj->_end_position, eased);
}

extern void glitter_t_alpha(const glitter_t *obj, int *alpha)
{
	//���Ԃ��擾
	float elapsed_sec = 0.0f;
	glitter_t_elapsed_sec(obj, &elapsed_sec);
	//�C�[�W���O����
	*alpha = 255 - (int)call_easing(et_Linear, elapsed_sec, 0.0f, 255.0f, obj->_life_sec);
}
