#pragma once
#ifndef BULLET_MANAGER_H
#define BULLET_MANAGER_H

#include "bullet.h"

typedef struct Rect Rect;
typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

typedef struct BulletManager {
    Bullet *bullets;
    int quantity;
    int max;
} BulletManager;

BulletManager * create_bullet_manager(int max, BulletConfig cfg, ALLEGRO_BITMAP * sprite);

void fire_bullet(BulletManager * manager, Rect hitbox);

void deactive_bullet_by_id(BulletManager * manager, int id);

void destroy_bullet_manager(BulletManager * manager);

void update_bullets(BulletManager * manager);

void draw_bullets(BulletManager *manager);

#endif