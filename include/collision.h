#pragma once
#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>

typedef struct Rect Rect;
typedef struct Player Player;
typedef struct AlienManager AlienManager;
typedef struct UFO UFO;

bool check_collision (Rect a, Rect b);

void handle_collisions(Player *player, AlienManager *alien_manager, UFO *ufo);

#endif