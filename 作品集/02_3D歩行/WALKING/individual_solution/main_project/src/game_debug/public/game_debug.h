
#pragma once


extern void __game_debug_infomation__();

#ifdef _DEBUG
#define game_debug_infomation() __game_debug_infomation__()
#else
#define game_debug_infomation()
#endif


