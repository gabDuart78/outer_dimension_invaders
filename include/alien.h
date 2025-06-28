#pragma once
#ifndef ALIEN_H
#define ALIEN_H

#include <stdbool.h>
#include <allegro5/allegro_color.h>
#include "utils.h"

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP; 
typedef struct BulletManager BulletManager;
typedef struct Animator Animator;

typedef struct Alien {
    Animator *animator;
    Point pos;
    int width;
    int height;
    int speed;
    bool is_alive;
    int descent_step;
    ALLEGRO_COLOR color;
    int id;
    ALLEGRO_BITMAP *sprite_sheet;
    bool draw_hitbox;
    int points;
} Alien;

typedef struct AlienConfig {
    Point pos;
    int width;
    int height;
    int speed;
    bool is_alive;
    int descent_step;
    RGB color;
    int id;
    bool draw_hitbox;
    int points;
    char *sprite_path;
} AlienConfig;

Alien create_alien(AlienConfig cfg, int id, int animation_frames, float animation_interval);

void draw_alien(Alien *alien);

void move_alien_horizontal(Alien *alien, MoveDir mov_dir, int amount);

void move_alien_vertical(Alien *alien, MoveDir move_dir, int amount);

void kill_alien(Alien *alien);

void destroy_alien(Alien *alien);

#endif