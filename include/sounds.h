#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

typedef struct sound sound_t;

sound_t *music_sound_init(char *sound_path, size_t frequency, size_t chunk_size, size_t loops);

sound_t *chunk_sound_init(char *sound_path, size_t frequency, size_t chunk_size);

char *sound_get_path(sound_t *sound);

size_t sound_get_frequency(sound_t *sound);

size_t sound_get_chunk_size(sound_t *sound);

size_t sound_get_loops(sound_t *sound);

Mix_Music *sound_get_music(sound_t *sound);

Mix_Chunk *sound_get_wave(sound_t *sound);

void sound_free(sound_t *sound);