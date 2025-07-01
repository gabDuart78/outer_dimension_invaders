#include "game_context.h"

/**
 * @brief Estrutura utilizada para compartilhar informações entre os estados do game
 * como o score do player, o estado do game, o gerenciador de backgrounds e de estágios,
 * etc.
 */
typedef struct GameContext {
    int player_score;
    bool is_game_over;
    bool player_win;
    HighScoreTable *score_table;
    BackgroundManager *bg_manager;
    StageManager *stage_manager;
} GameContext;

static GameContext gc;

/**
 * @brief Cria e inicializa uma ScoreTable, além de corregar os scores salvos se houver. 
 */
void set_up_scores_table() {
    gc.score_table = create_high_score_table();
    init_high_score_table(gc.score_table);
    load_scores(gc.score_table, SCORES_PATH);
}

/**
 * @brief Define alumas variáveis importantes que serão utilizadas entres os estado do game. 
 */
void set_up_game_variables() {
    gc.player_score = 0;
    gc.is_game_over = false;
    gc.player_win = false;
}

/**
 * @brief Cria e incializa o gerenciador de estágios.
 */
void set_up_stage_manager() {
    gc.stage_manager = create_stage_manager();
    init_stage_manager(gc.stage_manager);
}

/**
 * @brief Cria e inicializa o gerenciador de backgrounds.
 */
void set_up_background_manager() {
    gc.bg_manager = create_background_manager();
    init_background_manager(gc.bg_manager);
}

/**
 * @brief Inicializa a estrura GameContext.
 */
void init_game_context() {
    set_up_scores_table();
    set_up_background_manager();
    set_up_game_variables();
    set_up_stage_manager();
}

/**
 * @brief Define o score do player.
 * 
 * @param score Player score.
 */
void set_player_score(int score) {
    gc.player_score = score;
}

/**
 * @brief Define o estado lógico do game, se o jogo acabou ou não.
 * 
 * @param is_game_over Indicando se o jogo acabou ou não.
 */
void set_game_over(bool is_game_over) {
    gc.is_game_over = is_game_over;
} 

/**
 * @brief Define se o player ganhou ou não.
 * 
 * @param has_player_win Indicando se o player ganhou ou não.
 */
void set_player_win(bool has_player_win) {
    gc.player_win = has_player_win;
}

/**
 * @brief Retorna o gerenciador de backgrounds.
 * 
 * @return BackgroundManager.
 */
BackgroundManager* get_background_manager() {
    return gc.bg_manager;
}

/**
 * @brief Retorna uma ScoreTable contendo os maiores scores.
 * 
 * @return HighScoreTable.
 */
HighScoreTable* get_score_table() {
    return gc.score_table;
}

/**
 * @brief Retorna o gerenciador de estágios.
 * 
 * @return StageManager.
 */
StageManager* get_stage_manager() {
    return gc.stage_manager;
}

/**
 * @brief Retorna o score do player.
 * 
 * @return Score do player.
 */
int get_player_score() {
    return gc.player_score;
}

/**
 * @brief Retorna o estado lógico do game.
 * 
 * @return Bool Indicando se o jogo acabou ou não.
 */
bool is_game_over() {
    return gc.is_game_over;
}

/**
 * @brief Retorna o resultado da partida, se o player ganhou ou não.
 * 
 * @return Bool Indicando se o player ganhou ou não.
 */
bool has_player_win() {
    return gc.player_win;
}

/**
 * @brief Reseta o score do player, define tanto o estado lógico do game quanto 
 * se o player ganhou como falso e reseta o gerenciador de estágios.
 */
void reset_game_context() {
    gc.player_score = 0;
    gc.is_game_over = false;
    gc.player_win = false;
    reset_stage_manager(gc.stage_manager);
}

/**
 * @brief Libera os recursos utilizados pelo GameContext.
 */
void destroy_game_context() {
    if (gc.score_table) destroy_high_score_table(gc.score_table);
    if (gc.bg_manager) destroy_background_manager(gc.bg_manager);
    if (gc.stage_manager) destroy_stage_manager(gc.stage_manager);
}