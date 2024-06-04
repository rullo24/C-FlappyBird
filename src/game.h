#ifndef GAME_H
#define GAME_H

#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "macros.h"
#include "structs.h"

void pre_start_waiting_screen_render_loop(SDL_Renderer *renderer, bool *p_running_flag, SDL_Event *p_event, struct flappy_obj *p_flappy_bird, SDL_Texture *flappy_bird_texture, SDL_Texture *background_texture);
void change_bird_position(flappy_obj *p_flappy_bird, int game_window_height, bool *p_running_flag);
pipe_obj *create_pipe(int game_window_width, int game_window_height);
void destroy_pipes(pipe_obj **p_pipe_ptr_arr, int p_pipe_arr_size);
void change_pipe_positions(pipe_obj **p_pipe_ptr_arr, int p_pipe_arr_size, int game_window_width);
bool render_pipes(SDL_Renderer *p_renderer, pipe_obj **p_pipe_ptr_arr, SDL_Texture *p_up_pipe_texture, SDL_Texture *p_down_pipe_texture, int pipe_arr_size);
void randomise_pipe_loc_y(pipe_obj *p_pipe, int game_window_height);
void handle_keypress_in_game(SDL_Event *p_event, bool *p_running_flag, struct flappy_obj *p_flappy_bird);
bool simple_check_collision(SDL_Rect a, SDL_Rect b);
bool check_bird_pipe_collision(flappy_obj *p_flappy, pipe_obj **pipe_ptr_arr, int pipe_arr_size);

#endif //!GAME_H