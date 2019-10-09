#pragma once

#include <d3dx9.h>

struct makabe_mesh_t;

extern void makabe_mesh_creator_start_up(IDirect3DDevice9 *device);

extern void makabe_mesh_creator_end();

extern void makabe_mesh_creator_create(makabe_mesh_t **makabe_mesh, IDirect3DDevice9 *device);

extern void makabe_mesh_creator_destroy(makabe_mesh_t **makabe_mesh);

extern void makabe_mesh_creator_draw(makabe_mesh_t *makabe_mesh, IDirect3DDevice9 *device, const D3DXMATRIX *world);

