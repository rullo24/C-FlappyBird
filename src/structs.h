#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

// Used for the flappy bird object
typedef struct flappy_obj {
  int size_x;
  int size_y;
  int loc_x;
  int loc_y;
  float velocity_ms_y;
  SDL_Rect flappy_dimensions;
} flappy_obj;

// Used for pipe object
typedef struct pipe_obj {
  float f_loc_x;
  float f_loc_y_top;
  float f_loc_y_bottom;
  int size_x;
  int size_y_top;
  int size_y_bottom;
  SDL_Rect pipe_dimensions_top;
  SDL_Rect pipe_dimensions_bottom;
  bool in_use;
} pipe_obj;

#endif // !STRUCTS_H