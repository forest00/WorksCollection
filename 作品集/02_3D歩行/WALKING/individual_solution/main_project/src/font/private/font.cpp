

#include <stdio.h>
#include <string.h>
#include "..\public\font.h"
#include "font_base.h"


struct font_draw_data_t
{
	char str[FILENAME_MAX];
	int x;
	int y;
	size_t height;
	int alpha;
	int red;
	int green;
	int blue;
};

enum font_draw_buffer_max_t
{
	c_font_draw_buffer_max = 256,
};

static font_draw_data_t buffer[c_font_draw_buffer_max];

static size_t buffer_p = 0;

static font_base_t *g_font_base;

/*--------------------------------*/
//ローカル関数
static void write(const char *str, int x, int y, size_t height, int alpha, int red, int green, int blue)
{
	strcpy_s(buffer[buffer_p].str, FILENAME_MAX, str);
	buffer[buffer_p].x = x;
	buffer[buffer_p].y = y;
	buffer[buffer_p].height = height;
	buffer[buffer_p].alpha = alpha;
	buffer[buffer_p].red = red;
	buffer[buffer_p].green = green;
	buffer[buffer_p].blue = blue;
	buffer_p++;
}
/*--------------------------------*/



extern int font_init(IDirect3DDevice9 *device)
{
	g_font_base = font_base_init(device);
	if (g_font_base == nullptr)
	{
		return -1;
	}
	return 0;
}

extern void font_draw(IDirect3DDevice9 *device)
{
	for (size_t i = 0; i < buffer_p; i++)
	{
		font_base_draw(
			device,
			g_font_base,
			buffer[i].str,
			buffer[i].x,
			buffer[i].y,
			buffer[i].height,
			buffer[i].alpha,
			buffer[i].red,
			buffer[i].green,
			buffer[i].blue
		);
	}
	buffer_p = 0;
}

extern void font_end()
{
	font_base_end(&g_font_base);
}

//バッファに書き込む
extern void font_draw_buffer(const char *str, int x, int y, size_t height)
{
	font_draw_buffer_with_color(str, x, y, height, 255, 255, 255, 255);
}

//バッファに書き込む(アルファ指定)
extern void font_draw_buffer(const char *str, int x, int y, size_t height, int alpha)
{
	font_draw_buffer_with_color(str, x, y, height, alpha, 255, 255, 255);
}

//バッファに書き込む(色指定付き)
extern void font_draw_buffer_with_color(const char *str, int x, int y, size_t height, int alpha, int red, int green, int blue)
{
	write(str, x, y, height, alpha, red, green, blue);
}

//バッファに書き込む(色指定付き)
extern void font_draw_buffer_with_color(const char *str, int x, int y, size_t height, int alpha, font_color_t color)
{
	switch (color)
	{
	case c_font_color_white:
		font_draw_buffer_with_color(str, x, y, height, alpha, 255, 255, 255);
		break;
	case c_font_color_red:
		font_draw_buffer_with_color(str, x, y, height, alpha, 255, 0, 0);
		break;
	case c_font_color_green:
		font_draw_buffer_with_color(str, x, y, height, alpha, 0, 128, 0);
		break;
	case c_font_color_blue:
		font_draw_buffer_with_color(str, x, y, height, alpha, 0, 0, 255);
		break;
	case c_font_color_magenta:
		font_draw_buffer_with_color(str, x, y, height, alpha, 218, 0, 255);
		break;
	case c_font_color_lime:
		font_draw_buffer_with_color(str, x, y, height, alpha, 0, 255, 0);
		break;
	case c_font_color_aqua:
		font_draw_buffer_with_color(str, x, y, height, alpha, 0, 255, 255);
		break;
	case c_font_color_purple:
		font_draw_buffer_with_color(str, x, y, height, alpha, 255, 0, 255);
		break;
	case c_font_color_black:
		font_draw_buffer_with_color(str, x, y, height, alpha, 0, 0, 0);
		break;
	default:
		font_draw_buffer_with_color(str, x, y, height, 0, 0, 0, 0);
		break;
	}
}

//printfの書式に基づいてフォント描画
extern void font_draw_format(int x, int y, size_t height, const char *format, ...)
{
	static char buffer[FILENAME_MAX];
	va_list list;
	va_start(list, format);
	vsnprintf(buffer, FILENAME_MAX, format, list);
	va_end(list);
	font_draw_buffer(buffer, x, y, height);
}

//printfの書式に基づいてフォント描画(色指定付き)
extern void font_draw_format(int x, int y, size_t height, int alpha, int red, int green, int blue, const char *format, ...)
{
	static char buffer[FILENAME_MAX];
	va_list list;
	va_start(list, format);
	vsnprintf(buffer, FILENAME_MAX, format, list);
	va_end(list);
	font_draw_buffer_with_color(buffer, x, y, height, alpha, red, green, blue);
}