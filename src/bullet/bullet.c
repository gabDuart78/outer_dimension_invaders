#include "bullet.h"
#include "collision.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>

/**
 * @breif Cria uma nova bala com base em um retângulo gerador.
 * 
 * @param spawner Retângulo de origem da bala.
 * @param cfg Configurações da bala (tamanho, direção, velocidade etc).
 * @param sprite Bitmap a ser usado para desenhar a bala.
 * 
 * @return Bullet.
 */
Bullet create_bullet(Rect spawner, BulletConfig cfg, ALLEGRO_BITMAP * sprite) {
    Bullet bullet = {
        .width = cfg.width,
        .height = cfg.height,
        .speed = cfg.speed,
        .move_dir = cfg.move_dir,
        .is_active = cfg.is_active,
        .color = al_map_rgb(cfg.color.red, cfg.color.green, cfg.color.blue),
        .id = cfg.id,
        .sprite = sprite,
        .draw_hitbox = cfg.draw_hitbox,
    };

    bullet.pos = get_bullet_spawn_pos(spawner, bullet.width, bullet.height);

    return bullet;
}

/**
 * @brief Calcula a posição inicial da bala com base em um retângulo e dimensões.
 * 
 * @param spawner Retângulo de origem.
 * @param width Largura da bala.
 * @param height Altura da bala.
 * 
 * @return Point representando onde a bala deve ser colocada na tela.
 */
Point get_bullet_spawn_pos(Rect spawner, int width, int height) {
    return get_centered_pos_inside_rect(spawner, width, height);
}

/**
 * @brief Move a bala verticalmente, de acordo com sua direção e velocidade.
 * 
 * @param bullet Ponteiro para a bala.
 */
void move_bullet(Bullet * bullet) {
    bullet->pos.y += bullet->move_dir == MOVE_DOWN ? bullet->speed : -bullet->speed;
}

/**
 * @brief Desenha a bala na tela. Opcionalmente, desenha sua hitbox.
 * 
 * @param bullet Ponteiro para a bala.
 */
void draw_bullet(Bullet * bullet) {
    al_draw_bitmap(bullet->sprite, bullet->pos.x, bullet->pos.y, 0);

    if (bullet->draw_hitbox)
        al_draw_rectangle(bullet->pos.x, bullet->pos.y, 
                         bullet->pos.x + bullet->width, 
                         bullet->pos.y + bullet->height, 
                         bullet->color, 0);
}

/**
 * @brief Libera os recursos utilizados pela Bullet.
 * 
 * @param bullet Ponteiro para a bala.
 */
void destroy_bullet(Bullet * bullet) {
    al_destroy_bitmap(bullet->sprite);
}

/**
 * @brief Ativa a bala para que ela possa ser atualizada/desenhada.
 * 
 * @param bullet Ponteiro para a bala.
 */
void active_bullet(Bullet * bullet) {
    bullet->is_active = true;
}

/**
 * @brief Desativa a bala em caso de colisão ou fora da tela.
 * 
 * @param bullet Ponteiro para a bala.
 */
void deactive_bullet(Bullet * bullet) {
    bullet->is_active = false;
}
