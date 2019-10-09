//�e�w�b�_�[
#include "avi.h"
//���̑��̃w�b�_�[
#include <windows.h>
#include <objbase.h>
#include <tchar.h>
#include <d3d9.h>
#include <dshow.h>
#include <Vmr9.h>
#include "..\mylib\mylib.h"
#include "..\input\input.h"
#include "..\char_ptr\char_ptr.h"
#pragma comment(lib, "Strmiids.lib")

//����Đ��ɂ��ĎQ�l�ɂ����T�C�g
//[���~�i�܂�؂��j����[�ǂ��ƃR��], URL[http://marupeke296.com/DSW_No5_FullScreenProgram.html]
//[�܂݂����v���O���~���O], URL[http://codeit.blog.fc2.com/blog-entry-13.html]

#define _WIN32_DCOM     // CoInitializeEx�֐��̌Ăяo���ɕK�v


//���̊֐���true��Ԃ����瓮��Đ��𒆒f���ďI������
static bool skip()
{
	for (int i = 0; i < 0x100;i++)
	{
		if (input_keyboard_repeat(i))
		{
			return true;
		}
	}
	for (int i = 0; i < (int)(imc_max);i++)
	{
		if (input_mouse_repeat((input_mouse_code_t)(i)))
		{
			return true;
		}
	}
	for (int i = 0; i < (int)(icc_max);i++)
	{
		if (input_controller_repeat((input_controller_code_t)(i)))
		{
			return true;
		}
	}
	return false;
}

extern void play_avi(const wchar_t *filename)
{

	//������
	{
		static bool already_init = false;
		if (already_init == false)
		{
			CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		}
	}
	//�t�B���^�O���t�}�l�[�W���[�̎擾
	IGraphBuilder *graph = nullptr;
	if (FAILED(CoCreateInstance(
		CLSID_FilterGraph,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void**)&graph
	)))
	{
		MessageBox(nullptr, "���[�r�[�ǂݍ��ݕs�\", "�G���[", MB_OK);
		return;
	}
	//VMR9�t�B���^�̎擾
	IBaseFilter *vmr9_filter;
	if (FAILED(CoCreateInstance(
		CLSID_VideoMixingRenderer9,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void**)&vmr9_filter
	)))
	{
		MessageBox(nullptr, "���[�r�[�ǂݍ��ݕs�\", "�G���[", MB_OK);
		return;
	}
	graph->AddFilter(vmr9_filter, L"_bf");
	///�t���X�N���[���w��
	IVMRFilterConfig *vmr_config = nullptr;
	vmr9_filter->QueryInterface(IID_IVMRFilterConfig9, (void**)&vmr_config);
	//VMR9�t�B���^���E�B���h�E���X���[�h�ɐ؂�ւ�
	vmr_config->SetRenderingMode(VMRMode_Windowless);
	vmr_config->Release();
	//�`���E�B���h�E�w��
	IVMRWindowlessControl *vmr_windowless_controll;
	vmr9_filter->QueryInterface(IID_IVMRWindowlessControl9, (void**)&vmr_windowless_controll);
	vmr_windowless_controll->SetVideoClippingWindow(mylib_get_window());
	IBaseFilter *pSource = nullptr;
	graph->AddSourceFilter(filename, filename, &pSource);
	//ICaptureGraphBuilder2�C���^�[�t�F�C�X�̎擾
	ICaptureGraphBuilder2 *capture_graph_2;
	CoCreateInstance(CLSID_CaptureGraphBuilder2, nullptr, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&capture_graph_2);
	capture_graph_2->SetFiltergraph(graph);
	//�t�B���^�̐ڑ�
	capture_graph_2->RenderStream(nullptr, nullptr, pSource, nullptr, vmr9_filter);
	capture_graph_2->RenderStream(nullptr, &MEDIATYPE_Audio, pSource, nullptr, nullptr);
	// �`��̈�̐ݒ�i�ڑ���łȂ��ƃG���[�ɂȂ�j
	RECT video_rect;
	LONG video_rect_w;
	LONG video_rect_h;
	vmr_windowless_controll->GetNativeVideoSize(&video_rect_w, &video_rect_h, nullptr, nullptr);
	SetRect(&video_rect, 0, 0, video_rect_w, video_rect_h);
	RECT window_rect;
	GetClientRect(mylib_get_window(), &window_rect);
	vmr_windowless_controll->SetVideoPosition(&video_rect, &window_rect);
	IMediaControl *media_controll;
	graph->QueryInterface(IID_IMediaControl, (void**)&media_controll);
	//Get media controller interfaces
	IMediaSeeking *seek;
	graph->QueryInterface(IID_IMediaSeeking, (void**)&seek);
	//Get media length
	LONGLONG media_length;
	seek->GetDuration(&media_length);
	//����Đ�
	media_controll->Run();
	// ���b�Z�[�W ���[�v
	{
		while (mylib_process_message())
		{
			//Input
			mylib_input_update();
			//Get current position
			LONGLONG current;
			seek->GetCurrentPosition(&current);
			//Break when finished
			if (current >= media_length)
			{
				break;
			}
			//Break when the key is pressed
			if (skip())
			{
				break;
			}
		}
	}
	// �����~
	media_controll->Stop();
	//�������j��
	vmr_windowless_controll->Release();
	media_controll->Release();
	vmr9_filter->Release();
	capture_graph_2->Release();
	graph->Release();
	CoUninitialize();
}