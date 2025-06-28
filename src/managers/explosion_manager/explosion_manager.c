#include "explosion_manager.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "animator.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define EXPLOSION_SPRITE_PATH "../assets/images/sprites/explosion.png"

#define EXPLOSION_WIDTH 40
#define EXPLOSION_HEIGTH 40
#define EXPLOSION_FRAMES 6
#define EXPLOSION_FRAME_DURATION 1.f / 18.f

typedef struct Explosion {
    Point pos;
    float timer;
    float duration;
    bool active;
    ALLEGRO_BITMAP *sprite;
    Animator *animator;
} Explosion;

typedef struct ExplosionManager {
    Explosion *explosions;
    int count;
    int max;
} ExplosionManager;

ExplosionManager* create_explosion_manager() {
    ExplosionManager *manager = (ExplosionManager *) malloc(sizeof(ExplosionManager));

    if (!manager) {
        fprintf(stderr, "Failed to create explosion manager.\n");
        exit(-1);
    }

    return manager;
}

Explosion create_explosion(const char *sprite_path, Animator *animator, float duration) {
    Explosion explosion;
    explosion.sprite = get_sprite(sprite_path);
    explosion.animator = animator;
    explosion.active = false;
    explosion.duration = duration; 

    return explosion;
}

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
            EXPLOSION_HEIGTH, EXPLOSION_FRAME_DURATION, false);

        manager->explosions[i] = create_explosion(EXPLOSION_SPRITE_PATH, animator, 
            EXPLOSION_FRAME_DURATION * EXPLOSION_FRAMES);
    }

    manager->count = 0;
}

void trigger_explosion(ExplosionManager *manager, Rect collider) {
    if (manager->count >= manager->max) return;

    Explosion *explosion = NULL;

    for (int i = 0; i < manager->max; i++) {
        if (!((explosion = &manager->explosions[i])->active))
            break;
    }

    explosion->pos = get_centered_pos_inside_rect(collider, 
        EXPLOSION_WIDTH, EXPLOSION_HEIGTH);
    explosion->active = true;  
    explosion->timer = 0;
    manager->count++;
}

void deactive_explosion(Explosion *explosion) {
    reset_animation(explosion->animator);
    explosion->active = false;
}

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

void draw_explosions(ExplosionManager *manager) {
    for (int i = 0; i < manager->max; i++) {
        Explosion *explosion = &manager->explosions[i];

        if (!explosion->active) continue;

        draw_animated_sprite(explosion->sprite, explosion->animator, 
            explosion->pos.x, explosion->pos.y);
    }
}  

void destroy_explosion(Explosion *explosion) {
    if (explosion->sprite) al_destroy_bitmap(explosion->sprite);
    if (explosion->animator) destroy_animator(explosion->animator);
}

void destroy_explosions(ExplosionManager *manager) {
    for (int i = 0; i < manager->max; i++) 
        destroy_explosion(&manager->explosions[i]);

    free(manager->explosions);
}

void destroy_explosion_manager(ExplosionManager *manager) {
    if (!manager) return;

    if (manager->explosions) 
        destroy_explosions(manager);   

    free(manager);
}