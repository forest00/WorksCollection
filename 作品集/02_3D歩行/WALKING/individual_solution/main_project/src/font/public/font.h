
#include <d3dx9.h>


enum font_color_t
{
	c_font_color_white,			//��
	c_font_color_red,			//��
	c_font_color_green,			//��
	c_font_color_blue,			//��
	c_font_color_magenta,		//���邢��
	c_font_color_lime,			//���邢��
	c_font_color_aqua,			//���邢��
	c_font_color_purple,		//��
	c_font_color_black,			//��
};

//������
extern int font_init(IDirect3DDevice9 *device);

//�o�b�t�@�ɕۑ����Ă��������̂�`��
extern void font_draw(IDirect3DDevice9 *device);

//�I��
extern void font_end();


//�o�b�t�@�ɏ�������
extern void font_draw_buffer(const char *str, int x, int y, size_t height);

//�o�b�t�@�ɏ�������(�A���t�@�w��)
extern void font_draw_buffer(const char *str, int x, int y, size_t height, int alpha);

//�o�b�t�@�ɏ�������(�F�w��t��)
extern void font_draw_buffer_with_color(const char *str, int x, int y, size_t height, int alpha, int red, int green, int blue);

//�o�b�t�@�ɏ�������(�F�w��t��)
extern void font_draw_buffer_with_color(const char *str, int x, int y, size_t height, int alpha, font_color_t color);

//printf�̏����Ɋ�Â��ăt�H���g�`��
extern void font_draw_format(int x, int y, size_t height, const char *format, ...);

//printf�̏����Ɋ�Â��ăt�H���g�`��(�F�w��t��)
extern void font_draw_format(int x, int y, size_t height, int alpha, int red, int green, int blue, const char *format, ...);

