#include "vector.h"
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct text text_t;

text_t *text_init(char *message, size_t width, size_t height, int32_t x, int32_t y, TTF_Font *text_font, SDL_Color text_color);

char *text_get_message(text_t *text);

size_t text_get_width(text_t *text);

size_t text_get_height(text_t *text);

int32_t text_get_x(text_t *text);

int32_t text_get_y(text_t *text);

TTF_Font *text_get_font(text_t *text);

SDL_Color text_get_color(text_t *text);

void text_free(text_t *text);