#include "allegro_stuff.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <stdio.h>

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

