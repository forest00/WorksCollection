//親ヘッダー
#include "startup_option.h"
//その他のヘッダー
#include <assert.h>
#include "..\file_data\file_data.h"

static const datum_t *g_data = nullptr;
static size_t g_data_num= 0;

static void startup_option_load()
{
	if (g_data == nullptr)
	{
		g_data = file_data_get(fdi_window_startup_option, &g_data_num);
		assert(g_data_num >= 7);
	}
}
extern int startup_option_get_screen_mode()
{
	startup_option_load();
	return g_data[1]._Int;
}
extern int startup_option_get_left()
{
	startup_option_load();
	return g_data[2]._Int;
}
extern int startup_option_get_top()
{
	startup_option_load();
	return g_data[3]._Int;
}
extern int startup_option_get_width()
{
	startup_option_load();
	return g_data[4]._Int;
}
extern int startup_option_get_height()
{
	startup_option_load();
	return g_data[5]._Int;
}
extern bool startup_option_get_hide_mouse()
{
	startup_option_load();
	return g_data[6]._Bool;
}
extern const char *startup_option_get_window_title()
{
	startup_option_load();
	return g_data[0]._String;
}


