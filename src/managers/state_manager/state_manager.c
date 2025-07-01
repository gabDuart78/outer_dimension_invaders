#include "state_manager.h"
#include "playing_scene.h"
#include "menu.h"
#include "transition_state.h"
#include "game_over_state.h"
#include "score_rank_state.h"
#include <stdio.h>
#include <allegro5/allegro.h>

static GameState current_state;

const char states[][100] = {"STATE_MENU", "STATE_PLAYING","STATE_GAME_OVER", 
    "STATE_EXIT", "STATE_TRANSITION", "STATE_SAVE_SCORE", "STATE_SCORE_RANK"};

/**
 * @brief Define o estado atual do game.
 * 
 * @param new_state Novo estado a ser definido
 * @param enter_state Boolean define se deve ou não ser executada a função enter do estado. 
 */
void set_game_state(GameState new_state, bool enter_state) {
    current_state = new_state;

    if (!enter_state) return;

    switch(new_state) {
        case STATE_MENU:
            enter_menu_state();
            break;
        case STATE_PLAYING:
            enter_playing_state();
            break;
        case STATE_GAME_OVER:
            enter_game_over_state();
            break;
        case STATE_SCORE_RANK:
            enter_score_rank_state();
            break;
        default:
            break; 
    }
}  

/**
 * @brief Entra em uma estado primeiro passando por um estado de transição.
 * 
 * @param next_state Próximo estado a ser definido.
 * @param time Tempo em segudos até que seja definido o novo estado.
 * @param enter_state Boolean define se deve ou não ser executada a função enter do estado.  
 */
void enter_state_with_transition(GameState next_state, double time, bool enter_state) {
    set_game_state(STATE_TRANSITION, false);
    enter_transition_state(next_state, time, enter_state);
}

/**
 * @brief Retorna o estado atual do game. 
 * 
 * @return GameState Esatado atual.
 */
GameState get_game_state() {
    return current_state;
}
