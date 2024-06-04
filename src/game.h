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
#include "load_image.h"

void pre_start_waiting_screen_render_loop(SDL_Renderer *renderer, bool *p_running_flag, SDL_Event *p_event, FLAPPY_OBJ *p_flappy_bird, SDL_Texture *flappy_bird_texture, SDL_Texture *background_texture);
void change_bird_position(FLAPPY_OBJ *p_flappy_bird, int game_window_height, bool *p_running_flag);
PIPE_OBJ *create_pipe(int game_window_width, int game_window_height);
void destroy_pipes(PIPE_OBJ **p_pipe_ptr_arr, int p_pipe_arr_size);
void change_pipe_positions(PIPE_OBJ **p_pipe_ptr_arr, int p_pipe_arr_size, int game_window_width);
bool render_pipes(SDL_Renderer *p_renderer, PIPE_OBJ **p_pipe_ptr_arr, SDL_Texture *p_up_pipe_texture, SDL_Texture *p_down_pipe_texture, int pipe_arr_size);
void randomise_pipe_loc_y(PIPE_OBJ *p_pipe, int game_window_height);
void handle_keypress_in_game(SDL_Event *p_event, bool *p_running_flag, FLAPPY_OBJ *p_flappy_bird);
bool simple_check_collision(SDL_Rect a, SDL_Rect b);
bool check_bird_pipe_collision(FLAPPY_OBJ *p_flappy, PIPE_OBJ **pipe_ptr_arr, int pipe_arr_size);
void flappy_score_func(FLAPPY_OBJ *p_flappy, PIPE_OBJ **pipe_ptr_arr, int pipe_arr_size, int *p_current_game_score);
void render_score(SDL_Renderer *p_renderer, OBJS_TO_CLOSE *p_close_objs, int *p_current_game_score, bool *p_running_flag);

#endif //!GAME_H