#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdbool.h>

/**
 * @brief Enumeração que representa os possíveis estados do programa. 
 */
typedef enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER,
    STATE_EXIT,
    STATE_TRANSITION,
    STATE_SAVE_SCORE,
    STATE_SCORE_RANK,
} GameState;

void set_game_state(GameState new_state, bool enter_state);

void enter_state_with_transition(GameState next_state, double time, bool enter_state);

GameState get_game_state();

#endif