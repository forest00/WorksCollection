
//親ヘッダー
#include "x_file_loader.h"
//その他のヘッダー
#include <stdio.h>
#include <strsafe.h>
#include "..\..\common_def\public\common_def.h"


//xファイルからメッシュ情報とテクスチャ情報を読み込む
extern void x_file_loader_load_from_x_file(x_file_t *strage, IDirect3DDevice9 *device, const char *file_name)
{
	//ファイル名がnullなら、適当な値をセットして終わり
	if (strlen(file_name) <= 0)
	{
		strage->_mesh = nullptr;
		strage->_materials = nullptr;
		strage->_textures = nullptr;
		strage->_materials_num = 0;
		return;
	}

	x_file_t *m = strage;
	LPD3DXBUFFER pD3DXMtrlBuffer;
	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromX(file_name, D3DXMESH_SYSTEMMEM,
		device, NULL,
		&pD3DXMtrlBuffer, NULL, &m->_materials_num,
		&m->_mesh)))
	{
		// If model is not in current folder, try parent folder
		char file_name_2[FILENAME_MAX] = "..\\";
		strcat_s(file_name_2, sizeof(file_name_2), file_name);
		if (FAILED(D3DXLoadMeshFromX(file_name_2, D3DXMESH_SYSTEMMEM,
			device, NULL,
			&pD3DXMtrlBuffer, NULL, &m->_materials_num,
			&m->_mesh)))
		{
			char error_message[FILENAME_MAX] = "Could not find ";
			strcat_s(error_message, sizeof(error_message), file_name);
			MessageBox(NULL, error_message, "Meshes.exe", MB_OK);
			return;
		}
	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m->_materials = new D3DMATERIAL9[m->_materials_num];
	m->_textures = new LPDIRECT3DTEXTURE9[m->_materials_num];

	for (DWORD i = 0; i < m->_materials_num; i++)
	{
		// Copy the material
		m->_materials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		m->_materials[i].Ambient = m->_materials[i].Diffuse;

		m->_textures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFile(device,
				d3dxMaterials[i].pTextureFilename,
				&m->_textures[i])))
			{
				// If texture is not in current folder, try parent folder
				const TCHAR* strPrefix = TEXT("..\\");
				TCHAR strTexture[MAX_PATH];
				StringCchCopy(strTexture, MAX_PATH, strPrefix);
				StringCchCat(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				// If texture is not in current folder, try parent folder
				if (FAILED(D3DXCreateTextureFromFile(device,
					strTexture,
					&m->_textures[i])))
				{
					MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
				}
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();
}

//読み込んだモデルを解放
extern void x_file_loader_release(x_file_t *x_file)
{
	safe_delete_array(x_file->_materials);
	if (x_file->_textures != nullptr)
	{
		for (DWORD i = 0; i < x_file->_materials_num; i++)
		{
			x_file->_textures[i]->Release();
		}
		safe_delete_array(x_file->_textures);
	}
	safe_release(x_file->_mesh);
}
