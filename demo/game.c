#include "body.h"
#include "text.h"
#include "sounds.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "list.h"
#include "sdl_wrapper.h"
#include "vector.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// general constants
const vector_t WINDOW = (vector_t){.x = 1000, .y = 500};
const size_t MAX_LIVES = 3;
const vector_t BACKGROUND_DIMENSIONS = (vector_t) {.x = 7000, .y = 700};
const size_t LEVEL_LENGTH = 5000;
const size_t GROUND_CORRECTION = 100;
const size_t PORTAL_CORRECTION = 175;
const double DELAY = 0.1;
const size_t COINS_PER_LEVEL = 3;
size_t ROTATION_NUM = 0;
const double REMOVE_BODY_DISTANCE = -500;
const size_t END_WALL_OFFSET = 600;
info_t START_LEVEL = WELCOME_SCREEN;

// sound
char *FIRST_LEVEL_MUSIC_PATH = "assets/first.wav";
char *SECOND_LEVEL_MUSIC_PATH = "assets/second.wav";
char *THIRD_LEVEL_MUSIC_PATH = "assets/third.wav";
char *FOURTH_LEVEL_MUSIC_PATH = "assets/fourth.wav";
char *FIFTH_LEVEL_MUSIC_PATH = "assets/fifth.wav";
char *JUMPING_SOUND_PATH = "assets/jump.wav";
char *DIE_SOUND_PATH = "assets/die.wav";
char *COIN_SOUND_PATH = "assets/coin.wav";
char *PLUS_ONE_SOUND_PATH = "assets/plus_one.wav";
char *VICTORY_SOUND_PATH = "assets/victory.wav";
char *LOSS_SOUND_PATH = "assets/loss.wav"; 
size_t FREQUENCY = 1000;
size_t CHUNK_SIZE = 4096;

// text
SDL_Color INFO_TEXT_COLOR = (SDL_Color) {189,189,189};
SDL_Color LEVEL_TEXT_COLOR = (SDL_Color) {59,58,58};
SDL_Color LIGHT_LEVEL_TEXT_COLOR = (SDL_Color) {214,210,210};
const char *TITLE_FONT_PATH = "assets/FFF_Tusj.ttf";
const char *LEVEL_CHANGE_FONT_PATH = "assets/ARCADECLASSIC.TTF";
const char *INFO_FONT_PATH = "assets/ChunkFive-Regular.otf";
size_t TITLE_FONT_SIZE = 200;
size_t INFO_FONT_SIZE = 200;

// colors 
rgb_color_t PLAYER_COLOR = {.r = (double) 0 / 255, .g = (double) 0 / 255, 
                            .b = (double) 0 / 255};
rgb_color_t TRAIL_COLOR1 = {.r = (double) 237 / 255, .g = (double) 102 / 255,
                            .b = (double) 102 / 255};
rgb_color_t TRAIL_COLOR2 = {.r = (double) 109 / 255, .g = (double) 237 / 255,
                            .b = (double) 102 / 255};
rgb_color_t TRAIL_COLOR3 = {.r = (double) 102 / 255, .g = (double) 143 / 255,
                            .b = (double) 237 / 255};
rgb_color_t PORTAL_COLOR = {.r = (double) 0, .g = (double) 0, .b = (double) 1};
rgb_color_t COIN_COLOR = {.r = (double) 153 / 255, .g = (double) 141 / 255, 
                          .b = (double) 52 / 255};
rgb_color_t SPEED_BLOCK_COLOR = {.r = (double) 219 / 255, .g = (double) 114 / 255, 
                                 .b = (double) 191 / 255};
rgb_color_t SLOW_BLOCK_COLOR = {.r = (double) 131 / 255, .g = (double) 209 / 255, 
                                .b = (double) 52 / 255};
rgb_color_t TEXT_BLOCK_COLOR = {.r = (double) 28 / 255, .g = (double) 8 / 255, 
                                .b = (double) 36 / 255};
rgb_color_t BACKGROUND_BLOCK_COLOR = {.r = (double) 105 / 255, .g = (double) 89 / 255, 
                                .b = (double) 54 / 255};
rgb_color_t FIRST_BLOCK_COLOR = {.r = (double) 79 / 255, .g = (double) 17 / 255, 
                                 .b = (double) 68 / 255};
rgb_color_t SECOND_BLOCK_COLOR = {.r = (double) 59 / 255, .g = (double) 61 / 255, 
                                  .b = (double) 59 / 255};
rgb_color_t THIRD_BLOCK_COLOR = {.r = (double) 79 / 255, .g = (double) 17 / 255, 
                                 .b = (double) 17 / 255};
rgb_color_t FOURTH_BLOCK_COLOR = {.r = (double) 17 / 255, .g = (double) 79 / 255, 
                                  .b = (double) 19 / 255};
rgb_color_t FIFTH_BLOCK_COLOR = {.r = (double) 44 / 255, .g = (double) 60 / 255, 
                                 .b = (double) 163 / 255};

// gravities
const double FIRST_GRAV_CONST = 5.5 * 1e4;
const double SECOND_GRAV_CONST = 2.5 * 1e4;
const double THIRD_GRAV_CONST = 7 * 1e4;
const double FOURTH_GRAV_CONST = 5.5 * 1e4;
const double FIFTH_GRAV_CONST = -5.5 * 1e4;

// pi divisors
const size_t GROUND_PI_DIV = 250;
const double PORTAL_PI_DIV = 75;
const double TEXT_BOX_PI_DIV = 5;
const double BACKGROUND_PI_DIV = 31.52;

// radii
const double PLAYER_RAD = 30;
const double TRAIL_SHAPE_RAD = 3;
const double STAR_INNER_RAD = 5;
const double STAR_OUTER_RAD = 15;
const double MOVING_STAR_INNER_RAD = 15;
const double MOVING_STAR_OUTER_RAD = 45;
const double COIN_RAD = 20;
const double PORTAL_RAD = 50;
const double MYSTERY_BLOCK_RAD = 25;
const double TEXT_BOX_RAD = 350;
const double BACKGROUND_BLOCK_RAD = 3517.46;

// masses
const double PLAYER_MASS = 50.0;
const double STAR_MASS = 50;
const double BIG_MASS = INFINITY;

// positions
const vector_t TEXT_BOX_PLAYER_POS = (vector_t){.x = 500, .y = 250};
const vector_t PLAYER_POS = (vector_t){.x = 100, .y = 140};
const vector_t UPSIDE_DOWN_PLAYER_POS = (vector_t){.x = 100, .y = 360};
const vector_t GROUND_POS = (vector_t) {LEVEL_LENGTH / 2.0, 60};

// velocities
const vector_t UNIVERSAL_VELOCITY = {-150, 0};
const vector_t BACKGROUND_VELOCITY = {-80, 0};
const vector_t SPEED_VELOCITY = {-300, 0};
const vector_t SLOW_VELOCITY = {-75, 0};
const vector_t TRAIL_VELOCITY = (vector_t){.x = -120, .y = -20};
const vector_t UPSIDE_DOWN_TRAIL_VELOCITY = (vector_t){.x = -120, .y = 20};
const double MOVING_STAR_X_VEL = -210;
const double MOVING_STAR_Y_VEL = 38;

// physics
const double JUMP_ROTATION_CONST = -2 * M_PI / 30;
const vector_t JUMP_IMPULSE = (vector_t){0, 2 * 1e4};
const double ELASTICITY = 0.0;

typedef struct state {
    scene_t *scene;
    list_t *bodies;
    double time;
    bool key_held;
    bool single_jumped;
    bool double_jumped;
    size_t lives;
    size_t coins_collected_in_level;
    size_t total_coins_between_levels;
    size_t total_coins_across_game;
    bool plus_one_life;
    list_t *chunks;
} state_t;

