#include "save_score_state.h"
#include "font_manager.h"
#include "screen_config.h"
#include "utils.h"
#include "score_manager.h"
#include "game_context.h"
#include "background_manager.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string.h>
#include <stdio.h>

#define SAVE_SCORE_BACKGROUND_PATH "../assets/images/bg/save_score_bg.png"

static char name[NAME_LENGTH + 1] = "";
static GameState _next_state = STATE_MENU;

/**
 * @brief função usada para carregar os artefatos necessários ao save score state, e recebe o próximo state para ser acessado
 * quando terminar de coletar o score do player.
 * 
 * @param next_state próximo estado a ser acessado.
 */
void enter_save_score_state(GameState next_state) {
    _next_state = next_state;
    load_background(get_background_manager(), SAVE_SCORE_BACKGROUND_PATH);
}

/**
 * @brief Reseta a string name.
 */
void clean_up_score_state() {
    name[0] = '\0';
}

/**
 * @brief converte uma entrada alfabética de um input event.keyboard.keycode em um char maiúsculo.
 * 
 * @param event Evento allegro a ser convertido.
 * 
 * return Um caracter que representa a tecla alfabética que foi apertada.
 */
char convert_alphabetic_input_to_char(ALLEGRO_EVENT event) {
    return 64 + event.keyboard.keycode;
}

/**
 * @brief Verifica se um keyboardcode pode ser convertido para um caracter alfabético.
 * 
 * @param event Evento allegro a ser testado.
 * 
 * @return Bool identificando se aquele input pode ou não ser convertido.
 * */
bool is_alphabetic_input(ALLEGRO_EVENT event) {
    return event.keyboard.keycode >= 1 && event.keyboard.keycode <= 26;
}

/**
 * @brief Lida com os inputs do save score state.
 * 
 * @param event Um ALLEGRO_EVENT a ser verificado.
 */
void handle_input_save_score(ALLEGRO_EVENT event) {
    if (close_display(event)) {
        set_game_state(STATE_EXIT, false);
        clean_up_score_state();
    }

    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (is_alphabetic_input(event) && strlen(name) < NAME_LENGTH) {
            char buffer[2];
            sprintf(buffer, "%c", convert_alphabetic_input_to_char(event));
            strcat(name, buffer);
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(name) > 0) {
            name[strlen(name) - 1] = '\0';
        }

        if (event.keyboard.keycode == ALLEGRO_KEY_ENTER && is_valid_name(name)) {
            save_score(get_score_table(), name, get_player_score());
            reset_game_context();
            clean_up_score_state();
            set_game_state(_next_state, true);
        }
    }
}

/**
 * @brief Retorna o nome escolhido pelo player formatado, caracteres ainda não preenchidos são
 * representado por '-'.
 * 
 * @param dest String que irá receber o nome formatado.
 * */
void get_formatted_name(char *dest) {
    for (int i = 0; i < NAME_LENGTH; i++) {
        if (i >= strlen(name)) {
            dest[i] = '-';
            continue;
        }
        dest[i] = name[i];
    }
    
    dest[NAME_LENGTH] = '\0';
}

/**
 * @brief Desenha as opções save score state.
 * 
 * @param current_y Um ponteiro indicando onde começar as opções.
 */
void draw_save_score_options(float *current_y) {
    *current_y += 50;

    if (strlen(name) == NAME_LENGTH) {
       al_draw_text(get_small_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5, *current_y, ALLEGRO_ALIGN_CENTER, 
        "-> Enter to continue"); 
    }
}

/**
 * @brief Desenha a tela de save score, a table contendo os maiores scores registrados 
 * as entradas do player para definir seu nome e o seu score.
 */
void draw_save_score_state() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_background(get_background_manager());

    float current_y = SCREEN_HEIGHT * 0.2;

    al_draw_text(get_medium_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5, current_y, ALLEGRO_ALIGN_CENTER, 
        "Save your score.");

    current_y += 50;

    al_draw_text(get_small_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5, current_y, ALLEGRO_ALIGN_CENTER, 
        "Enter Three letters:");

    draw_score_table(&current_y, get_small_font());

     current_y += 20;

    char buffer[24];
    char formatted_name[NAME_LENGTH + 1];
    get_formatted_name(formatted_name);

    sprintf(buffer, "%s : %.7d", formatted_name, get_player_score());

    al_draw_text(get_small_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5, current_y, ALLEGRO_ALIGN_CENTER, 
        buffer);

    draw_save_score_options(&current_y);

    al_flip_display();
}