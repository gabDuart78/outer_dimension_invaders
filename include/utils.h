#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <stdlib.h>

typedef enum MoveDir {
    MOVE_RIGHT, 
    MOVE_LEFT, 
    MOVE_UP, 
    MOVE_DOWN,
    NO_MOVE,
} MoveDir;

typedef struct ALLEGRO_BITMAP ALLEGRO_BITMAP;

typedef struct Point {
    float x, y;
} Point;

typedef struct RGB {
    int red, green, blue;
} RGB;

typedef struct Rect {
    Point pos;
    float width, height;
} Rect;

Rect get_collider(Point pos, int width, int height);

float clamp(float value, float min, float max);

int random_integer(int min, int max);

float random_float();

void set_time_seed();

ALLEGRO_BITMAP *get_sprite(const char *path);

bool key_pressed(ALLEGRO_EVENT ev, int key_code);

bool close_display(ALLEGRO_EVENT event);

void draw_screen_overlay(RGB color, unsigned char alpha);

void draw_wrapped_text(ALLEGRO_FONT *font, ALLEGRO_COLOR color, 
                        float x, float y, float max_width, const char *text, int gap);

bool file_exists(const char *file_path);

bool is_valid_number(char *string);

void strip_newline(char *str);

void draw_score_table(float *current_y, ALLEGRO_FONT *font);

Point get_centered_pos_inside_rect(Rect a, int width, int height);

#endif