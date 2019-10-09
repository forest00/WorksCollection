
//親ヘッダー
#include "x_file_drawer.h"

//モデルを描画
extern void x_file_drawer_draw(IDirect3DDevice9 *device, const x_file_t *x_file)
{
	for (DWORD i = 0; i < x_file->_materials_num; i++)
	{
		device->SetMaterial(&x_file->_materials[i]);
		device->SetTexture(0, x_file->_textures[i]);
		x_file->_mesh[i].DrawSubset(i);
	}
}
