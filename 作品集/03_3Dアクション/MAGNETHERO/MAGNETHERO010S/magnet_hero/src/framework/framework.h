#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")


#include <d3d9.h>
#include <d3dx9.h>

/*
*@fn frame_init
*/
extern int frame_init(HINSTANCE instance);

/*
*@fn frame_end
*/
extern void frame_end();

/*
*@fn frame_continue
*/
extern bool frame_continue();

/*
*@fn frame_update
*/
extern void frame_update();

/*
*@fn frame_draw
*/
extern void frame_draw();
