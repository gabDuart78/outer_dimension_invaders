#pragma once
#ifndef UI_H
#define UI_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "utils.h"

typedef struct UI {
    ALLEGRO_COLOR text_color;
    ALLEGRO_FONT *font;
    Rect wrapper;
    ALLEGRO_BITMAP *life_icon_active;
    ALLEGRO_BITMAP *life_icon_deactive;   
} UI;

void init_ui(); 
void draw_ui(int max_lifes, int lifes, int player_score);
void destroy_ui();

#endif