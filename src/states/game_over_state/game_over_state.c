#include "game_over_state.h"
#include "state_manager.h"
#include "font_manager.h"
#include "screen_config.h"
#include "utils.h"
#include "game_context.h"
#include "save_score_state.h"
#include "score_manager.h"
#include "sound_manager.h"
#include "background_manager.h"
#include "state_manager.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <string.h>

#define GAME_OVER_BG_PATH "../assets/images/bg/game_over_bg.png"
#define PLAYER_WIN_MESSAGE "The alien ivasion has been stopped. Good work!"
#define PLAYER_LOSE_MESSAGE "Earth is now under the control of the aliens"

static SoundID sfx_id;

void enter_game_over_state() {
    load_background(get_background_manager(), GAME_OVER_BG_PATH);
    sfx_id = has_player_win() ? SFX_GAME_WIN : SFX_GAME_OVER;
    play_sound(sfx_id);
}

void exit_game_over_state() {
    remove_sound(sfx_id);
    rewind_sound(sfx_id);
}

void handle_game_over_input(ALLEGRO_EVENT event) {
    if (close_display(event)) {
        exit_game_over_state();
        set_game_state(STATE_EXIT, false);
    }

    if (key_pressed(event, ALLEGRO_KEY_ESCAPE)) {
        if (is_eligible_high_score(get_score_table(), get_player_score())) {
            set_game_state(STATE_SAVE_SCORE, false);
            exit_game_over_state();
            enter_save_score_state(STATE_MENU);
            return;
        }

        exit_game_over_state();
        set_game_state(STATE_MENU, true);
        reset_game_context();
    }

    if (key_pressed(event, ALLEGRO_KEY_ENTER)) {
        if (!has_player_win()) {
            if (is_eligible_high_score(get_score_table(), 
                get_player_score())) {
                exit_game_over_state();
                set_game_state(STATE_SAVE_SCORE, false);
                enter_save_score_state(STATE_PLAYING);
                return;
            }

            exit_game_over_state();
            reset_game_context();
            enter_state_with_transition(STATE_PLAYING, 0.3, true);
            return;
        } 

        exit_game_over_state();
        next_stage(get_stage_manager());
        enter_state_with_transition(STATE_PLAYING, 0.3, true);
    }
}

void draw_options(float *current_y) {
    al_draw_text(get_small_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5f, *current_y, ALLEGRO_ALIGN_CENTER, 
        "Press Enter to play again");

    *current_y += 50;

     al_draw_text(get_small_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5f, *current_y, ALLEGRO_ALIGN_CENTER, 
        "Press Esc to return to menu");
}

void draw_game_over_message(const char *message, float *current_y) {
     draw_wrapped_text(get_small_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5f, *current_y, 300, message, 10);
}

void draw_game_over() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_background(get_background_manager());
    draw_screen_overlay((RGB) {.red = 0, .green = 0, .blue = 0}, 150);

    float current_y = 120;

    al_draw_text(get_large_font(), al_map_rgb(255, 255, 255), 
        SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.2f, ALLEGRO_ALIGN_CENTER, "Game Over");

    current_y += 100;

    draw_game_over_message(has_player_win() ? PLAYER_WIN_MESSAGE : 
        PLAYER_LOSE_MESSAGE, &current_y);

    current_y += 100;
  
    draw_options(&current_y);

    al_flip_display();
}