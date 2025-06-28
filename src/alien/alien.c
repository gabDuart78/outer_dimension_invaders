#include "alien.h"
#include <stdlib.h>
#include <stdio.h>
#include "animator.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "sound_manager.h"

Alien create_alien(AlienConfig cfg, int id, int animation_frames, float animation_interval) {
    Animator * animator = (Animator *) malloc(sizeof(Animator));

    if (animator == NULL) {
        fprintf(stderr, "Failed to create animator.\n");
        exit(-1);
    }
    
    init_animator(animator, animation_frames, 
        cfg.width, cfg.height, animation_interval, true);

    Alien alien = {
        .animator = animator,
        .pos = cfg.pos,
        .width = cfg.width,
        .height = cfg.height,
        .color = al_map_rgb(cfg.color.red, cfg.color.green, cfg.color.blue),
        .speed = cfg.speed,
        .is_alive = cfg.is_alive,
        .descent_step = cfg.descent_step,
        .id = id,
        .draw_hitbox = cfg.draw_hitbox,
        .sprite_sheet = get_sprite(cfg.sprite_path),
        .points = cfg.points,
    };
    
    return alien;
}

void draw_alien(Alien* alien) {
    draw_animated_sprite(alien->sprite_sheet, alien->animator, alien->pos.x, alien->pos.y);

    if (alien->draw_hitbox)
        al_draw_rectangle(alien->pos.x, alien->pos.y, 
                                 alien->pos.x + alien->width, 
                                 alien->pos.y + alien->height, alien->color, 0);
}

void move_alien_horizontal(Alien * alien, MoveDir mov_dir, int amount) {
    alien->pos.x += mov_dir == MOVE_RIGHT ? amount : -amount;
}

void move_alien_vertical(Alien * alien, MoveDir move_dir, int amount) {
    alien->pos.y += move_dir == MOVE_DOWN ? amount : -amount; 
}

void kill_alien(Alien *alien) {
    alien->is_alive = false;
    play_sound(SFX_ALIEN_DIE);
}

void destroy_alien(Alien *alien) {
    if (!alien) return;
    al_destroy_bitmap(alien->sprite_sheet);
    destroy_animator(alien->animator);
}
