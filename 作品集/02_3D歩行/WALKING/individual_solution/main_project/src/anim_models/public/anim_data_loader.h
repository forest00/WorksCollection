#pragma once

#include <d3dx9.h>


//�A�j���[�V�������f���̎��
enum anim_model_kind_t
{
	anim_model_kind_none,			//��
	anim_model_kind_robot,			//���{�b�g
	anim_model_kind_ken,			//���������
	anim_model_kind_golem,			//�S�[����
	anim_model_kind_josh,			//josh����
	anim_model_kind_usagi,			//�E�T�M����
	anim_model_kind_block_man,		//�����u���b�N�j
	anim_model_kind_max,			//�ő�
};

//X�t�@�C���ɓ����Ă���A�j���[�V�����̏���ǂݍ��ލ\����
//���ۂɓǂݍ��ނɂ̓q�G�����L�[���K�v�ł�
struct anim_data_t
{
	D3DXFRAME *pFrameRoot;								//?
	ID3DXAnimationSet **pAnimSet;						//?
	ID3DXAnimationController *pAnimController;			//?
};




//�A�j���f�[�^��S���ǂݍ���
extern void anim_data_load_all(IDirect3DDevice9 *device);

//�ǂݍ��񂾃A�j���f�[�^��S���j��
extern void anim_data_destroy_all();

//�ǂݍ��񂾃A�j���f�[�^���擾
extern const anim_data_t *anim_data_get(anim_model_kind_t anim_kind);

//�A�j���[�V�����R���g���[���[�̃N���[���𐶐�
extern void anim_controller_clone(anim_model_kind_t anim_kind, ID3DXAnimationController **destination);

