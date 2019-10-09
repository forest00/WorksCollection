
//親ヘッダー
#include "..\public\flatpo_texture_loader.h"

//マクロ
#define safe_release(p)			{ if(p) { (p)->Release(); (p) = nullptr; } }


//ファイル名の羅列
static const char *g_file_names[flatpo_texture_id_max] = {
	"",
	"res/sprite_tex/amg_logo.png",
	//"res/sprite_tex/ICEPALADIN010E/Resource/png/ファフニール/ファフニール頭部_倒れ.png",
	//"res/sprite_tex/ICEPALADIN010E/Resource/png/ファフニール/ファフニール尻尾_攻撃.png",
	//"res/sprite_tex/ICEPALADIN010E/Resource/png/",
	//"res/sprite_tex/",
	//"res/sprite_tex/",
	//"res/sprite_tex/",
	//"res/sprite_tex/",
};




static LPDIRECT3DTEXTURE9 g_all_textures[flatpo_texture_id_max] = {};



//テクスチャを全部読み込む
extern void flatpo_texture_loader_load_all(IDirect3DDevice9 *device)
{
	g_all_textures[0] = nullptr;
	for (size_t i = 1; i < (size_t)(flatpo_texture_id_max); i++)
	{
		//読み込む
		if (FAILED(D3DXCreateTextureFromFileEx(
			device,
			g_file_names[i],
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0x00000000,
			NULL,
			NULL,
			&g_all_textures[i]
		)))
		{
			//読み込み失敗メッセージを出す
			static char message[0x400];
			message[0] = '\0';
			strcat_s(message, 0x400, "板ポリゴン初期化時\n\n");
			strcat_s(message, 0x400, "テクスチャの読み込みに失敗しました\n");
			strcat_s(message, 0x400, "ファイル名 : ");
			strcat_s(message, 0x400, g_file_names[i]);
			MessageBox(0, message, "", MB_OK);
		}
	}
}

//テクスチャを全部破棄
extern void flatpo_texture_loader_destroy_all()
{
	for (size_t i = 0; i < (size_t)(flatpo_texture_id_max); i++)
	{
		safe_release(g_all_textures[i]);
	}
}

//idに対応するテクスチャを得る
extern LPDIRECT3DTEXTURE9 flatpo_texture_loader_get(flatpo_texture_id_t texture_id)
{
	return g_all_textures[(size_t)(texture_id)];
}