#ifndef LOAD_IMAGE_H
#define LOAD_IMAGE_H

#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "macros.h"

SDL_Texture *load_texture_from_image_file(SDL_Renderer* renderer, char *exec_dir_path_buffer, const char* file_name_only);

#endif // !LOAD_IMAGE_H
