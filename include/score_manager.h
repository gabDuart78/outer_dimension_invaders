#pragma once
#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <stdbool.h>

#define SCORE_TABLE_AMOUNT 5
#define NAME_LENGTH 3
#define SCORES_PATH "../scores/scores.dat"

/**
 * @brief Estrutura que representa um score.
 */
typedef struct ScoreEntry {
    int score;
    char name[NAME_LENGTH + 1];
} ScoreEntry;

/**
 * @brief Estrutura usada para armazenar scores. 
 */
typedef struct HighScoreTable {
    ScoreEntry entries[SCORE_TABLE_AMOUNT];
    int count;
} HighScoreTable;

HighScoreTable* create_high_score_table();

bool is_valid_name(char *name);

void init_high_score_table(HighScoreTable *table);

void save_score(HighScoreTable *table, const char *name, int score);

void save_scores_to_file(HighScoreTable *table, const char *file_path);

void print_scores(HighScoreTable *table);

void load_scores(HighScoreTable *table, const char *file_path);

int get_highest_score(HighScoreTable *table);

int get_high_score_position(HighScoreTable *table, int new_score);

bool is_eligible_high_score(HighScoreTable *table, int new_score);

void destroy_high_score_table(HighScoreTable *table);

bool is_score_table_empty(HighScoreTable *table);

#endif