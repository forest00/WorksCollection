//親ヘッダー
#include "avi.h"
//その他のヘッダー
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

//動画再生について参考にしたサイト
//[○×（まるぺけ）つくろーどっとコム], URL[http://marupeke296.com/DSW_No5_FullScreenProgram.html]
//[つまみぐいプログラミング], URL[http://codeit.blog.fc2.com/blog-entry-13.html]

#define _WIN32_DCOM     // CoInitializeEx関数の呼び出しに必要


//この関数がtrueを返したら動画再生を中断して終了する
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

	//初期化
	{
		static bool already_init = false;
		if (already_init == false)
		{
			CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		}
	}
	//フィルタグラフマネージャーの取得
	IGraphBuilder *graph = nullptr;
	if (FAILED(CoCreateInstance(
		CLSID_FilterGraph,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void**)&graph
	)))
	{
		MessageBox(nullptr, "ムービー読み込み不能", "エラー", MB_OK);
		return;
	}
	//VMR9フィルタの取得
	IBaseFilter *vmr9_filter;
	if (FAILED(CoCreateInstance(
		CLSID_VideoMixingRenderer9,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void**)&vmr9_filter
	)))
	{
		MessageBox(nullptr, "ムービー読み込み不能", "エラー", MB_OK);
		return;
	}
	graph->AddFilter(vmr9_filter, L"_bf");
	///フルスクリーン指定
	IVMRFilterConfig *vmr_config = nullptr;
	vmr9_filter->QueryInterface(IID_IVMRFilterConfig9, (void**)&vmr_config);
	//VMR9フィルタをウィンドウレスモードに切り替え
	vmr_config->SetRenderingMode(VMRMode_Windowless);
	vmr_config->Release();
	//描画先ウィンドウ指定
	IVMRWindowlessControl *vmr_windowless_controll;
	vmr9_filter->QueryInterface(IID_IVMRWindowlessControl9, (void**)&vmr_windowless_controll);
	vmr_windowless_controll->SetVideoClippingWindow(mylib_get_window());
	IBaseFilter *pSource = nullptr;
	graph->AddSourceFilter(filename, filename, &pSource);
	//ICaptureGraphBuilder2インターフェイスの取得
	ICaptureGraphBuilder2 *capture_graph_2;
	CoCreateInstance(CLSID_CaptureGraphBuilder2, nullptr, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&capture_graph_2);
	capture_graph_2->SetFiltergraph(graph);
	//フィルタの接続
	capture_graph_2->RenderStream(nullptr, nullptr, pSource, nullptr, vmr9_filter);
	capture_graph_2->RenderStream(nullptr, &MEDIATYPE_Audio, pSource, nullptr, nullptr);
	// 描画領域の設定（接続後でないとエラーになる）
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
	//動画再生
	media_controll->Run();
	// メッセージ ループ
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
	// 動画停止
	media_controll->Stop();
	//もろもろ破棄
	vmr_windowless_controll->Release();
	media_controll->Release();
	vmr9_filter->Release();
	capture_graph_2->Release();
	graph->Release();
	CoUninitialize();
}