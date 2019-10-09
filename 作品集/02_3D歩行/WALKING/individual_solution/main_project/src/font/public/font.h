
#include <d3dx9.h>


enum font_color_t
{
	c_font_color_white,			//白
	c_font_color_red,			//赤
	c_font_color_green,			//緑
	c_font_color_blue,			//青
	c_font_color_magenta,		//明るい赤
	c_font_color_lime,			//明るい緑
	c_font_color_aqua,			//明るい青
	c_font_color_purple,		//紫
	c_font_color_black,			//黒
};

//初期化
extern int font_init(IDirect3DDevice9 *device);

//バッファに保存しておいたものを描画
extern void font_draw(IDirect3DDevice9 *device);

//終了
extern void font_end();


//バッファに書き込む
extern void font_draw_buffer(const char *str, int x, int y, size_t height);

//バッファに書き込む(アルファ指定)
extern void font_draw_buffer(const char *str, int x, int y, size_t height, int alpha);

//バッファに書き込む(色指定付き)
extern void font_draw_buffer_with_color(const char *str, int x, int y, size_t height, int alpha, int red, int green, int blue);

//バッファに書き込む(色指定付き)
extern void font_draw_buffer_with_color(const char *str, int x, int y, size_t height, int alpha, font_color_t color);

//printfの書式に基づいてフォント描画
extern void font_draw_format(int x, int y, size_t height, const char *format, ...);

//printfの書式に基づいてフォント描画(色指定付き)
extern void font_draw_format(int x, int y, size_t height, int alpha, int red, int green, int blue, const char *format, ...);

