#pragma once
//	
//	“~Šú§ìuMagnet Herov
//	
//	ƒJƒƒ‰
//	
//	æ‚ÉDirect3D‚Ì‰Šú‰»‚ğ‚µ‚Ä‚¨‚­•K—v‚ª‚ ‚è‚Ü‚·
//	
//	ì¬Ò	X	mu
//	


#include <d3dx9.h>

#include "camera_definition.h"

#include "..\fbx_object\object_manager.h"




extern void camera_init(game_main_camera_t *camera, const camera_database_t *database);

extern void camera_end(game_main_camera_t *camera);

extern void camera_update(game_main_fbx_object_array_t *object_array, game_main_camera_t *camera);

extern void camera_set_view_and_projection(game_main_camera_t *camera, float near_clip, float far_clip);
