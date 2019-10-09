

//親ヘッダー
#include "..\public\back.h"
//その他のヘッダー
#include <assert.h>
#include "..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\x_object\public\x_object.h"


/*------------------------------*/
struct back_t
{
	x_object_t *_object;
	x_object_t *_sky;
};

static back_t *back_t_allocation();

static void back_t_init(back_t *back);

static void back_t_end(back_t *back);

static void back_t_update(back_t *back);

static void back_t_draw(back_t *back, IDirect3DDevice9 *device);
/*------------------------------*/



/*------------------------------*/
static back_t *back_t_allocation()
{
	void *v = malloc(sizeof(back_t));
	assert(v != nullptr);
	return (back_t*)(v);
}

static void back_t_destroy(back_t **back)
{
	safe_free(*back);
}

static void back_t_init(back_t *back)
{
	x_object_t_init(&back->_object,
		x_file_type_back01,
		&vector3_t(0.0f, 0.0f, 0.0f),
		&vector3_t(0.0f, 0.0f, 0.0f),
		&vector3_t(5.0f, 0.0f, 5.0f)
	);
	x_object_t_init(&back->_sky,
		x_file_type_sky,
		&vector3_t(0.0f, -100.0f, 0.0f),
		&vector3_t(0.0f, 0.0f, 0.0f),
		&vector3_t(0.03f, 1.0f, 0.03f)
	);
}

static void back_t_end(back_t *back)
{
	x_object_t_end(&back->_object);
	x_object_t_end(&back->_sky);
}

static void back_t_update(back_t *back)
{
	;
}

static void back_t_draw(back_t *back, IDirect3DDevice9 *device)
{
	x_object_t_draw(back->_object, device);
	device->SetRenderState(D3DRS_CULLMODE, false);
	//x_object_t_draw(back->_sky, device);
}
/*------------------------------*/



/*------------------------------*/
static back_t *g_back = nullptr;
/*------------------------------*/







extern void back_manager_init()
{
	g_back = back_t_allocation();
	back_t_init(g_back);
}

extern void back_manager_update()
{
	back_t_update(g_back);
}

extern void back_manager_draw(IDirect3DDevice9 *device)
{
	back_t_draw(g_back, device);
}

extern void back_manager_end()
{
	back_t_end(g_back);
	back_t_destroy(&g_back);
}

