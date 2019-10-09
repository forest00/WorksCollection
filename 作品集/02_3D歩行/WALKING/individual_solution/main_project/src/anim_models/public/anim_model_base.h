//	
//	�A�j���[�V�����t�����b�V���̓ǂݍ��݂Ɣj���A�`��ƃA�j���Đ���S������t�@�C���ł�
//	
//	�����͂قڑS�ăR�s�y�ł�
//	
#pragma once
//�K�v�ȃw�b�_�[�t�@�C���̃C���N���[�h
#include <windows.h>
#include <d3dx9.h>
#include <stdio.h>
#include "anim_data_loader.h"

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
//// �V���{����`�y�у}�N��
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define THING_AMOUNT 1


//�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};
//�h�����b�V���R���e�i�[�\����(
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;//�d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD dwBoneNum;//�{�[���̐�
	LPD3DXBUFFER pBoneBuffer;//�{�[���E�e�[�u��
	D3DXMATRIX** ppBoneMatrix;//�S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
	D3DXMATRIX* pBoneOffsetMatrices;//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[
};
//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, D3DXFRAME **);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCTSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

//�{�[���̍s����m��(?)
extern HRESULT AllocateBoneMatrix(D3DXFRAME *pFrameRoot, LPD3DXMESHCONTAINER);

//�S�Ẵ{�[���ɑ΂��čs����m��(?)
//�X�L�����b�V���ɑ΂��Ďg���炵���ł�
extern HRESULT AllocateAllBoneMatrices(D3DXFRAME *pFrameRoot, D3DXFRAME *pFrame);

//�t���[�����̂��ꂼ��̃��b�V���������_�����O����
extern VOID RenderMeshContainer(LPDIRECT3DDEVICE9, MYMESHCONTAINER*, MYFRAME*);

//�t���[���������_�����O����B
extern VOID UpdateFrameMatrices(LPD3DXFRAME, const D3DXMATRIX*);

//�t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V����
extern VOID RenderFrame(LPDIRECT3DDEVICE9, LPD3DXFRAME);

//�L���ȃq�G�����L�[�̃|�C���^��Ԃ�
///ID3DXAllocateHierarchy�̃|�C���^�ɈÖٕϊ��\
//ID3DXAllocateHierarchy�̃|�C���^�ɕϊ��ς݂̂��̂�Ԃ��܂�
//�q�G�����L�[���K�v�ȏꏊ�Ŏg���Ă�
extern ID3DXAllocateHierarchy *GetValidHierarchy();

//�K�w���A�S�Ă�DirectX�I�u�W�F�N�g�������[�X����
extern void FreeAnim(D3DXFRAME *pFrame, ID3DXAllocateHierarchy *cHierarchy);

///�ȏ�R�s�y
///�ȉ�����


// ���̂�\���p�����[�^�[
struct thing_parameter_t
{
	D3DXVECTOR3 _position;		//�ʒu
	D3DXVECTOR3 _angle;			//�p�x
	D3DXVECTOR3 _scale;			//�g��{��
};

//�A�j���[�V�������I��������ɌJ��Ԃ��Đ����邩
enum anim_repeat_flag_t
{
	anim_repeat_on,				//�J��Ԃ�
	anim_repeat_off,			//�~�߂�(�J��Ԃ��Ȃ�)
};

//�A�j���[�V�����̃p�����[�^�[
struct anim_parameter_t
{
	anim_model_kind_t _anim_kind;					//�ǂ̃A�j���[�V�����f�[�^���Q�Ƃ��Ă��邩
	const anim_data_t *_data;						//�Q�Ƃ���A�j���[�V�����f�[�^
	ID3DXAnimationController *_controller;			//�A�j���[�V�����R���g���[���[
	bool _playing;									//�A�j���[�V�������Ă��邩
	float _total_time;								//�A�j���[�V�������v����
	float _last_changed_time;						//�Ō�ɐ؂�ւ�����u��
	int _playing_index;								//�Đ����̃A�j���̔ԍ�
	anim_repeat_flag_t _repeat_flag;				//�A�j���[�V�������I��������ɌJ��Ԃ��Đ����邩
};


//�A�j���[�V�����̏�����
void anim_init(anim_parameter_t *anim);

//�A�j���[�V�����̏���
void anim_setup(anim_parameter_t *anim, anim_model_kind_t anim_kind);

//�A�j���[�V��������߂�
void anim_end(anim_parameter_t *anim);

//�A�j���[�V�������Đ�����
/*
*@param anim						�Ώۂ̃p�����[�^�[�\����
*@param change						���Ԃ̃A�j���[�V�����ɂ��邩
*@param do_at_the_end				�A�j���[�V�������I��������ɌJ��Ԃ��Đ����邩
*@return 0...���� -1...�A�j���[�V�����R���g���[���[�����݂��Ȃ� -2...change���͈͊O�ł���
*/
int anim_play(anim_parameter_t *anim, int change, anim_repeat_flag_t repeat_flag);

//�A�j���[�V������i�߂�
/*
*@param anim						�Ώۂ̃p�����[�^�[�\����
*@param advance_time				�i�߂鎞��
*@param *duration					�A�j���̈�񕪂̍Đ����Ԃ��i�[����
*@param *duration_nextafter0		�A�j�����Đ�����؂�Ȃ����肬��̎��Ԃ��i�[����
*@param *anim_count					�A�j�������[�v���ꂽ�񐔂��i�[����
*@param *anim_position				������̃A�j���̈ʒu���i�[����
*@param *anim_prev_position			�����O�̃A�j���̈ʒu���i�[����
*@param *anim_is_ended				�A�j�����Đ�����؂�Ȃ����肬��̎��ԂɒB���Ă��邩���i�[����(anim_position >= duration_nextafter0)
*@return ����
*/
void anim_advance(
	anim_parameter_t *anim,
	float advance_time,
	float *duration,
	float *duration_nextafter0,
	size_t *anim_count,
	float *anim_position,
	float *anim_prev_position,
	bool *anim_is_ended
);

//�A�j���[�V�������镨�̂�`��
/*
*@param *device			Direct3D�̃f�o�C�X
*@param *world			���[���h�ϊ��ς݂̍s��
*@param *anim			�������ς݂̃A�j���[�V�����p�����[�^�[
*@return ����
*/
void anim_thing_draw(IDirect3DDevice9 *device, const D3DXMATRIX *world, const anim_parameter_t *anim);


