#pragma once

#include "..\public\x_file.h"
#include "..\public\x_file_type.h"


//�g�p����x�t�@�C����S���ǂݍ���
extern void x_file_database_init(IDirect3DDevice9 *device);

//�ǂݍ���x�t�@�C�������
extern void x_file_database_end();

//�ǂݍ��ݍς݂�x�t�@�C�����擾
extern void x_file_database_get(x_file_t *strage, x_file_type_t type);
extern void x_file_database_get(x_file_t *strage, size_t type);

