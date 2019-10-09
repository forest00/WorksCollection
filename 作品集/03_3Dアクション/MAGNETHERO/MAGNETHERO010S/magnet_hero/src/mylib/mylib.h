//�G���Ȋ֐���񋟂���ꏊ�ł�
//�쐬�ҁ@�X�@�m�u
#pragma once
/*----------------------------------------------------------------------------------------------------------------------*/

// D3D9���C�u����
#pragma comment( lib, "d3d9")
// D3DX9���C�u����
#pragma comment( lib, "d3dx9")
// DirectSound���C�u����
#pragma comment( lib, "dsound")
// DirectX GUID���C�u����
#pragma comment( lib, "dxguid")
// �}���`���f�B�A���C�u����
#pragma comment( lib, "winmm")
// �f�W�^���r�f�I���C�u����
#pragma comment( lib, "vfw32")
// �������k�𓀃��C�u����
//#pragma comment( lib, "msacm32")

//Windows�C���N���[�h
#include <windows.h>
//DirectX�̃��C�u�����C���N���[�h
#include <d3d9.h>
#include <d3dx9.h>
//���w�֐����C���N���[�h
#define _USE_MATH_DEFINES
#include <math.h>
#undef _USE_MATH_DEFINES

//���z�̓���
#include "..\input\virtual_input.h"

// �R�[�h�y�[�W�̌x���}�~
#pragma warning( disable : 4819 )

/*----------------------------------------------------------------------------------------------------------------------*/
//�}�N����

#define _mylib_safe_free(p)\
{\
	if(p)\
	{\
		free(p);\
		(p) = nullptr;\
	}\
}
#define _mylib_safe_release(p)\
{\
	if(p)\
	{\
		(p)->Release();\
		(p) = nullptr;\
	}\
}
#define _mylib_safe_delete(p)\
{\
	if(p)\
	{\
		delete (p);\
		(p) = nullptr;\
	}\
}
#define _mylib_texture_handle_max		0x1000
#define _mylib_error_box(message)		mylib_error_box(__FILE__,__LINE__,message,"�G���[",MB_OK)
/*----------------------------------------------------------------------------------------------------------------------*/
//�^��`��

//�������@
enum mylib_blend_mode_t
{
	mbm_none,
	mbm_alpha,
	mbm_add,
	mbm_factor,
	mbm_max,
};
/*----------------------------------------------------------------------------------------------------------------------*/
//�֐���`��

/*
*@fn mylib_init
*@brief ���̃��C�u�����̏�����
*@param instance �C���X�^���X
*@param full_opt �t���X�N���[���ŃE�B���h�E���쐬����I�v�V����
*        0...�m�F�_�C�A���O���o��
*        1...�t���X�N���[��
*        2...�E�B���h�E���[�h
*@param x,y �E�B���h�E���[�h�ɂ���Ƃ��̍���ʒu
*@param width �E�B���h�E���[�h�ɂ���Ƃ��̃E�B���h�E��
*@param height �E�B���h�E���[�h�ɂ���Ƃ��̃E�B���h�E����
*@param window_title �E�B���h�E���[�h�ɂ��鎞�̃E�B���h�E�^�C�g��
*@param icon_filename �E�B���h�E�̍���ƍŏ��������Ƃ��Ɏg���A�C�R���̃t�@�C�����ł�
*@return  0...����
 *       -1...���[�U�[�����s���ۂ���
 *       -2...�����ł��Ȃ�
*@detail ���̊֐����g�����Ԃɂ��܂�
*        Direct3D�̃f�o�C�X�Ƃ����Ǘ����܂�
*        ���̊֐����ĂԂƃE�B���h�E����쐬���āADirectX���C�u�����̏��������s���܂�
*        �֐������s�����ꍇ�͑�����mylib_end���Ăяo���������ŃA�v���P�[�V�������I�����Ȃ��Ă͂����܂���
*/
extern int mylib_init(
	HINSTANCE instance,
	int full_opt,
	int x, int y,
	int width,
	int height,
	const char *window_title,
	const char *icon_filename
);

