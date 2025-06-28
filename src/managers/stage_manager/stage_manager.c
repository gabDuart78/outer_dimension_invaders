#include "stage_manager.h"
#include "alien_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_STAGE 8
#define FIRE_INTERVAL 1.2f
#define MOVE_INTERVAL .63f
#define ALIENS_TYPE_AMOUNT 3

typedef struct AlienDistribution {
    AlienType type;
    int rows;
} AlienDistribution;

typedef struct StageConfig {
    AlienDistribution alien_distribution[ALIENS_TYPE_AMOUNT];
    int columns;
} StageConfig;

const StageConfig STAGE0 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 5};

const StageConfig STAGE1 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 6};

const StageConfig STAGE2 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 8};

const StageConfig STAGE3 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 2}}, 
    .columns = 8};

const StageConfig STAGE4 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 2}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 10};

const StageConfig STAGE5 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 3}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

const StageConfig STAGE6 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 2}, 
    {.type = RAGE_ALIEN, .rows = 2}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

const StageConfig STAGE7 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 3}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

const StageConfig STAGE8 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 4}, 
    {.type = RAGE_ALIEN, .rows = 0}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

const StageConfig STAGE9 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 2}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 12};

const StageConfig STAGES[] = {STAGE0, STAGE1, STAGE2, STAGE3, STAGE4, 
    STAGE5, STAGE6, STAGE7, STAGE8, STAGE9};

StageConfig get_stage_config(int current_stage) {
    return STAGES[current_stage];
}

StageManager* create_stage_manager() {
    StageManager *stage_manager = (StageManager *) malloc(sizeof(StageManager));

    if (!stage_manager) {
        fprintf(stderr, "Failed to create stage manager.\n");
        exit(-1);
    }

    return stage_manager;
} 

void init_stage_manager(StageManager *stage_manager) {
    stage_manager->current_stage = 0;
    stage_manager->move_interval_multiplier = ALIENS_MOVE_INTERVAL_MULTIPLIER;
    stage_manager->fire_rate_multiplier = ALIENS_FIRE_RATE_MULTIPLIER;
    stage_manager->stage_cleared = false;
}

void destroy_stage_manager(StageManager *stage_manager) {
    if (!stage_manager) return;
    free(stage_manager); 
}

void fill_aliens_matrix(StageConfig config, AlienManager *manager) {
    int index = 0;
    for (int i = 0; i < ALIENS_TYPE_AMOUNT; i++) {
        for (int j = 0; j < config.alien_distribution[i].rows * manager->columns; j++, index++) {
            manager->aliens[index] = create_alien(get_alien_config(config.alien_distribution[i].type), index, 
            ALIEN_ANIMATION_FRAMES, manager->move_interval);
        }
    }
}

int get_total_rows(StageConfig config) {
    int total = 0;
    for (int i = 0; i < ALIENS_TYPE_AMOUNT; i++) 
        total += config.alien_distribution[i].rows; 
 
  return total;   
}

void start_stage(StageManager *stage_manager, AlienManager *alien_manager) {
    StageConfig config = get_stage_config(stage_manager->current_stage);
    int rows = get_total_rows(config);

    init_alien_manager(alien_manager, rows, config.columns, 
        MOVE_INTERVAL * pow(stage_manager->move_interval_multiplier, stage_manager->current_stage), 
        FIRE_INTERVAL * pow(stage_manager->fire_rate_multiplier, stage_manager->current_stage));

    fill_aliens_matrix(config, alien_manager);
    stage_manager->stage_cleared = false;
}

bool is_stage_cleared(StageManager *stage_manager) {
    return stage_manager->stage_cleared;
}

void check_stage_progress(StageManager *stage_manager, AlienManager *alien_manager) {
    if (all_aliens_dead(alien_manager))
        stage_manager->stage_cleared = true;
}

void next_stage(StageManager *stage_manager) {
    if (stage_manager->current_stage < MAX_STAGE)
        stage_manager->current_stage++;
}

void reset_stage_manager(StageManager *stage_manager) {
    stage_manager->current_stage = 0;
}