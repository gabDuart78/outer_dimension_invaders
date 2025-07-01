#include "background_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

/**
 * @brief Estrutura usada para armazenar um ALLEGRO_BITMAP que será utilizada 
 * para representa o background nos vários estado do game.
 */
typedef struct BackgroundManager {
    ALLEGRO_BITMAP *background;
} BackgroundManager;    

/**
 * @brief Alloca memória para um BackgroundManager.
 * 
 * @return BackgroundManager.
 */
BackgroundManager* create_background_manager() {
    BackgroundManager *bg_manager = (BackgroundManager *) malloc(sizeof(BackgroundManager));

    if (!bg_manager) {
        fprintf(stderr, "Failed to create Background Manager.\n");
        exit(-1);
    }

    return bg_manager;
}

/**
 * @brief Inicializa a estrutura BackgroundManager.
 * 
 * @param manager Ponteiro para um BackgroundManager.
 */
void init_background_manager(BackgroundManager *manager) {
    manager->background = NULL;
}

/**
 * @brief Libera os recursos utilizados pelo BackgroundManager.
 * 
 * @param background Imagem allegro bitmap.
 */
void destroy_background(ALLEGRO_BITMAP *background) {
    if (!background) return;

    al_destroy_bitmap(background);
}

/**
 * @brief Carrega um ALLEGRO_BITMAP.
 * 
 * @param manager Ponteiro para o BackgroundManager.
 * @param path Caminho para a imagem a ser carregada.
 */
void load_background(BackgroundManager *manager, const char *path) {
    if (manager->background) 
        destroy_background(manager->background);

    manager->background = al_load_bitmap(path);

    if (!manager->background) {
        fprintf(stderr, "Failed to load background at path: %s\n", path);
        exit(-1);
    }
}

/**
 * @brief Desenha a imagem na tela.
 * 
 * @param manager Ponteiro para um BackgroundManager.
 */
void draw_background(BackgroundManager *manager) {
    if (manager->background)
        al_draw_bitmap(manager->background, 0, 0, 0);
}


/**
 * @brief Libera os recursos utilizados pelo BackgroundManager.
 * 
 * @param manager Ponteiro para um BackgroundManager.
 */
void destroy_background_manager(BackgroundManager *manager) {
    if (!manager) return;
    if (manager->background)
        destroy_background(manager->background);
    free(manager);
}