void make_background(state_t *state, char *file_path) {
    char *background_name = malloc(sizeof(char) * 50);
    sprintf(background_name, "%s", file_path);
    scene_set_background(state->scene, background_name, BACKGROUND_DIMENSIONS);
}

void make_player_body(state_t *state, double x, double y, double angle, info_t type) {
    polygon_t *player_poly = make_player(x, y, PLAYER_RAD);
    polygon_rotate(polygon_get_vertices(player_poly), angle, 
                                        (vector_t){x, y});
    body_t *player_body = body_init_with_info(polygon_get_vertices(player_poly), 
                                              PLAYER_MASS, PLAYER_COLOR, 
                                              (void *)type, NULL);
    scene_add_body(state->scene, player_body);
    free(player_poly);
}

void make_ground_body(state_t *state, double y, rgb_color_t color) {
    polygon_t* ground_poly = make_horizontal_rectangle(GROUND_POS.x, y, LEVEL_LENGTH + GROUND_CORRECTION, GROUND_PI_DIV);
    body_t* ground_body = body_init_with_info(polygon_get_vertices(ground_poly), 
                                              BIG_MASS, color, (void*) 
                                              MAIN_GROUND, NULL);
    body_set_velocity(ground_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, ground_body);
    free(ground_poly);
}

void make_block_body(state_t *state, double x, double y, double rad, double pi_divisor, rgb_color_t color) {
    polygon_t* block_poly = make_horizontal_rectangle(x, y, rad, pi_divisor);
    body_t* block_body = body_init_with_info(polygon_get_vertices(block_poly), 
                                              BIG_MASS, color, 
                                              (void*) GROUND, NULL);
    body_set_velocity(block_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, block_body);
    free(block_poly);
}

void make_vertical_block_body(state_t *state, double x, double y, double rad, double pi_divisor, rgb_color_t color) {
    polygon_t* block_poly = make_vertical_rectangle(x, y, rad, pi_divisor);
    body_t* block_body = body_init_with_info(polygon_get_vertices(block_poly), 
                                              BIG_MASS, color, 
                                              (void*) GROUND, NULL);
    body_set_velocity(block_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, block_body);
    free(block_poly);
}

void make_obstacle_body(state_t *state, double x, double y, rgb_color_t color) {
    polygon_t* obs_poly = make_polygon(x, y, 12, STAR_INNER_RAD, STAR_OUTER_RAD);
    body_t* obs_body = body_init_with_info(polygon_get_vertices(obs_poly), BIG_MASS, 
                                            color, (void*) OBSTACLE, NULL);
    body_set_velocity(obs_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, obs_body);
    free(obs_poly);
}

void make_coin_body(state_t *state, double x, double y) {
    polygon_t* coin_poly = make_polygon(x, y, 12, COIN_RAD, COIN_RAD);
    body_t* coin_body = body_init_with_info(polygon_get_vertices(coin_poly), BIG_MASS, 
                                             COIN_COLOR, (void*) COIN, NULL);
    body_set_velocity(coin_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, coin_body);
    free(coin_poly);
}

void make_end_wall_body(state_t *state, rgb_color_t color) {
    polygon_t* end_wall_poly = make_vertical_rectangle(LEVEL_LENGTH + END_WALL_OFFSET, WINDOW.y / 2, WINDOW.y + GROUND_CORRECTION, 3);
    body_t* end_wall_body = body_init_with_info(polygon_get_vertices(end_wall_poly), BIG_MASS, color, (void*) OBSTACLE, NULL);
    body_set_velocity(end_wall_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, end_wall_body);
    free(end_wall_poly);
}

void make_speed_block_body(state_t *state, double x, double y) {
    polygon_t* speed_poly = make_horizontal_rectangle(x, y, MYSTERY_BLOCK_RAD, 4);
    body_t* speed_body = body_init_with_info(polygon_get_vertices(speed_poly), BIG_MASS, SPEED_BLOCK_COLOR, (void*) SPEED_MYSTERY_BLOCK, NULL);
    body_set_velocity(speed_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, speed_body);
    free(speed_poly);
}

void make_slow_block_body(state_t *state, double x, double y) {
    polygon_t* slow_poly = make_horizontal_rectangle(x, y, MYSTERY_BLOCK_RAD, 4);
    body_t* slow_body = body_init_with_info(polygon_get_vertices(slow_poly), BIG_MASS, SLOW_BLOCK_COLOR, (void*) SLOW_MYSTERY_BLOCK, NULL);
    body_set_velocity(slow_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, slow_body);
    free(slow_poly);
}

void make_portal_body(state_t *state, double y) {
    double x;

    if (scene_get_curr_level(state->scene) == FIFTH_LEVEL) {
        x = LEVEL_LENGTH - 170;
    } else {
        x = LEVEL_LENGTH;
    }

    polygon_t* portal_poly = make_vertical_rectangle(x, y, PORTAL_RAD, PORTAL_PI_DIV);
    body_t* portal_body = body_init_with_info(polygon_get_vertices(portal_poly), STAR_MASS, PORTAL_COLOR, (void*) PORTAL, NULL);
    body_set_velocity(portal_body, UNIVERSAL_VELOCITY);
    scene_add_body(state->scene, portal_body);
    free(portal_poly);
}

void make_moving_obstacle_body(state_t *state, double x, double y, double x_vel, double y_vel, rgb_color_t color) {
    polygon_t* mobs_poly = make_polygon(x, y, 12, MOVING_STAR_INNER_RAD, 
                                         MOVING_STAR_OUTER_RAD);
    body_t* mobs_body = body_init_with_info(polygon_get_vertices(mobs_poly), BIG_MASS, 
                                             color, (void*) OBSTACLE, NULL);
    body_set_velocity(mobs_body, (vector_t) {x_vel, y_vel});
    scene_add_body(state->scene, mobs_body);
    free(mobs_poly);
}

void make_text_box_body(state_t *state) {
    polygon_t *text_box_poly = make_horizontal_rectangle(WINDOW.x / 2, WINDOW.y / 2, 
                                                         TEXT_BOX_RAD, TEXT_BOX_PI_DIV);
    body_t* text_box_body = body_init_with_info(polygon_get_vertices(text_box_poly), 
                                                BIG_MASS, TEXT_BLOCK_COLOR, 
                                                (void*) TEXT_BLOCK, NULL);
    scene_add_body(state->scene, text_box_body);
    free(text_box_poly);
}

void make_trail_body(state_t *state, vector_t centroid, bool is_rolly_poly, rgb_color_t color) {
    double offset = rand() / (double)RAND_MAX * 35;

    polygon_t *trail_poly;

    if (is_rolly_poly) {
        trail_poly = make_horizontal_rectangle(centroid.x - 20, centroid.y - 10 + offset, TRAIL_SHAPE_RAD, 4);
    } else {
        trail_poly = make_horizontal_rectangle(centroid.x - 20, centroid.y + 10 - offset, TRAIL_SHAPE_RAD, 4);
    }

    body_t* trail_body = body_init_with_info(polygon_get_vertices(trail_poly), STAR_MASS, color, (void*) TRAIL_SHAPE, NULL);
    free(trail_poly);

    if (is_rolly_poly) {
        body_set_velocity(trail_body, TRAIL_VELOCITY);
    } else {
        body_set_velocity(trail_body, UPSIDE_DOWN_TRAIL_VELOCITY);
    }

    body_set_rotation(trail_body, M_PI / 4);
    scene_add_body(state->scene, trail_body);
}

