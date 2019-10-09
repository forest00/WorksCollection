//�e�w�b�_�[
#include "..\public\anim_model_test.h"
//���̑��̃w�b�_�[
#include "..\public\anim_data_loader.h"
#include "things\robot.h"
#include "things\ken.h"
#include "things\golem.h"
#include "things\josh.h"
#include "things\usagi.h"
#include "things\block_man.h"
#include "..\..\input\public\input.h"
#include "..\..\sound\public\sound.h"



//�S�Ẵe�X�g�p�̃��f�������������AX�t�@�C����ǂݍ��݁A�o��������
extern void anim_model_tester_all_init(IDirect3DDevice9 *device)
{
	//�t�@�C����ǂݍ���
	anim_data_load_all(device);
	//�A�j���[�V�����������S���o��
	robot_init();
	ken_init();
	golem_init();
	josh_init();
	usagi_init();
	block_man_init();
}

//�S�Ẵ��f����j������
extern void anim_model_tester_all_end()
{
	//�A�j���[�V�����������S���j��
	robot_end();
	ken_end();
	golem_end();
	josh_end();
	usagi_end();
	block_man_end();
	//�ǂݍ��񂾃t�@�C����j��
	anim_data_destroy_all();
}

//�S�Ẵ��f�����X�V����
extern void anim_model_tester_all_update()
{
	//�A�j���[�V�����������S���X�V
	robot_update();
	ken_update();
	golem_update();
	josh_update();
	usagi_update();
	block_man_update();
	//�����o���Ă݂�
	if (input_keyboard_trigger(DIK_F9))
	{
		sound_play(sound_index_se_1);
	}
}

//�S�Ẵ��f����`�悷��
extern void anim_model_tester_all_draw(IDirect3DDevice9 *device)
{
	//�A�j���[�V�����������S���`��
	robot_draw(device);
	ken_draw(device);
	golem_draw(device);
	josh_draw(device);
	usagi_draw(device);
	block_man_draw(device);
}




//�v���C���[�Ƃ��Ďg�����f���̈ʒu�𓾂�
void anim_model_get_the_position_of_the_model_to_use_as_a_player(D3DXVECTOR3 *out)
{
	golem_get_position_special(out);
}

