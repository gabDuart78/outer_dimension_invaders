#pragma once
#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <stdbool.h>

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

/**
 * @brief Estrutura responsável por animar os sprites utilizados no game.
 * */
typedef struct Animator {
    int current_frame;
    int frame_count;
    int frame_width;
    int frame_height;
    double frame_duration;
    double last_update;
    bool loop;
} Animator;

void init_animator(Animator *a, int count, int width, int height, double duration, bool loop);

void update_animator(Animator *a);

void draw_animated_sprite(ALLEGRO_BITMAP *sprite_sheet, Animator *a, float x, float y);

void reset_animation(Animator *animator);

void destroy_animator(Animator * animator);

#endif