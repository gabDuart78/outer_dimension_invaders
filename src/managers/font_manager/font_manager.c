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

bool try_load_font(const char *path, int small, int medium, int large) {
    fm.small_font = al_load_ttf_font(path, small, 0);
    fm.medium_font = al_load_ttf_font(path, medium, 0);
    fm.large_font = al_load_ttf_font(path, large, 0);

    if (!fm.small_font || !fm.medium_font || !fm.large_font) {
        fprintf(stderr, "Failed to lond font in path: %s\n", path);
        return false;
    }

    return true;
}

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

void destroy_font_manager() {
    if (fm.small_font) al_destroy_font(fm.small_font);
    if (fm.medium_font) al_destroy_font(fm.medium_font);
    if (fm.large_font) al_destroy_font(fm.large_font);
}

ALLEGRO_FONT* get_small_font() {
    return fm.small_font;
}

ALLEGRO_FONT* get_medium_font() {
    return fm.medium_font;
}

ALLEGRO_FONT* get_large_font() {
    return fm.large_font;
}