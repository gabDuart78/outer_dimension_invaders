#include "score_manager.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELIMITER "|"

HighScoreTable* create_high_score_table() {
    HighScoreTable *table = (HighScoreTable *) malloc(sizeof(HighScoreTable));

    if (!table) {
        fprintf(stderr, "Failed to high score table.\n");
        exit(-1);
    }

    return table;
}

void init_high_score_table(HighScoreTable *table) {
    table->count = 0;
}

void save_score(HighScoreTable *table, const char *name, int score) {
    int index = get_high_score_position(table, score);
    ScoreEntry new_entry = {.score = score};
    strcpy(new_entry.name, name);

    if (table->count < SCORE_TABLE_AMOUNT) 
        table->count++;

    ScoreEntry temp;
    for (int i = table->count - 1; i > index; i--) {
        temp = table->entries[i - 1];
        table->entries[i - 1] = table->entries[i];
        table->entries[i] = temp;
    }
   
    table->entries[index] = new_entry;
}

void save_score_to_file(FILE *stream, ScoreEntry entry) {
    fprintf(stream, "%s|%d\n", entry.name, entry.score);
}

void save_scores_to_file(HighScoreTable *table, const char *file_path) {
    FILE *stream = fopen(file_path, "w");

    if (!stream) {
        fprintf(stderr, "Failed to create file. File path: %s\n", file_path);
        exit(-1);
    }

    for (int i = 0; i < table->count; i++)
        save_score_to_file(stream, table->entries[i]);
}

bool is_valid_name(char *name) {
    return strlen(name) == NAME_LENGTH;
}

void load_scores(HighScoreTable *table, const char *file_path) {
    FILE *stream = fopen(file_path, "r");

    if (!stream)
        return;
    
    char buffer[24];
    for (int i = 0; i < SCORE_TABLE_AMOUNT; i++) {
        if (fgets(buffer, sizeof(buffer), stream) != NULL) {
            char *name = strtok(buffer, DELIMITER);
            char *score = strtok(NULL, DELIMITER);
            strip_newline(score);

            if (name && score && is_valid_name(name) && 
                is_valid_number(score)) 
                save_score(table, name, atoi(score));
        }
    }
}

int get_highest_score(HighScoreTable *table) {
    if (table->count == 0) return 0;

    int highest_score = table->entries[0].score;

    for (int i = 1; i < table->count; i++) {
        if (table->entries[i].score > highest_score) 
            highest_score = table->entries[i].score;
        
    }

    return highest_score;
};

void print_scores(HighScoreTable *table) {
    for (int i = 0; i < table->count; i++) 
        printf("%s %.7d\n", table->entries[i].name, 
            table->entries[i].score);
}

int get_high_score_position(HighScoreTable *table, int new_score) {
    for (int i = 0; i < table->count; i++) 
        if (table->entries[i].score < new_score) 
            return i;
    
    if (new_score > 0 && table->count < SCORE_TABLE_AMOUNT) 
        return table->count;

    return -1;
} 

bool is_eligible_high_score(HighScoreTable *table, int new_score) {
    return get_high_score_position(table, new_score) >= 0;
}

void destroy_high_score_table(HighScoreTable *table) {
    if (table) free(table);
}

bool is_score_table_empty(HighScoreTable *table) {
    return table->count == 0;
}