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
int initialise_window() {
  // Initialise base SDL
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "\nERROR: Failed to intialise the SDL program lib.");
    return -1;
  }

  // Initialise SDL_image
  if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    fprintf(stderr, "\nERROR: Failed to initialise SDL_image");
    return -1;
  }
  return 0;
}


