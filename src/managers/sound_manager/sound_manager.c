#include "sound_manager.h"
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#define PLAYER_BULLET_SFX "../assets/sounds/laser_gun.wav"
#define PLAYER_DIE_SFX "../assets/sounds/player_die.wav"
#define PLAYER_HIT_SFX "../assets/sounds/retro_hit.wav"
#define ALIEN_BULLET_SFX "../assets/sounds/laser1.wav"
#define ALIEN_DIE_SFX "../assets/sounds/retro_die.wav"
#define UFO_TAKE_OFF_SFX "../assets/sounds/ufo_take_off.wav"
#define GAME_OVER_SFX "../assets/sounds/game_over_arcade.wav"
#define GAME_WIN_SFX "../assets/sounds/game_win.wav"
#define HIT_UFO_SFX "../assets/sounds/hit_ufo.wav"

#define BG_MUSIC "../assets/music/main_game.ogg"
#define TITLE_SCREEN_MUSIC "../assets/music/title_screen_synthwave.wav"
#define CALM_MUIC "../assets/music/calm_music.ogg"

#define SFX_GAIN .8
#define MUSIC_GAIN .4

#define MUSIC_BUFFERS 4
#define MUSIC_BUFFERS_SIZE 2048
#define RESERVE_SAMPLES 16

// TODO
// Refatorar completamente o sound manager
// Foco em funções sensíveis como rewind e remove
// Falta de consistência SoundID usado tanto para musics e sfxs
// Switch case muito grande pode ser simplificado usado arrays

static SoundBank sb;

/**
 * @brief Inicializa os componentes necessários da biblioteca allegro para o sistema de som.
 * 
 * @return Bool indicando se todos os componentes foram inicializados corretamente.
 */
bool init_sound_manager() {
    if (!al_install_audio()) {
        fprintf(stderr, "Failed to install audio.\n");
        return false;
    }

    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Failed to init acodec addon.\n");
        return false;
    }


    if (!al_reserve_samples(RESERVE_SAMPLES)) {
        fprintf(stderr, "Failed to reserve samples.\n");
        return false;
    }

    return true;
}

/**
 * @brief Carrega um sound effect.
 * 
 * @param sample_path Caminho para o sound effect.
 * 
 * @return Ponteiro para SFX. 
 */
SFX* load_sound(char *sample_path) {
    SFX *sfx_wrapper = (SFX *) malloc(sizeof(SFX));

    if (!sfx_wrapper) {
        fprintf(stderr, "Failed to create sfx wrapper.\n");
        exit(-1);
    }

    sfx_wrapper->sfx = al_load_sample(sample_path);

    if (!sfx_wrapper->sfx) {
        fprintf(stderr, "Unable to load sample in path %s.\n", sample_path);
        exit(-1);
    }

    strcpy(sfx_wrapper->path, sample_path);

    return sfx_wrapper;
}

/**
 * @brief Carrega um música.
 * 
 * @param music_path Caminho para uma música.
 * 
 * @return Ponteiro para Music. 
 */
Music* load_music(char *music_path) {
    Music *music_wrapper = (Music *) malloc(sizeof(Music));

    if (!music_wrapper) {
        fprintf(stderr, "Failed to create music wrapper.\n");
        exit(-1);
    } 

    music_wrapper->music = al_load_audio_stream(
        music_path, MUSIC_BUFFERS, MUSIC_BUFFERS_SIZE);

    if (!music_wrapper->music) {
        fprintf(stderr, "Unable to load music in path %s.\n", music_path);
        exit(-1);
    }

    strcpy(music_wrapper->path, music_path);

    return music_wrapper;
}

/**
 * @brief Carrega todas os sons e músicas utilizados no game.
 */
void load_sounds() {
    sb.player_shoot = load_sound(PLAYER_BULLET_SFX);
    sb.alien_shoot = load_sound(ALIEN_BULLET_SFX);
    sb.alien_die = load_sound(ALIEN_DIE_SFX);
    sb.player_hit = load_sound(PLAYER_HIT_SFX);
    sb.ufo_take_off = load_sound(UFO_TAKE_OFF_SFX);
    sb.game_win = load_sound(GAME_WIN_SFX);
    sb.hit_ufo = load_sound(HIT_UFO_SFX);
    sb.game_over = load_sound(GAME_OVER_SFX);
    sb.calm_music = load_music(CALM_MUIC);
    sb.playing_bg = load_music(BG_MUSIC);
    sb.title_screen = load_music(TITLE_SCREEN_MUSIC);
}

/**
 * @brief Retorna o sound effect correspondete ao id passado.
 * 
 * @param id SoundID identificação de um efeito sonoro.
 * 
 * @return Ponteiro para SFX desejado.
 */
SFX* get_sfx(SoundID id) {
    switch(id) {
        case SFX_PLAYER_SHOOT:
            return sb.player_shoot;
        case SFX_PLAYER_HIT:
            return sb.player_hit;
        case SFX_ALIEN_SHOOT:
            return sb.alien_shoot;
        case    SFX_ALIEN_DIE:
            return sb.alien_die;
        case SFX_UFO:
            return sb.ufo_take_off;
        case SFX_GAME_WIN:
            return sb.game_win;
        case SFX_HIT_UFO:
            return sb.hit_ufo;
        case SFX_GAME_OVER:
            return sb.game_over;
        default:
            return NULL;
    }
}