/*
*@fn mylib_init
*@brief ���̃��C�u�����̏I������
*@detail �Ăяo���Ƒ��̊֐����g���Ȃ��Ȃ�܂�
*        �������Ɏ��s�����Ƃ��ƃQ�[�����I������Ƃ��ɌĂяo���K�v������܂�
*/
extern void mylib_end();

/*
*@fn mylib_process_message
*@brief ���b�Z�[�W���������܂�
*@return �Q�[���𑱍s����ׂ��Ȃ�true��Ԃ��܂�
*/
extern bool mylib_process_message();

/*
*@fn mylib_get_frame_count
*@brief ���Ԃ�m�邽�߂̊֐��ł�
*@return ���b�Z�[�W�������s���֐����Ă񂾉񐔂�Ԃ��܂�
*/
extern time_t mylib_get_frame_count();

/*
*@fn mylib_get_instance
*@brief ���������ɓn���ꂽHINSTANCE�����̂܂ܕԂ��܂�
*/
extern HINSTANCE mylib_get_instance();

/*
*@fn mylib_get_window
*@brief mylib�����������E�B���h�E�̃n���h����Ԃ��܂�
*/
extern HWND mylib_get_window();

/*
*@fn mylib_get_d3d
*@brief �������ς݂�D3D�I�u�W�F�N�g��Ԃ��܂�
*/
extern IDirect3D9 *mylib_get_d3d();

/*
*@fn mylib_get_d3d_device
*@brief �������ς݂�D3D�f�o�C�X��Ԃ��܂�
*/
extern IDirect3DDevice9 *mylib_get_d3d_device();

/*
*@fn mylib_is_full_mode
*@return �t���X�N���[�����[�h�Ŏ��s���Ȃ�true
*/
extern bool mylib_is_full_mode();

/*
*@fn mylib_input_update
*@brief ���͂̍X�V
*/
extern void mylib_input_update();

extern const virtual_input_t *mylib_get_virtual_input_repeat();
extern const virtual_input_t *mylib_get_virtual_input_trigger();
extern const virtual_input_t *mylib_get_virtual_input_release();

/*
*@fn mylib_get_aspect
*@brief ��ʂ̃A�X�y�N�g����擾
*/
extern float mylib_get_aspect();

/*
*@fn mylib_calc_view
*@brief �Z�b�g���ꂽ�J�����̏�񂩂�r���[�s����v�Z����
*@param view �v�Z���ʂ��i�[����
*@param eye �J�����̈ʒu
*@param at �J���������Ă���ʒu
*@param up �J�����̏����
*@detail ���߂��r���[�s����Amylib_set_view���g���Ċi�[���Ȃ���΂����܂���
*/
extern void mylib_calc_view(D3DXMATRIX *view, const D3DXVECTOR3 *eye, const D3DXVECTOR3 *at, const D3DXVECTOR3 *up);

/*
*@fn mylib_set_view
*@brief �r���[�s���ݒ肷��
*/
extern void mylib_set_view(const D3DXMATRIX *view);

/*
*@fn mylib_get_view
*@brief �r���[�s����擾����
*/
extern void mylib_get_view(D3DXMATRIX *view);

/*
*@fn mylib_calc_projection
*@brief �Z�b�g���ꂽ�J�����̏�񂩂�v���W�F�N�V�����s����v�Z����
*@param projection �v�Z���ʂ��i�[����
*@param fov_y ����
*@param near_clip �߃N���b�v��
*@param far_clip ���N���b�v��
*@detail ���߂��v���W�F�N�V�����s����Amylib_set_projection���g���Ċi�[���Ȃ���΂����܂���
*/
extern void mylib_calc_projection(D3DXMATRIX *projection, float fov_y, float near_clip, float far_clip);

/*
*@fn mylib_set_projection
*@brief �v���W�F�N�V�����s���ݒ肷��
*/
extern void mylib_set_projection(const D3DXMATRIX *projection);

