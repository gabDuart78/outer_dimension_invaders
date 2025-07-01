#include "utils.h"
#include "player.h"
#include "bullet.h"
#include "bullet_manager.h"
#include "sound_manager.h"

#define PLAYER_SPRITE_PATH "../assets/images/sprites/player/player_sprite_sheet.png"
#define PLAYER_BULLET_SPRITE_PATH "../assets/images/sprites/player/player_bullet.png"
#define MAX_SCORE 9999999

/**
 * @brief Aloca memária para a estrutura Player, inicializa com base nas configurações passadas e retorna
 * um ponteiro para a estrutura criada.
 * 
 * @param cfg Configurações para a estrutura player (velocidade, altura, largura etc).
 * 
 * @return Player.
 */
Player *create_player(PlayerConfig cfg) {
    Player *p = (Player *) malloc(sizeof(Player));

    if (!p) {
        fprintf(stderr, "Failed to create player.\n");
        exit(-1);
    }

    BulletManager *bm = create_bullet_manager(cfg.max_bullets, 
            cfg.bullet_config, get_sprite(PLAYER_BULLET_SPRITE_PATH));

    Animator *animator = (Animator *) malloc(sizeof(Animator));

    if (!animator) {
        fprintf(stderr, "Failed to create player animator.\n");
        exit(-1);
    }

    init_animator(animator, cfg.animation_frames, cfg.width, cfg.height, 
        cfg.frame_duration, true);

    Point pos = {SCREEN_WIDTH / 2.0f - (cfg.width / 2.0f), 
            SCREEN_HEIGHT - cfg.height - SCREEN_BOTTOM_MARGIN};

    *p = (Player) {
        .pos = pos,
        .width = cfg.width,
        .height = cfg.height,
        .move_left = cfg.move_left,
        .move_right = cfg.move_right,
        .speed = cfg.speed,
        .acc = cfg.acc,
        .acc_step = cfg.acc_step,
        .dec_step = cfg.dec_step,
        .max_acc = cfg.max_acc,
        .vx = cfg.vx,
        .is_shooting = cfg.is_shooting,
        .max_bullets = cfg.max_bullets,
        .bm = bm,
        .lifes = cfg.lifes,
        .max_life = cfg.max_life,
        .fire_interval = cfg.fire_interval,
        .last_fire_time = al_get_time(),
        .color = al_map_rgb(cfg.color.red, cfg.color.green, cfg.color.blue),
        .score = cfg.score,
        .is_alive = cfg.is_alive,
        .sprite = get_sprite(PLAYER_SPRITE_PATH),
        .animator = animator,
        .draw_hitbox = cfg.draw_hitbox,
    };

    return p;
}

/**
 * @brief Define a posição do player, caso a nova posição esteja fora dos limites da tela
 * ela será corrigida para dentro dos limites.
 * 
 * @param new_pos Nova posição a ser definida.
 * @param width Largura do player.
 * 
 * @return Bool indicando se a posição do player sofreu clamp.
 */
bool set_player_pos(Player *p, Point new_pos, int width) {
    Point original = new_pos;

    new_pos.x = clamp(new_pos.x, 0, width - p->width);

    p->pos = new_pos;

    return (original.x != new_pos.x || original.y != new_pos.y);
}

/**
 * @brief Retorna a direção de movimento do player.
 * 
 * @param p Ponteiro para o player.
 * 
 * @return MoveDir.
 */
MoveDir get_player_dir(Player *p) {
    if (p->move_left && !p->move_right) return MOVE_LEFT;
    if (p->move_right && !p->move_left) return MOVE_RIGHT;
    return NO_MOVE;
}

/**
 * @brief Define a acelação do player com base no seu movimento.
 * 
 * @param p Ponteiro para o player.
 */
void set_player_acc(Player *p) {
     if (get_player_dir(p) != NO_MOVE)  
        p->acc += p->acc_step;
    else 
        p->acc -= p->dec_step;
    
    p->acc = clamp(p->acc, 0.0f, p->max_acc);
}

/**
 * @brief Zera a velocidade no eixo x e a aceleração do player.
 * 
 * @param p Ponteiro para o player.
 */
void stop_player(Player *p) {
    p->vx = .0f;
    p->acc = .0f;
}

/**
 * @brief Retorna 1 ou -1 ou 0 que será usado para 
 * definir a direção de movimento do player ou pará-lo.
 * 
 * @param p Ponteiro para o player.
 * 
 * @return O modificador da velocidade do player.
 */
int get_player_speed_modifier(Player *p) {
    MoveDir dir = get_player_dir(p);
    return (dir == MOVE_RIGHT ? 1 : dir == MOVE_LEFT ? - 1 : 0);
}

/**
 * @brief Gerencia o movimento horizontal do player.
 * 
 * @param p Ponteiro para o player.
 */
void handle_player_movement(Player *p) {
    set_player_acc(p);

    p->vx = p->speed * p->acc * get_player_speed_modifier(p);

    Point new_pos = {p->vx + p->pos.x, p->pos.y};

    bool has_reach_boundary = false;

    if (p->vx != 0) 
        has_reach_boundary = set_player_pos(p, new_pos, SCREEN_WIDTH);
    
    if ((p->move_left || p->move_right) && has_reach_boundary) 
        stop_player(p);
}

/**
 * @brief Verifica se o player pode disparar com base na quantidade de balas disponíveis
 * e no intervalo do último disparo.
 * 
 * @param p Ponteiro para o player.
 * 
 * @return Bool indicando se o player pode atirar.
 */
bool player_can_fire(Player *p) {
    double now = al_get_time();
    double delta_time = now - p->last_fire_time;

    return (p->bm->quantity < p->bm->max) && 
        p->is_shooting && delta_time >= p->fire_interval;
}

