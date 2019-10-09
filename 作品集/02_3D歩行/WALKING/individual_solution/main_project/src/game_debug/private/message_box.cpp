//親ヘッダー
#include "..\public\message_box.h"
//その他のヘッダー
#include <d3dx9.h>
#include <strsafe.h>

//printfの書式に従いメッセージを生成したあと、普通にメッセージボックスを出します
//音も指定できます
//内部ではMessageBoxを呼び出し、その返し値をそのまま返します
int message_box(HWND window_handle, long box_type, long beep_type, const char *format, ...)
{
	static char message[0x800];
	message[0] = '\0';
	va_list list;
	va_start(list, format);
	vsnprintf(message, 0x800, format, list);
	va_end(list);
	MessageBeep(beep_type);
	MessageBox(window_handle, message, nullptr, box_type);
}

