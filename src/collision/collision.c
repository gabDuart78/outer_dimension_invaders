#include "collision.h"
#include "utils.h"
#include "alien.h"
#include "player.h"
#include "bullet.h"
#include "bullet_manager.h"
#include "alien_manager.h"
#include "ufo_manager.h"
#include "playing_scene.h"

bool check_collision (Rect a, Rect b) {
    return a.pos.x < b.pos.x + b.width &&
           a.pos.x + a.width > b.pos.x &&
           a.pos.y < b.pos.y + b.height &&
           a.pos.y + a.height > b.pos.y;
}

bool bullet_hits_ufo(Bullet *b, UFO *ufo) {
    return check_collision(get_collider(b->pos, b->width, b->height), 
    get_collider(ufo->pos, ufo->width, ufo->height));
}

bool bullet_hits_enemy(Bullet *b, Alien *a) {
    return check_collision(get_collider(b->pos, b->width, b->height), 
        get_collider(a->pos, a->width, a->height));
}

bool bullet_hits_player(Bullet *b, Player *p) {
    return check_collision(get_collider(b->pos, b->width, b->height), 
        get_collider(p->pos, p->width, p->height));
}

void handle_player_bullets_collision_with_ufo(Player *player, UFO *ufo) {
    if (!ufo->is_active) return;

    for (int i = 0; i < player->bm->max; i++) {
        Bullet *bullet = &player->bm->bullets[i];

        if (!bullet->is_active) continue;

        if (bullet_hits_ufo(bullet, ufo)) {
            player_hits_enemy(player, ufo->points);
            kill_ufo(ufo);
            on_kill_enemy(get_collider(ufo->pos, ufo->width, ufo->height));
            deactive_bullet_by_id(player->bm, bullet->id);
        }
    }
}

void handle_player_bullets_collision_with_aliens(Player *player, AlienManager *alien_manager) {
    for (int i = 0; i < player->bm->max; i++) {
        Bullet *bullet = &player->bm->bullets[i];

        if (!bullet->is_active) continue;

        for (int j = 0; j < alien_manager->count; j++) {
            Alien *alien = &alien_manager->aliens[j];

            if (!alien->is_alive) continue;

             if (bullet_hits_enemy(bullet, alien)) {
                player_hits_enemy(player, alien->points);
                on_kill_enemy(get_collider(alien->pos, alien->width, alien->height));
                kill_alien_by_id(alien_manager, alien->id);
                deactive_bullet_by_id(player->bm, bullet->id);
                break;
            }
        }
    }
}

void handle_player_bullets_collision(Player *player, AlienManager *alien_manager, UFO *ufo) {
    handle_player_bullets_collision_with_aliens(player, alien_manager);
    handle_player_bullets_collision_with_ufo(player, ufo);
}

void handle_aliens_bullets_collision(AlienManager *alien_manager, Player *player) {
    for (int i = 0; i < alien_manager->bm->max; i++) {
        Bullet * bullet = &alien_manager->bm->bullets[i];

        if (!bullet->is_active) continue;

        if (bullet_hits_player(bullet, player)) {
            deactive_bullet_by_id(alien_manager->bm, bullet->id);
            hit_player(player);
        }
    }
}

void handle_collisions(Player *player, AlienManager *alien_manager, UFO *ufo) {
    handle_player_bullets_collision(player, alien_manager, ufo);
    handle_aliens_bullets_collision(alien_manager, player);
}

