#pragma once
#ifndef SOUND_MANGER_H
#define SOUND_MANGER_H

#include <allegro5/allegro_audio.h>
#include <stdbool.h>
#include <limits.h>

/**
 * @brief Enumeração dos sons e músicas disponíveis no game. 
 */
typedef enum SoundID {
    SFX_PLAYER_SHOOT,
    SFX_ALIEN_SHOOT,
    SFX_ALIEN_DIE,
    SFX_PLAYER_HIT,
    PLAYING_BG_MUSIC,
    TITLE_SCREEN,
    SFX_GAME_WIN,
    SFX_HIT_UFO,
    SFX_UFO,
    CALM_MUSIC,
    SFX_GAME_OVER,
} SoundID;

typedef struct SFX {
    ALLEGRO_SAMPLE *sfx;
    char path[PATH_MAX];
    ALLEGRO_SAMPLE_ID id;
} SFX;

typedef struct Music {
    ALLEGRO_AUDIO_STREAM *music;
    char path[PATH_MAX];
} Music;

typedef struct SoundBank {
    SFX *player_shoot;
    SFX *player_hit;
    SFX *alien_shoot;
    SFX *alien_die;
    SFX *ufo_take_off;
    SFX *game_win;
    SFX *hit_ufo;
    Music *playing_bg;
    Music *title_screen;
    Music *calm_music;
    SFX *game_over;
} SoundBank;

bool init_sound_manager();

void play_sound(SoundID id);

void destroy_sound_bank();

void rewind_music(SoundID id);

void play_music(SoundID id);

void remove_music(SoundID id);

void load_sounds();

void rewind_sound(SoundID id);

void remove_sound(SoundID id);

#endif