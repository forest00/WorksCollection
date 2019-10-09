#pragma once


#include <d3dx9.h>


extern void makabe_mash_init(IDirect3DDevice9 *device);

extern void makabe_mash_end();

extern void makabe_mash_update();

extern void makabe_mash_draw(IDirect3DDevice9 *device);

extern void makabe_mash_get_position(D3DXVECTOR3 *position);

