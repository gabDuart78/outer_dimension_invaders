#pragma once
#ifndef UFO_MANAGER_H
#define UFO_MANAGER_H

#include "utils.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;
typedef struct Animator Animator;

typedef struct UFO {
    Point pos;
    float speed;
    int width;
    int height;
    MoveDir mov_dir;
    bool is_active;
    int points;
    float spawn_probability;
    double spawn_interval;
    double last_spawn;
    ALLEGRO_BITMAP *sprite_sheet;
    Animator *animator;
    bool draw_hitbox;
    ALLEGRO_COLOR color;
} UFO;

UFO * create_ufo();

void update_ufo(UFO *ufo);

void draw_ufo(UFO *ufo);

void destroy_ufo(UFO *ufo);

void deactive_ufo(UFO *ufo);

void kill_ufo(UFO *ufo);

#endif