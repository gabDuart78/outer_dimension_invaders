#include "stage_manager.h"
#include "alien_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_STAGE 9
#define FIRE_INTERVAL 1.2f
#define MOVE_INTERVAL .63f
#define ALIENS_TYPE_AMOUNT 3

/**
 * @brief Estrutura usada para representar a distribuição de um alien
 * na matrix aliens.
 */
typedef struct AlienDistribution {
    AlienType type;
    int rows;
} AlienDistribution;

/**
 * @brief Estrutura usada para representar a configuração de um estágio, quantidade de colunas e
 * de aliens em cada row, além dos tipos de aliens usados em cada linha.
 */
typedef struct StageConfig {
    AlienDistribution alien_distribution[ALIENS_TYPE_AMOUNT];
    int columns;
} StageConfig;

// Configuração estágio 0
const StageConfig STAGE0 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 5};

// Configuração estágio 1
const StageConfig STAGE1 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 6};

// Configuração estágio 2
const StageConfig STAGE2 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 8};

// Configuração estágio 3
const StageConfig STAGE3 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 2}}, 
    .columns = 8};

// Configuração estágio 4
const StageConfig STAGE4 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 2}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 10};

// Configuração estágio 5
const StageConfig STAGE5 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 3}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

// Configuração estágio 6
const StageConfig STAGE6 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 2}, 
    {.type = RAGE_ALIEN, .rows = 2}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

// Configuração estágio 7
const StageConfig STAGE7 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 3}, 
    {.type = RAGE_ALIEN, .rows = 1}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

// Configuração estágio 8
const StageConfig STAGE8 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 4}, 
    {.type = RAGE_ALIEN, .rows = 0}, {.type = SPOOKY_ALIEN, .rows = 0}}, 
    .columns = 12};

// Configuração estágio 9
const StageConfig STAGE9 = {.alien_distribution = {{.type = TOXIC_ALIEN, .rows = 1}, 
    {.type = RAGE_ALIEN, .rows = 2}, {.type = SPOOKY_ALIEN, .rows = 1}}, 
    .columns = 13};

const StageConfig STAGES[] = {STAGE0, STAGE1, STAGE2, STAGE3, STAGE4, 
    STAGE5, STAGE6, STAGE7, STAGE8, STAGE9};

/**
 * @brief Retorna a configuração do estágio desejado.
 * 
 * @param stage Representa o estágio desejado.
 * 
 * @return StageConfig configuração do estágio correspondente.
 */
StageConfig get_stage_config(int stage) {
    return STAGES[stage];
}

/**
 * @brief Cria um StageManager utilizado para gerenciar os estágios do game.
 * 
 * @return Ponteiro para o StageManager.
 */
StageManager* create_stage_manager() {
    StageManager *stage_manager = (StageManager *) malloc(sizeof(StageManager));

    if (!stage_manager) {
        fprintf(stderr, "Failed to create stage manager.\n");
        exit(-1);
    }

    return stage_manager;
} 

/**
 * @brief Inicializa o StageManager.
 * 
 * @param stage_manager Ponteiro para um StageManager.
 */
void init_stage_manager(StageManager *stage_manager) {
    stage_manager->current_stage = 0;
    stage_manager->move_interval_multiplier = ALIENS_MOVE_INTERVAL_MULTIPLIER;
    stage_manager->fire_rate_multiplier = ALIENS_FIRE_RATE_MULTIPLIER;
    stage_manager->stage_cleared = false;
}

/**
 * @brief Libera os recursos utilizados pelo StageManager.
 * 
 * @param stage_manager Ponteiro para um StageManager.
 */
void destroy_stage_manager(StageManager *stage_manager) {
    if (!stage_manager) return;
    free(stage_manager); 
}

/**
 * @brief Preenche a o vetor de aliens do AlienManager de acordo com a configuração recebida.
 * 
 * @param config Um StageConfig representando a configuração pretendida.
 * @param alien_manager Ponteiro para um AlienManager.
 */
void distribute_aliens(StageConfig config, AlienManager *alien_manager) {
    int index = 0;
    for (int i = 0; i < ALIENS_TYPE_AMOUNT; i++) {
        for (int j = 0; j < config.alien_distribution[i].rows * alien_manager->columns; j++, index++) {
            alien_manager->aliens[index] = create_alien(get_alien_config(config.alien_distribution[i].type), index, 
            ALIEN_ANIMATION_FRAMES, alien_manager->move_interval);
        }
    }
}

/**
 * @brief Retorna o número total de linhas de um StageConfig.
 * 
 * @param config Um StageConfig representando a configuração pretendida.
 * 
 * @return O número total de linhas da configuração fornecida.
 */
int get_total_rows(StageConfig config) {
    int total = 0;
    for (int i = 0; i < ALIENS_TYPE_AMOUNT; i++) 
        total += config.alien_distribution[i].rows; 
 
  return total;   
}

/**
 * @brief Inicia o estágio posicionando os aliens no AlienManager de acordo com o estágio atual.
 * 
 * @param stage_manager Ponteiro para um StageManager.
 * @param alien_manager Ponteiro para um AlienManager.
 */
void start_stage(StageManager *stage_manager, AlienManager *alien_manager) {
    StageConfig config = get_stage_config(stage_manager->current_stage);
    int rows = get_total_rows(config);

    init_alien_manager(alien_manager, rows, config.columns, 
        MOVE_INTERVAL * pow(stage_manager->move_interval_multiplier, stage_manager->current_stage), 
        FIRE_INTERVAL * pow(stage_manager->fire_rate_multiplier, stage_manager->current_stage));

    distribute_aliens(config, alien_manager);
    stage_manager->stage_cleared = false;
}

/**
 * @brief Verifica se todos os aliens foram mortos, se sim define o estágio como completo.
 * 
 * @param stage_manager Ponteiro para um StageManager.
 * @param alien_manager Ponteiro para um AlienManager.
 * 
 * @return Bool indicando se o estágio esta completo.
 */
bool check_stage_progress(StageManager *stage_manager, AlienManager *alien_manager) {
    bool cleared = all_aliens_dead(alien_manager);
    stage_manager->stage_cleared = cleared;
    return cleared;
        
}

/**
 * @brief Se o estágio atual não for o último avança um estágio.
 * 
 * @param stage_manager Ponteiro para um StageManager.
 */
void next_stage(StageManager *stage_manager) {
    if (stage_manager->current_stage < MAX_STAGE)
        stage_manager->current_stage++;
}

/**
 * @brief Reseta o StageManager para o primeiro estágio.
 * 
 * @param stage_manager Ponteiro para um StageManager.
 */
void reset_stage_manager(StageManager *stage_manager) {
    stage_manager->current_stage = 0;
}