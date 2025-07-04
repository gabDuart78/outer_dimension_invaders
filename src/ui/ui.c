#include "ui.h"
#include "screen_config.h"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "font_manager.h"
#include "game_context.h"
#include "score_manager.h"
#include <allegro5/allegro_color.h>

#define LIFE_ICON_ACTIVE_PATH "../assets/images/icons/player_life.png"
#define LIFE_ICON_DEACTIVE_PATH "../assets/images/icons/player_life_low_opacity.png"
#define ICONS_GAP 10
#define ICONS_WIDTH 24
#define ICONS_HEIGHT 24

#define UI_POS_X 10
#define UI_POS_Y 10
#define UI_WIDTH SCREEN_WIDTH - UI_POS_X * 2
#define UI_HEIGHT 20

/// Retângulo usado para representar a posição da UI na tela e suas dimensões.
const Rect UI_wrapper = {
    {UI_POS_X, UI_POS_Y},
    UI_WIDTH,
    UI_HEIGHT,
};

static UI ui;

/**
 * @brief Inicializa a UI, cor do texto, fonte utilizada, icons, etc.
 */
void init_ui() {
    ui.text_color = al_map_rgb(255, 255, 255);
    ui.font = get_small_font();
    ui.wrapper = UI_wrapper;
    ui.life_icon_active = get_sprite(LIFE_ICON_ACTIVE_PATH);
    ui.life_icon_deactive = get_sprite(LIFE_ICON_DEACTIVE_PATH);
}

/**
 * @brief Desenha icons na tela que representam a vida atual do player.
 * 
 * @param max_lifes Vida máxima do player.
 * @param lifes Quantidade atual de vidas do player.
 */
void draw_player_lifes(int max_lifes, int lifes) {
    for (int i = 0; i < max_lifes; i++) {
        al_draw_bitmap(i < lifes ? ui.life_icon_active : ui.life_icon_deactive, 
            ui.wrapper.pos.x + ((ICONS_WIDTH + ICONS_GAP) * i), ui.wrapper.pos.y, 0);
    }
}

/**
 * @brief Desenha o maior score registrado e o score atual do player na tela.
 * 
 * @param max_score Maior score registrado.
 * @param player_score Score atual do player.
 */
void draw_score(int max_score, int player_score) {
    char buffer1[64];
    sprintf(buffer1, "Stage: %d  <Max: %.7d>  Score: %.7d",
        get_stage_manager()->current_stage,  max_score, player_score);

    al_draw_text(ui.font, ui.text_color, ui.wrapper.pos.x + ui.wrapper.width, 
        ui.wrapper.pos.y, ALLEGRO_ALIGN_RIGHT, buffer1);
}

/**
 * @brief Desenha a UI na tela contendo informações sobre o player e sobre o estágio atual.
 * 
 * @param max_lifes Vida máxima do player.
 * @param lifes Vida atual do player.
 * @param player_score Score atual do player. 
 */
void draw_ui(int max_lifes, int lifes, int player_score) {
    draw_score(get_highest_score(get_score_table()), player_score);
    draw_player_lifes(max_lifes, lifes);
}

/**
 * @brief Libera os recursos utilizados pela UI.
 */
void destroy_ui() {
    if (ui.life_icon_active)
        al_destroy_bitmap(ui.life_icon_active);

    if (ui.life_icon_deactive)
        al_destroy_bitmap(ui.life_icon_deactive);
}
