#pragma once

#include "..\public\x_file_structure.h"

//x�t�@�C�����烁�b�V�����ƃe�N�X�`������ǂݍ���
extern void x_file_loader_load_from_x_file(x_file_t *strage, IDirect3DDevice9 *device, const char *file_name);

//�ǂݍ��񂾃��f�������
extern void x_file_loader_release(x_file_t *x_file);
