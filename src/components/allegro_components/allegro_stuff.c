#include "allegro_stuff.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <stdio.h>

/**
 * @brief Inicializa os componentes da biblioteca Allegro necessários
 * para o game.
 * 
 * @return bool indicando se que todos os componentes forem inicializados com sucesso ou não.
 */
bool init_all_necessary_allegro_components() {
    if (!al_init()) {
        fprintf(stderr, "Failed to init Allegro5.\n");
        return false;
    }

    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Failed to init Primitives.\n");
        return false;
    }

    if (!al_init_image_addon()) {
        fprintf(stderr, "Failed to init image addon.\n");
        return false;
    }

    return true;
}

/**
 * @brief Instala o mouse e o teclado para receber inputs. 
 * 
 * @return bool indicando se que todos os componentes forem instalados com sucesso ou não.
 */
bool install_all_necessary_allegro_components() {
    if (!al_install_keyboard()) {
        fprintf(stderr, "Falied to install keyboard.\n");
        return false;
    }

    if (!al_install_mouse()) {
        fprintf(stderr, "Failed to install mouse.\n");
        return false;
    }

    return true;
}

