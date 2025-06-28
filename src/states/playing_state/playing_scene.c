#include "playing_scene.h"
#include "player.h"
#include "bullet.h"
#include "bullet_manager.h"
#include "screen_config.h"
#include "alien.h"
#include "alien_manager.h"
#include "collision.h"
#include "ui.h"
#include "sound_manager.h"
#include "state_manager.h"
#include "background_manager.h"
#include "game_context.h"
#include "ufo_manager.h"
#include "stage_manager.h"
#include "explosion_manager.h"
#include <allegro5/allegro_image.h>

#define BG1_PATH "../assets/images/bg/playing_bg.png"

#define DANGER_LINE_Y SCREEN_HEIGHT - (PLAYER_CONFIG.height + SCREEN_BOTTOM_MARGIN)
#define GAME_MAX_EXPLOSIONS 16

static double _last_update = 0;
static Player *player = NULL;
static ExplosionManager *explosion_manager = NULL; 
static AlienManager *alien_manager = NULL;
static UFO *ufo = NULL;

void enter_playing_state() {
    player = create_player(PLAYER_CONFIG);
    player->score = get_player_score();
    alien_manager = create_alien_manager();
    ufo = create_ufo();
    explosion_manager = create_explosion_manager();
    init_explosion_manager(explosion_manager, GAME_MAX_EXPLOSIONS);
    start_stage(get_stage_manager(), alien_manager);
    spawn_aliens(alien_manager);
    load_background(get_background_manager(), BG1_PATH);
    play_music(PLAYING_BG_MUSIC);
    init_ui();
}

void clean_up_game_state() {
    remove_music(PLAYING_BG_MUSIC);
    rewind_music(PLAYING_BG_MUSIC);
    destroy_player(player);
    destroy_alien_manager(alien_manager);
    destroy_explosion_manager(explosion_manager);
    destroy_ufo(ufo);
    destroy_ui();
    player = NULL;
    alien_manager = NULL;
    ufo = NULL;
    explosion_manager = NULL;
} 

void exit_game_state(GameState new_state, bool enter_state) {
    clean_up_game_state();
    set_game_state(new_state, enter_state);
}

void exit_game_state_with_transition(GameState new_state, bool enter_state) {
    clean_up_game_state();
    enter_state_with_transition(new_state, .2, enter_state);
}

bool update_game_logic() {
    bool invasion_succeeded = aliens_crossed_threshold(alien_manager, DANGER_LINE_Y);

    if (invasion_succeeded || !player->is_alive) {
        if (invasion_succeeded) {
            kill_player(player);
        }

        set_player_win(false);
        set_game_over(true);

        return false;
    }

    if (get_stage_manager()->stage_cleared) {
        set_game_over(true);
        set_player_win(true);

        return false;
    }

    update_player(player);
    update_aliens(alien_manager);
    update_ufo(ufo);
    handle_collisions(player, alien_manager, ufo);
    check_stage_progress(get_stage_manager(), alien_manager);

    return true;
}

void update_game() {
    double now = al_get_time();
    double delta_time = now  - _last_update;
    _last_update = now;

    if (!update_game_logic()) {
        set_player_score(player->score);
        exit_game_state(STATE_GAME_OVER, true);
        return;
    }

    update_explosions(explosion_manager, delta_time);
}

void handle_game_input(ALLEGRO_EVENT event) {
    if (key_pressed(event, ALLEGRO_KEY_ESCAPE)) {
        reset_game_context();
        exit_game_state_with_transition(STATE_MENU, true);
        return;
    }

    if (close_display(event)) {
        exit_game_state(STATE_EXIT, false);
        return;
    }

    handle_player_events(player, interpret_player_event(&event));
}

void draw_game() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    draw_background(get_background_manager());
    draw_bullets(player->bm);
    draw_bullets(alien_manager->bm);
    draw_player(player);
    draw_aliens(alien_manager);
    draw_ufo(ufo);
    draw_explosions(explosion_manager);
    draw_ui(player->max_life, player->lifes, player->score);
    al_flip_display();
}

void on_kill_enemy(Rect collider) {
    trigger_explosion(explosion_manager, collider);
}
