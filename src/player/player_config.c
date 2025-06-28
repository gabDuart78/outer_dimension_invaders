#include "player_config.h"

const BulletConfig PLAYER_BULLET_CONFIG = {
    .width = 8.0f,
    .height = 19.0f,
    .speed = 12.0f,
    .move_dir = MOVE_UP,
    .is_active = false,
    .color = (RGB) {.red = 0, .green = 255, .blue = 0}, 
};

const PlayerConfig PLAYER_CONFIG = {
    .width = 50,
    .height = 50,
    .speed = 9.0f,
    .acc = .0f,
    .acc_step = 0.1f,
    .dec_step = 0.05f,
    .max_acc = 1.0f,
    .vx = .0f,
    .is_shooting = false,
    .move_left = false,
    .move_right = false,
    .max_bullets = 1,
    .lifes = 3,
    .max_life = 3,
    .fire_interval = 0.0f,
    .color = (RGB) {
        .red = 255,
        .green = 255,
        .blue = 255,
    },
    .score = 0,
    .is_alive = true,
    .draw_hitbox = false,
    .animation_frames = 2,
    .frame_duration = 1.f / 7.f, 
    .bullet_config = PLAYER_BULLET_CONFIG,
};


