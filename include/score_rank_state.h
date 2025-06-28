#pragma once
#ifndef SCORE_RANK_STATE_H
#define SCORE_RANK_STATE_H

#include <allegro5/allegro.h>

void enter_score_rank_state();

void update_score_rank();

void handle_score_rank_input(ALLEGRO_EVENT ev);

void draw_score_rank();

#endif