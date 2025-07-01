#include "game_stuff.h"
#include "sound_manager.h"
#include "font_manager.h"
#include <stdio.h>

/**
 * @ Inicializa o gerenciador de souns e de 
 * fontes necessários ao game.
 * 
 * @return bool indicando se todos os componentes foram inicializados com sucesso ou não.
 */

bool init_game_components() {
    if (!init_sound_manager()) {
        fprintf(stderr, "Failed to init sound manager.\n");
        return false;
    }

    if(!init_font_manager()) {
        fprintf(stderr, "Failed to init font manager.\n");
        return false;
    }

    return true;
}