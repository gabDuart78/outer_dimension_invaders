#include "score_manager.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DELIMITER "|"

/**
 * @brief Alloca memória para uma ScoreTable.
 * 
 * @return HighScoreTable.
 */
HighScoreTable* create_high_score_table() {
    HighScoreTable *table = (HighScoreTable *) malloc(sizeof(HighScoreTable));

    if (!table) {
        fprintf(stderr, "Failed to create high score table.\n");
        exit(-1);
    }

    return table;
}

/**
 * @brief Inicializa a ScoreTable.
 * 
 * @param table Ponteiro para a ScoreTable.
 */
void init_high_score_table(HighScoreTable *table) {
    table->count = 0;
}

/**
 * @brief Salva um score na ScoreTable.
 * 
 * @param table Ponteiro para a ScoreTable
 * @param name Nome do responsável pelo score.
 * @param score Pontuação conquistada.
 */
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

/**
 * @brief Salva um score em um aquivo.
 * 
 * @param stream Ponteiro para um arquivo.
 * @param entry Estrura que representa um score.
 */
void save_score_to_file(FILE *stream, ScoreEntry entry) {
    fprintf(stream, "%s|%d\n", entry.name, entry.score);
}

/**
 * @brief Salva todos os scores de uma ScoreTable em um arquivo.
 * 
 * @param table Ponteiro para uma ScoreTable.
 * @param file_path Caminho para o arquivo em que será salvo os scores.
 */
void save_scores_to_file(HighScoreTable *table, const char *file_path) {
    FILE *stream = fopen(file_path, "w");

    if (!stream) {
        fprintf(stderr, "Failed to create file. File path: %s\n", file_path);
        exit(-1);
    }

    for (int i = 0; i < table->count; i++)
        save_score_to_file(stream, table->entries[i]);
}

/**
 * @brief Verifica se uma nome é valido, ou seja, se possui a quantidade certa de caracteres.
 * 
 * @param name Nome a ser verificado.
 * 
 * @return Bool indicando se o nome é válido.
 */
bool is_valid_name(char *name) {
    return strlen(name) == NAME_LENGTH;
}

/**
 * @brief Carrega os scores salvos em um arquivo.
 * 
 * @param table Ponteiro para uma ScoreTable em que será colocado os scores.
 * @param file_path Caminho para o arquivo.
 */
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

/**
 * @brief Retorna o maior score salvo.
 * 
 * @param table Ponteiro para uma ScoreTable.
 * 
 * @return Maior score registrado.
 */
int get_highest_score(HighScoreTable *table) {
    if (table->count == 0) return 0;

    int highest_score = table->entries[0].score;

    for (int i = 1; i < table->count; i++) {
        if (table->entries[i].score > highest_score) 
            highest_score = table->entries[i].score;
        
    }

    return highest_score;
};

/**
 * @brief Printa todos os scores salvo no terminal.
 * 
 * @param table Ponteiro para uma ScoreTable.
 */
void print_scores(HighScoreTable *table) {
    for (int i = 0; i < table->count; i++) 
        printf("%s %.7d\n", table->entries[i].name, 
            table->entries[i].score);
}


/**
 * @brief Retorna a posição onde deve ser inserido o novo score, coso o novo
 * score não seja maior do que os outros scores salvos ou seja igual a 0
 * retorna -1.
 * 
 * @param table Ponteiro para uma ScoreTable.
 * @param new_score Novo score a ser salvo.
 * 
 * @return Posição do score na table.
 */
int get_high_score_position(HighScoreTable *table, int new_score) {
    for (int i = 0; i < table->count; i++) 
        if (table->entries[i].score < new_score) 
            return i;
    
    if (new_score > 0 && table->count < SCORE_TABLE_AMOUNT) 
        return table->count;

    return -1;
} 

/**
 * @brief Verifica se uma novo score é eligível para o score rank, 
 * retorna true caso for elegível, false caso contrário.
 * 
 * @param table Ponteiro para uma ScoreTable.
 * @param new_score Score a ser verificado.
 * 
 * @return Bool indicando se o score é elegível ou não.
 */
bool is_eligible_high_score(HighScoreTable *table, int new_score) {
    return get_high_score_position(table, new_score) >= 0;
}

/**
 * @brief Libera os recursos utilizados pela ScoreTable.
 * 
 * @param table Ponteiro para uma ScoreTable.
 */
void destroy_high_score_table(HighScoreTable *table) {
    if (table) free(table);
}

/**
 * @brief Verifica se a ScoreTable está vazia.
 * 
 * @param table Ponteiro para uma ScoreTable.
 * 
 * @return Bool indicando se a ScoreTable está vazia.
 */
bool is_score_table_empty(HighScoreTable *table) {
    return table->count == 0;
}