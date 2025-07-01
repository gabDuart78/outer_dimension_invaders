#include "transition_state.h"
#include <allegro5/allegro.h>
#include <stdbool.h>
#include "font_manager.h"
#include "screen_config.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define BOTTOM_PADDING 30

static GameState next_state;
static double state_timer;
static double transition_time;
static double enter_state;

/**
 * @brief Faz as configurações para a transição para o próximo state.
 * 
 * @param state Próximo GameState.
 * @param time Tempo em segundo da transição.
 * @param enter Um bool indicando se a função enter do próximo estado deve ser executada. 
 */
void enter_transition_state(GameState state, double time, bool enter) {
    next_state = state;
    state_timer = al_get_time();
    transition_time = time;
    enter_state = enter;
}

/**
 * @brief Sai do estado de transição.
 */
void exit_transition() {
    set_game_state(next_state, enter_state);
}

/**
 * @brief Desenha uma mensagem de carregamento.
 */
void draw_transition() {
    ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255);
    char *text = "Loading...";

    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_draw_text(get_small_font(), text_color, 
        SCREEN_HORIZONTAL_MARGIN, SCREEN_HEIGHT - BOTTOM_PADDING, 
        ALLEGRO_ALIGN_LEFT, text);

    al_flip_display();
}

/**
 * @brief Fax a atualização da lógica do transition state, verifica se 
 * já é o momento para sair do estado.
 */
void update_transition_state() {
    double now = al_get_time();
    double delta_time = now - state_timer;

    if (delta_time >= transition_time) 
        exit_transition();   
}
