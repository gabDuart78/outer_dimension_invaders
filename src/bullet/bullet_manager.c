#include "bullet_manager.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "screen_config.h"

BulletManager* create_bullet_manager(int max, BulletConfig cfg, ALLEGRO_BITMAP *sprite) {
    BulletManager *manager = (BulletManager *) malloc(sizeof(BulletManager));

    if (!manager) {
        fprintf(stderr, "Failed to create bullet manager.\n");
        exit(-1);
    }

    manager->bullets = (Bullet *) malloc(sizeof(Bullet) * max);

    if (!manager->bullets) {
        fprintf(stderr, "Falied to create bullets array.\n");
        exit(-1);
    } 

    manager->max = max;
    manager->quantity = 0;

    Point pos = {-10.f, -10.f};

    for (int i = 0; i < manager->max; i++) {
        cfg.id = i;
        Bullet b = create_bullet(get_collider(pos, 0, 0), cfg, sprite);
        manager->bullets[i] = b;
    }

     return manager;
}

bool is_bullet_off_boundary(Bullet *bullet) {
    return bullet->pos.y + bullet->height < 0 || bullet->pos.y > SCREEN_HEIGHT; 
}

void update_bullets(BulletManager * manager) {
    for (int i = 0; i < manager->max; i++) {
        Bullet *bullet = &manager->bullets[i];

        if (!bullet->is_active) continue;

        move_bullet(bullet);

        if (is_bullet_off_boundary(bullet)) 
            deactive_bullet_by_id(manager, bullet->id);
        
    }
}

void destroy_bullet_manager(BulletManager * manager) {
    if (!manager) return;

    for (int i = 0; i < manager->quantity; i++) {
        Bullet *bullet = &manager->bullets[i];
        destroy_bullet(bullet);
    }

    if (!manager->bullets) {
        free(manager->bullets);
    }

    free(manager);
}

Bullet* get_inactive_bullet(BulletManager *manager) {
    for (int i = 0; i < manager->max; i++)
        if (!manager->bullets[i].is_active) return &manager->bullets[i];

    return NULL;
}

void active_bullet_by_id(BulletManager *manager, int id) {
    active_bullet(&manager->bullets[id]);
    manager->quantity++;
}

void deactive_bullet_by_id(BulletManager *manager, int id) {
    deactive_bullet(&manager->bullets[id]);
    manager->quantity--;
}

void fire_bullet(BulletManager *manager, Rect hitbox) {
    if (manager->quantity == manager->max) return;

    Bullet *bullet = get_inactive_bullet(manager);
    bullet->pos = get_bullet_spawn_pos(hitbox, bullet->width, bullet->height);
    active_bullet_by_id(manager, bullet->id);
}

void draw_bullets(BulletManager *manager) {
    for (int i = 0; i < manager->max; i++) {
        Bullet *bullet = &manager->bullets[i];

        if (!bullet->is_active) continue;

        draw_bullet(bullet);
    }
}
