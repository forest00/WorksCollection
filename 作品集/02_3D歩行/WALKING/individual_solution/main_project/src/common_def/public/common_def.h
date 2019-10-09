#pragma once

#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")



//ƒƒ‚ƒŠ‚Ìƒ}ƒNƒ
/*------------------------------*/
#define safe_delete(p)			{ if(p) { delete (p);     (p) = nullptr; } }
#define safe_delete_array(p)	{ if(p) { delete[] (p);   (p) = nullptr; } }
#define safe_release(p)			{ if(p) { (p)->Release(); (p) = nullptr; } }
#define safe_free(p)			{ if(p) { free(p);        (p) = nullptr; } }
/*------------------------------*/

//D3DX_PI‚Ífloat‚È‚Ì‚ÅŽg‚í‚È‚¢
#undef D3DX_PI




typedef D3DXVECTOR2 vector2_t;
typedef D3DXVECTOR3 vector3_t;
typedef D3DXVECTOR4 vector4_t;
typedef D3DXMATRIX matrix4x4;
