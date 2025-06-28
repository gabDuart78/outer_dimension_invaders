#include "animator.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>

void init_animator(Animator *animator, int count, int width, int height, double duration, bool loop) {
    animator->current_frame = 0;
    animator->frame_count = count;
    animator->frame_width = width;
    animator->frame_height = height;
    animator->frame_duration = duration;
    animator->loop = loop;
    animator->last_update = 0;
}

void update_animator(Animator *animator) {
    double now = al_get_time();
    double delta_time = now - animator->last_update;

    if (delta_time >= animator->frame_duration) {
         animator->current_frame++;

        if (animator->current_frame == animator->frame_count) 
            animator->current_frame = animator->loop ? 0 : animator->current_frame - 1;
        
        animator->last_update = now;
    }

}

void draw_animated_sprite(ALLEGRO_BITMAP *sprite_sheet, Animator *animator, float x, float y) {
    al_draw_bitmap_region(
        sprite_sheet, 
        animator->current_frame * animator->frame_width,
        0,
        animator->frame_width,
        animator->frame_height, 
        x, y, 0);
}

void destroy_animator(Animator * animator) {
    free(animator);
}

void reset_animation(Animator *animator) {
    animator->current_frame = 0;
}