#include "game_stuff.h"
#include "sound_manager.h"
#include "font_manager.h"
#include <stdio.h>

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