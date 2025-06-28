#pragma once
#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>

typedef struct ALLEGRO_FONT ALLEGRO_FONT;
typedef struct FontManager FontManager;
    
void handle_menu_inupt(ALLEGRO_EVENT event);

void update_menu();

void draw_menu();

void enter_menu_state();

#endif