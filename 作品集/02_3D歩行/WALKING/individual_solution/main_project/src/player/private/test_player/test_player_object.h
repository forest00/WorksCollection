#pragma once

#include "test_player_database.h"
#include "..\..\public\test_player\test_player_initializer.h"
#include "..\..\public\test_player\test_player_object_declaration.h"
#include "..\..\..\angle\public\angle.h"
#include "..\..\..\my_3d_lib\public\my_3d_lib.h"
#include "..\..\..\x_file\public\x_file_type.h"


extern test_player_object_t *test_player_object_t_init(const test_player_initializer_t *initializer, const test_player_database_t *database);

extern void test_player_object_t_update(test_player_object_t *object);

extern void test_player_object_t_draw(const test_player_object_t *object, IDirect3DDevice9 *device);

extern void test_player_object_t_end(test_player_object_t **object);

extern const vector3_t *test_player_object_t_get_position(const test_player_object_t *object);
