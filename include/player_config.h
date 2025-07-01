#ifndef PLAYER_CONFIG_H
#define PLAYER_CONFIG_H

#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"
#include "bullet.h"

/**
 * @brief Estrutura usada para definir configurações para o player.
 */
typedef struct {
    int width;
    int height;
    float speed;
    bool move_left;
    bool move_right;
    float acc;
    float acc_step;
    float dec_step;
    float max_acc;
    float vx;
    bool is_shooting;
    int max_bullets;
    int lifes;
    int max_life;
    float last_fire_time;
    float fire_interval;
    RGB color;
    int score;
    bool is_alive;
    bool draw_hitbox;
    int animation_frames;
    double frame_duration;
    BulletConfig bullet_config;
} PlayerConfig;

extern const PlayerConfig PLAYER_CONFIG;

#endif