/*
*@fn mylib_get_projection
*@brief �v���W�F�N�V�����s����擾����
*/
extern void mylib_get_projection(D3DXMATRIX *projection);

/*
*@fn mylib_clear
*@brief �N���A���܂�
*/
extern void mylib_clear(int red, int green, int blue);

/*
*@fn mylib_begin_scene
*@brief �V�[�����J�n���܂�
*@detail ���������0���A���s�����0�ȊO�̒l��Ԃ��܂�
*/
extern int mylib_begin_scene();

/*
*@fn mylib_end_scene
*@brief �V�[�����I�����܂�
*/
extern void mylib_end_scene();

/*
*@fn mylib_present
*@brief �v���[���g���܂�
*/
extern void mylib_present();

/*
*@fn mylib_create_texture_can_render_target
*@brief �����_�[�^�[�Q�b�g�Ɏw��\�ȃe�N�X�`�����쐬���܂�
*@param texture �쐬�����e�N�X�`�����󂯎��
*@param width �쐬����e�N�X�`���̕�
*@param height �쐬����e�N�X�`���̍���
*@detail todo:�ڍא���������
*/
extern void mylib_create_texture_can_render_target(IDirect3DTexture9 **texture, int width, int height);

/*
*@fn mylib_create_depth_stencil
*@brief �[�x�X�e���V���o�b�t�@���쐬���܂�
*@param surface �쐬�����T�[�t�F�X���󂯎��
*@param width �쐬����T�[�t�F�X�̕�
*@param height �쐬����T�[�t�F�X�̍���
*@detail todo:�ڍא���������
*/
extern void mylib_create_depth_stencil(IDirect3DSurface9 **surface, int width, int height);

/*
*@fn mylib_set_render_target_surface
*@brief �����_�[�^�[�Q�b�g��؂�ւ��܂�
*@param surface �^�[�Q�b�g�Ɏw�肷��T�[�t�F�X
*/
extern void mylib_set_render_target_surface(IDirect3DSurface9 *surface);

/*
*@fn mylib_set_render_target_texture
*@brief �����_�[�^�[�Q�b�g���e�N�X�`���ɐ؂�ւ��܂�
*@param texture �^�[�Q�b�g�Ɏw�肷��e�N�X�`��
*@detail ���ۂ̃����_�[�^�[�Q�b�g�̓e�N�X�`������擾�����T�[�t�F�X�ɂȂ�܂�
*/
extern void mylib_set_render_target_texture(IDirect3DTexture9 *texture);

/*
*@fn mylib_set_depth_stencil
*@brief �[�x�X�e���V���o�b�t�@��؂�ւ��܂�
*@param surface �^�[�Q�b�g�Ɏw�肷��T�[�t�F�X
*/
extern void mylib_set_depth_stencil(IDirect3DSurface9 *surface);

/*
*@fn mylib_set_render_target_manage
*@brief �����_�[�^�[�Q�b�g�����C�u�����Ǘ����̂��̂ɐ؂�ւ��܂�
*@param target �^�[�Q�b�g���w�肵�܂�
*@detail 1���w�肷��ƃe�N�X�`�����^�[�Q�b�g�ɐݒ肵�A0���ƃo�b�N�o�b�t�@���^�[�Q�b�g�ɂ��܂�
*/
extern void mylib_set_render_target_manage(int target);

/*
*@fn mylib_get_render_target
*@brief �����_�[�^�[�Q�b�g���擾���܂�
*@param surface �󂯎��T�[�t�F�X
*/
extern void mylib_get_render_target(IDirect3DSurface9 **surface);

/*
*@fn mylib_get_depth_stencil
*@brief �[�x�X�e���V���o�b�t�@���擾���܂�
*@param surface �󂯎��T�[�t�F�X
*/
extern void mylib_get_depth_stencil(IDirect3DSurface9 **surface);

