#pragma once
#ifndef BACKGROUND_MANAGER_H
#define BACKGROUND_MANAGER_H

typedef struct BackgroundManager BackgroundManager;

BackgroundManager* create_background_manager();

void init_background_manager(BackgroundManager *manager);

void load_background(BackgroundManager *manager, const char *path);

void draw_background(BackgroundManager *manager);

void destroy_background_manager(BackgroundManager *manager);

#endif