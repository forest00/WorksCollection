#pragma once
//char�^�̃|�C���^�𑀍삷��֐��������ꂽ�w�b�_�[�t�@�C���ł�
//�쐬�ҁ@�X�@�m�u

extern void create_string_from_file(const char *file_name, char **string, size_t *string_size);

//�^����ꂽ������̊��S�ȃN���[�������
extern void clone_string(const char *string, size_t string_size, char **cloned, size_t *cloned_size);

//��������������
extern void destroy_string(char **string);

//printf�Ɠ��������ɏ]���ĕ�����쐬
extern char *format(const char *format, ...);
