#include "images.h"
#include "vector.h"
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct image {
    vector_t dimensions;
    SDL_Surface *sdl_surface;
} image_t;

image_t *image_init(char *image_name, vector_t dimensions) {
    image_t *image = malloc(sizeof(image_t));
    image->dimensions = dimensions;
    image->sdl_surface = IMG_Load(image_name);
    assert(image->sdl_surface != NULL);

    return image;
}

vector_t image_get_dimensions(image_t *image) {
    return image->dimensions;
}

SDL_Surface *image_get_sdl_surface(image_t *image) {
    return image->sdl_surface;
}

void image_free(image_t *image) {
    SDL_FreeSurface(image->sdl_surface);
    free(image);
}