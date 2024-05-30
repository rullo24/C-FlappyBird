#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Used for the flappy bird object
typedef struct flappy_obj {
  int size_x;
  int size_y;
  int loc_x;
  int loc_y;
  float velocity_ms_y;
} flappy_obj;

// Used for pipe object
typedef struct pipe_obj {
  float f_loc_x;
  float f_loc_y_mid;
  int size_x;
  int size_y;
  SDL_Texture* p_up_pipe_texture;
  SDL_Texture* p_down_pipe_texture;
  SDL_Rect pipe_dimensions;
  int pipe_arr_index;
} pipe_obj;

#endif // !STRUCTS_H