//親ヘッダー
#include "..\public\makabe_mesh_creator.h"
//その他のヘッダー
#include <stdlib.h>
#include <assert.h>

static const char *g_makabe_texture_file_name = "res//sprite_tex//amg_logo.png";

static IDirect3DTexture9 *g_makabe_texture;

struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; // The position
	D3DCOLOR    color;    // The color
	FLOAT       tu, tv;   // The texture coordinates
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct makabe_mesh_t
{
	CUSTOMVERTEX _vertices[12 * 3];
};



extern void makabe_mesh_creator_start_up(IDirect3DDevice9 *device)
{
	D3DXCreateTextureFromFile(device, g_makabe_texture_file_name, &g_makabe_texture);
	if (g_makabe_texture == nullptr)
	{
		assert(0);
	}
}

extern void makabe_mesh_creator_end()
{
	g_makabe_texture->Release();
}

extern void makabe_mesh_creator_create(makabe_mesh_t **makabe_mesh, IDirect3DDevice9 *device)
{
	*makabe_mesh = (makabe_mesh_t*)malloc(sizeof(makabe_mesh_t));
	assert(*makabe_mesh);
	(*makabe_mesh)->_vertices[0] = { D3DXVECTOR3(-1.0f,-1.0f,-1.0f), 0xffffffff,0.0,0.0 };
	(*makabe_mesh)->_vertices[1] = { D3DXVECTOR3(+1.0f,-1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[2] = { D3DXVECTOR3(-1.0f,-1.0f,+1.0f), 0xffffffff,0.0,1.0 };

	(*makabe_mesh)->_vertices[3] = { D3DXVECTOR3(+1.0f,-1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[4] = { D3DXVECTOR3(-1.0f,-1.0f,+1.0f), 0xffffffff,0.0,1.0 };
	(*makabe_mesh)->_vertices[5] = { D3DXVECTOR3(+1.0f,-1.0f,+1.0f), 0xffffffff,1.0,1.0 };

	(*makabe_mesh)->_vertices[6] = { D3DXVECTOR3(-1.0f,+1.0f,-1.0f), 0xffffffff,0.0,0.0 };
	(*makabe_mesh)->_vertices[7] = { D3DXVECTOR3(+1.0f,+1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[8] = { D3DXVECTOR3(-1.0f,+1.0f,+1.0f), 0xffffffff,0.0,1.0 };

	(*makabe_mesh)->_vertices[9] = { D3DXVECTOR3(+1.0f,+1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[10] = { D3DXVECTOR3(-1.0f,+1.0f,+1.0f), 0xffffffff,0.0,1.0 };
	(*makabe_mesh)->_vertices[11] = { D3DXVECTOR3(+1.0f,+1.0f,+1.0f), 0xffffffff,1.0,1.0 };


	(*makabe_mesh)->_vertices[12] = { D3DXVECTOR3(-1.0f,-1.0f,-1.0f), 0xffffffff,0.0,0.0 };
	(*makabe_mesh)->_vertices[13] = { D3DXVECTOR3(-1.0f,+1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[14] = { D3DXVECTOR3(-1.0f,-1.0f,+1.0f), 0xffffffff,0.0,1.0 };

	(*makabe_mesh)->_vertices[15] = { D3DXVECTOR3(-1.0f,+1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[16] = { D3DXVECTOR3(-1.0f,-1.0f,+1.0f), 0xffffffff,0.0,1.0 };
	(*makabe_mesh)->_vertices[17] = { D3DXVECTOR3(-1.0f,+1.0f,+1.0f), 0xffffffff,1.0,1.0 };

	(*makabe_mesh)->_vertices[18] = { D3DXVECTOR3(+1.0f,-1.0f,-1.0f), 0xffffffff,0.0,0.0 };
	(*makabe_mesh)->_vertices[19] = { D3DXVECTOR3(+1.0f,+1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[20] = { D3DXVECTOR3(+1.0f,-1.0f,+1.0f), 0xffffffff,0.0,1.0 };

	(*makabe_mesh)->_vertices[21] = { D3DXVECTOR3(+1.0f,+1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[22] = { D3DXVECTOR3(+1.0f,-1.0f,+1.0f), 0xffffffff,0.0,1.0 };
	(*makabe_mesh)->_vertices[23] = { D3DXVECTOR3(+1.0f,+1.0f,+1.0f), 0xffffffff,1.0,1.0 };


	(*makabe_mesh)->_vertices[24] = { D3DXVECTOR3(-1.0f,-1.0f,-1.0f), 0xffffffff,0.0,0.0 };
	(*makabe_mesh)->_vertices[25] = { D3DXVECTOR3(+1.0f,-1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[26] = { D3DXVECTOR3(-1.0f,+1.0f,-1.0f), 0xffffffff,0.0,1.0 };

	(*makabe_mesh)->_vertices[27] = { D3DXVECTOR3(+1.0f,-1.0f,-1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[28] = { D3DXVECTOR3(-1.0f,+1.0f,-1.0f), 0xffffffff,0.0,1.0 };
	(*makabe_mesh)->_vertices[29] = { D3DXVECTOR3(+1.0f,+1.0f,-1.0f), 0xffffffff,1.0,1.0 };

	(*makabe_mesh)->_vertices[30] = { D3DXVECTOR3(-1.0f,-1.0f,+1.0f), 0xffffffff,0.0,0.0 };
	(*makabe_mesh)->_vertices[31] = { D3DXVECTOR3(+1.0f,-1.0f,+1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[32] = { D3DXVECTOR3(-1.0f,+1.0f,+1.0f), 0xffffffff,0.0,1.0 };

	(*makabe_mesh)->_vertices[33] = { D3DXVECTOR3(+1.0f,-1.0f,+1.0f), 0xffffffff,1.0,0.0 };
	(*makabe_mesh)->_vertices[34] = { D3DXVECTOR3(-1.0f,+1.0f,+1.0f), 0xffffffff,0.0,1.0 };
	(*makabe_mesh)->_vertices[35] = { D3DXVECTOR3(+1.0f,+1.0f,+1.0f), 0xffffffff,1.0,1.0 };
}

extern void makabe_mesh_creator_destroy(makabe_mesh_t **makabe_mesh)
{
	if (*makabe_mesh)
	{
		free(*makabe_mesh);
		*makabe_mesh = nullptr;
	}
}

extern void makabe_mesh_creator_draw(makabe_mesh_t *makabe_mesh, IDirect3DDevice9 *device, const D3DXMATRIX *world)
{
	D3DMATERIAL9 m;
	m.Ambient.a = 1.0f;
	m.Ambient.r = 1.0f;
	m.Ambient.g = 1.0f;
	m.Ambient.b = 1.0f;
	m.Diffuse.a = 1.0f;
	m.Diffuse.r = 1.0f;
	m.Diffuse.g = 1.0f;
	m.Diffuse.b = 1.0f;
	m.Emissive.a = 1.0f;
	m.Emissive.r = 1.0f;
	m.Emissive.g = 1.0f;
	m.Emissive.b = 1.0f;
	m.Specular.a = 1.0f;
	m.Specular.r = 1.0f;
	m.Specular.g = 1.0f;
	m.Specular.b = 1.0f;
	D3DMATERIAL9 s;
	device->GetMaterial(&s);
	device->SetRenderState(D3DRS_CULLMODE, false);
	device->SetMaterial(&m);
	device->SetTransform(D3DTS_WORLD, world);
	device->SetTexture(0, g_makabe_texture);
	device->SetFVF(D3DFVF_CUSTOMVERTEX);
	device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 12, makabe_mesh->_vertices, sizeof(CUSTOMVERTEX));
	device->SetMaterial(&s);
}
