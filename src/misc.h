#ifndef MISC_H
#define MISC_H 

#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include "structs.h"
#include "macros.h"
#include "game.h"
#include "load_image.h"

int grab_full_dir_location_of_exe(char **p_exe_path_buffer, int exe_path_size);
int initialise_window(OBJS_TO_CLOSE *p_close_objs);
void close_sdl_mem(OBJS_TO_CLOSE *p_close_objs);

#endif // !MISC_H
