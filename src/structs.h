#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Used for the flappy bird object
typedef struct FLAPPY_OBJ {
  int size_x;
  int size_y;
  int loc_x;
  int loc_y;
  float velocity_ms_y;
  SDL_Rect flappy_dimensions;
} FLAPPY_OBJ;

// Used for pipe object
typedef struct PIPE_OBJ {
  float f_loc_x;
  float f_loc_y_top;
  float f_loc_y_bottom;
  int size_x;
  int size_y_top;
  int size_y_bottom;
  SDL_Rect pipe_dimensions_top;
  SDL_Rect pipe_dimensions_bottom;
  bool in_use;
  bool point_given;
} PIPE_OBJ;

typedef struct NUMBER_IMAGES {
  SDL_Texture *zero;
  SDL_Texture *one;
  SDL_Texture *two;
  SDL_Texture *three;
  SDL_Texture *four;
  SDL_Texture *five;
  SDL_Texture *six;
  SDL_Texture *seven;
  SDL_Texture *eight;
  SDL_Texture *nine;
} NUMBER_IMAGES;

typedef struct OBJS_TO_CLOSE {
  bool exec_dir_path;
  char *p_exec_dir_path;

  bool sdl_init;
  bool img_init;
  bool ttf_init;

  bool pipes;
  PIPE_OBJ **pipe_ptr_arr;
  int *p_pipe_arr_size;

  bool window;
  SDL_Window *p_window;

  bool renderer; 
  SDL_Renderer *p_renderer;

  bool background_texture;
  SDL_Texture *p_background_texture;
  bool flappy_texture;
  SDL_Texture *p_flappy_texture;
  bool up_pipe_texture;
  SDL_Texture *p_up_pipe_texture;
  bool down_pipe_texture; 
  SDL_Texture *p_down_pipe_texture;

  bool num_images;
  NUMBER_IMAGES *p_num_images;
} OBJS_TO_CLOSE;

#endif // !STRUCTS_H