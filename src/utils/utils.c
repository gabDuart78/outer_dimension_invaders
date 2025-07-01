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

/**
 * @brief Retorna o valor passado caso enteja dentro dos limites estipulados, do contrário retorna os limites.
 * 
 * @param value Valor que deve ser verificado.
 * @param min Limite mínimo.
 * @param max Limite máximo.
 * 
 * @return Valor o valor que a ser verificado caso enteja dentro do limites estipulados, do contrário retorna os limites.
 */
float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/**
 * @brief Retorna um retângulo contendo as informações recebidas como argumento.
 *
 * @param ponit Estrutura Point representando a posição do objeto.
 * @param width Largura do retângulo.
 * @param height Altura do retângulo.
 * 
 * @return Rect com as especificações desejadas. 
 */
Rect get_collider(Point pos, int width, int height) {
    Rect collider = {
        .pos = pos,
        .width = width,
        .height = height,
    };

    return collider;
}

/**
 * @brief Define a seed a função srand com a função time. 
 */
void set_time_seed() {
    srand(time(NULL));
}

/**
 * @brief Gera um inteiro pseudo-aleatório dentro de um intervalo fechado.
 * 
 * @param min Limite mínimo.
 * @param max Limite máximo.
 * 
 * @return Um número inteiro pseudo-aleatório dentro do intervalo estipulado.
 */
int random_integer(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/**
 * @brief Gera um float pseudo-aleatório entre (0, 1).
 * 
 * @return Um float pseudo-aleatório.
 */
float random_float() {
    return ((float)rand()) / RAND_MAX; 
}

/**
 * @brief Carrega um ALLEGRO_BITMAP usando a função al_load_bitmap.
 * 
 * @param path Caminho para a imagem a ser carregada.
 * 
 * @return Um ALLEGRO_BITMAP representado uma imagem.
 */
ALLEGRO_BITMAP * get_sprite(const char *path) {
     ALLEGRO_BITMAP * sprite = al_load_bitmap(path);

    if (!sprite) {
        fprintf(stderr, "Unable to load sprite in path: %s.\n", path);
        exit(-1);
    }

    return sprite;
}

/**
 * @breif Verifica se uma determinada tecla do teclado foi pressionada.
 * 
 * @param event O evento disparado.
 * @param key_code Representando o codigo da tecla a ser verificada.
 * 
 * @return Bool indicando se a tecla foi pressionada.
 */
bool key_pressed(ALLEGRO_EVENT event, int key_code) {
    return event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == key_code;
}

/**
 * @brief Verifica se o usuário disparou o evento de fechar a window criada pelo allegro.
 * 
 * @param event O evento disparado.
 *
 * @reuturn Bool indicado se o evento foi um ALLEGRO_EVENT_DISPLAY_CLOSE. 
 */
bool close_display(ALLEGRO_EVENT event) {
    return event.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

/**
 * @brief Desenha um retângulo do tamanho da tela que serve como 
 * uma camada para cobrir o que foi desenhado átras.
 * 
 * @param color Estrutura RGB.
 * @param alpha A 'opacidade' da cor desejada. 
 */
void draw_screen_overlay(RGB color, unsigned char alpha) {
    al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 
        al_map_rgba(color.red, color.green, color.blue, alpha));
}

/**
 * @brief Remove o primeiro carater de uma string caso for um whitespace.
 * 
 * @param str String a ser verificada. 
 */
void trim_space_at_begin(char *str) {
    if (*str != ' ') return; 
    for (char *p1 = str, *p2 = str + 1; p2 <= str + strlen(str); p1++, p2++)
        *p1 = *p2;
}

/**
 * @brief Remove o último carater de uma string caso for um whitespace.
 * 
 * @param str String a ser verificada.
 */
void trim_space_at_end(char *str) {
    char *end = str + strlen(str) - 1;

    if (*end == ' ')
        *end = '\0';
}

/**
 * @brief Remove whitespace do ínicio e do início de uma string.
 * 
 * @para str String a ser verificada.
 */
void trim_space(char *str) {
    trim_space_at_begin(str);
    trim_space_at_end(str);
}

/**
 * @brief Desenha um texto dentro de um limite de comprimento, 
 * caso a linha for maior, quebra essa linha e continua o desenho.
 * 
 * @param font Ponteiro para a ALLEGRO_FONT a ser usada para desenhar o texto.
 * @param color ALLEGRO_COLOR a ser usada para o texto.
 * @param x Coordenada horizontal de onde começar a desenhar.
 * @param y Coordenada vertical de onde começar a desenhar.
 * @param max_width Comprimento máximo da linha.
 * @param text Texto a ser escrito.
 * @param gap Espaço entre cada linha do texto.
 * 
 */
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

/**
 * @brief Verifica se uma arquivo existe.
 * 
 * @param file_path Caminho para o arquivo a ser checado.
 * 
 * @return Bool indicando se o arquivo existe ou não.
 */
bool file_exists(const char *file_path) {
    FILE *stream = fopen(file_path, "r");

    if(stream) {
        fclose(stream);
        return true;
    }

    return false;
}

/**
 * @brief Verifica se uma string pode ser convertida para um inteiro.
 * 
 * @param string String a ser verificada.
 * 
 * @return Bool indicando se a string pode ser convertida ou não.
 */
bool is_valid_number(char *string) {
    if(!string) return false;

    while(*string != '\0') {
        if (!isdigit(*(string)++)) 
            return false;
    }

    return true;
}

/**
 * @brief Remove o carater de newline de uma string.
 * 
 * @param str String da qual será removido o caracter newline.  
 */
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

/**
 * @brief Cores personalisadas para cada posição do score na table de scores.
 * 
 * @param pos Posição do score na tabela de scores.
 * 
 * @return Um ALLEGRO_COLOR correspondente a posição do score na tabel de scores. 
 */
ALLEGRO_COLOR get_color_for_score(int pos) {
    if (pos == 0) 
        return al_map_rgb(230, 30, 200);

    if (pos < 3)
        return al_map_rgb(230, 30, 60);

    return al_map_rgb(120, 230, 30);
}

/**
 * @brief Desenha a tabela de scores na tela.
 * 
 * @param current_y Ponteiro para a posição horizontal onde se iniciará o a construção da tabela de scores.
 * @param font Ponteiro para a fonte que será utilizada no desenho da tabela.
 * */
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

/**
 * @brief Centraliza um retângulo dentro de um outro.
 * 
 * @param a Retângulo que será usado como referência para a centralização.
 * @param width Comprimento do retângulo a ser centralizado.
 * @param height Altura do retângulo a ser centralizado.
 * 
 * @return Um Point contendo as coordenadas do retângulo que foi centralizado.
 */
Point get_centered_pos_inside_rect(Rect a, int width, int height) {
    return (Point) {
        .x = a.pos.x + ((a.width - width) / 2),
        .y = a.pos.y + ((a.height - height) / 2),
    };
}