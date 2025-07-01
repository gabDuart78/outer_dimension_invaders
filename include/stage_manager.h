#pragma once
#ifndef STAGE_MANGER_H
#define STAGE_MANGER_H

#include <stdbool.h>

typedef struct AlienManager AlienManager;

typedef struct StageManager {
    int current_stage;
    float move_interval_multiplier;
    float fire_rate_multiplier;
    bool stage_cleared;
} StageManager;

StageManager* create_stage_manager();
   
void init_stage_manager(StageManager *stage_manager);

void destroy_stage_manager(StageManager *stage_manager);

void start_stage(StageManager *stage_manager, AlienManager *alien_manager);

bool check_stage_progress(StageManager *stage_manager, AlienManager *alien_manager);

void next_stage(StageManager *stage_manager);

void reset_stage_manager(StageManager *stage_manager);

#endif