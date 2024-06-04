// Include libraries
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "macros.h" // macros used in the main function
#include "structs.h" // Include all structs used in the main function
#include "load_image.h"
#include "game.h"
#include "misc.h"

// Start of program
int main() {
  // Initialising all necessary variables
  srand(time(NULL)); // Seeding the random generator
  int pipe_spawn_interval = PIPE_SPAWN_INTERVAL * 1000;
  bool running_flag = false; // Set to run on spacebar press
  char *exec_dir_path_buffer = malloc(sizeof(char) * MAX_PATH);
  int exec_dir_path_result = grab_full_dir_location_of_exe(&exec_dir_path_buffer, MAX_PATH);
  if(exec_dir_path_result == -1) {
    return -1; // Handle error --> Error message already printed within grab_full_location_of_exe() func
  }
  
  // Initialising SDL
  int init_result = initialise_window();
  if(init_result != 0) {
    return -1; // Close the program safely --> Error message already printed through initialise_window() func
  }

  // Window sizing definitions --> based on original game
  int game_window_width = 300 * WINDOW_SCALE;
  int game_window_height = 400 * WINDOW_SCALE;

  // Create a game window using SDL
  SDL_Window *p_window = SDL_CreateWindow("Flappy Bird (Remake)",
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        game_window_width, game_window_height, 
                                        SDL_WINDOW_SHOWN
                                        ); 
  if(p_window == NULL) { // Error checking failed window creation
    fprintf(stderr, "\nERROR: Failed to create SDL window.");
    SDL_Quit(); // NOTE: Don't need to dealloc the memory for the window first as this isn't successful.
    return -1;
  }

  // Creating a renderer for the window
  SDL_Renderer *p_renderer = SDL_CreateRenderer(p_window, -1, SDL_RENDERER_ACCELERATED); // SDL_CreateRenderer(window, index, flags)
  if(p_renderer == NULL) {
    fprintf(stderr, "\nERROR: Failed to create a renderer for the window");
    SDL_DestroyWindow(p_window);
    SDL_Quit();
    return -1;
  }

  // Creating an event in SDL
  SDL_Event event;
  
  // Creating all relevant textures and objs for game rendering
  SDL_Texture *p_background_texture = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "background_2432x1828.png");
  if(p_background_texture == NULL) {
    fprintf(stderr, "\nERROR: Failed to load in the background image");
    SDL_DestroyWindow(p_window);
    SDL_DestroyRenderer(p_renderer);
    return -1;
  }
  SDL_Texture *p_flappy_texture = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "flappy_640x460.png");
  if(p_flappy_texture == NULL) {
    fprintf(stderr, "\nERROR: Failed to load in the flappy bird image");
    SDL_DestroyWindow(p_window);
    SDL_DestroyRenderer(p_renderer);
    return -1;
  }
  SDL_Texture *p_up_pipe_texture = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "up_pipe_560x2500.png");
  if(p_up_pipe_texture == NULL) {
    fprintf(stderr, "\nERROR: Failed to load in the up pipe image");
    SDL_DestroyWindow(p_window);
    SDL_DestroyRenderer(p_renderer);
    return -1;
  }
  SDL_Texture *p_down_pipe_texture = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "down_pipe_560x2500.png");
  if(p_down_pipe_texture == NULL) {
    fprintf(stderr, "\nERROR: Failed to load in the down pipe image");
    SDL_DestroyWindow(p_window);
    SDL_DestroyRenderer(p_renderer);
    return -1;
  }
  free(exec_dir_path_buffer); // To be called after the executable path is used for the last time
  
  //////////////////////////////////////////////////////
  //////////// Object behaviour definitions ////////////
  //////////////////////////////////////////////////////
  
  // Flappy object definitions
  float flappy_width_div_height_ratio = 1.40; // Based on the flappy image pixel ratio
  flappy_obj flappy_bird = {
    .size_x = 25 * WINDOW_SCALE * flappy_width_div_height_ratio,
    .size_y = 25 * WINDOW_SCALE,
    .loc_x = (game_window_width/2) - (flappy_bird.size_x/2),
    .loc_y = (game_window_height/2) - (flappy_bird.size_y/2),
    .velocity_ms_y = 0.0f,
    .flappy_dimensions = {flappy_bird.loc_x, flappy_bird.loc_y, flappy_bird.size_x, flappy_bird.size_y},
  };

  // Pre-Game Loop --> Stored within function (game.h)
  uint32_t prev_frame_time = 0; // Initialise a variable to track time for speed relevance
  pre_start_waiting_screen_render_loop(p_renderer, &running_flag, &event, &flappy_bird, p_flappy_texture, p_background_texture); // Starting the game on space bar press

  // Creating an empty array to hold the pointers of the pipe objects that are created
  int pipe_arr_size = MAX_NUM_PIPES;
  pipe_obj *pipe_ptr_arr[pipe_arr_size]; // Array to hold all of the pipe object pointers
  
  // Creating a pipe object for each array index --> Not to be used until called upon
  for(int current_pipe_index = 0; current_pipe_index < pipe_arr_size; current_pipe_index++) {
    pipe_ptr_arr[current_pipe_index] = create_pipe(game_window_width, game_window_height); // Creating a pipe on the heap
    if(pipe_ptr_arr[current_pipe_index] == NULL) {
      fprintf(stderr, "\nERROR: Failed to create a pipe using create_pipe for index [%d]", current_pipe_index);
      return -1;
    }
  }

  // Creating main program loop
  while(running_flag) {
    // Bird Position calculations
    uint32_t current_frame_time = SDL_GetTicks(); // Returns the number of milliseconds since program start
    if(current_frame_time >= UINT32_MAX-128) {return 0;} // End the program if it reaches the max time available using SDL (~42 days) --> 64 used to avoid buffer overflow (base 2 for speed optimisations)

    // Adding a new pipe each set time interval --> Defined in macros.h
    if((current_frame_time % pipe_spawn_interval) == 0) { 
      for(int index=0; index<pipe_arr_size; index++) {
        pipe_obj *current_pipe = pipe_ptr_arr[index]; // Setting an alias for improved readability

        // Adding a new pipe if one is available --> Else continue looking in array
        if(current_pipe->in_use == false) { 
          randomise_pipe_loc_y(current_pipe, game_window_height); // Randomising the y values of the new pipe
          current_pipe->in_use = true; // Setting the pipe to start moving
          break; // Once an available pipe is found --> Stop searching
        } 
      }
    }

    // Change object positioning every 5 milliseconds
    if((current_frame_time - prev_frame_time) >= 5) { 
      change_bird_position(&flappy_bird, game_window_height, &running_flag); // Changing location of bird based on flap velocity + gravity
      change_pipe_positions(pipe_ptr_arr, pipe_arr_size, game_window_width); // Changing the location of the all of the "active" pipes --> Constant velocity
      prev_frame_time = current_frame_time; // Updating the frame_time variable each iteration
    }

    // Rendering background
    SDL_RenderCopy(p_renderer, p_background_texture, NULL, NULL); // Rendering the game's background

    // Rendering pipe(s) --> Checking for NULL pointer because of the dynamic nature of this
    render_pipes(p_renderer, pipe_ptr_arr, p_up_pipe_texture, p_down_pipe_texture, pipe_arr_size);

    // Rendering flappy
    flappy_bird.flappy_dimensions = (SDL_Rect){flappy_bird.loc_x, flappy_bird.loc_y, flappy_bird.size_x, flappy_bird.size_y}; // Updating the bird location
    SDL_RenderCopy(p_renderer, p_flappy_texture, NULL, &(flappy_bird.flappy_dimensions)); // Rendering the bird 

    // Check if the bird has collided with the hitbox of either of the pipes
    int collision_result = check_bird_pipe_collision(&flappy_bird, pipe_ptr_arr, pipe_arr_size);
    if(collision_result) { running_flag = false; } // Game has ended

    // Updating the screen based on the render calculations
    SDL_RenderPresent(p_renderer);

    // Acting on key presses in game
    handle_keypress_in_game(&event, &running_flag, &flappy_bird); 
  }
  
  // User-created memory frees
  destroy_pipes(pipe_ptr_arr, pipe_arr_size); // Freeing the memory assigned on the heap for all of the pipes


  // SDL memory frees
  SDL_DestroyWindow(p_window);
  SDL_DestroyRenderer(p_renderer);
  SDL_DestroyTexture(p_background_texture);
  SDL_DestroyTexture(p_flappy_texture);
  SDL_DestroyTexture(p_up_pipe_texture);
  SDL_DestroyTexture(p_down_pipe_texture);
  SDL_Quit();

  return 0;
}