void player_on_key(char key, key_event_type_t type, double held_time,
                   void *state) {
    state_t *new_state = (state_t *)state;
    body_t *player = scene_get_body(new_state->scene, 0);
    vector_t centroid = body_get_centroid(player);
    info_t level = scene_get_curr_level(new_state->scene);
    bool hit = body_get_hit_ground(player);    
    bool on_text_screen = (level == WELCOME_SCREEN || 
                           level == FIRST_RULES || 
                           level == SECOND_RULES || 
                           level == THIRD_RULES || 
                           level == FOURTH_RULES || 
                           level == FIFTH_RULES || 
                           level == WIN_SCREEN ||
                           level == LOSE_SCREEN);

    if (hit) {
        hit = true;
        new_state->double_jumped = false;
    }

    if ((hit || level == FOURTH_LEVEL || on_text_screen) && type == KEY_PRESSED && 
         !new_state->key_held) {
        switch (key) {
        case RETURN:
            if (on_text_screen) {
                body_set_centroid(player, (vector_t) {centroid.x - 5, centroid.y});
            }
            break;
        case SPACE:
            new_state->key_held = true;
            
            if (!on_text_screen) {
                sound_t* jump_sound = chunk_sound_init(JUMPING_SOUND_PATH, FREQUENCY, CHUNK_SIZE);
                list_add(new_state->chunks, jump_sound);
            }

            if (level == FIRST_LEVEL) {
                body_set_centroid(player, (vector_t) {centroid.x, centroid.y + 15});
                body_add_impulse(player, JUMP_IMPULSE);
                body_set_hit_ground(player, false);
                body_set_sliding(player, false);
                body_set_is_rotating(player, true);
            } else if (level == SECOND_LEVEL) {
                body_set_centroid(player, (vector_t) {centroid.x, centroid.y + 15});
                body_add_impulse(player, JUMP_IMPULSE);
                body_set_hit_ground(player, false);
                body_set_sliding(player, false);
                body_set_is_rotating(player, true);
            } else if (level == THIRD_LEVEL) {
                if (!new_state->single_jumped && !new_state->double_jumped) {
                    body_set_centroid(player, (vector_t) {centroid.x, centroid.y + 18});
                    body_add_impulse(player, JUMP_IMPULSE);
                    new_state->single_jumped = true;
                    body_set_sliding(player, false);
                    body_set_is_rotating(player, true);
                } else if (!new_state->double_jumped) {
                    body_set_centroid(player, (vector_t) {centroid.x, centroid.y + 18});
                    body_set_velocity(player, VEC_ZERO);
                    body_add_impulse(player, JUMP_IMPULSE);
                    new_state->single_jumped = false;
                    new_state->double_jumped = true;
                    body_set_hit_ground(player, false);
                    body_set_sliding(player, false);
                    body_set_is_rotating(player, true);
                }
            } else if (level == FOURTH_LEVEL) {
                body_set_velocity(player, VEC_ZERO);
                body_add_impulse(player, JUMP_IMPULSE);
                body_set_is_rotating(player, true);
            } else if (level == FIFTH_LEVEL) {
                body_set_centroid(player, (vector_t) {centroid.x, centroid.y - 15});
                body_add_impulse(player, (vector_t) {JUMP_IMPULSE.x, -JUMP_IMPULSE.y});
                body_set_hit_ground(player, false);
                body_set_sliding(player, false);
                body_set_is_rotating(player, true);
            }
            break;
        }
    } else if (type == KEY_RELEASED) {
        new_state->key_held = false;
    }

    body_tick(player, time_since_last_tick());
}

