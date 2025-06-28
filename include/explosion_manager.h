#pragma once
#ifndef EXPLOSION_MANAGER_H
#define EXPLOSION_MANAGER_H

#include "utils.h"

typedef struct ExplosionManager ExplosionManager;

ExplosionManager* create_explosion_manager();

void init_explosion_manager(ExplosionManager *manager, int max);

void trigger_explosion(ExplosionManager *manager, Rect collider);

void update_explosions(ExplosionManager *manager, double delta_time);

void draw_explosions(ExplosionManager *manager);

void destroy_explosion_manager(ExplosionManager *manager);

#endif