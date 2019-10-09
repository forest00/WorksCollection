//親ヘッダー
#include "aiming_cursor.h"
//その他のヘッダー
#include "..\aiming\aiming.h"
#include "..\connecter\info_holder.h"
#include "..\..\..\..\mylib\mylib.h"
#include "..\..\..\..\texture_manager\texture_manager.h"


//ファイル内グローバル変数
struct global_t
{
	IDirect3DTexture9 *_cursor_tex[2];
	D3DXVECTOR3 _pos;
	D3DXVECTOR3 _target_pos;
	D3DXVECTOR3 _draw_pos;
	D3DXVECTOR3 _draw_target_pos;
	int _use_tex_id;
};

static global_t g;

//初期化
extern void aiming_cursor_init()
{
	g._cursor_tex[0] = texture_manager_get(ti_aiming_cursor);
	g._cursor_tex[1] = texture_manager_get(ti_aiming_cursor2);
}

//終了
extern void aiming_cursor_end()
{

}

//更新
extern void aiming_cursor_update()
{
	if (player_beam_hit_object_get())
	{
		g._use_tex_id = 0;
	}
	else
	{
		g._use_tex_id = 1;
	}
	aiming_cursor_get_pos(&g._pos.x, &g._pos.y, &g._pos.z);
	aiming_cursor_get_target_pos(&g._target_pos.x, &g._target_pos.y, &g._target_pos.z);
	g._draw_pos = g._pos;
	g._draw_target_pos = g._target_pos;
}

//描画
extern void aiming_cursor_draw()
{
	//カーソル描画
	if (0.0f <= g._draw_pos.z && g._draw_pos.z <= 1.0f)
	{
		D3DXVECTOR2 pos = D3DXVECTOR2(g._draw_pos.x, g._draw_pos.y);
		D3DXVECTOR2 scale = D3DXVECTOR2(0.1f, 0.15f);
		D3DXVECTOR2 uv[4];
		uv[0] = D3DXVECTOR2(0.0f, 0.0f);
		uv[1] = D3DXVECTOR2(1.0f, 0.0f);
		uv[2] = D3DXVECTOR2(0.0f, 1.0f);
		uv[3] = D3DXVECTOR2(1.0f, 1.0f);
		mylib_draw_point_sprite(&pos, 0.0f, &scale, 0xffffffff, &uv[0], &uv[1], &uv[2], &uv[3], g._cursor_tex[g._use_tex_id], mbm_alpha);
	}
}