state_t *start_welcome_screen(state_t *state) {
    state->lives = MAX_LIVES;
    state->plus_one_life = false;
    state->total_coins_across_game = 0;
    state->coins_collected_in_level = 0;
    state->total_coins_between_levels = 0;
    list_t *chunks = list_init(0, (free_func_t) sound_free);
    state->chunks = chunks;

    // make background
    make_background(state, "assets/Level_1_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *welcome_screen_text = list_init(0, (free_func_t) text_free);
    text_t *title = text_init("Rolly Poly", 300, 200, 350, 100, TTF_OpenFont(TITLE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(welcome_screen_text, title);
    text_t *welcome_message1 = text_init("Welcome to Rolly Poly!", 200, 30, 400, 325, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(welcome_screen_text, welcome_message1);
    text_t *welcome_message2 = text_init("Press 'Enter' to move to next screen", 300, 30, 350, 355, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(welcome_screen_text, welcome_message2);

    scene_set_texts(state->scene, welcome_screen_text);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_first_level_rules(state_t *state) {
    // make background
    make_background(state, "assets/Level_1_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *first_rules_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("L e v e l    1", 300, 200, 350, 50, TTF_OpenFont(LEVEL_CHANGE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, level_title);
    text_t *rules1 = text_init("Press 'Space' to jump! Try to avoid  ", 400, 30, 300, 250, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, rules1);
    text_t *rules2 = text_init("obstacles and collect coins. You have", 400, 30, 300, 275, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, rules2);
    text_t *rules3 = text_init("three lives. If you lose a life, you ", 400, 30, 300, 300, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, rules3);
    text_t *rules4 = text_init("can gain one back by collecting three ", 400, 30, 300, 325, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, rules4);
    text_t *rules5 = text_init("coins. Hit the blue portal to progress", 400, 30, 300, 350, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, rules5);
    text_t *rules6 = text_init("to the next level. Good luck!         ", 400, 30, 300, 375, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, rules6);

    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives = text_init(lives_message, 60, 20, 240, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, lives);

    char *coins_message = malloc(sizeof(char) * 50);
    sprintf(coins_message, "Coins: %zu", state->total_coins_across_game);
    text_t *coins = text_init(coins_message, 60, 20, 700, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(first_rules_text, coins);

    scene_set_texts(state->scene, first_rules_text);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_first_level(state_t *state) {
    // make background
    make_background(state, "assets/Level_1_Background.png");

    // make player
    make_player_body(state, PLAYER_POS.x, PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);
    body_t *player_body;

    // find player index
    size_t player_index = INFINITY;
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
        body_t *curr = scene_get_body(state->scene, i);
        if ((int)body_get_info(curr) == ROLLY_POLY) {
            player_index = i;
            player_body = curr;
        }
    }

    create_gravity(state->scene, FIRST_GRAV_CONST, 
                   scene_get_body(state->scene, player_index));

    // make ground
    make_ground_body(state, GROUND_POS.y, FIRST_BLOCK_COLOR);

    // make blocks
    make_block_body(state, 680, 125, 312.5, 30, FIRST_BLOCK_COLOR);
    make_block_body(state, 680, 125, 187.5, 8, FIRST_BLOCK_COLOR);
    make_block_body(state, 1440, 125, 200, 20, FIRST_BLOCK_COLOR);
    make_block_body(state, 1500, 125, 187.5, 8, FIRST_BLOCK_COLOR);
    make_block_body(state, 1830, 125, 100, 4, FIRST_BLOCK_COLOR);
    make_block_body(state, 2370, 125, 125, 10, FIRST_BLOCK_COLOR);
    make_block_body(state, 2781.25, 146.25, 31.25, 4, FIRST_BLOCK_COLOR);
    make_block_body(state, 2875, 187.5, 31.25, 4, FIRST_BLOCK_COLOR);
    make_block_body(state, 2968.75, 228.75, 31.25, 4, FIRST_BLOCK_COLOR);
    make_block_body(state, 3531.25, 125, 70, 5, FIRST_BLOCK_COLOR);
    make_block_body(state, 3642, 187, 31.25, 4, FIRST_BLOCK_COLOR);
    make_block_body(state, 3760, 145, 125, 3, FIRST_BLOCK_COLOR);
    make_block_body(state, 4200, 146.25, 31.25, 4, FIRST_BLOCK_COLOR);

    // make obstacles
    make_obstacle_body(state, 680, 200, FIRST_BLOCK_COLOR);
    make_obstacle_body(state, 1709.5, 127, FIRST_BLOCK_COLOR);
    make_obstacle_body(state, 1955, 127, FIRST_BLOCK_COLOR);
    make_obstacle_body(state, 2210, 127, FIRST_BLOCK_COLOR);
    make_obstacle_body(state, 2540, 127, FIRST_BLOCK_COLOR);

    // make coins
    make_coin_body(state, 1120, 150);
    make_coin_body(state, 2968.25, 280);
    make_coin_body(state, 4220, 220);

    // make speed up mystery block
    make_speed_block_body(state, -250, 250);

    // make slow down mystery block
    make_slow_block_body(state, -250, 250);

    // make end wall
    make_end_wall_body(state, FIRST_BLOCK_COLOR);

    // make portal
    make_portal_body(state, PORTAL_CORRECTION);

    // add all forces
    for (size_t j = 0; j < scene_bodies(state->scene); j++) {
        body_t *curr_body = scene_get_body(state->scene, j);

        if ((int) body_get_info(curr_body) == OBSTACLE) {
            create_destructive_collision(state->scene, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == GROUND || 
                   (int) body_get_info(curr_body) == MAIN_GROUND) {
            create_physics_collision(state->scene, ELASTICITY, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == PORTAL) {
            create_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == COIN){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        }
    }

    // make text
    list_t *first_level_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("Level 1", 60, 20, 10, 10, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LEVEL_TEXT_COLOR);
    
    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives_title = text_init(lives_message, 60, 20, 10, 470, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LIGHT_LEVEL_TEXT_COLOR);
    
    list_add(first_level_text, level_title);
    list_add(first_level_text, lives_title);

    scene_set_texts(state->scene, first_level_text);

    // make music
    music_sound_init(FIRST_LEVEL_MUSIC_PATH, FREQUENCY, CHUNK_SIZE, 2);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_second_level_rules(state_t *state) {
    // make background
    make_background(state, "assets/Level_2_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *second_rules_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("L e v e l    2", 300, 200, 350, 50, TTF_OpenFont(LEVEL_CHANGE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, level_title);
    text_t *rules1 = text_init("Something is weird after going through", 400, 30, 300, 250, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, rules1);
    text_t *rules2 = text_init("that portal... Gravity doesn't seem to", 400, 30, 300, 275, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, rules2);
    text_t *rules3 = text_init("pull you down like it used to. Weird  ", 400, 30, 300, 300, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, rules3);
    text_t *rules4 = text_init("boxes also seem to have appeared. I   ", 400, 30, 300, 325, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, rules4);
    text_t *rules5 = text_init("wonder what happens if you hit them...", 400, 30, 300, 350, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, rules5);

    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives = text_init(lives_message, 60, 20, 240, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, lives);

    char *coins_message = malloc(sizeof(char) * 50);
    sprintf(coins_message, "Coins: %zu", state->total_coins_across_game);
    text_t *coins = text_init(coins_message, 60, 20, 700, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(second_rules_text, coins);

    if (state->plus_one_life) {
        state->plus_one_life = false;
        text_t *plus_one = text_init("Plus one life!", 200, 30, 400, 400, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
        list_add(second_rules_text, plus_one);
    }

    scene_set_texts(state->scene, second_rules_text);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_second_level(state_t *state) {
    // make background
    make_background(state, "assets/Level_2_Background.png");

    // make player
    make_player_body(state, PLAYER_POS.x, PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);
    body_t *player_body;

    // find player index
    size_t player_index = INFINITY;
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
        body_t *curr = scene_get_body(state->scene, i);
        if ((int)body_get_info(curr) == ROLLY_POLY) {
            player_index = i;
            player_body = curr;
        }
    }

    create_gravity(state->scene, SECOND_GRAV_CONST, scene_get_body(state->scene, 
                   player_index));


    // make ground
    make_ground_body(state, GROUND_POS.y, SECOND_BLOCK_COLOR);

    // make blocks
    make_block_body(state, 600, 125, 125, 10, SECOND_BLOCK_COLOR);
    make_block_body(state, 1000, 125, 125, 10, SECOND_BLOCK_COLOR);
    make_block_body(state, 1700, 125, 180, 13, SECOND_BLOCK_COLOR);
    make_block_body(state, 1700, 180, 60, 8, SECOND_BLOCK_COLOR);
    make_block_body(state, 2350, 125, 125, 10, SECOND_BLOCK_COLOR);
    make_block_body(state, 2500, 125, 150, 6, SECOND_BLOCK_COLOR);
    make_block_body(state, 2600, 125, 130, 3, SECOND_BLOCK_COLOR);
    make_block_body(state, 3000, 210, 50, 7, SECOND_BLOCK_COLOR);
    make_block_body(state, 3300, 210, 50, 7, SECOND_BLOCK_COLOR);
    make_block_body(state, 3700, 125, 150, 6, SECOND_BLOCK_COLOR);

    // make obstacles
    // first obstacle chunk
    for (size_t o1 = 0; o1 < 5; o1++) {
        make_obstacle_body(state, 732 + 34 * o1, 127, SECOND_BLOCK_COLOR);
    }

    // second obstacle chunk
    for (size_t o2 = 0; o2 < 27; o2++) {
        make_obstacle_body(state, 2675 + 34 * o2, 127, SECOND_BLOCK_COLOR);
    }

    // make coins
    make_coin_body(state, 1800, 240);
    make_coin_body(state, 2995, 265);
    make_coin_body(state, 4080, 265);

    // make speed up mystery block
    make_speed_block_body(state, 2100, 300);

    // make slow down mystery block
    make_slow_block_body(state, 3750, 250);

    // make end wall
    make_end_wall_body(state, SECOND_BLOCK_COLOR);

    // make portal
    make_portal_body(state, PORTAL_CORRECTION);

    for (size_t j = 0; j < scene_bodies(state->scene); j++) {
        body_t *curr_body = scene_get_body(state->scene, j);

        if ((int) body_get_info(curr_body) == OBSTACLE) {
            create_destructive_collision(state->scene, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == GROUND || 
                   (int) body_get_info(curr_body) == MAIN_GROUND) {
            create_physics_collision(state->scene, ELASTICITY, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == PORTAL) {
            create_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == COIN){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == SPEED_MYSTERY_BLOCK){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == SLOW_MYSTERY_BLOCK){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        }
    }

    // make text
    list_t *second_level_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("Level 2", 60, 20, 10, 10, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LEVEL_TEXT_COLOR);
    
    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives_title = text_init(lives_message, 60, 20, 10, 470, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LIGHT_LEVEL_TEXT_COLOR);

    list_add(second_level_text, level_title);
    list_add(second_level_text, lives_title);

    scene_set_texts(state->scene, second_level_text);

    // make music
    music_sound_init(SECOND_LEVEL_MUSIC_PATH, FREQUENCY, CHUNK_SIZE, 2);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_third_level_rules(state_t *state) {
    // make background
    make_background(state, "assets/Level_3_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *third_rules_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("L e v e l    3", 300, 200, 350, 50, TTF_OpenFont(LEVEL_CHANGE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, level_title);
    text_t *rules1 = text_init("It seems like the pink boxes speed you up ", 400, 30, 300, 250, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, rules1);
    text_t *rules2 = text_init("and the green ones slow you down. Strange.", 400, 30, 300, 275, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, rules2);
    text_t *rules3 = text_init("Gravity also seems to have changed again -", 400, 30, 300, 300, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, rules3);
    text_t *rules4 = text_init("it feels stronger somehow. Luckily, you   ", 400, 30, 300, 325, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, rules4);
    text_t *rules5 = text_init("can double jump now! Nice!                ", 400, 30, 300, 350, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, rules5);
    scene_set_texts(state->scene, third_rules_text);

    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives = text_init(lives_message, 60, 20, 240, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, lives);

    char *coins_message = malloc(sizeof(char) * 50);
    sprintf(coins_message, "Coins: %zu", state->total_coins_across_game);
    text_t *coins = text_init(coins_message, 60, 20, 700, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(third_rules_text, coins);

    if (state->plus_one_life) {
        state->plus_one_life = false;
        text_t *plus_one = text_init("Plus one life!", 200, 30, 400, 400, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
        list_add(third_rules_text, plus_one);
    }

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_third_level(state_t *state) {
    // make background
    make_background(state, "assets/Level_3_Background.png");

    // make player
    make_player_body(state, PLAYER_POS.x, PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);
    body_t *player_body;

    // find player index
    size_t player_index = INFINITY;
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
        body_t *curr = scene_get_body(state->scene, i);
        if ((int)body_get_info(curr) == ROLLY_POLY) {
            player_index = i;
            player_body = curr;
        }
    }

    create_gravity(state->scene, THIRD_GRAV_CONST, scene_get_body(state->scene, player_index));


    // make ground
    make_ground_body(state, GROUND_POS.y, THIRD_BLOCK_COLOR);

    // make blocks
    make_block_body(state, 850, 125, 100, 12, THIRD_BLOCK_COLOR);
    make_block_body(state, 1000, 170, 50, 9, THIRD_BLOCK_COLOR);
    make_block_body(state, 1450, 125, 90, 8, THIRD_BLOCK_COLOR);
    make_block_body(state, 1600, 180, 31.5, 4, THIRD_BLOCK_COLOR);
    make_block_body(state, 1865, 180, 31.5, 4, THIRD_BLOCK_COLOR);
    make_block_body(state, 2430, 125, 180, 25, THIRD_BLOCK_COLOR);
    make_block_body(state, 2550, 110, 100, 5, THIRD_BLOCK_COLOR);
    make_block_body(state, 2780, 110, 85, 4, THIRD_BLOCK_COLOR);
    make_block_body(state, 2930, 125, 180, 25, THIRD_BLOCK_COLOR);
    make_block_body(state, 3200, 155, 50, 10, THIRD_BLOCK_COLOR);
    make_block_body(state, 3350, 175, 50, 10, THIRD_BLOCK_COLOR);
    make_block_body(state, 3580, 125, 100, 8, THIRD_BLOCK_COLOR);
    make_block_body(state, 3990, 165, 50, 10, THIRD_BLOCK_COLOR);
    make_vertical_block_body(state, 1720, 125, 110, 8, THIRD_BLOCK_COLOR);
    make_vertical_block_body(state, 3770, 105, 90, 6, THIRD_BLOCK_COLOR);

    // make obstacles
    make_obstacle_body(state, 1650, 127, THIRD_BLOCK_COLOR);
    make_obstacle_body(state, 1790, 127, THIRD_BLOCK_COLOR);
    make_obstacle_body(state, 2665, 127, THIRD_BLOCK_COLOR);
    make_obstacle_body(state, 3700, 127, THIRD_BLOCK_COLOR);
    make_obstacle_body(state, 4260, 127, THIRD_BLOCK_COLOR);
    make_obstacle_body(state, 4278, 162, THIRD_BLOCK_COLOR);

    // make coins
    make_coin_body(state, 3350, 220);
    make_coin_body(state, 3987, 220);
    make_coin_body(state, 4298, 200);

    // make speed up mystery block
    make_speed_block_body(state, 3225, 325);

    // make slow down mystery block
    make_slow_block_body(state, 2525, 250);

    // make end wall
    make_end_wall_body(state, THIRD_BLOCK_COLOR);

    // make portal
    make_portal_body(state, PORTAL_CORRECTION);

    for (size_t j = 0; j < scene_bodies(state->scene); j++) {
        body_t *curr_body = scene_get_body(state->scene, j);

        if ((int) body_get_info(curr_body) == OBSTACLE) {
            create_destructive_collision(state->scene, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == GROUND || 
                   (int) body_get_info(curr_body) == MAIN_GROUND) {
            create_physics_collision(state->scene, ELASTICITY, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == PORTAL) {
            create_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == COIN){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == SPEED_MYSTERY_BLOCK){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == SLOW_MYSTERY_BLOCK){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        }
    }
    
    // make text
    list_t *third_level_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("Level 3", 60, 20, 10, 10, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LEVEL_TEXT_COLOR);
    
    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives_title = text_init(lives_message, 60, 20, 10, 470, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LIGHT_LEVEL_TEXT_COLOR);

    list_add(third_level_text, level_title);
    list_add(third_level_text, lives_title);

    scene_set_texts(state->scene, third_level_text);

    // make music
    music_sound_init(THIRD_LEVEL_MUSIC_PATH, FREQUENCY, CHUNK_SIZE, 2);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_fourth_level_rules(state_t *state) {
    // make background
    make_background(state, "assets/Level_4_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *fourth_rules_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("L e v e l    4", 300, 200, 350, 50, TTF_OpenFont(LEVEL_CHANGE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, level_title);
    text_t *rules1 = text_init("Oh no! What happened to the ground? Make ", 400, 30, 300, 250, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, rules1);
    text_t *rules2 = text_init("sure not to hit the spikes above or below", 400, 30, 300, 275, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, rules2);
    text_t *rules3 = text_init("you. Good thing you have unlimited jumps!", 400, 30, 300, 300, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, rules3);
    text_t *rules4 = text_init("Watch out, though. I have a feeling some ", 400, 30, 300, 325, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, rules4);
    text_t *rules5 = text_init("new obstacles might be coming your way.  ", 400, 30, 300, 350, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, rules5);
    scene_set_texts(state->scene, fourth_rules_text);

    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives = text_init(lives_message, 60, 20, 240, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, lives);

    char *coins_message = malloc(sizeof(char) * 50);
    sprintf(coins_message, "Coins: %zu", state->total_coins_across_game);
    text_t *coins = text_init(coins_message, 60, 20, 700, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fourth_rules_text, coins);

    if (state->plus_one_life) {
        state->plus_one_life = false;
        text_t *plus_one = text_init("Plus one life!", 200, 30, 400, 400, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
        list_add(fourth_rules_text, plus_one);
    }

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_fourth_level(state_t *state) {
    // make background
    make_background(state, "assets/Level_4_Background.png");

    // make player
    make_player_body(state, UPSIDE_DOWN_PLAYER_POS.x, UPSIDE_DOWN_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);
    body_t *player_body;

    // find player index
    size_t player_index = INFINITY;
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
        body_t *curr = scene_get_body(state->scene, i);
        if ((int)body_get_info(curr) == ROLLY_POLY) {
            player_index = i;
            player_body = curr;
        }
    }

    create_gravity(state->scene, FOURTH_GRAV_CONST, scene_get_body(state->scene, 
                   player_index));

    // make ground
    make_ground_body(state, GROUND_POS.y - 80, FOURTH_BLOCK_COLOR);
    make_ground_body(state, WINDOW.y - GROUND_POS.y + 80, FOURTH_BLOCK_COLOR);

    // make obstacles
    // first obstacle chunk
    for (size_t o1 = 0; o1 < 149; o1++) {
        make_obstacle_body(state, 20 + 34 * o1, 453, FOURTH_BLOCK_COLOR);
    }

    // second obstacle chunk
    for (size_t o2 = 0; o2 < 149; o2++) {
        make_obstacle_body(state, 20 + 34 * o2, 47, FOURTH_BLOCK_COLOR);
    }

    // make coins
    make_coin_body(state, 1500, 250);
    make_coin_body(state, 3000, 250);
    make_coin_body(state, 3750, 250);

    // make moving obstacles
    make_moving_obstacle_body(state, 1000, 500, MOVING_STAR_X_VEL, -MOVING_STAR_Y_VEL, FOURTH_BLOCK_COLOR);
    make_moving_obstacle_body(state, 2300, -50, MOVING_STAR_X_VEL, MOVING_STAR_Y_VEL, FOURTH_BLOCK_COLOR);
    make_moving_obstacle_body(state, 3000, -400, MOVING_STAR_X_VEL, MOVING_STAR_Y_VEL, FOURTH_BLOCK_COLOR);
    make_moving_obstacle_body(state, 4000, 1080, MOVING_STAR_X_VEL, -MOVING_STAR_Y_VEL, FOURTH_BLOCK_COLOR);
    make_moving_obstacle_body(state, 5000, -7000, MOVING_STAR_X_VEL, MOVING_STAR_Y_VEL, FOURTH_BLOCK_COLOR);
    make_moving_obstacle_body(state, 6100, 1300, MOVING_STAR_X_VEL, -MOVING_STAR_Y_VEL, FOURTH_BLOCK_COLOR);

    // make end wall
    make_end_wall_body(state, FOURTH_BLOCK_COLOR);
    
    // make speed up mystery block
    make_speed_block_body(state, -250, 250);

    // make slow down mystery block
    make_slow_block_body(state, -250, 250);

    // make portal
    make_portal_body(state, WINDOW.y / 2);

    for (size_t j = 0; j < scene_bodies(state->scene); j++) {
        body_t *curr_body = scene_get_body(state->scene, j);

        if ((int) body_get_info(curr_body) == OBSTACLE) {
            create_destructive_collision(state->scene, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == GROUND || 
                   (int) body_get_info(curr_body) == MAIN_GROUND) {
            create_physics_collision(state->scene, ELASTICITY, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == PORTAL) {
            create_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == COIN){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        }
    }

    // make text
    list_t *fourth_level_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("Level 4", 60, 20, 10, 10, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LIGHT_LEVEL_TEXT_COLOR);
    
    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives_title = text_init(lives_message, 60, 20, 10, 470, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LIGHT_LEVEL_TEXT_COLOR);

    list_add(fourth_level_text, level_title);
    list_add(fourth_level_text, lives_title);

    scene_set_texts(state->scene, fourth_level_text);

    // make music
    music_sound_init(FOURTH_LEVEL_MUSIC_PATH, FREQUENCY, CHUNK_SIZE, 2);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_fifth_level_rules(state_t *state) {
    // make background
    make_background(state, "assets/Level_5_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *fifth_rules_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("L e v e l    5", 300, 200, 350, 50, TTF_OpenFont(LEVEL_CHANGE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, level_title);
    text_t *rules1 = text_init("*Phew* That was tough. Let's see if you  ", 400, 30, 300, 250, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, rules1);
    text_t *rules2 = text_init("can handle this one. The whole world is  ", 400, 30, 300, 275, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, rules2);
    text_t *rules3 = text_init("turned upside down, and the flying spikes", 400, 30, 300, 300, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, rules3);
    text_t *rules4 = text_init("are still coming for you! It seems the   ", 400, 30, 300, 325, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, rules4);
    text_t *rules5 = text_init("power-ups are also back.                 ", 400, 30, 300, 350, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, rules5);
    scene_set_texts(state->scene, fifth_rules_text);

    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives = text_init(lives_message, 60, 20, 240, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, lives);

    char *coins_message = malloc(sizeof(char) * 50);
    sprintf(coins_message, "Coins: %zu", state->total_coins_across_game);
    text_t *coins = text_init(coins_message, 60, 20, 700, 425, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(fifth_rules_text, coins);

    if (state->plus_one_life) {
        state->plus_one_life = false;
        text_t *plus_one = text_init("Plus one life!", 200, 30, 400, 400, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
        list_add(fifth_rules_text, plus_one);
    }
    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_fifth_level(state_t *state) {
    // make background
    make_background(state, "assets/Level_5_Background.png");

    // make player
    make_player_body(state, UPSIDE_DOWN_PLAYER_POS.x, UPSIDE_DOWN_PLAYER_POS.y, M_PI / 6, UPSIDE_DOWN_PLAYER);
    body_t *player_body;

    // find player index
    size_t player_index = INFINITY;
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
        body_t *curr = scene_get_body(state->scene, i);
        if ((int)body_get_info(curr) == UPSIDE_DOWN_PLAYER) {
            player_index = i;
            player_body = curr;
        }
    }

    create_gravity(state->scene, FIFTH_GRAV_CONST, scene_get_body(state->scene, 
                                                                  player_index));

    // make ground
    make_ground_body(state, WINDOW.y - GROUND_POS.y, FIFTH_BLOCK_COLOR);

    // make blocks
    make_block_body(state, 700, 375, 125, 10, FIFTH_BLOCK_COLOR);
    make_block_body(state, 850, 375, 150, 6, FIFTH_BLOCK_COLOR);
    make_block_body(state, 950, 375, 130, 3, FIFTH_BLOCK_COLOR);
    make_block_body(state, 1200, 280, 50, 7, FIFTH_BLOCK_COLOR);
    make_block_body(state, 1800, 375, 100, 8, FIFTH_BLOCK_COLOR);
    make_block_body(state, 2050, 325, 50, 9, FIFTH_BLOCK_COLOR);
    make_block_body(state, 2275, 325, 50, 9, FIFTH_BLOCK_COLOR);
    make_block_body(state, 2500, 325, 50, 9, FIFTH_BLOCK_COLOR);
    make_block_body(state, 2720, 370, 125, 10, FIFTH_BLOCK_COLOR);
    make_block_body(state, 2570, 370, 150, 6, FIFTH_BLOCK_COLOR);
    make_block_body(state, 3757, 375, 312.5, 30, FIFTH_BLOCK_COLOR);
    make_block_body(state, 3750, 375, 187.5, 8, FIFTH_BLOCK_COLOR);
    make_block_body(state, 4220, 345, 50, 9, FIFTH_BLOCK_COLOR);
    make_block_body(state, 4370, 320, 50, 9, FIFTH_BLOCK_COLOR);
    make_block_body(state, 4750, 350, 50, 12, FIFTH_BLOCK_COLOR);

    // make obstacles
    // first obstacle chunk
    for (size_t o1 = 0; o1 < 8; o1++) {
        make_obstacle_body(state, 1030 + 34 * o1, 373, FIFTH_BLOCK_COLOR);
    }

    // second obstacle chunk
    for (size_t o2 = 0; o2 < 16; o2++) {
        make_obstacle_body(state, 1910 + 34 * o2, 373, FIFTH_BLOCK_COLOR);
    }

    // make coins
    make_coin_body(state, 1195, 230);
    make_coin_body(state, 2720, 250);
    make_coin_body(state, 4750, 300);

    // make moving obstacles
    make_moving_obstacle_body(state, 1400, 340, MOVING_STAR_X_VEL, -MOVING_STAR_Y_VEL, FIFTH_BLOCK_COLOR);
    make_moving_obstacle_body(state, 3800, -350, MOVING_STAR_X_VEL, MOVING_STAR_Y_VEL, FIFTH_BLOCK_COLOR);

    // make end wall
    make_end_wall_body(state, FIFTH_BLOCK_COLOR);
    
    // make speed up mystery block
    make_speed_block_body(state, 1500, 250);

    // make slow down mystery block
    make_slow_block_body(state, 3200, 280);

    // make portal
    make_portal_body(state, 270);

    for (size_t j = 0; j < scene_bodies(state->scene); j++) {
        body_t *curr_body = scene_get_body(state->scene, j);

        if ((int) body_get_info(curr_body) == OBSTACLE) {
            create_destructive_collision(state->scene, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == GROUND || 
                   (int) body_get_info(curr_body) == MAIN_GROUND) {
            create_physics_collision(state->scene, ELASTICITY, player_body, curr_body);
        } else if ((int) body_get_info(curr_body) == PORTAL) {
            create_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == COIN){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == SPEED_MYSTERY_BLOCK){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        } else if ((int) body_get_info(curr_body) == SLOW_MYSTERY_BLOCK){
            create_one_body_destructive_collision(state->scene, curr_body, player_body);
        }
    }

    // make text
    list_t *fifth_level_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("Level 5", 60, 20, 10, 10, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LIGHT_LEVEL_TEXT_COLOR);
    
    char *lives_message = malloc(sizeof(char) * 50);
    sprintf(lives_message, "Lives: %zu", state->lives);
    text_t *lives_title = text_init(lives_message, 60, 20, 10, 470, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), LEVEL_TEXT_COLOR);

    list_add(fifth_level_text, level_title);
    list_add(fifth_level_text, lives_title);

    scene_set_texts(state->scene, fifth_level_text);

    // make music
    music_sound_init(FIFTH_LEVEL_MUSIC_PATH, FREQUENCY, CHUNK_SIZE, 2);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_win_screen(state_t *state) {
    // make background
    make_background(state, "assets/Level_5_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *win_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("Y O U    W I N !", 300, 200, 350, 50, TTF_OpenFont(LEVEL_CHANGE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(win_text, level_title);
    text_t *rules1 = text_init("Congratulations! You won. You collected", 400, 30, 300, 250, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(win_text, rules1);
    char *rules2_message = malloc(sizeof(char) * 50);
    sprintf(rules2_message, "%zu coins and had %zu lives remaining! ", state->total_coins_across_game, state->lives);
    text_t *rules2 = text_init(rules2_message, 400, 30, 300, 275, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(win_text, rules2);
    text_t *rules3 = text_init("Very impressive!                       ", 400, 30, 300, 300, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(win_text, rules3);
    text_t *restart_option= text_init("Refresh page to restart!", 200, 30, 400, 400, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(win_text, restart_option);

    scene_set_texts(state->scene, win_text);
    
    sound_t* victory_sound = chunk_sound_init(VICTORY_SOUND_PATH, FREQUENCY, CHUNK_SIZE);
    list_add(state->chunks, victory_sound);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

state_t *start_lose_screen(state_t *state) {
    // make background
    make_background(state, "assets/Level_5_Background.png");

    // make player
    make_player_body(state, TEXT_BOX_PLAYER_POS.x, TEXT_BOX_PLAYER_POS.y, -M_PI / 6, ROLLY_POLY);

    // make text box
    make_text_box_body(state);

    // make text
    list_t *lose_text = list_init(0, (free_func_t) text_free);
    text_t *level_title = text_init("Y O U    L O S E", 300, 200, 350, 50, TTF_OpenFont(LEVEL_CHANGE_FONT_PATH, TITLE_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(lose_text, level_title);
    text_t *rules1 = text_init("You lost. Bummer. Still, good job - you", 400, 30, 300, 250, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(lose_text, rules1);
    char *rules2_message = malloc(sizeof(char) * 50);
    sprintf(rules2_message, "collected %zu coins! Play again soon!  ", state->total_coins_across_game);
    text_t *rules2 = text_init(rules2_message, 400, 30, 300, 275, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(lose_text, rules2);
    text_t *restart_option= text_init("Refresh page to restart!", 200, 30, 400, 400, TTF_OpenFont(INFO_FONT_PATH, INFO_FONT_SIZE), INFO_TEXT_COLOR);
    list_add(lose_text, restart_option);

    scene_set_texts(state->scene, lose_text);
    
    sound_t* loss_sound = chunk_sound_init(LOSS_SOUND_PATH, FREQUENCY, CHUNK_SIZE);
    list_add(state->chunks, loss_sound);

    // add on key
    sdl_on_key(player_on_key);

    return state;
}

void start_game(state_t *state) {
    info_t curr_level = scene_get_curr_level(state->scene);
    state->key_held = false;
    state->coins_collected_in_level = 0;

    if (curr_level == WELCOME_SCREEN){
        state = start_welcome_screen(state);
    } else if (curr_level == FIRST_RULES) {
        state = start_first_level_rules(state);
    } else if (curr_level == FIRST_LEVEL) {
        state = start_first_level(state);
    } else if (curr_level == SECOND_RULES) {
        state = start_second_level_rules(state);
    } else if (curr_level == SECOND_LEVEL) {
        state = start_second_level(state);
    } else if (curr_level == THIRD_RULES) {
        state = start_third_level_rules(state);
    } else if (curr_level == THIRD_LEVEL) {
        state = start_third_level(state);
    } else if (curr_level == FOURTH_RULES) {
        state = start_fourth_level_rules(state);
    } else if (curr_level == FOURTH_LEVEL) {
        state = start_fourth_level(state);
    } else if (curr_level == FIFTH_RULES) {
        state = start_fifth_level_rules(state);
    } else if (curr_level == FIFTH_LEVEL) {
        state = start_fifth_level(state);
    } else if (curr_level == WIN_SCREEN) {
        state = start_win_screen(state);
    } else if (curr_level == LOSE_SCREEN) {
        state = start_lose_screen(state);
    } 
}

state_t *emscripten_init() {
    sdl_init(VEC_ZERO, WINDOW);
    
    state_t *state = malloc(sizeof(state_t));

    // can be deleted if relying on welcome screen, but gives option 
    // of making different start level
    assert(state != NULL);
    state->lives = MAX_LIVES;
    state->plus_one_life = false;
    state->total_coins_across_game = 0;
    state->coins_collected_in_level = 0;
    state->total_coins_between_levels = 0;
    list_t *chunks = list_init(0, (free_func_t) sound_free);
    state->chunks = chunks;

    state->scene = scene_init();
    scene_set_curr_level(state->scene, START_LEVEL);

    // initialize everything that will reset at the beginning of each game
    start_game(state);

    return state;
}

void emscripten_main(state_t *state) {
    sdl_clear();
    
    if (scene_get_curr_level(state->scene) == WIN_SCREEN || scene_get_curr_level(state->scene) == LOSE_SCREEN) {
        exit(0);
    }

    double dt = time_since_last_tick();
    bool restart = false;
    state->time += dt;
    double scene_time = scene_get_time(state->scene);
    scene_set_time(state->scene, scene_time + dt);
    double curr_time = state->time;
    scene_set_alive(state->scene, false);
    bool hit_portal = true;
    bool speed = true;
    bool slow = true;
    vector_t curr_vel;
    vector_t new_vel;
    size_t coin_count = 0;
    body_t *player;
    info_t curr_level = scene_get_curr_level(state->scene);
    bool on_text_screen = (curr_level == WELCOME_SCREEN || 
                           curr_level == FIRST_RULES || 
                           curr_level == SECOND_RULES || 
                           curr_level == THIRD_RULES || 
                           curr_level == FOURTH_RULES || 
                           curr_level == FIFTH_RULES || 
                           curr_level == WIN_SCREEN || 
                           curr_level == LOSE_SCREEN);

    if (on_text_screen) {
        for (size_t i = 0; i < scene_bodies(state->scene); i++) {
            body_t *curr_body = scene_get_body(state->scene, i);
            
            if ((int) body_get_info(curr_body) == ROLLY_POLY || 
                (int) body_get_info(curr_body) == UPSIDE_DOWN_PLAYER) {
                player = curr_body;
            }
        }

        if (body_get_centroid(player).x < 499.9) {
            restart = true;
            if (curr_level == WELCOME_SCREEN) {
                scene_set_curr_level(state->scene, FIRST_RULES);
            } else if (curr_level == FIRST_RULES) {
                scene_set_curr_level(state->scene, FIRST_LEVEL);
            } else if (curr_level == SECOND_RULES) {
                scene_set_curr_level(state->scene, SECOND_LEVEL);
            } else if (curr_level == THIRD_RULES) {
                scene_set_curr_level(state->scene, THIRD_LEVEL);
            } else if (curr_level == FOURTH_RULES) {
                scene_set_curr_level(state->scene, FOURTH_LEVEL);
            } else if (curr_level == FIFTH_RULES) {
                scene_set_curr_level(state->scene, FIFTH_LEVEL);
            } 
        }
    } else {
        for (size_t i = 0; i < scene_bodies(state->scene); i++) {
            body_t *curr_body = scene_get_body(state->scene, i);
            
            if ((int) body_get_info(curr_body) == ROLLY_POLY || 
                (int) body_get_info(curr_body) == UPSIDE_DOWN_PLAYER) {
                scene_set_alive(state->scene, true);
                player = curr_body;
                double x_pos = body_get_centroid(player).x;
                if (x_pos < 99.9) {
                    scene_set_alive(state->scene, false);
                    restart = true;
                }
            } else if ((int) body_get_info(curr_body) == PORTAL) {
                hit_portal = false;
                curr_vel = body_get_velocity(curr_body);
            } else if ((int) body_get_info(curr_body) == COIN) {
                coin_count++;
            } else if ((int) body_get_info(curr_body) == SPEED_MYSTERY_BLOCK) {
                speed = false;
            } else if ((int) body_get_info(curr_body) == SLOW_MYSTERY_BLOCK) {
                slow = false;
            }
        }
        if (speed == true && slow == true) {
            new_vel = UNIVERSAL_VELOCITY;
        }
        else if (speed == true) {
            new_vel = SPEED_VELOCITY;
        }
        else if (slow == true) {
            new_vel = SLOW_VELOCITY;
        }
        if (speed == true || slow == true) {
            for (size_t j = 0; j < scene_bodies(state->scene); j++) {
                if (body_get_velocity(scene_get_body(state->scene, j)).x == curr_vel.x
                    && body_get_velocity(scene_get_body(state->scene, j)).y == curr_vel.y) {
                    body_set_velocity(scene_get_body(state->scene, j), new_vel);
                }
            }
        }

        if (scene_get_alive(state->scene)){
            if (curr_time > DELAY) {
                vector_t centroid = body_get_centroid(player);
                state->time = 0;
                bool is_rolly_poly = ((int) body_get_info(player) == ROLLY_POLY);

                make_trail_body(state, centroid, is_rolly_poly, TRAIL_COLOR1);
                make_trail_body(state, centroid, is_rolly_poly, TRAIL_COLOR2);
                make_trail_body(state, centroid, is_rolly_poly, TRAIL_COLOR3);
            }

            if (body_get_is_rotating(player) && ROTATION_NUM < 10) {
                if ((int) body_get_info(player) == ROLLY_POLY) {
                    body_set_rotation(player, body_get_angle(player) + 
                                      JUMP_ROTATION_CONST);
                } else {
                    body_set_rotation(player, body_get_angle(player) - 
                                      JUMP_ROTATION_CONST);
                }

                ROTATION_NUM++;

                if (ROTATION_NUM == 10) {
                    body_set_rotation(player, 0);
                    body_set_is_rotating(player, false);
                    ROTATION_NUM = 0;
                }
            } 

            for (size_t j = 0; j < scene_bodies(state->scene); j++) {
                body_t *b2 = scene_get_body(state->scene, j);

                if ((int) body_get_info(b2) == GROUND || 
                    (int) body_get_info(b2) == MAIN_GROUND) {
                    vector_t old_centroid = body_get_centroid(player);
                    vector_t new_centroid = old_centroid;

                    if ((int) body_get_info(player) == ROLLY_POLY) {
                        new_centroid = (vector_t) {old_centroid.x, old_centroid.y - 1.5};
                    } else if ((int) body_get_info(player) == UPSIDE_DOWN_PLAYER) {
                        new_centroid = (vector_t) {old_centroid.x, old_centroid.y + 1.5};
                    }
                    body_set_centroid(player, new_centroid);
                    list_t *player_shape = body_get_shape(player);
                    list_t *b2_shape = body_get_shape(b2);
                    collision_info_t collision = find_collision(player_shape, b2_shape);
                    body_set_centroid(player, old_centroid);
                    list_free(player_shape);
                    list_free(b2_shape);

                    if (!collision.collided ) {
                        body_set_sliding(player, false);
                    } else {
                        body_set_sliding(player, true);
                        body_set_hit_ground(player, true);
                        state->single_jumped = false;
                        break;
                    }
                }
            }

            for (size_t b = 0; b < scene_bodies(state->scene); b++) {
                body_t *curr_body = scene_get_body(state->scene, b);
                double x_pos = body_get_centroid(curr_body).x;

                if (x_pos < REMOVE_BODY_DISTANCE && 
                    ((int) body_get_info(curr_body) == GROUND || 
                    (int) body_get_info(curr_body) == OBSTACLE || 
                    (int) body_get_info(curr_body) == TRAIL_SHAPE)) {
                    body_remove(curr_body);
                }
            }
        } else if (!scene_get_alive(state->scene)) {
            Mix_HaltMusic();
            Mix_FreeMusic(scene_get_music(state->scene));
            Mix_CloseAudio();
            Mix_Quit();

            if (hit_portal) {
                if (state->total_coins_between_levels >= 3 && state->lives < MAX_LIVES) {
                    state->total_coins_between_levels -= 3;
                    state->lives++;
                    state->plus_one_life = true;
                    sound_t *plus_one_sound = chunk_sound_init(PLUS_ONE_SOUND_PATH, FREQUENCY, CHUNK_SIZE);
                    list_add(state->chunks, plus_one_sound);
                }
 
                if (curr_level == FIRST_LEVEL) {
                    scene_set_curr_level(state->scene, SECOND_RULES);
                    restart = true;
                } else if (curr_level == SECOND_LEVEL) {
                    scene_set_curr_level(state->scene, THIRD_RULES);
                    restart = true;
                } else if (curr_level == THIRD_LEVEL) {
                    scene_set_curr_level(state->scene, FOURTH_RULES);
                    restart = true;
                } else if (curr_level == FOURTH_LEVEL) {
                    scene_set_curr_level(state->scene, FIFTH_RULES);
                    restart = true;
                } else if (curr_level == FIFTH_LEVEL) {
                    scene_set_curr_level(state->scene, WIN_SCREEN);
                    restart = true;
                }
            } else {
                state->total_coins_between_levels -= state->coins_collected_in_level;
                state->total_coins_across_game -= state->coins_collected_in_level;

                state->lives--;
                scene_set_alive(state->scene, true);

                if (state->lives == 0) {
                    scene_set_curr_level(state->scene, LOSE_SCREEN);
                    restart = true;
                } 
                
                if (state->lives > 0) {
                    sound_t *die_sound = chunk_sound_init(DIE_SOUND_PATH, FREQUENCY, CHUNK_SIZE);  
                    list_add(state->chunks, die_sound);

                    if (curr_level == FIRST_LEVEL) {
                        scene_set_curr_level(state->scene, FIRST_RULES);
                        restart = true;
                    } else if (curr_level == SECOND_LEVEL) {
                        scene_set_curr_level(state->scene, SECOND_RULES);
                        restart = true;
                    } else if (curr_level == THIRD_LEVEL) {
                        scene_set_curr_level(state->scene, THIRD_RULES);
                        restart = true;
                    } else if (curr_level == FOURTH_LEVEL) {
                        scene_set_curr_level(state->scene, FOURTH_RULES);
                        restart = true;
                    } else if (curr_level == FIFTH_LEVEL) {
                        scene_set_curr_level(state->scene, FIFTH_RULES);
                        restart = true;
                    }
                }
            }
        }

        if (state->coins_collected_in_level != COINS_PER_LEVEL - coin_count) {
            state->coins_collected_in_level = COINS_PER_LEVEL - coin_count;
            state->total_coins_between_levels++;
            state->total_coins_across_game++;
            sound_t* coin_sound = chunk_sound_init(COIN_SOUND_PATH, FREQUENCY, CHUNK_SIZE);
            list_add(state->chunks, coin_sound);
        }
    }

    if (restart) {
        scene_set_time(state->scene, 0);

        for (size_t i = 0; i < scene_bodies(state->scene); i++) {
            body_t *body = scene_get_body(state->scene, i);
            body_remove(body);
        }

        start_game(state);
    }

    scene_tick(state->scene, dt);
    sdl_render_scene(state->scene);

    for (size_t i = 0; i < list_size(state->chunks); i++) {
        list_remove(state->chunks, i);
    }
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  list_free(scene_get_texts(state->scene));
  list_free(state->chunks);
  free(state);
}
