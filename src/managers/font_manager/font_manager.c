#include "font_manager.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

#define FONT1_PATH "../assets/fonts/PixelEmulator.ttf"
#define FONT2_PATH "../assets/fonts/PressStart2P.ttf"

#define SMALL_FONT1_SIZE 16
#define MEDIUM_FONT1_SIZE 24
#define LARGE_FONT1_SIZE 42

#define SMALL_FONT2_SIZE 14
#define MEDIUM_FONT2_SIZE 20
#define LARGE_FONT2_SIZE 32

static FontManager fm;

/**
 * @brief Tentar carregar um ttf fonte, se for possível retorna true, caso contrário, falso.
 * 
 * @param path Caminho para o fonte.
 * @param small Tamanho para a fonte pequenha.
 * @param medium Tamanho para a fonte média.
 * @param large Tamanho para a fonte grande.
 * 
 * @return Bool indicando se foi possível carregar uma das duas fontes.
 */
bool try_load_font(const char *path, int small, int medium, int large) {
    ALLEGRO_FONT *small_f = al_load_ttf_font(path, small, 0);
    ALLEGRO_FONT *medium_f = al_load_ttf_font(path, medium, 0);
    ALLEGRO_FONT *large_f = al_load_ttf_font(path, large, 0);


    if (!small_f || !medium_f || !large_f) {
        fprintf(stderr, "Failed to lond font in path: %s\n", path);

        if (small_f) al_destroy_font(small_f);
        if (medium_f) al_destroy_font(medium_f);
        if (large_f) al_destroy_font(large_f);

        return false;
    }

    fm.small_font = small_f;
    fm.medium_font = medium_f;
    fm.large_font = large_f;

    return true;
}

/**
 * @brief Inicializa as bibliotecas allegro necessárias e tenta carregar uma das duas fontes 
 * disponíveis, retorna true caso isso ocorra normalmente, caso algum passo falhe retorna falso.
 * 
 * @return Bool indicando se foi possível iniciar os módulos do allegro responsáveis por lidar com fontes.
 */
bool init_font_manager() {
    if (!al_init_font_addon() || !al_init_ttf_addon()) {
        fprintf(stderr, 
            "Unable to init font manager. Font addon or ttf addon wasn't initialized..\n");
        return false;
    }

    if (!try_load_font(FONT1_PATH, SMALL_FONT1_SIZE, MEDIUM_FONT1_SIZE, LARGE_FONT1_SIZE)) 
        return try_load_font(FONT2_PATH, SMALL_FONT2_SIZE, MEDIUM_FONT2_SIZE, LARGE_FONT2_SIZE);

    return true;
}

/**
 * @brief Libera os recursos utilizados pelo FontManager.
 */
void destroy_font_manager() {
    if (fm.small_font) al_destroy_font(fm.small_font);
    if (fm.medium_font) al_destroy_font(fm.medium_font);
    if (fm.large_font) al_destroy_font(fm.large_font);
}

/**
 * @brief Retorna a fonte no tamanho pequeno.
 * 
 * @return ALLEGRO_FONT pequena.
 */
ALLEGRO_FONT* get_small_font() {
    return fm.small_font;
}

/**
 * @brief Retorna a fonte no tamanho médio.
 * 
 * @return ALLEGRO_FONT média.
 */
ALLEGRO_FONT* get_medium_font() {
    return fm.medium_font;
}

/**
 * @brief Retorna a fonte no tamanho grande.
 * 
 * @return ALLEGRO_FONT grande.
 */
ALLEGRO_FONT* get_large_font() {
    return fm.large_font;
}