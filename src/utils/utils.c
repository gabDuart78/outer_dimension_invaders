#include "utils.h"
#include "screen_config.h"
#include "score_manager.h"
#include "game_context.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Rect get_collider(Point pos, int width, int height) {
    Rect collider = {
        .pos = pos,
        .width = width,
        .height = height,
    };

    return collider;
}

void set_time_seed() {
    srand(time(NULL));
}

int random_integer(int min, int max) {
    return rand() % (max - min + 1) + min;
}

float random_float() {
    return ((float)rand()) / RAND_MAX; 
}

ALLEGRO_BITMAP * get_sprite(const char *path) {
     ALLEGRO_BITMAP * sprite = al_load_bitmap(path);

    if (sprite == NULL) {
        fprintf(stderr, "Unable to load sprite in path: %s.\n", path);
        exit(-1);
    }

    return sprite;
}

bool key_pressed(ALLEGRO_EVENT ev, int key_code) {
    return ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == key_code;
}

bool close_display(ALLEGRO_EVENT event) {
    return event.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

void draw_screen_overlay(RGB color, unsigned char alpha) {
    al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 
        al_map_rgba(color.red, color.green, color.blue, alpha));
}

void trim_space_at_start(char *str) {
    if (*str != ' ') return; 
    for (char *p1 = str, *p2 = str + 1; p2 <= str + strlen(str); p1++, p2++)
        *p1 = *p2;
}

void trim_space_at_end(char *str) {
    char *end = str + strlen(str) - 1;

    if (*end == ' ')
        *end = '\0';
}

void trim_space(char *str) {
    trim_space_at_start(str);
    trim_space_at_end(str);
}

void draw_wrapped_text(ALLEGRO_FONT *font, ALLEGRO_COLOR color, 
                        float x, float y, float max_width, const char *text, int gap) {
    char buffer[1024];
    strcpy(buffer, text);
    char *token = strtok(buffer, " ");
    char line[1024] = "";

    float line_height = al_get_font_line_height(font);
    float current_y = y;

    while(token != NULL) {
        char temp_line[1025];
        snprintf(temp_line, sizeof(temp_line), "%s %s", line, token);

        if (al_get_text_width(font, temp_line) > max_width) {
            trim_space(line);
            al_draw_text(font, color, x, current_y, ALLEGRO_ALIGN_CENTER, line);
            strcpy(line, token);
            current_y += line_height + gap;
        } else {
            strcpy(line, temp_line);
        }

        token = strtok(NULL, " ");
    }

    al_draw_text(font, color, x, current_y, ALLEGRO_ALIGN_CENTER, line);
}

bool file_exists(const char *file_path) {
    FILE *stream = fopen(file_path, "r");

    if(stream) {
        fclose(stream);
        return true;
    }

    return false;
}

bool is_valid_number(char *string) {
    if(!string) return false;

    while(*string != '\0') {
        if (!isdigit(*(string)++)) 
            return false;
    }

    return true;
}

void strip_newline(char *str) {
    if (!str) return;

    char *p = str;

    while(*p) {
        if (*p == '\n' || *p =='\r') {
            *p = '\0';
            break;
        }
        p++;
    }
}

ALLEGRO_COLOR get_color_for_score(int pos) {
    if (pos == 0) 
        return al_map_rgb(230, 30, 200);

    if (pos < 3)
        return al_map_rgb(230, 30, 60);

    return al_map_rgb(120, 230, 30);
}

void draw_score_table(float *current_y, ALLEGRO_FONT *font) {
    HighScoreTable *table = get_score_table();
    float line_height = al_get_font_line_height(font);
    char buffer[24];
    
    *current_y += 50;

    for (int i = 0; i < table->count; i++) {
        sprintf(buffer, "%s %.7d", table->entries[i].name, table->entries[i].score);
         al_draw_text(font, get_color_for_score(i), 
            SCREEN_WIDTH * 0.5, *current_y, ALLEGRO_ALIGN_CENTER, 
            buffer);

        *current_y += line_height * 1.2f;
    }
}

Point get_centered_pos_inside_rect(Rect a, int width, int height) {
    return (Point) {
        .x = a.pos.x + ((a.width - width) / 2),
        .y = a.pos.y + ((a.height - height) / 2),
    };
}