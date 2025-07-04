#include "menu.h"
#include "state_manager.h"
#include "screen_config.h"
#include "utils.h"
#include "sound_manager.h"
#include "font_manager.h"
#include "game_context.h"
#include "background_manager.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

#define MENU_BACKGROUND_PATH "../assets/images/bg/menu_bg.png"
#define OPTIONS_X SCREEN_WIDTH / 2
#define OPTIONS_GAP 50
#define OPTS_COUNT 3
#define TITLE_SHADOW_OFFSET_X 5
#define TITLE_SHADOW_OFFSET_Y 5
#define GAME_TITLE "Outer Dimension Invaders"

const char OPTIONS[OPTS_COUNT][100] = {"Press enter to Start", "Press H to score rank",
    "Press esc to exit"};

/**
 * @brief função usada para carregar os artefatos necessários ao menu state.
 */
void enter_menu_state() {
    play_music(TITLE_SCREEN);
    load_background(get_background_manager(), MENU_BACKGROUND_PATH);
}

/**
 * @brief Libera os recursos usados pelo menu state.
 */
void clean_up_menu_state() {
    remove_music(TITLE_SCREEN);
    rewind_music(TITLE_SCREEN);
}

/**
 * @brief Libera os recursos usados pelo menu state e define o próximo estado.
 * 
 * @param new_state Próximo GameState.
 * @param enter_state Um bool indicando se deve ser executada a função enter do próximo state.
 */
void exit_menu_state(GameState state, bool enter_state) {
    clean_up_menu_state();
    set_game_state(state, enter_state);
}

/**
 * @brief Libera os recursos usados pelo menu state e define o próximo estado, sai com transição.
 */
void exit_menu_state_with_transition(GameState state) {
    clean_up_menu_state();
    enter_state_with_transition(state, .2, true);
}

/**
 * @brief Lida com os inputs do menu state.
 * 
 * @param event Um ALLEGRO_EVENT a ser verificado.
 */
void handle_menu_inupt(ALLEGRO_EVENT event) {
    if (key_pressed(event, ALLEGRO_KEY_ENTER)) {
        exit_menu_state_with_transition(STATE_PLAYING);
    }

    if (key_pressed(event, ALLEGRO_KEY_ESCAPE) || close_display(event)) {
        exit_menu_state(STATE_EXIT, false);
    }

    if (key_pressed(event, ALLEGRO_KEY_H)) {
        exit_menu_state(STATE_SCORE_RANK, true);
    }
}

/**
 * @brief Faz a atualização da lógica do menu state.
 */
void update_menu() {
    return;
}

/**
 * @brief Desenha as opções do menu state.
 * 
 * @param current_y Um ponteiro indicando onde começar as opções.
 * @param text_color ALLEGRO_COLOR indicando qual cor usar no texto.
 * @param gap Espaço entre as opções do menu.
 */
void draw_menu_options(int *current_y, ALLEGRO_COLOR text_color, int gap) {
    for (int i = 0; i < OPTS_COUNT; i++) {
        al_draw_text(get_small_font(), text_color, OPTIONS_X, 
        *current_y, ALLEGRO_ALIGN_CENTER, OPTIONS[i]);
        *current_y += gap;
    }
}

/**
 * @brief Desenha o título usado no menu.
 * 
 * @param text_color ALLEGRO_COLOR indicando qual cor usar no texto.
 * @param current_y Um ponteiro indicando onde começar o título.
 */
void draw_menu_title(ALLEGRO_COLOR text_color, int *current_y) {
    int max_line_width = 550;

    draw_wrapped_text(get_large_font(), al_map_rgb(40, 25, 45), SCREEN_WIDTH / 2 + TITLE_SHADOW_OFFSET_X, 
        *current_y + TITLE_SHADOW_OFFSET_Y, max_line_width, GAME_TITLE, 0);

    draw_wrapped_text(get_large_font(), text_color, SCREEN_WIDTH / 2, *current_y, 
        max_line_width, GAME_TITLE, 0);
}

/**
 * @brief Desenha o menu, titulo e opções.
 */
void draw_menu() {
    ALLEGRO_COLOR text_color = al_map_rgb(255, 255, 255);
    int current_y = 100;

    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_background(get_background_manager());

    draw_menu_title(text_color, &current_y);

    current_y += 170;

    draw_menu_options(&current_y, text_color, OPTIONS_GAP);
    
    al_flip_display();
}