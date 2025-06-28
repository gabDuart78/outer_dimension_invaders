#include "utils.h"
#include "screen_config.h"
#include "sound_manager.h"
#include "state_manager.h"
#include "game_over_state.h"
#include "menu.h"
#include "playing_scene.h"
#include "save_score_state.h"
#include "score_rank_state.h"
#include "font_manager.h"
#include "transition_state.h"
#include "game_context.h"
#include "game_stuff.h"
#include "allegro_stuff.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>

void handle_input(ALLEGRO_EVENT event) {
    switch(get_game_state()) {
        case STATE_MENU:
            handle_menu_inupt(event);
            break;
        case STATE_PLAYING:
            handle_game_input(event);
            break;
        case STATE_GAME_OVER:
            handle_game_over_input(event);
            break;
        case STATE_SAVE_SCORE:
            handle_input_save_score(event);
            break;
        case STATE_SCORE_RANK:
            handle_score_rank_input(event);
            break;
        default:
            break;
    }
}

bool update() {
    switch(get_game_state()) {
        case STATE_MENU:
            update_menu();
            break;
        case STATE_PLAYING:
            update_game();
            break;
        case STATE_TRANSITION:
            update_transition_state();
            break;
        case STATE_EXIT:
            return false;
        case STATE_SCORE_RANK:
            update_score_rank();
            break;
        default:
            break;
    }

    return true;
}

void draw() {
    switch(get_game_state()) {
        case STATE_MENU:
            draw_menu();
            break;
        case STATE_PLAYING:
            draw_game();
            break;
        case STATE_TRANSITION:
            draw_transition();
            break;
        case STATE_GAME_OVER:
            draw_game_over();
            break;
        case STATE_SAVE_SCORE:
            draw_save_score_state();
            break;
        case STATE_SCORE_RANK:
            draw_score_rank();
            break;
        default:
            break;
    }
}

//TODO 
// Make a error state
// Refactor code
// Add player power ups

int main() {
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_EVENT event;
    bool redraw = false;
    bool is_running = true;
    
    if (!init_all_necessary_allegro_components() || 
            !install_all_necessary_allegro_components()) 
        return -1;

    if (!init_game_components()) 
        return -1;

    timer = al_create_timer(FPS);
    if (!timer) {
        fprintf(stderr, "Failed to create timer.\n");
        return -1;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Failed to create display.\n");
        return -1;
    }

    queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Failed to create event queue.\n");
        return -1;
    }

    set_time_seed();
    load_sounds();
    init_game_context();

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    
    al_start_timer(timer);

    set_game_state(STATE_MENU, true);

    while (is_running) {
         al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_KEY_UP || 
             event.type == ALLEGRO_EVENT_KEY_DOWN ||
             event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)  {
            handle_input(event);
        }
                  
        if (event.type == ALLEGRO_EVENT_TIMER) {
            is_running = update();
            redraw = true;
        }

        if (redraw && al_is_event_queue_empty(queue)) {
            draw();
            redraw = false;
        }
    }

    save_scores_to_file(get_score_table(), SCORES_PATH);
    destroy_sound_bank();
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    destroy_game_context();

    return 0;
}