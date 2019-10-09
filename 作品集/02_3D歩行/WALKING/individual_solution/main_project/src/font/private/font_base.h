#pragma once

#include <d3dx9.h>

struct font_base_t;

//������
extern font_base_t *font_base_init(IDirect3DDevice9 *device);

//�I��
extern void font_base_end(font_base_t **font_base);

//�`��
extern void font_base_draw(IDirect3DDevice9 *device, font_base_t *font_base, const char *str, int x, int y, size_t height, int alpha, int red, int green, int blue);