/*
*@fn mylib_get_rendered_texture
*@brief �����_�[�^�[�Q�b�g�Ƃ��Ďg���Ă���e�N�X�`�����擾���܂�
*@detail ���̊֐��̕Ԃ�l��mylib_change_render_target�֐���1��n�������ɐݒ肳���e�N�X�`���Ɠ����ł�
*/
extern IDirect3DTexture9 *mylib_get_rendered_texture();

/*
*@fn mylib_set_fvf
*@brief Direct3D�̃f�o�C�X�ɑ΂��āAFVF�̐ݒ���s���܂�
*@param fvf �t�H�[�}�b�g
*/
extern void mylib_set_fvf(DWORD fvf);

/*
*@fn mylib_set_blend_mode
*@brief Direct3D�̃f�o�C�X�ɑ΂��āA�u�����h���[�h�̎w����s���܂�
*@param mode �u�����h���[�h
*/
extern void mylib_set_blend_mode(mylib_blend_mode_t mode);

/*
*@fn mylib_get_blend_mode
*@brief ���݂̃u�����h���[�h���擾���܂�
*@param mode �i�[����ϐ�
*/
extern void mylib_get_blend_mode(mylib_blend_mode_t *mode);

/*
*@fn mylib_set_z_write
*@brief Direct3D�̃f�o�C�X�ɑ΂��āAZ�o�b�t�@�ւ̏������݂����邩�w�肵�܂�
*@param value ���̒l��true�Ȃ�Z�o�b�t�@�ւ̏������݂����܂�
*/
extern void mylib_set_z_write(bool value);

/*
*@fn mylib_get_z_write
*@brief Z�o�b�t�@�ւ̏������݂��s�������ׂ܂�
*@param value ���ʂ��Ԃ���܂��Btrue�Ȃ�Z�o�b�t�@�ւ̏������݂����܂�
*/
extern void mylib_get_z_write(bool *value);

/*
*@fn mylib_set_z_check
*@brief Direct3D�̃f�o�C�X�ɑ΂��āAZ�o�b�t�@�փe�X�g���s�����w�肵�܂�
*@param value ���̒l��true�Ȃ�Z�o�b�t�@�փe�X�g���s���܂�
*/
extern void mylib_set_z_check(bool value);

/*
*@fn mylib_get_z_check
*@brief Z�o�b�t�@�փe�X�g���s�����擾���܂�
*@param value ���ʂ��Ԃ���܂��Btrue�Ȃ�Z�o�b�t�@�փe�X�g���s���܂�
*/
extern void mylib_get_z_check(bool *value);

/*
*@fn mylib_set_cull_mode
*@brief �J�����O�̐ݒ������
*/
extern void mylib_set_cull_mode(D3DCULL mode);

/*
*@fn mylib_get_cull_mode
*@brief ���݂̃J�����O�̐ݒ���擾����
*/
extern void mylib_get_cull_mode(D3DCULL *mode);

/*
*@fn mylib_set_lighting
*@brief ���C�e�B���O�̐ݒ���w�肷��
*/
extern void mylib_set_lighting(bool value);

/*
*@fn mylib_get_lighting
*@brief ���C�e�B���O�̐ݒ���擾����
*/
extern void mylib_get_lighting(bool *value);

/*
*@fn mylib_shader_open
*@brief �V�F�[�_�[�t�@�C�����J��
*@return 0...�����A-1...���s
*/
extern int mylib_shader_open(const char* filename, ID3DXEffect **shader);

/*
*@fn mylib_shader_error_output
*@brief �V�F�[�_�[�t�@�C�����J�����Ƃ��̃G���[�����o�͂���
*/
extern void mylib_shader_error_message(ID3DXBuffer *compile_errors);

