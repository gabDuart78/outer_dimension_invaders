#include "animator.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>

/**
 * @brief Inicializa um objeto Animator com os parâmetros fornecidos.
 * 
 * @param animator Ponteiro para o Animator a ser inicializado.
 * @param count Número de quadros da animação.
 * @param width Largura de cada quadro.
 * @param height Altura de cada quadro.
 * @param duration Tempo de exibição de cada quadro (em segundos).
 * @param loop Define se a animação deve reiniciar ao final.
 */
void init_animator(Animator *animator, int count, int width, int height, double duration, bool loop) {
    animator->current_frame = 0;
    animator->frame_count = count;
    animator->frame_width = width;
    animator->frame_height = height;
    animator->frame_duration = duration;
    animator->loop = loop;
    animator->last_update = 0;
}

/**
 * @brief Atualiza o quadro atual da animação com base no tempo.
 * 
 * @param animator Ponteiro para o Animator a ser atualizado.
 */
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

/**
 * @brief Desenha o quadro atual da animação na tela.
 * 
 * @param sprite_sheet Bitmap contendo os quadros da animação.
 * @param animator Ponteiro para o Animator.
 * @param x Posição horizontal.
 * @param y Posição vertical.
 */
void draw_animated_sprite(ALLEGRO_BITMAP *sprite_sheet, Animator *animator, float x, float y) {
    al_draw_bitmap_region(
        sprite_sheet, 
        animator->current_frame * animator->frame_width,
        0,
        animator->frame_width,
        animator->frame_height, 
        x, y, 0);
}

/**
 * @brief Libera a memória alocada para o Animator.
 * 
 * @param animator Ponteiro para o Animator a ser destruído.
 */
void destroy_animator(Animator * animator) {
    free(animator);
}

/**
 * @brief Reinicia a animação para o primeiro quadro.
 * 
 * @param animator Ponteiro para o Animator.
 */
void reset_animation(Animator *animator) {
    animator->current_frame = 0;
}