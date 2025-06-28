#include "game_context.h"

typedef struct GameContext {
    int player_score;
    bool is_game_over;
    bool player_win;
    HighScoreTable *score_table;
    BackgroundManager *bg_manager;
    StageManager *stage_manager;
} GameContext;

static GameContext gc;

void set_up_scores_table() {
    gc.score_table = create_high_score_table();
    init_high_score_table(gc.score_table);
    load_scores(gc.score_table, SCORES_PATH);
}

void set_up_game_variables() {
    gc.player_score = 0;
    gc.is_game_over = false;
    gc.player_win = false;
}

void set_up_stage_manager() {
    gc.stage_manager = create_stage_manager();
    init_stage_manager(gc.stage_manager);
}

void set_up_background_manager() {
    gc.bg_manager = create_background_manager();
    init_background_manager(gc.bg_manager);
}

void init_game_context() {
    set_up_scores_table();
    set_up_background_manager();
    set_up_game_variables();
    set_up_stage_manager();
}

void set_player_score(int score) {
    gc.player_score = score;
}

void set_game_over(bool is_game_over) {
    gc.is_game_over = is_game_over;
} 

void set_player_win(bool has_player_win) {
    gc.player_win = has_player_win;
}

BackgroundManager* get_background_manager() {
    return gc.bg_manager;
}

HighScoreTable* get_score_table() {
    return gc.score_table;
}

StageManager* get_stage_manager() {
    return gc.stage_manager;
}

int get_player_score() {
    return gc.player_score;
}

bool is_game_over() {
    return gc.is_game_over;
}

bool has_player_win() {
    return gc.player_win;
}

void reset_game_context() {
    gc.player_score = 0;
    gc.is_game_over = false;
    gc.player_win = false;
    reset_stage_manager(gc.stage_manager);
}

void destroy_game_context() {
    if (gc.score_table) destroy_high_score_table(gc.score_table);
    if (gc.bg_manager) destroy_background_manager(gc.bg_manager);
    if (gc.stage_manager) destroy_stage_manager(gc.stage_manager);
}