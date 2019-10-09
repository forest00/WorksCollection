
//�e�w�b�_�[
#include "..\public\x_file.h"
//���̑��̃w�b�_�[
#include <strsafe.h>
#include "x_file_database.h"
#include "x_file_drawer.h"


//x�t�@�C���g�p�J�n
extern void x_file_init(IDirect3DDevice9 *device)
{
	//�f�[�^�x�[�X��p��
	x_file_database_init(device);
}

//x�t�@�C���g�p�I��
extern void x_file_end()
{
	//�f�[�^�x�[�X��j��
	x_file_database_end();
}


//���f�������f�[�^�x�[�X�������Ă���
extern void x_file_t_load_from_database(x_file_t *strage, x_file_type_t type)
{
	x_file_database_get(strage, type);
}
extern void x_file_t_load_from_database(x_file_t *strage, size_t type)
{
	x_file_database_get(strage, type);
}


//���f����`��
extern void x_file_t_draw(IDirect3DDevice9 *device, const x_file_t *x_file)
{
	x_file_drawer_draw(device, x_file);
}
