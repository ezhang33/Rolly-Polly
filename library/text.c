#include "text.h"
#include "vector.h"
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct text {
    char *message;
    size_t w;
    size_t h;
    int32_t x;
    int32_t y;
    TTF_Font *text_font;
    SDL_Color text_color;
} text_t;

text_t *text_init(char *message, size_t width, size_t height, int32_t x, int32_t y, TTF_Font *text_font, SDL_Color text_color) {
    text_t *text = malloc(sizeof(text_t));
    text->message = message;
    text->w = width;
    text->h = height;
    text->x = x;
    text->y = y;
    text->text_font = text_font;
    assert(text->text_font != NULL);
    text->text_color = text_color;

    return text;
}

char *text_get_message(text_t *text) {
    char *message = text->message;
    return message; 
}

size_t text_get_width(text_t *text) {
    size_t width = text->w;
    return width;
}

size_t text_get_height(text_t *text) {
    size_t height = text->h;
    return height;
}

int32_t text_get_x(text_t *text) {
    int32_t x = text->x;
    return x;
}

int32_t text_get_y(text_t *text) {
    int32_t y = text->y;
    return y;
}

TTF_Font *text_get_font(text_t *text) {
    TTF_Font *text_font = text->text_font;
    return text_font;
}

SDL_Color text_get_color(text_t *text) {
    SDL_Color text_color = text->text_color;
    return text_color;
}

void text_free(text_t *text) {
    TTF_CloseFont(text->text_font);
    free(text);
}