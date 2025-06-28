#pragma once
#ifndef BULLET_H
#define BULLET_H

#include <stdbool.h>
#include <allegro5/allegro_color.h>
#include "utils.h"

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

typedef struct Bullet {
    Point pos;
    int id;
    int width;
    int height;
    MoveDir move_dir;
    bool is_active;
    float speed;
    ALLEGRO_COLOR color;
    ALLEGRO_BITMAP *sprite;
    bool draw_hitbox;
} Bullet;

typedef struct BulletConfig {
    int id;
    int width;
    int height;
    int speed;
    MoveDir move_dir;
    bool is_active;
    RGB color;
    bool draw_hitbox;
} BulletConfig;

extern const BulletConfig BULLET_CONFIG;

Bullet create_bullet(Rect spawner, BulletConfig cfg, ALLEGRO_BITMAP * sprite);

void move_bullet(Bullet *bullet);

void draw_bullet(Bullet *bullet);

Point get_bullet_spawn_pos(Rect spawner, int width, int height);

void destroy_bullet(Bullet *bullet);

void active_bullet(Bullet *bullet);

void deactive_bullet(Bullet *bullet);

#endif