#include "alien.h"
#include <stdlib.h>
#include <stdio.h>
#include "animator.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "sound_manager.h"

/**
 * @brief Cria e inicializa um novo alien com base na configuração forncedia.
 *     
 * @param cfg Estrutura de configuração do alien.
 * @param id Identificação do alien.    
 * @param animation_frames Número de frames da animação. 
 * @param animation_interval Tempo em quadros da animação.
 * 
 * @return Alien.
 */
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

/**
 * @brief Desenha o alien na tela incluíndo o srite e opcionalmente a hitbox. 
 */

void draw_alien(Alien* alien) {
    draw_animated_sprite(alien->sprite_sheet, alien->animator, alien->pos.x, alien->pos.y);

    if (alien->draw_hitbox)
        al_draw_rectangle(alien->pos.x, alien->pos.y, 
                                 alien->pos.x + alien->width, 
                                 alien->pos.y + alien->height, alien->color, 0);
}

/**
 * @brief Move o alien horizontalemente.
 * 
 * @param alien Ponteiro para o alien.
 * @param mov_dir Direção do movimento (MOVE_LEFT ou MOVE_RIGHT).
 * @param amount Quantidade pixels a mover.
 */

void move_alien_horizontal(Alien * alien, MoveDir mov_dir, int amount) {
    alien->pos.x += mov_dir == MOVE_RIGHT ? amount : -amount;
}

/**
 * @brief Move o alien verticalmente.
 * 
 * @param alien Ponteiro para o alien.
 * @param mov_dir Direção do movimento (MOVE_UP ou MOVE_DOWN).
 * @param amount Quantidade pixels a mover.
 */
void move_alien_vertical(Alien * alien, MoveDir move_dir, int amount) {
    alien->pos.y += move_dir == MOVE_DOWN ? amount : -amount; 
}

/**
 * @brief Muda o estado lógico do alien (marca como morto) e toca o som de morte.
 * 
 * @param Não libera memória.
 */
void kill_alien(Alien *alien) {
    alien->is_alive = false;
    play_sound(SFX_ALIEN_DIE);
}

/**
 * @brief Libera os recursos utilizados pelo alien.
 * 
 * @param alien Ponteiro para o alien.
 */
void destroy_alien(Alien *alien) {
    if (!alien) return;
    al_destroy_bitmap(alien->sprite_sheet);
    destroy_animator(alien->animator);
}
