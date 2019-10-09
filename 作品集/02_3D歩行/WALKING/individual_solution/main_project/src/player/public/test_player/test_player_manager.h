#pragma once

#include "test_player_object_declaration.h"
#include "..\..\public\test_player\test_player_initializer.h"


struct test_player_manager_t;

extern test_player_manager_t *test_player_manager_t_init();

extern void test_player_manager_t_update(test_player_manager_t *manager);

extern void test_player_manager_t_draw(const test_player_manager_t *manager, IDirect3DDevice9 *device);

extern void test_player_manager_t_end(test_player_manager_t **manager);

extern size_t test_player_manager_t_get_num_of_active(const test_player_manager_t *manager);

extern size_t test_player_manager_t_get_capacity(const test_player_manager_t *manager);

extern test_player_object_t **test_player_manager_t_add(test_player_manager_t *manager, const test_player_initializer_t *initializer);

extern void test_player_manager_t_delete(test_player_manager_t *manager, test_player_object_t **object);

extern test_player_object_t **test_player_manager_t_search_active(test_player_manager_t *manager);

extern const vector3_t *test_player_manager_t_get_position(const test_player_manager_t *manager, const test_player_object_t *object);