/*
*@fn mylib_load_texture
*@brief �摜��ǂݍ���
*@param filename �p�X���܂߂��t�@�C�����ł�
*@param handle �ǂݍ��񂾉摜�̃n���h�����󂯎��
*@param image_info �ǂݍ��񂾉摜�̏ڍ׏��Anull���w�肷��Ǝ󂯎��Ȃ�
*@param error_code �G���[�����󂯎��Anull���w�肷��Ǝ󂯎��Ȃ�
*@detail ���̊֐��œǂݍ��񂾉摜�̓��C�u�����̒��ŊǗ�����܂�
*        �ǂݍ��ݎ��s���ɂ�handle��-1���i�[����
*        error_code�Ɋi�[�����l�̈ꗗ�ł���
*         0 �ǂݍ��݂ł���
*        -1 �t�@�C����������Ȃ�
*        -2 �ǂݍ��݂Ɏ��s����
*/
extern void mylib_load_texture(const char *filename, int *handle, D3DXIMAGE_INFO *image_info, int *error_code);

/*
*@fn mylib_get_texture
*@brief �ǂݍ��񂾉摜���擾����
*@param handle �摜�̃n���h��
*@param image_info �摜�̏����󂯎��Anull���w�肷��Ǝ󂯎��Ȃ�
*@return �e�N�X�`��
*/
extern IDirect3DTexture9 *mylib_get_texture(int handle, D3DXIMAGE_INFO *image_info);

/*
*@fn mylib_destroy_texture
*@brief �ǂݍ��񂾉摜��S���j��
*/
extern void mylib_destroy_texture();

//�}�E�X�J�[�\�����B���֐��ł�
extern void mylib_hide_mouse_cursor();

//�}�E�X�J�[�\�����w�肵����`�̒��ɕ����߂�֐��ł�
extern void mylib_confine_mouse_cursor(RECT *region);

//�w�肳�ꂽ�t�@�C�������݂���Ȃ�true��Ԃ�
extern bool mylib_file_is_exist(const char *file_name);

//�t���[���P�ʂ���b�P�ʂɕϊ�����
extern float mylib_frame_to_second(time_t frame, float fps);

//���[���h�ϊ����s��
extern void mylib_world_transform(D3DXMATRIX *world, const D3DXVECTOR3 *position, const D3DXVECTOR3 *rotation, const D3DXVECTOR3 *scale);

/*
*@fn mylib_make_billboard_matrix
*@brief �w��̍s��̉�]��ł������s����쐬
*@param out �v�Z���ʂ��󂯎��܂�
*@param in �v�Z�̂��ƂɂȂ�s��ł�
*/
extern void mylib_make_billboard_matrix(D3DXMATRIX *out, const D3DXMATRIX *in);

/*
*@fn mylib_get_vpv
*@brief ���[���h���W����X�N���[�����W�ւ̕ϊ�������s����擾���܂�
*@param vpv ���ʂ��󂯎��ϐ�
*@detail ���̊֐��̓r���[�s��ƃv���W�F�N�V�����s��ƃr���[�|�[�g�s����|���Z�������ʂ�Ԃ��܂�
*/
extern void mylib_get_vpv(D3DXMATRIX *vpv);

/*
*@fn mylib_to_world_pos
*@brief �X�N���[�����W�ɑΉ����郏�[���h���W���擾���܂�
*@param world_pos ���ʂ��󂯎��ϐ�
*@param screen_pos ���[���h���W
*/
extern void mylib_to_world_pos(D3DXVECTOR3 *world_pos, const D3DXVECTOR3 *screen_pos);

/*
*@fn mylib_to_screen_pos
*@brief ���[���h���W�ɑΉ�����X�N���[�����W���擾���܂�
*@param world_pos ���ʂ��󂯎��ϐ�
*@param screen_pos �X�N���[�����W(����(-1, 1),�E��(1, -1))
*/
extern void mylib_to_screen_pos(D3DXVECTOR3 *screen_pos, const D3DXVECTOR3 *world_pos);

/*
*@fn mylib_sprintf_1024
*@brief printf�Ɠ���������1024�����ȉ��̕�������쐬���܂�
*@param format ����
*@param ... ���낢��
*@return �쐬����������
*/
extern const char *mylib_sprintf_1024(const char *format, ...);

