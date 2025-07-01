#include "bullet_manager.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include "screen_config.h"

/**
 * @brief Cria e inicializa um gerenciador de balas com balas pré-criadas e inativas.
 * 
 * @param max Número máximo.
 * @param cfg Configuração padrão de cada bala.
 * @param sprite Bitmap a ser usado por todas as balas.
 * 
 * @return BulletManager.
 */
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

/**
 * @brief Verifica se a bala está fora da tela.
 * 
 * @param bullet Ponteiro para a bala.
 * 
 * @return Bool representado se a bala está fora dos limites da tela.
 */
bool is_bullet_off_boundary(Bullet *bullet) {
    return bullet->pos.y + bullet->height < 0 || bullet->pos.y > SCREEN_HEIGHT; 
}

/**
 * @brief Atualiza todas as balas ativas (movimento e descarte).
 * 
 * @param manager Ponteiro para o gerenciador de balas.
 */
void update_bullets(BulletManager * manager) {
    for (int i = 0; i < manager->max; i++) {
        Bullet *bullet = &manager->bullets[i];

        if (!bullet->is_active) continue;

        move_bullet(bullet);

        if (is_bullet_off_boundary(bullet)) 
            deactive_bullet_by_id(manager, bullet->id);
        
    }
}

/**
 * @brief Libera os recursos usados pelas balas e o gerenciador.
 * 
 * @param manager Ponteiro para o gerenciador.
 */
void destroy_bullet_manager(BulletManager * manager) {
    if (!manager) return;

    for (int i = 0; i < manager->quantity; i++) {
        Bullet *bullet = &manager->bullets[i];
        destroy_bullet(bullet);
    }

    if (manager->bullets) 
        free(manager->bullets);
    

    free(manager);
}

/**
 * @brief Retorna um ponteiro para a próxima bala inativa disponível.
 * 
 * @param manager Ponteiro para o gerenciador.
 * 
 * @return Bullet desativada.
 */
Bullet* get_inactive_bullet(BulletManager *manager) {
    for (int i = 0; i < manager->max; i++)
        if (!manager->bullets[i].is_active) return &manager->bullets[i];

    return NULL;
}

/**
 * @brief Ativa uma bala específica por ID e incrementa o contador.
 * 
 * @param manager Ponteiro para o gerenciador.
 * @param id Index da bala no array.
 */
void active_bullet_by_id(BulletManager *manager, int id) {
    active_bullet(&manager->bullets[id]);
    manager->quantity++;
}

/**
 * @brief Desativa uma bala específica por ID e decrementa o contador.
 * 
 * @param manager Ponteiro para o gerenciador.
 * @param id Índice da bala no array.
 */
void deactive_bullet_by_id(BulletManager *manager, int id) {
    deactive_bullet(&manager->bullets[id]);
    manager->quantity--;
}

/**
 * @brief Ativa uma bala inativa com base na hitbox do atirador.
 * 
 * @param manager Ponteiro para o gerenciador.
 * @param hitbox Retângulo de onde a bala será disparada.
 */
void fire_bullet(BulletManager *manager, Rect hitbox) {
    if (manager->quantity == manager->max) return;

    Bullet *bullet = get_inactive_bullet(manager);
    bullet->pos = get_bullet_spawn_pos(hitbox, bullet->width, bullet->height);
    active_bullet_by_id(manager, bullet->id);
}

/**
 * @brief Desenha todas as balas ativas na tela.
 * 
 * @param manager Ponteiro para o gerenciador de balas.
 */
void draw_bullets(BulletManager *manager) {
    for (int i = 0; i < manager->max; i++) {
        Bullet *bullet = &manager->bullets[i];

        if (!bullet->is_active) continue;

        draw_bullet(bullet);
    }
}
