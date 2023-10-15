#include "vector.h"
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct image image_t;

image_t *image_init(char *image_name, vector_t dimensions);

vector_t image_get_dimensions(image_t *image);

bool image_get_shown(image_t *image);

SDL_Surface *image_get_sdl_surface(image_t *image);

void image_set_shown(image_t *image, bool shown);

void image_free(image_t *image);