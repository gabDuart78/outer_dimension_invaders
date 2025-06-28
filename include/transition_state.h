#pragma once
#ifndef TRANSITION_STATE_H
#define TRANSITION_STATE_H

#include "state_manager.h"

typedef struct FontManager FontManager;

void enter_transition_state(GameState state, double time, bool enter);

void update_transition_state();

void draw_transition();

#endif