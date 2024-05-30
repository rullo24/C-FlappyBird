#ifndef GAME_H
#define GAME_H

#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "macros.h"
#include "structs.h"

void pre_start_waiting_screen_render_loop(SDL_Renderer *renderer, bool *p_running_flag, SDL_Event *p_event, struct flappy_obj *p_flappy_bird, SDL_Texture *flappy_bird_texture, SDL_Texture *background_texture);
void change_bird_position(flappy_obj *p_flappy_bird, int game_window_height, bool *p_running_flag);
void *create_pipe(int game_window_width, SDL_Texture *p_up_pipe_texture, SDL_Texture *p_down_pipe_texture, pipe_obj **p_pipe_ptr_arr, int *p_current_num_pipes);
bool destroy_pipe(pipe_obj *p_pipe, pipe_obj **p_pipe_ptr_arr, int *p_current_num_pipes);
int change_pipe_position(pipe_obj *p_pipe, pipe_obj **p_pipe_ptr_arr, int *p_current_num_pipes);
bool render_pipes(SDL_Renderer *p_renderer, pipe_obj **p_pipe_ptr_arr, int num_arr_elements, SDL_Texture *p_texture);
void handle_keypress_in_game(SDL_Event *p_event, bool *p_running_flag, struct flappy_obj *p_flappy_bird);

#endif //!GAME_H