//親ヘッダー
#include "glitter.h"
//その他のヘッダー
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
	obj->_spawn_sec = mylib_now_sec();								//生まれた瞬間(秒)
	obj->_life_sec = initializer->_life_sec;						//寿命(秒)
	obj->_scale_base = initializer->_scale_base;					//拡大率
	obj->_easing_type = initializer->_easing_type;					//イージングタイプ
	obj->_texture_id = initializer->_texture_id;					//テクスチャID
	obj->_start_position = initializer->_start_position;			//開始位置
	obj->_end_position = initializer->_end_position;				//終了位置
	obj->_red = initializer->_red;									//赤成分
	obj->_green = initializer->_green;								//緑成分
	obj->_blue = initializer->_blue;								//青成分
}

extern void glitter_t_end_with_free(glitter_t **obj)
{
	glitter_t_end(*obj);
	free(*obj);
	*obj = nullptr;
}

extern void glitter_t_end(glitter_t *obj)
{
	//特に何もしない
	UNREFERENCED_PARAMETER(obj);
}

extern void glitter_t_update(glitter_t *obj, bool *should_end)
{
	//時間を取得
	float elapsed_sec = 0.0f;
	glitter_t_elapsed_sec(obj, &elapsed_sec);
	//時間が一定以上経過していたら終了すべき
	if (elapsed_sec > obj->_life_sec)
	{
		*should_end = true;
		return;
	}
	*should_end = false;
}

extern void glitter_t_draw(const glitter_t *obj)
{
	//UV座標を用意
	static D3DXVECTOR2 uv[4];
	uv[0] = D3DXVECTOR2(0.0f, 0.0f);
	uv[1] = D3DXVECTOR2(1.0f, 0.0f);
	uv[2] = D3DXVECTOR2(0.0f, 1.0f);
	uv[3] = D3DXVECTOR2(1.0f, 1.0f);
	//現在位置に描画
	//todo:きらきらエフェクト専用のシェーダーを作って使う
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
	//時間を取得
	float elapsed_sec = 0.0f;
	glitter_t_elapsed_sec(obj, &elapsed_sec);
	//イージングする
	float eased = (float)call_easing(obj->_easing_type, elapsed_sec, 0.0f, 1.0f, obj->_life_sec);
	//座標を求める
	D3DXVec3Lerp(now_position, &obj->_start_position, &obj->_end_position, eased);
}

extern void glitter_t_alpha(const glitter_t *obj, int *alpha)
{
	//時間を取得
	float elapsed_sec = 0.0f;
	glitter_t_elapsed_sec(obj, &elapsed_sec);
	//イージングする
	*alpha = 255 - (int)call_easing(et_Linear, elapsed_sec, 0.0f, 255.0f, obj->_life_sec);
}
