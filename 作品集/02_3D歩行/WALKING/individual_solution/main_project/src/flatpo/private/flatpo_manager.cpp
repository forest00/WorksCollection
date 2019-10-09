
//�e�w�b�_�[
#include "..\public\flatpo_manager.h"
//���̑��̃w�b�_�[
#include "..\public\flatpo_texture_loader.h"
#include "flatpo_drawer.h"
#include "flatpo_test.h"

//�|���̏�����
extern void flatpo_manager_init(IDirect3DDevice9 *device)
{
	flatpo_texture_loader_load_all(device);
	flatpo_tester_all_init();
}

//�|���̏I��
extern void flatpo_manager_end()
{
	flatpo_tester_all_end();
	flatpo_texture_loader_destroy_all();
}

//�|���̍X�V
extern void flatpo_manager_update()
{
	flatpo_tester_all_update();
}

//�|���̕`��
extern void flatpo_manager_draw(IDirect3DDevice9 *device, double aspect)
{
	//�|���`��J�n
	flatpo_begin_scene(device, aspect);
	//�e�X�g�p�̔|����`��
	flatpo_tester_all_draw(device);
	//�|���`��I��
	flatpo_end_scene();
}

