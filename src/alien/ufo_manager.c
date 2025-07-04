#include "ufo_manager.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "animator.h"
#include "screen_config.h"
#include "sound_manager.h"
#include "stage_manager.h"
#include "game_context.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define UFO_SPRITE_PATH "../assets/images/sprites/alien/ufo.png" 
#define UFO_SPEED 2
#define UFO_WIDTH 60
#define UFO_HEIGHT 40
#define UFO_Y_SPAWN 45
#define UFO_SPAWN_PROBABILITY .05f
#define UFO_SPAWN_INTERVAL 20.0f
#define UFO_SPAWN_INTERVAL_MULTIPLIER 0.8f
#define UFO_ANIMATION_FRAMES 7
#define UFO_BASE_POINTS 50
#define UFO_MAX_SPEED 5
#define UFO_ACCELERATION_RATE 0.008f

/**
 * @brief Inicializa a estrutura UFO com configurações pre-definidas.
 * 
 * @param ufo Ponteiro para UFO.
 */
void init_ufo(UFO *ufo) {
    ufo->speed = UFO_SPEED;
    ufo->width = UFO_WIDTH;
    ufo->height = UFO_HEIGHT;
    ufo->spawn_probability = UFO_SPAWN_PROBABILITY;
    ufo->spawn_interval = UFO_SPAWN_INTERVAL * pow(UFO_SPAWN_INTERVAL_MULTIPLIER, 
        get_stage_manager()->current_stage);
    ufo->sprite_sheet = get_sprite(UFO_SPRITE_PATH); 
    ufo->draw_hitbox = false;
    ufo->color = al_map_rgb(255, 255, 255);
    ufo->last_spawn = al_get_time();
    ufo->is_active = false;

    Animator *animator = (Animator *) malloc(sizeof(Animator));

    if (!animator) {
        fprintf(stderr, "Failed to create animator.\n");
        exit(-1);
    }

    init_animator(animator, UFO_ANIMATION_FRAMES, 
        UFO_WIDTH, UFO_HEIGHT, 1.0f / UFO_ANIMATION_FRAMES, true);

    ufo->animator = animator;
}

/**
 * @brief Aloca memária para a estrutura UFO e retorna um ponteiro para a estrutura criada.
 * 
 * @return UFO.
 */
UFO * create_ufo() {
    UFO *ufo = (UFO *) malloc(sizeof(UFO));

    if (!ufo) {
        fprintf(stderr, "Failed to create UFO.\n");
        exit(-1);
    }

    init_ufo(ufo);

    return ufo;
}

/**
 * @brief Verifica se o UFO saiu dos limites da tela.
 * 
 * @param ufo Ponteiro para UFO.
 * 
 * @return Bool representando se o UFO saiu dos limites da tela.
 */
bool is_off_screen(UFO *ufo) {
    if (ufo->mov_dir == MOVE_RIGHT)
        return ufo->pos.x > SCREEN_WIDTH;

    return ufo->pos.x + ufo->width < 0;
}

/**
 * @brief Define a direção de movimente do UFO e o posiciona.
 * 
 * @param ufo Ponteiro para UFO.
 */
void spawn_ufo(UFO *ufo) {
    int mov_dir = random_integer(0, 1) == 0 ? MOVE_RIGHT : MOVE_LEFT;
    Point pos = {.x = mov_dir == MOVE_RIGHT ? - ufo->width : SCREEN_WIDTH, 
        .y = UFO_Y_SPAWN,};

    ufo->mov_dir = mov_dir;
    ufo->pos = pos;
}

/**
 * @brief Define quantos pontos o UFO terá.
 * 
 * @param ufo Ponteiro para UFO.
 */
void set_ufo_points(UFO *ufo) {
    ufo->points = random_integer(2, 10) * UFO_BASE_POINTS;
}

/**
 * @brief Ativa a estrutura UFO, posiciona-a na tela, define seus pontos e 
 * toca seu efeito sonoro.
 * 
 * @param ufo Ponteiro para UFO.
 */
void active_ufo(UFO *ufo) {
    ufo->is_active = true;
    ufo->speed = UFO_SPEED;
    ufo->last_spawn = al_get_time();
    spawn_ufo(ufo);
    set_ufo_points(ufo);
    play_sound(SFX_UFO);
}

