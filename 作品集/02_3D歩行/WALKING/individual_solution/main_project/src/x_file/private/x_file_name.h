#pragma once

#include "..\public\x_file_type.h"


//x�t�@�C���̃p�X���܂Ƃ߂ď����ꂽ�t�@�C���̖��O
extern const char *x_file_path_unit();

//x�t�@�C���̃p�X��ǂݍ���
//�����ɂ�x_model_file_path_unit()�̕Ԃ�l�����̂܂ܓn��
extern void x_file_path_read(const char *file_name);

//�^�C�v�ɑΉ������t�@�C������Ԃ�
extern const char *x_file_path_get(x_file_type_t type);
extern const char *x_file_path_get(size_t type);
