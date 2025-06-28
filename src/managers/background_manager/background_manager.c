#include "background_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

typedef struct BackgroundManager {
    ALLEGRO_BITMAP *background;
} BackgroundManager;    

BackgroundManager* create_background_manager() {
    BackgroundManager *bg_manager = (BackgroundManager *) malloc(sizeof(BackgroundManager));

    if (!bg_manager) {
        fprintf(stderr, "Failed to create Background Manager.\n");
        exit(-1);
    }

    return bg_manager;
}

void init_background_manager(BackgroundManager *manager) {
    manager->background = NULL;
}

void destroy_background(ALLEGRO_BITMAP *background) {
    if (!background) return;

    al_destroy_bitmap(background);
}

void load_background(BackgroundManager *manager, const char *path) {
    if (manager->background) 
        destroy_background(manager->background);

    manager->background = al_load_bitmap(path);

    if (!manager->background) {
        fprintf(stderr, "Failed to load background at path: %s\n", path);
        exit(-1);
    }
}

void draw_background(BackgroundManager *manager) {
    if (manager->background)
        al_draw_bitmap(manager->background, 0, 0, 0);
}

void destroy_background_manager(BackgroundManager *manager) {
    if (!manager) return;
    if (manager->background)
        destroy_background(manager->background);
    free(manager);
}