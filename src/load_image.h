#ifndef LOAD_IMAGE_H
#define LOAD_IMAGE_H

#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "macros.h"
#include "structs.h"

SDL_Texture *load_texture_from_image_file(SDL_Renderer* renderer, char *exec_dir_path_buffer, const char* file_name_only);
NUMBER_IMAGES *load_number_image_textures_from_file(SDL_Renderer *p_renderer, char *exec_dir_path_buffer);
void free_all_number_texture_mem(NUMBER_IMAGES *p_num_images);
SDL_Texture *select_image_from_passed_char(NUMBER_IMAGES *p_num_images, char passed_char);

#endif // !LOAD_IMAGE_H
