#ifndef MISC_H
#define MISC_H 

#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "macros.h"

int grab_full_dir_location_of_exe(char **p_exe_path_buffer, int exe_path_size);
int initialise_window();

#endif // !MISC_H
