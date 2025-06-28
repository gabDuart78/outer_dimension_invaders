#pragma once
#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <stdbool.h>

typedef struct ALLEGRO_FONT ALLEGRO_FONT;

typedef struct FontManager {
    ALLEGRO_FONT *small_font;
    ALLEGRO_FONT *medium_font;
    ALLEGRO_FONT *large_font;
} FontManager;

bool init_font_manager();
void destroy_font_manager();

ALLEGRO_FONT *get_small_font();
ALLEGRO_FONT *get_medium_font();
ALLEGRO_FONT *get_large_font();

#endif