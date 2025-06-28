#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdbool.h>
#include "player_config.h"
#include "screen_config.h"
#include "animator.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>

typedef struct BulletManager BulletManager;

typedef enum PlayerInput{
    INPUT_NONE,
    INPUT_MOVE_LEFT,
    INPUT_MOVE_RIGHT,
    INPUT_STOP_MOVE_RIGHT,
    INPUT_STOP_MOVE_LEFT,
    INPUT_SHOOT,
    INPUT_STOP_SHOOT,
} PlayerInput;

typedef struct Player{
    Point pos;
    int width;
    int height;
    float speed;
    bool move_left;
    bool move_right;
    float acc;
    float acc_step;
    float dec_step;
    float max_acc;   
    float vx;
    BulletManager *bm;
    int max_bullets;
    bool is_shooting;
    int lifes;
    int max_life;
    float fire_interval;
    float last_fire_time;
    ALLEGRO_COLOR color;
    int score;
    bool is_alive;
    ALLEGRO_BITMAP *sprite;
    bool draw_hitbox;
    Animator *animator;
} Player;

Player * create_player(PlayerConfig cfg);

void draw_player(Player *p);

void update_player(Player *p);

void destroy_player(Player *p);

PlayerInput interpret_player_event(ALLEGRO_EVENT *ev);

void handle_player_events(Player *p, PlayerInput input);

void hit_player(Player *p);

void player_hits_enemy(Player *p, int points);

void kill_player(Player *p);
   
#endif