/*
*@fn mylib_now_sec
*@brief ���ݕb
*/
extern float mylib_now_sec();

/*
*@fn mylib_wait_sec
*@param �҂���������(�b)
*@brief �w�莞�Ԃ����S�Ă̏������~�߂�
*/
extern void mylib_wait_sec(float sec);

/*
*@fn mylib_draw_string
*@brief ������`�悵�܂�
*@param x �`�悷��ʒu(���[)
*@param y �`�悷��ʒu(��[)
*@param string �`�悷�镶����
*@param color �`�悷��F
*/
extern void mylib_draw_string(int x, int y, const char *string, D3DCOLOR color);

/*
*@fn mylib_draw_ray
*@brief ���C�̕`��
*@param start			���C�̊J�n�n�_
*@param vector			���C�̏I���n�_�܂ł̃x�N�g��
*@param color			�F
*@return ����
*/
extern void mylib_draw_ray(const D3DXVECTOR3 *start_position, const D3DXVECTOR3 *vector, DWORD color);

/*
*@fn mylib_draw_sphere
*@brief ���̕`��
*@param position		���̒��S�ʒu
*@param radius			���̔��a
*@param color			�F
*@return ����
*/
extern void mylib_draw_sphere(const D3DXVECTOR3 *position, float radius, DWORD color);

/*
*@fn mylib_draw_board
*@brief 3D��ԏ�Ƀ|���S����񖇕`�悷��֐��ł�
*@param world ���[���h�s��
*@param uv0,color0 ���㒸�_�̏��ł�
*@param uv1,color1 �E�㒸�_�̏��ł�
*@param uv2,color2 �������_�̏��ł�
*@param uv3,color3 �E�����_�̏��ł�
*@param *texture �|���S���ɓ\��t����e�N�X�`���ł�
*@param blend_mode �u�����h���@���w�肵�܂�(0...�A���t�@�u�����f�B���O, 1...���Z�u�����f�B���O)
*@detail ���̊֐���3������ԏ�Ɏl�p�`�̃|���S����z�u����̂ɓK���Ă��܂�
*/
extern void mylib_draw_board_3d(
	const D3DXMATRIX *world,
	const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture, mylib_blend_mode_t blend_mode
);

/*
*@fn mylib_draw_board
*@brief �X�N���[����Ƀ|���S����񖇕`�悷��֐��ł�
*@param position0,uv0,color0 ���㒸�_�̏��ł�
*@param position1,uv1,color1 �E�㒸�_�̏��ł�
*@param position2,uv2,color2 �������_�̏��ł�
*@param position3,uv3,color3 �E�����_�̏��ł�
*@param *texture �|���S���ɓ\��t����e�N�X�`���ł�
*@param blend_mode �u�����h���@���w�肵�܂�
*@detail ���̊֐��͓n���ꂽ���_�����̂܂܃X�N���[�����W�Ƃ݂Ȃ����̂ŁA2�����̕`��ɓK���Ă��܂�(����)
*/
extern void mylib_draw_board_2d(
	const D3DXVECTOR3 *position0, const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR3 *position1, const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR3 *position2, const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR3 *position3, const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture, mylib_blend_mode_t blend_mode
);

/*
*@fn mylib_draw_board_post
*@brief �Ȃ݂Ȃ݃|�X�g�G�t�F�N�g��������3D��ԏ�Ƀ|���S����񖇕`�悷��֐��ł�
*@param mirage_texture 凋C�O�e�N�X�`��
*@param position0,uv0,color0 ���㒸�_�̏��ł�
*@param position1,uv1,color1 �E�㒸�_�̏��ł�
*@param position2,uv2,color2 �������_�̏��ł�
*@param position3,uv3,color3 �E�����_�̏��ł�
*@param *texture �|���S���ɓ\��t����e�N�X�`���ł�
*/
extern void mylib_draw_board_2d_wave(IDirect3DTexture9 *mirage_texture,
	const D3DXVECTOR3 *position0, const D3DXVECTOR2 *uv0, D3DCOLOR color0,
	const D3DXVECTOR3 *position1, const D3DXVECTOR2 *uv1, D3DCOLOR color1,
	const D3DXVECTOR3 *position2, const D3DXVECTOR2 *uv2, D3DCOLOR color2,
	const D3DXVECTOR3 *position3, const D3DXVECTOR2 *uv3, D3DCOLOR color3,
	IDirect3DTexture9 *texture
);