/**
 * @brief Retorna o música correspondete ao id passado.
 * 
 * @param id SoundID identificação de uma música sonoro.
 * 
 * @return Ponteiro para Music desejada..
 */
Music* get_music(SoundID id) {
    switch(id) {
        case PLAYING_BG_MUSIC:
            return sb.playing_bg;
        case TITLE_SCREEN:
            return sb.title_screen;
        case CALM_MUSIC:
            return sb.calm_music;
        default:
            return NULL;
    }
}

/**
 * @brief Utiliza al_play_sample para tocar um efeito sonoro.
 * 
 * @param id Um SoundID para o efeito sonoro desejado.
 */
void play_sound(SoundID id) {
    SFX *sfx_wrapper = get_sfx(id);
    al_play_sample(sfx_wrapper->sfx, SFX_GAIN, 0, 1.0, 
        ALLEGRO_PLAYMODE_ONCE, &sfx_wrapper->id);
}

/**
 * @brief Utiliza algumas funções da biblioteca allegro para colocar música no mixer e reproduzí-la.
 * 
 * @param id Um SoundID para a música desejada.
 */
void play_music(SoundID id) {
    Music *music_wrapper = get_music(id);
    al_attach_audio_stream_to_mixer(music_wrapper->music, al_get_default_mixer());
    al_set_audio_stream_playmode(music_wrapper->music, ALLEGRO_PLAYMODE_LOOP);
    al_set_audio_stream_gain(music_wrapper->music, MUSIC_GAIN);
}

/**
 * @brief Libera os recursos utilizados pelo ALLEGRO_SAMPLE.
 * 
 * @param sfx Ponteiro para um ALLEGRO_SAMPLE.
 */
void destroy_sample(ALLEGRO_SAMPLE *sfx) {
    if (!sfx) return;

    al_destroy_sample(sfx);
}

/**
 * @brief Libera os recursos utilizados pelo ALLEGRO_AUDIO_STREAM.
 * 
 * @param music Ponteiro para um ALLEGRO_AUDIO_STREAM.
 */
void destroy_audio_stream(ALLEGRO_AUDIO_STREAM *music) {
    if (!music) return;

    al_destroy_audio_stream(music);
}

/**
 * @brief Reseta uma música para o início.
 * 
 * @param id Um SoundID para a música desejada. 
 */
void rewind_music(SoundID id) {
    Music *music_wrapper = get_music(id);

    destroy_audio_stream(music_wrapper->music);

    music_wrapper->music = al_load_audio_stream(
        music_wrapper->path, MUSIC_BUFFERS, MUSIC_BUFFERS_SIZE);

    if (!music_wrapper->music) {
        fprintf(stderr, "Failed to load music in path %s.\n", music_wrapper->path);
        exit(-1);
    }
}

/**
 * @brief Para a repodução de uma efeito sonoro.
 * 
 * @param id Um SoundID para o efeito sonoro desejado. 
 */
void remove_sound(SoundID id) {
    al_stop_sample(&get_sfx(id)->id);
} 

/**
 * @brief Reseta um efeito sonoro para o início.
 * 
 * @param id Um SoundID para o efeito sonoro desejado. 
 */
void rewind_sound(SoundID id) {
    SFX *sfx_wrapper = get_sfx(id);

    destroy_sample(sfx_wrapper->sfx);

    sfx_wrapper->sfx = al_load_sample(sfx_wrapper->path);

    if (!sfx_wrapper->sfx) {
        fprintf(stderr, "Failed to load sfx in path %s.\n", sfx_wrapper->path);
        exit(-1);
    }
}

/**
 * @brief Para a repodução de uma música.
 * 
 * @param id Um SoundID para a música desejada. 
 */
void remove_music(SoundID id) {
    al_detach_audio_stream(get_music(id)->music);
}

/**
 * @brief Libera os recursos utilizados por SFX.
 * 
 * @param sfx_wrapper Um SFX, envoltório para um allegro_sample.
 */
void destroy_sfx_wrapper(SFX *sfx_wrapper) {
    if (!sfx_wrapper) return;

    destroy_sample(sfx_wrapper->sfx);
    free(sfx_wrapper);
}

/**
 * @brief Libera os recursos utilizados por MUSIC.
 * 
 * @param music_wrapper Um MUSIC, envoltório para um allegro_audio_stream.
 */
void destroy_music_wrapper(Music *music_wrapper) {
    if (!music_wrapper) return;

    destroy_audio_stream(music_wrapper->music);
    free(music_wrapper);
}

/**
 * @brief Libera todos os recurosos utilizados pelo SoundBank.
 */
void destroy_sound_bank() {
    destroy_sfx_wrapper(sb.player_shoot);
    destroy_sfx_wrapper(sb.player_hit);
    destroy_sfx_wrapper(sb.alien_shoot);
    destroy_sfx_wrapper(sb.alien_die);
    destroy_sfx_wrapper(sb.game_over);
    destroy_music_wrapper(sb.playing_bg);
    destroy_music_wrapper(sb.title_screen);
    destroy_music_wrapper(sb.calm_music);
}
