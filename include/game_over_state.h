#pragma once
#ifndef GAME_OVER_STATE_H
#define GAME_OVER_STATE_H

#include <allegro5/allegro.h>

void enter_game_over_state();

void handle_game_over_input(ALLEGRO_EVENT event);

void draw_game_over();

#endif