#pragma once
#ifndef PLAYING_SCENE_H
#define PLAYING_SCENE_H

#include <allegro5/allegro.h>
#include "utils.h"

void enter_playing_state();

void update_game();

void handle_game_input(ALLEGRO_EVENT event);

void on_kill_enemy(Rect collider);

void draw_game();

#endif  