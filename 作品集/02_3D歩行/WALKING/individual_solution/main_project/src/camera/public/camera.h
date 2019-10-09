#pragma once

#include "..\..\angle\public\angle.h"
#include "..\..\common_def\public\common_def.h"



extern void camera_manager_init();

extern void camera_manager_update();

extern void camera_manager_end();

extern void camera_get_eye_pos(vector3_t *position);

extern void camera_get_look_pos(vector3_t *position);

extern void camera_get_up_dir(vector3_t *direction);

extern void camera_get_theta(angle_t *angle);

extern void camera_get_phi(angle_t *angle);


