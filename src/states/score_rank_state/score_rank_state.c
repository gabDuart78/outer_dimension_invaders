#include "score_rank_state.h"
#include "state_manager.h"
#include "background_manager.h" 
#include "utils.h"
#include "font_manager.h"
#include "screen_config.h"
#include "game_context.h"
#include "sound_manager.h"
#include <stdbool.h>

#define TOP_MARGIN 50
#define SCORE_RANK_STATE_BG_PATH "../assets/images/bg/score_rank_bg.png"
#define ENCOURAGING_PHRASE "No one survived. Ready to try?"

/**
 * @brief Libera os recursos usados pelo score rank state.
 */
void clean_up_score_rank_state() {
    remove_music(CALM_MUSIC);
    rewind_music(CALM_MUSIC);
}

/**
 * @brief Libera os recursos usados pelo score rank state e define o próximo estado.
 * 
 * @param new_state Próximo GameState.
 * @param enter_state Um bool indicando se deve ser executada a função enter do próximo state.
 */
void exit_score_rank_state(GameState next_state, bool enter_state) {
    clean_up_score_rank_state();
    set_game_state(next_state, enter_state);
}

/**
 * @brief função usada para carregar os artefatos necessários ao score rank state.
 */
void enter_score_rank_state() {
    load_background(get_background_manager(), SCORE_RANK_STATE_BG_PATH);
    play_music(CALM_MUSIC);
}

/**
 * @brief Faz o update da lógica do score rank state.
 */
void update_score_rank() {
    return;
}

/**
 * @brief Lida com os inputs do score rank state.
 * 
 * @param event Um ALLEGRO_EVENT a ser verificado.
 */
void handle_score_rank_input(ALLEGRO_EVENT event) {
    if (close_display(event)) {
        exit_score_rank_state(STATE_EXIT, false);
    }

    if (key_pressed(event, ALLEGRO_KEY_ESCAPE)) {
        exit_score_rank_state(STATE_MENU, true);
    }
}

/**
 * @brief Desenha as opções save rank state.
 * 
 * @param text_color ALLEGRO_COLOR representado a cor a ser usada no texto.
 * @param y Coordenada vertical de onde começar a desenhar as opções.
 */
void draw_score_rank_options(ALLEGRO_COLOR text_color, float y) {
    al_draw_text(get_small_font(), text_color, SCREEN_WIDTH / 2, y, 
        ALLEGRO_ALIGN_CENTER, "Press ESC to return");
}

/**
 * @brief Desenha o score rank state, table contendo os maiores scores.
 */
void draw_score_rank() {
    ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255);
    float current_y = TOP_MARGIN;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_background(get_background_manager());

    al_draw_text(get_large_font(), text_color, SCREEN_WIDTH / 2, current_y, 
        ALLEGRO_ALIGN_CENTER, "-- High Score Rank --");

    
    if (is_score_table_empty(get_score_table())) {
        current_y += 100;
        al_draw_text(get_small_font(), text_color, SCREEN_WIDTH / 2, current_y, ALLEGRO_ALIGN_CENTER, 
            ENCOURAGING_PHRASE);

        draw_score_rank_options(text_color, SCREEN_HEIGHT / 2);
    } else {
        current_y += 50;
        draw_score_table(&current_y, get_medium_font());
        current_y += 50;
        draw_score_rank_options(text_color, current_y);
    }
    
    al_flip_display();
}