/*
*@fn mylib_draw_point_sprite
*@brief �|�C���g�X�v���C�g��`�悷��֐��ł�
*@param pos �ʒu
*@param rot ��]�p�x
*@param scale �g��{��
*@param color �F
*@param uv0 ���㒸�_��UV���W
*@param uv1 �E�㒸�_��UV���W
*@param uv2 �������_��UV���W
*@param uv3 �E�����_��UV���W
*@param texture �e�N�X�`��
*@param blend_mode �u�����h���@
*/
extern void mylib_draw_point_sprite(
	const D3DXVECTOR2 *pos,
	float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture,
	mylib_blend_mode_t blend_mode
);

/*
*@fn
*@brief �r���{�[�h��`�悷��
*@param pos �ʒu
*@param rot ��]�p�x
*@param scale �g��{��
*@param color �F
*@param uv0 ���㒸�_��UV���W
*@param uv1 �E�㒸�_��UV���W
*@param uv2 �������_��UV���W
*@param uv3 �E�����_��UV���W
*@param texture �e�N�X�`��
*@param blend_mode �u�����h���@
*/
extern void mylib_draw_billboard(
	const D3DXVECTOR3 *pos,
	const float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture,
	mylib_blend_mode_t blend_mode
);

/*
*@fn
*@brief �r���{�[�h��`�悷��(����2)
*@param pos �ʒu
*@param rot ��]�p�x
*@param scale �g��{��
*@param color �F
*@param uv0 ���㒸�_��UV���W
*@param uv1 �E�㒸�_��UV���W
*@param uv2 �������_��UV���W
*@param uv3 �E�����_��UV���W
*@param texture �e�N�X�`��
*@detail �u�����h���[�h���w��s�\
         ���炫��̌���`�悷�邽�߂ɑ��݂���
*/
extern void mylib_draw_billboard2(
	const D3DXVECTOR3 *pos,
	const float rot,
	const D3DXVECTOR2 *scale,
	D3DCOLOR color,
	const D3DXVECTOR2 *uv0,
	const D3DXVECTOR2 *uv1,
	const D3DXVECTOR2 *uv2,
	const D3DXVECTOR2 *uv3,
	IDirect3DTexture9 *texture
);

/*
*@fn mylib_error_box
*@brief �G���[�����������Ƃ��Ɏg����p�̃��b�Z�[�W�{�b�N�X�ł�
*@param file �t�@�C����
*@param line �s�ԍ�
*@param message �G���[���b�Z�[�W
*@param title �^�C�g��
*@param mb_mode ���[�h
*/
extern void mylib_error_box(const char *file, int line, const char *message, const char *title, UINT mb_mode);

/*
*@fn mylib_register_debug_info
*@brief printf�Ɠ��������Ńf�o�b�O�p��������쐬���A������f�o�b�O�����񃊃X�g�ɓo�^���܂�
*@param format ����
*@param ... ���낢��
*@detail �f�o�b�O�����񃊃X�g�ɓo�^���ꂽ������́Amylib_output_debug_string()���Ăƕ`��ł��܂�
*/
extern void mylib_register_debug_string(const char *format, ...);

/*
*@fnmylib_output_debug_string
*@brief �f�o�b�O�����񃊃X�g�ɓo�^���ꂽ����������ׂĕ`�悵�܂�
*/
extern void mylib_output_debug_string();
/*----------------------------------------------------------------------------------------------------------------------*/
