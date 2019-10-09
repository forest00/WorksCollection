#pragma once

#include <stdarg.h>
#include <Windows.h>


//printfの書式に従いメッセージを生成したあと、普通にメッセージボックスを出します
//音も指定できます
//内部ではMessageBoxを呼び出し、その返し値をそのまま返します
int message_box(HWND window_handle, long box_type, long beep_type, const char *format, ...);