/**
 * @brief Dispara um projétil, toca o som de tiro e atualiza o a variável last_fire_time
 * utilizando al_get_time.
 * 
 * @param p Ponteiro para o player.
 */
void player_fire_bullet(Player *p) {
    if (player_can_fire(p)) {
        play_sound(SFX_PLAYER_SHOOT);
        fire_bullet(p->bm, get_collider(p->pos, p->width, p->height));
        p->last_fire_time = al_get_time();
    }
}

/**
 * @brief Verifica se o player pode disparar, se sim, dispara.
 * 
 * @param p Ponteiro para o player.
 */
void handle_player_fire(Player *p) {
    if (p->is_shooting) 
        player_fire_bullet(p);
}

/**
 * @brief Faz o update da lógica do player, disparos, movimento e animação.
 * 
 * @param p Ponteiro para o player.
 */
void update_player(Player * p) {
    handle_player_movement(p);
    handle_player_fire(p);
    update_bullets(p->bm);
    update_animator(p->animator);
}

/**
 * @brief Converte um ALLEGRO_EVENT em um PlayerEvent caso haja um evento correspondente, do contrário
 * retorna um INPUT_NONE.
 * 
 * @param ev Um evento allegro.
 * 
 * @return PlayerInput.
 */
PlayerInput interpret_player_event(ALLEGRO_EVENT *ev) {
    if (ev->type == ALLEGRO_EVENT_KEY_DOWN || ev->type == ALLEGRO_EVENT_KEY_UP) {
        int keycode = ev->keyboard.keycode;
        bool is_down = ev->type == ALLEGRO_EVENT_KEY_DOWN;

        switch(keycode) {
            case ALLEGRO_KEY_RIGHT:
                return is_down ? INPUT_MOVE_RIGHT : INPUT_STOP_MOVE_RIGHT;

            case ALLEGRO_KEY_LEFT:
                return is_down ? INPUT_MOVE_LEFT : INPUT_STOP_MOVE_LEFT;

            case ALLEGRO_KEY_SPACE:
                return is_down ? INPUT_SHOOT : INPUT_STOP_SHOOT;
        }
    }

    return INPUT_NONE;
}   

/**
 * @brief Define o movimento na horizontal à esquerda do player.
 * 
 * @param p Ponteiro para o player.
 * @param move_left Um valor booleano que representa se o player esta se movendo para a esquerda 
 */
void set_move_left(Player *p, bool move_left) {
    p->move_left = move_left;
}

/**
 * @brief Define o movimento na horizontal à direita do player.
 * 
 * @param p Ponteiro para o player.
 * @param move_right Um valor booleano que representa se o player esta se movendo para a direita
 */
void set_move_right(Player *p, bool move_right) {
    p->move_right = move_right;
}

/**
 * @brief Define se o player está disparando.
 * 
 * @param p Ponteiro para o player.
 * @param move_right Um valor booleano que indica se o player está disparando.
 */
void set_shooting(Player *p, bool is_shooting) {
    p->is_shooting = is_shooting;
}

/**
 * @brief Lida com os eventos do player, disparos e movimento horizontal.
 * 
 * @param p Ponteiro para o player.
 * @param input Um PlayerInput
 */
void handle_player_events(Player *p, enum PlayerInput input) {
    switch(input) {
        case INPUT_NONE:
            break;
        case INPUT_MOVE_LEFT:
            set_move_left(p, true);
            break;
        case INPUT_STOP_MOVE_LEFT:
            set_move_left(p, false);
            break;
        case INPUT_MOVE_RIGHT:
            set_move_right(p, true);
            break;
        case INPUT_STOP_MOVE_RIGHT:
            set_move_right(p, false);
            break;
        case INPUT_SHOOT:
            set_shooting(p, true);
            break;
        case INPUT_STOP_SHOOT:
            set_shooting(p, false);
            break;
        default:
            break;
    }
}

/**
 * @brief Desenha o player e opcionalmente sua hitbox.
 * 
 * @param p Ponteiro para o player.
 */
void draw_player(Player *p) {
    if (p->lifes <= 0) return;

    draw_animated_sprite(p->sprite, p->animator, p->pos.x, p->pos.y);

    if (p->draw_hitbox)
        al_draw_rectangle(p->pos.x, p->pos.y, p->pos.x + p->width, 
            p->pos.y + p->height, p->color, 0);
}

/**
 * @brief Libera os recusos utilizados pela estrutura Player.
 * 
 * @param p Ponteiro para o player.
 */
void destroy_player(Player *p) {
    if (!p) return;

    destroy_bullet_manager(p->bm);
    destroy_animator(p->animator);

    if (p->sprite)
        al_destroy_bitmap(p->sprite);
    
    free(p);
}

/**
 * @brief Define o estado lógico do player para morto.
 * 
 * @param p Ponteiro para o player.
 */
void kill_player(Player *p) {
    p->is_alive = false;
}

/**
 * @brief Reduz a vida do player e toca o som de hit, mata o player caso sua vida seja zerada.
 * 
 * @param p Ponteiro para o player.
 */
void hit_player(Player *p) {
    if (p->lifes > 0) {
        p->lifes--;
        play_sound(SFX_PLAYER_HIT);
    } 
     
    if (p->lifes == 0) 
        kill_player(p);
}

/**
 * @brief Aumenta o score do player com base em uma quantidade de pontos. 
 * 
 * @param p Ponteiro para o Player.
 * @param points Inteiro que representa quantos pontos o player deve receber
 */
void player_hits_enemy(Player *p, int points) {
    if (p->score < MAX_SCORE)
        p->score += points;
}