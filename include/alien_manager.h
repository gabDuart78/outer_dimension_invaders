#pragma once
#ifndef ALIEN_MANAGER_H
#define ALIEN_MANAGER_H

#define ALIEN_ANIMATION_FRAMES 2
#define ALIENS_MOVE_INTERVAL_MULTIPLIER .85
#define ALIENS_FIRE_RATE_MULTIPLIER .8

#include <stdbool.h>
#include "utils.h"
#include "alien.h"

typedef struct BulletManager BulletManager;
typedef struct Alien Alien;

typedef enum AlienType {
    TOXIC_ALIEN,
    RAGE_ALIEN,
    SPOOKY_ALIEN,
} AlienType;

typedef struct AlienManager {
    Alien *aliens;
    BulletManager *bm;
    int count;
    int rows;
    int columns;
    MoveDir mov_dir;
    int alives;
    double move_interval;
    double last_move_time;
    float group_width;
    float fire_probability;
    float fire_interval;
    float last_fire_time;
} AlienManager;

AlienManager *create_alien_manager();

void init_alien_manager(AlienManager *manager, int rows, int columns, float move_interval, 
    float fire_interval);

void destroy_alien_manager(AlienManager *manager);

void spawn_aliens(AlienManager *manager);

void update_aliens(AlienManager *manager);

bool all_aliens_dead(AlienManager *manager);

bool aliens_crossed_threshold(AlienManager *manager, float danger_line_y);

void kill_alien_by_id(AlienManager *manager, int id);

void draw_aliens(AlienManager *manager);

AlienConfig get_alien_config(AlienType type);

#endif