#include "explosion_manager.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "animator.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define EXPLOSION_SPRITE_PATH "../assets/images/sprites/explosion.png"

#define EXPLOSION_WIDTH 40
#define EXPLOSION_HEIGHT 40
#define EXPLOSION_FRAMES 6
#define EXPLOSION_FRAME_DURATION 1.f / 18.f

/**
 * @brief Estrura utilizada para representar uma explosão, 
 * posição, animação, duração, etc.
 */
typedef struct Explosion {
    Point pos;
    float timer;
    float duration;
    bool active;
    ALLEGRO_BITMAP *sprite;
    Animator *animator;
} Explosion;

/**
 * @brief Estrutura utilizada para administrar as explosões do game.
 */
typedef struct ExplosionManager {
    Explosion *explosions;
    int count;
    int max;
} ExplosionManager;

/**
 * @brief Alloca memoria para um ExplosionManager.
 * 
 * @return ExplosionManager.
 */
ExplosionManager* create_explosion_manager() {
    ExplosionManager *manager = (ExplosionManager *) malloc(sizeof(ExplosionManager));

    if (!manager) {
        fprintf(stderr, "Failed to create explosion manager.\n");
        exit(-1);
    }

    return manager;
}

/**
 * @brief Cria uma explosion.
 * 
 * @param sprite_path Caminho para o sprite da explosão
 * @param animator Estrutura Animator para animar o sprite da explosão.
 * @param duration Duração em segundos da explosão.
 * 
 * @return Explosion.
 */
Explosion create_explosion(const char *sprite_path, Animator *animator, float duration) {
    Explosion explosion;
    explosion.sprite = get_sprite(sprite_path);
    explosion.animator = animator;
    explosion.active = false;
    explosion.duration = duration; 

    return explosion;
}

/**
 * @brief Inicializa o gerenciador de explosões.
 * 
 * @param manager Ponteiro para o ExplosionManager.
 * @param max Quantidade máxima de exlosões suportadas.
 */
void init_explosion_manager(ExplosionManager *manager, int max) {
    manager->max = max;
    manager->explosions = (Explosion *) malloc(sizeof(Explosion) * max);

    if (!manager->explosions) {
        fprintf(stderr, "Failed to create explosions array.\n");
        exit(-1);
    } 

    for (int i = 0; i < max; i++) {
        Animator *animator = (Animator *) malloc(sizeof(Animator));
    
        if (!animator) {
            fprintf(stderr, "Failed to create animator.\n");
            exit(-1);
        }

        init_animator(animator, EXPLOSION_FRAMES, EXPLOSION_WIDTH, 
            EXPLOSION_HEIGHT, EXPLOSION_FRAME_DURATION, false);

        manager->explosions[i] = create_explosion(EXPLOSION_SPRITE_PATH, animator, 
            EXPLOSION_FRAME_DURATION * EXPLOSION_FRAMES);
    }

    manager->count = 0;
}

/**
 * @brief Ativa uma explosão.
 * 
 * @param manager Ponteiro para o ExplosionManager.
 * @param collider Um rectângulo que representa o objeto explodido.
 */
void trigger_explosion(ExplosionManager *manager, Rect collider) {
    if (manager->count >= manager->max) return;

    Explosion *explosion = NULL;

    for (int i = 0; i < manager->max; i++) {
        if (!manager->explosions[i].active){
            explosion = &manager->explosions[i];
            break;
        }
    }

    if (!explosion) return;

    explosion->pos = get_centered_pos_inside_rect(collider, 
        EXPLOSION_WIDTH, EXPLOSION_HEIGHT);
    explosion->active = true;  
    explosion->timer = 0;
    manager->count++;
}

/**
 * @brief Desativa uma explosão, somente altera seu estado lógico.
 * 
 * @param explosion Ponteiro para a explosão a ser desativada.
 */
void deactive_explosion(Explosion *explosion) {
    reset_animation(explosion->animator);
    explosion->active = false;
}

/**
 * @brief Atualiza as explosões e as desativa caso tenha terminado.
 * 
 * @param manager Ponteiro para o ExplosionManager
 * @param delta_time Tempo decorrido em segundos entre o último frame e o atual.
 */
void update_explosions(ExplosionManager *manager, double delta_time) {
    for (int i = 0; i < manager->max; i++) {
        Explosion *explosion = &manager->explosions[i];

        if (!explosion->active) continue;

        explosion->timer += delta_time;

        if (explosion->timer >= explosion->duration) {
            deactive_explosion(explosion);
            manager->count--;
        }

        update_animator(explosion->animator);
    }
}

/**
 * @brief Desenhas as explosões na tela.
 * 
 * @param manager Ponteiro para o ExplosionManager.
 */
void draw_explosions(ExplosionManager *manager) {
    for (int i = 0; i < manager->max; i++) {
        Explosion *explosion = &manager->explosions[i];

        if (!explosion->active) continue;

        draw_animated_sprite(explosion->sprite, explosion->animator, 
            explosion->pos.x, explosion->pos.y);
    }
}  

/**
 * @brief Libera os recursos utilizados pela Explosion.
 * 
 * @param explosion Ponteiro para uma Explosion.
 */
void destroy_explosion(Explosion *explosion) {
    if (explosion->sprite) al_destroy_bitmap(explosion->sprite);
    if (explosion->animator) destroy_animator(explosion->animator);
}

/**
 * @brief Libera os recursos utilizados por todas as explosões do game.
 * 
 * @param manager Ponteiro para o ExplosionManager.
 */
void destroy_explosions(ExplosionManager *manager) {
    for (int i = 0; i < manager->max; i++) 
        destroy_explosion(&manager->explosions[i]);

    free(manager->explosions);
}


/**
 * @brief Libera os recursos utilizados pelo ExplosionManager.
 * 
 * @param manager Ponteiro para o ExplosionManger.
 */
void destroy_explosion_manager(ExplosionManager *manager) {
    if (!manager) return;

    if (manager->explosions) 
        destroy_explosions(manager);   

    free(manager);
}