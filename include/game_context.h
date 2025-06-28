#pragma once
#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include <stdbool.h>
#include "score_manager.h"
#include "background_manager.h"
#include "stage_manager.h"

void init_game_context();

void set_player_score(int score);

void set_game_over(bool is_game_over);

void set_player_win(bool has_player_win);

int get_player_score();

BackgroundManager* get_background_manager();
  
bool is_game_over();

bool has_player_win();

void reset_game_context();

HighScoreTable* get_score_table();

StageManager* get_stage_manager();

void destroy_game_context();

#endif