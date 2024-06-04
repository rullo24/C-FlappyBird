#include "misc.h" // Contains all necessary imports

// Uses the linux API to find the real parent directory location of the current exe --> 0 = SUCCESS | -1 = FAILED
int grab_full_dir_location_of_exe(char **p_exe_path_buffer, int exe_path_size) {
  char *exe_path_buffer = *p_exe_path_buffer; // Pointing to the provided buffer memory

  // Grabbing the location of the executable
  ssize_t bytes_read = readlink("/proc/self/exe", exe_path_buffer, exe_path_size - 1);
  if(bytes_read == -1) {
    fprintf(stderr, "\nERROR: Failed to read the location of the executable correctly.");
    return -1;
  }

  // Getting the directory location from the executable
  char *last_backslash = strrchr(exe_path_buffer, '/'); // Returns a pointer to the address of the last backslash (just before the file name)
  if(last_backslash == NULL) {
    fprintf(stderr, "\nERROR: Failed to find the exec directory via strrchr()");
    return -1;
  }

  *last_backslash = '\0'; // Removing the string portion after this backslash from the final, returned string
  
  return 0;
}

// Initialises the required SDL pre-functions
int initialise_window(OBJS_TO_CLOSE *p_close_objs) {
  // Initialise base SDL
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "\nERROR: Failed to intialise the SDL program lib.");
    return -1;
  }
  p_close_objs->sdl_init = true;

  // // Initialize SDL_ttf
  // if (TTF_Init() == -1) {
  //   printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
  //   return 1;
  // }
  // p_close_objs->ttf_init = true;

  // Initialise SDL_image
  if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    fprintf(stderr, "\nERROR: Failed to initialise SDL_image");
    return -1;
  }
  p_close_objs->img_init = true;

  return 0;
}

// Closing all necessary memory that has been allocated
void close_sdl_mem(OBJS_TO_CLOSE *p_close_objs) {
  if(p_close_objs->exec_dir_path && p_close_objs->p_exec_dir_path != NULL) {free(p_close_objs->p_exec_dir_path); }
  if(p_close_objs->pipes && p_close_objs->pipe_ptr_arr != NULL) { destroy_pipes(p_close_objs->pipe_ptr_arr, *(p_close_objs->p_pipe_arr_size)); }
  if(p_close_objs->window && p_close_objs->p_window != NULL) { SDL_DestroyWindow(p_close_objs->p_window); }
  if(p_close_objs->renderer && p_close_objs->p_renderer != NULL) { SDL_DestroyRenderer(p_close_objs->p_renderer); }
  if(p_close_objs->background_texture && p_close_objs->p_background_texture != NULL) { SDL_DestroyTexture(p_close_objs->p_background_texture); }
  if(p_close_objs->flappy_texture && p_close_objs->p_flappy_texture != NULL) {SDL_DestroyTexture(p_close_objs->p_flappy_texture); }
  if(p_close_objs->up_pipe_texture && p_close_objs->p_up_pipe_texture != NULL) { SDL_DestroyTexture(p_close_objs->p_up_pipe_texture);}
  if(p_close_objs->down_pipe_texture && p_close_objs->p_down_pipe_texture != NULL) { SDL_DestroyTexture(p_close_objs->p_down_pipe_texture);}

  if(p_close_objs->num_images && p_close_objs->p_num_images != NULL) { free_all_number_texture_mem(p_close_objs->p_num_images); }

  if(p_close_objs->sdl_init) { SDL_Quit(); }
  if(p_close_objs->img_init) { IMG_Quit(); }
  // if(p_close_objs->ttf_init) { TTF_Quit(); }

}


