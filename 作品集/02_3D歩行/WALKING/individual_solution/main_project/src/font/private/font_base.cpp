
//親ヘッダー
#include "font_base.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\common_def\public\common_def.h"



struct font_base_t
{
	LPD3DXFONT _xfonts;
	LPD3DXSPRITE _text_sprite;
};


/*---------------------------------------------*/
//ローカル関数
static int font_base_create(IDirect3DDevice9 *device, font_base_t *font_base, const char *str, int height, int weight)
{
	//フォントを作成する
	HRESULT create = D3DXCreateFont(
		device,
		-height,
		0,
		weight,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		str,
		&font_base->_xfonts
	);
	//作成失敗
	if (FAILED(create))
	{
		return -1;
	}

	return 0;
}
/*---------------------------------------------*/



extern font_base_t *font_base_init(IDirect3DDevice9 *device)
{
	void *creature = malloc(sizeof(font_base_t));
	assert(creature != nullptr);
	font_base_t *font_base = (font_base_t*)(creature);
	font_base->_xfonts = {};
	font_base->_text_sprite = nullptr;
	//テキストスプライトの作成
	if (FAILED(D3DXCreateSprite(device, &font_base->_text_sprite)))
	{
		return nullptr;
	}
	return font_base;
}



extern void font_base_end(font_base_t **font_base)
{
	safe_release((*font_base)->_xfonts);
	safe_release((*font_base)->_text_sprite);
}





extern void font_base_draw(IDirect3DDevice9 *device, font_base_t *font_base, const char *str, int x, int y, size_t height, int alpha, int red, int green, int blue)
{
	int index = font_base_create(device, font_base, "ARIAL", height, 600);
	if (index < 0)
	{
		return;
	}
	RECT r = { x,y,0,0 };
	font_base->_text_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	font_base->_xfonts->DrawTextA(
		font_base->_text_sprite,
		str,
		-1,
		&r,
		DT_NOCLIP,
		D3DCOLOR_ARGB(alpha, red, green, blue)
	);
	font_base->_text_sprite->End();
	font_base->_xfonts->Release();
	font_base->_xfonts = nullptr;
}
