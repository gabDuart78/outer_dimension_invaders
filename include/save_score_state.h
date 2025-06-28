#pragma once
#ifndef SAVE_SCORE_STATE_H
#define SAVE_SCORE_STATE_H

#include <allegro5/allegro.h>
#include "state_manager.h"

void enter_save_score_state(GameState state);

void handle_input_save_score(ALLEGRO_EVENT event);

void draw_save_score_state();

#endif