/**
 * @brief Toca o efeito sonoro de morte para o UFO e o desativa.
 * 
 * @param ufo Ponteiro para UFO.
 */
void kill_ufo(UFO *ufo) {
    play_sound(SFX_HIT_UFO);
    deactive_ufo(ufo);
}

/**
 * @brief Troca o estado lógico do UFO para morte, define o ultimo spawn do UFO e 
 * reseta o seu efeito sonoro.
 * 
 * @param ufo Ponteiro para UFO.
 */
void deactive_ufo(UFO *ufo) {
    ufo->is_active = false;
    ufo->last_spawn = al_get_time();
    remove_sound(SFX_UFO);
    rewind_sound(SFX_UFO);
}

/**
 * @brief Lida com o movimento do UFO na horizontal, verifica se 
 * ele saiu da tela e o desativa caso verdade. 
 * 
 * @param ufo Ponteiro para UFO.
 */
void handle_ufo_movement(UFO *ufo) {
    float vx = ufo->mov_dir == MOVE_RIGHT ? ufo->speed : -ufo->speed;
    ufo->pos.x += vx;

     if (is_off_screen(ufo)) {
        deactive_ufo(ufo);
        return;
    }

}

/**
 * @brief Aumenta a valocida do UFO com base no tempo no seu tempo de vida.
 * 
 * @param ufo Ponteiro para UFO.
 * @param delta_time Representa o tempo intervalo entre o spawn do UFO e o momento atual.
 */
void increase_ufo_speed(UFO *ufo, double delta_time) {
    ufo->speed += delta_time * UFO_ACCELERATION_RATE;
}

/**
 * @brief Verifica se o UFO deve ser posicionado com base no intervalo último o seu último
 * spawn e o momento atual, se for o caso o ufo e ativado.
 * 
 * @param ufo Ponteiro para UFO.
 * @param delta_time Intervalo de tempo entre o último spwan do UFO e o momento atual.
 */
void maybe_spawn_ufo(UFO *ufo, double delta_time) {
    float spawn_change = random_float();

    if (spawn_change <= ufo->spawn_probability && 
        delta_time > ufo->spawn_interval) 
        active_ufo(ufo);
}

/**
 * @brief Verifica se o ufo está vivo.
 * 
 * @param ufo Ponteiro para UFO.
 * 
 * @return Bool representado se o ufo está vivo ou não.
 */
bool is_ufo_alive(UFO *ufo) {
    return ufo && ufo->is_active;
}

/**
 * @brief Faz a atualização do ufo, movimento, animação e velocidade caso ele estiver ativado, se não, verifica 
 * se ele deve ser ativado.
 * 
 * @param ufo Ponteiro para UFO.
 */
void update_ufo(UFO *ufo) {
    double now = al_get_time();
    double delta_time = now - ufo->last_spawn;

    if (is_ufo_alive(ufo)) {
        if (ufo->speed < UFO_MAX_SPEED) 
            increase_ufo_speed(ufo, delta_time);
         
        handle_ufo_movement(ufo);
        update_animator(ufo->animator);
        return;
    }

    maybe_spawn_ufo(ufo, delta_time);   
}

/**
 * @brief Desenha o UFO tela, seu sprite e (opcionalmente) sua hitbox. 
 * 
 * @param ufo Ponteiro para UFO.
 */
void draw_ufo(UFO *ufo) {
    if (!ufo->is_active) return;

    if (ufo->draw_hitbox) 
        al_draw_rectangle(ufo->pos.x, ufo->pos.y, 
            ufo->pos.x + ufo->width, ufo->pos.y + ufo->height, ufo->color, 0);

    draw_animated_sprite(ufo->sprite_sheet, 
        ufo->animator, ufo->pos.x, ufo->pos.y);
}


/**
 * @brief Libera os recursos utilizados pelo UFO.
 * 
 * @param ufo Ponteiro para UFO.
 */
void destroy_ufo(UFO *ufo) {
    if (!ufo) return;
    deactive_ufo(ufo);
    if (ufo->animator) destroy_animator(ufo->animator);
    if (ufo->sprite_sheet) al_destroy_bitmap(ufo->sprite_sheet);
    free(ufo);
}