#include "sounds.h"
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

typedef struct sound {
    char *sound_path;
    size_t frequency;
    size_t chunk_size;
    size_t loops;
    Mix_Music *music;
    Mix_Chunk *wave;
} sound_t;

sound_t *music_sound_init(char *sound_path, size_t frequency, size_t chunk_size, size_t loops) {
    SDL_Init(SDL_INIT_AUDIO);

    sound_t *sound = malloc(sizeof(sound_t));
    sound->sound_path = sound_path;
    sound->frequency = frequency;
    sound->chunk_size = chunk_size;
    sound->loops = loops;

    int32_t open_audio = Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, chunk_size);
    assert(open_audio != -1);

    Mix_Music *music = Mix_LoadMUS(sound_path);
    sound->music = music;
    printf("%s\n",Mix_GetError());
    assert(music != NULL);

    int32_t play_music = Mix_PlayMusic(music, -1);
    assert(play_music != -1);

    return sound;
}

sound_t *chunk_sound_init(char *sound_path, size_t frequency, size_t chunk_size) {
    SDL_Init(SDL_INIT_AUDIO);

    sound_t *sound = malloc(sizeof(sound_t));
    sound->sound_path = sound_path;
    sound->frequency = frequency;
    sound->chunk_size = chunk_size;

    int32_t open_audio = Mix_OpenAudio(frequency, MIX_DEFAULT_FORMAT, 2, chunk_size);
    assert(open_audio != -1);

    Mix_Chunk *wave = Mix_LoadWAV(sound_path);
    sound->wave = wave;
    printf("%s\n",Mix_GetError());
    assert(wave != NULL);

    Mix_PlayChannel(-1, wave, 0);

    return sound;
}

char *sound_get_path(sound_t *sound) {
    char *sound_path = sound->sound_path;
    return sound_path; 
}

size_t sound_get_frequency(sound_t *sound) {
    size_t frequency = sound->frequency;
    return frequency; 
}

size_t sound_get_chunk_size(sound_t *sound) {
    size_t chunk_size = sound->chunk_size;
    return chunk_size; 
}

size_t sound_get_loops(sound_t *sound) {
    size_t loops = sound->loops;
    return loops; 
}

Mix_Music *sound_get_music(sound_t *sound) {
    Mix_Music *music = sound->music;
    return music;
}

Mix_Chunk *sound_get_wave(sound_t *sound) {
    Mix_Chunk *wave = sound->wave;
    return wave;
}

void sound_free(sound_t *sound) {
    Mix_FreeChunk(sound->wave);
    free(sound);
}