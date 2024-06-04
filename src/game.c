#include "game.h"

// Looping the game starting screen until the space bar is pressed
void pre_start_waiting_screen_render_loop(SDL_Renderer *renderer, bool *p_running_flag, SDL_Event *p_event, FLAPPY_OBJ *p_flappy_bird, SDL_Texture *flappy_texture, SDL_Texture *background_texture) {
    SDL_RenderCopy(renderer, background_texture, NULL, NULL); // Rendering the game's background
    SDL_Rect flappy_hitbox = {p_flappy_bird->loc_x, p_flappy_bird->loc_y, p_flappy_bird->size_x, p_flappy_bird->size_y};
    SDL_RenderCopy(renderer, flappy_texture, NULL, &flappy_hitbox); // Rendering the bird 
    SDL_RenderPresent(renderer); // Initial render prior to game start
    while(!(*p_running_flag)) {
        if(SDL_PollEvent(p_event)) {
            if(p_event->type == SDL_KEYDOWN) {
                if(p_event->key.keysym.sym == SDLK_SPACE) {
                p_flappy_bird->velocity_ms_y = JUMP_INSTANT_SPEED_MILLISECOND; // Starting the game with a jump
                *p_running_flag = true; // Starting the game spacebar press
                }
            }
        }
    }
}

// Changing the positioning of the bird 
void change_bird_position(FLAPPY_OBJ *p_flappy_bird, int game_window_height, bool *p_running_flag) {
    float current_y_change = p_flappy_bird->velocity_ms_y * (int)WINDOW_SCALE; // Moving the bird position based on current velocity
    if(((float)p_flappy_bird->loc_y + current_y_change) >= (game_window_height - p_flappy_bird->size_y)) { // Bird hit the floor --> Game ends
        *p_running_flag = false; 
    }
    else if(((float)p_flappy_bird->loc_y + current_y_change) > 0) { // Bird at roof --> Cannot go any higher
        p_flappy_bird->loc_y = (int) ((float)p_flappy_bird->loc_y + current_y_change);
    }

    p_flappy_bird->velocity_ms_y += (float)GRAVITY_MILLISECOND_MMS2 * (float)WINDOW_SCALE; // Reducing the velocity every millisecond
}

// Creating a pipe off the right side of the screen to be brought in
PIPE_OBJ *create_pipe(int game_window_width, int game_window_height) { 
    PIPE_OBJ *p_pipe = malloc(sizeof(PIPE_OBJ)); // Allocating the memory for the pipe
    if(p_pipe == NULL) {
        fprintf(stderr, "\nERROR: Failed to assign memory for a pipe");
        return false;
    }

    // Performing necessary pre-calculations
    float screen_midpoint = game_window_height/2; // Random number assigned on initial move --> Start in the centre of the screen

    p_pipe->f_loc_x = game_window_width; // Start off the screen
    p_pipe->size_x = 50 * WINDOW_SCALE;

    // Calculating the size of the pipe textures
    p_pipe->size_y_top = game_window_height - screen_midpoint - ((PIXEL_SPACE_BETWEEN_PIPES)/2);
    p_pipe->size_y_bottom = game_window_height - screen_midpoint - ((PIXEL_SPACE_BETWEEN_PIPES)/2);

    // Calculating the starting pos of the pipe textures
    p_pipe->f_loc_y_top = 0; // Texture starts at 0
    p_pipe->f_loc_y_bottom = game_window_height - p_pipe->size_y_bottom; // Texture starts at the bottom border (game_window_height) --> Goes to just below midpoint

    // Formatting the two pipe locations on the screen (y dir)
    SDL_Rect top_pipe_dimensions = {p_pipe->f_loc_x, p_pipe->f_loc_y_top, p_pipe->size_x, p_pipe->size_y_top}; // {x_start, y_start, x_size, y_size}
    SDL_Rect bottom_pipe_dimensions = {p_pipe->f_loc_x, p_pipe->f_loc_y_bottom, p_pipe->size_x, p_pipe->size_y_bottom}; // {x_start, y_start, x_size, y_size}
    p_pipe->pipe_dimensions_top = top_pipe_dimensions;
    p_pipe->pipe_dimensions_bottom = bottom_pipe_dimensions;

    // Setting pipe use to start as false --> This is changed when the pipe starts moving
    p_pipe->in_use = false;

    return p_pipe; // Returning malloc'd memory pointer
}

// Destory the memory allocated to a pipe --> Works on p_pipe so that it is a specific destroy
void destroy_pipes(PIPE_OBJ **p_pipe_ptr_arr, int p_pipe_arr_size) {
    // O(N) destroying each pipe --> To be called at the end of the program run   
    for(int index=0; index<p_pipe_arr_size; index++) {
        if(p_pipe_ptr_arr[index] != NULL) {
            free(p_pipe_ptr_arr[index]);
        } else {
            fprintf(stderr, "\nERROR: One of the pipe array indexes is a NULL value.");
        }
    }
}

// Changing the positioning of all active pipes 
void change_pipe_positions(PIPE_OBJ **p_pipe_ptr_arr, int p_pipe_arr_size, int game_window_width) {
    // Iterating over each pipe in the pipe array
    for(int index=0; index<p_pipe_arr_size; index++) {
        PIPE_OBJ *current_pipe = p_pipe_ptr_arr[index]; // Alias for simplicities sake
        if(current_pipe == NULL) { // Checking for NULL pointers before utilising --> This should never happen following the game logic
            fprintf(stderr, "\nERROR: A NULL pointer was found in the pipe array");
            continue;
        }

        // Checking if the current pipe should be moved (in use)
        if(current_pipe->in_use == true) {
            // Checking if the pipe has moved completed out of view --> Move it to the start and set it to "not in use"
            if(((float)current_pipe->f_loc_x) <= (0 - current_pipe->size_x)) { // Current pipe moved out of view
                current_pipe->f_loc_x = game_window_width; // Move obj to off the screen on the right
                current_pipe->in_use = false; // Setting it to a static state (until called again)
                current_pipe->point_given = false; // Allowing the pipe to give points in future moves
            } else {
                // Calculating new position of the pipe obj --> Considering speed
                float pipe_x_speed = PIPE_STARTING_SPEED * WINDOW_SCALE;
                current_pipe->f_loc_x += pipe_x_speed; // Shifting the pipe ever so slightly

                // Updating the location of the two subpipes within the pipe obj
                SDL_Rect top_pipe_dimensions = {current_pipe->f_loc_x, current_pipe->f_loc_y_top, current_pipe->size_x, current_pipe->size_y_top}; 
                current_pipe->pipe_dimensions_top = top_pipe_dimensions; // Updating the top pipe dimensions
                SDL_Rect bottom_pipe_dimensions = {current_pipe->f_loc_x, current_pipe->f_loc_y_bottom, current_pipe->size_x, current_pipe->size_y_bottom}; 
                current_pipe->pipe_dimensions_bottom = bottom_pipe_dimensions; // Updating the bottom pipe dimensions
            }
        }
    }

    return;
}

// Rendering the pipe(s)
bool render_pipes(SDL_Renderer *p_renderer, PIPE_OBJ **p_pipe_ptr_arr, SDL_Texture *p_up_pipe_texture, SDL_Texture *p_down_pipe_texture, int pipe_arr_size) {
    // Checking if the provided textures are non-NULL pointers
    if(p_up_pipe_texture == NULL || p_down_pipe_texture == NULL) {
        fprintf(stderr, "\nERROR: Parsed a NULL pointer as a pipe texture to render_pipes");
        return false;
    }

    // Iterating through the pipe array and re-rendering each pipe --> This also re-renders pipes that are not in-use (off the screen)
    for(int pipe_index=0; pipe_index < pipe_arr_size; pipe_index++) {
        PIPE_OBJ *current_pipe = p_pipe_ptr_arr[pipe_index]; // Setting an alias for the current pipe in the iteration

        // Checking for NULL pointer --> This should never happen according to game logic
        if(current_pipe == NULL) { 
            fprintf(stderr, "\nERROR: A NULL pointer was found when trying to render all pipes within the pipe array.");
            continue; // Skip NULL pointers
        }

        // Re-rendering each pipe in the array 
        SDL_RenderCopy(p_renderer, p_down_pipe_texture, NULL, &(current_pipe->pipe_dimensions_top)); // Rendering the top subpipe
        SDL_RenderCopy(p_renderer, p_up_pipe_texture, NULL, &(current_pipe->pipe_dimensions_bottom)); // Rendering the bottom subpipe
    }

    return true;
}

// Randomly picking a y location for the pipe that is passed
void randomise_pipe_loc_y(PIPE_OBJ *p_pipe, int game_window_height) {

    // Calculating a random number between the two provided limits --> This will act as the midpoint of the new pipe y location
    int pipe_spawn_gap_from_borders = PIXEL_SPACE_BETWEEN_PIPES + 10; 
    int lower_limit = 0 + pipe_spawn_gap_from_borders;
    int upper_limit = game_window_height - pipe_spawn_gap_from_borders;
    int range_bound_random_num = (rand() % (upper_limit - lower_limit + 1)) + lower_limit; 
    
    // Updating the dimensions for top subpipe
    p_pipe->size_y_top = range_bound_random_num - ((PIXEL_SPACE_BETWEEN_PIPES)/2);
    p_pipe->f_loc_y_top = 0; // Texture starts at 0
    SDL_Rect top_pipe_dimensions = {p_pipe->f_loc_x, p_pipe->f_loc_y_top, p_pipe->size_x, p_pipe->size_y_top}; // {x_start, y_start, x_size, y_size}
    p_pipe->pipe_dimensions_top = top_pipe_dimensions;

    // Updating the dimensions for bottom subpipe
    p_pipe->size_y_bottom = game_window_height - p_pipe->size_y_top - (PIXEL_SPACE_BETWEEN_PIPES); // Basing bottom on size of top portion
    p_pipe->f_loc_y_bottom = game_window_height - p_pipe->size_y_bottom; // Texture starts at the bottom border (game_window_height) --> Goes to just below midpoint

    SDL_Rect bottom_pipe_dimensions = {p_pipe->f_loc_x, p_pipe->f_loc_y_bottom, p_pipe->size_x, p_pipe->size_y_bottom}; // {x_start, y_start, x_size, y_size}
    p_pipe->pipe_dimensions_bottom = bottom_pipe_dimensions;

}

// Checking for an SDL event and acting on it --> Keypress
void handle_keypress_in_game(SDL_Event *p_event, bool *p_running_flag, FLAPPY_OBJ *p_flappy_bird) {
    if(SDL_PollEvent(p_event)) {
        // Reacting to a window close function call
        if(p_event->type == SDL_QUIT) { 
        *p_running_flag = false; // Ending the program
        }
        
        // Reacting to a keypress
        if(p_event->type == SDL_KEYDOWN) {
            if(p_event->key.keysym.sym == SDLK_ESCAPE) { // Reacting to the esc key being pressed
                *p_running_flag = false; // End the program if the escape key is pressed
            }
            if(p_event->key.keysym.sym == SDLK_SPACE) {
                p_flappy_bird->velocity_ms_y = JUMP_INSTANT_SPEED_MILLISECOND; 
            }
        }
    }
}

// Collision detection function
bool simple_check_collision(SDL_Rect a, SDL_Rect b) {
    if (a.x + a.w <= b.x) return false;
    if (a.x >= b.x + b.w) return false;
    if (a.y + a.h <= b.y) return false;
    if (a.y >= b.y + b.h) return false;
    return true;
}

// Checking each of the pipes to see if a collision was detected --> Ending game if so
bool check_bird_pipe_collision(FLAPPY_OBJ *p_flappy, PIPE_OBJ **pipe_ptr_arr, int pipe_arr_size) {
    // Checking that the dimensions of each available pipe and flappy don't overlap --> Only checking those that are "in-use"
    for(int index=0; index<pipe_arr_size; index++) {
        PIPE_OBJ *current_pipe = pipe_ptr_arr[index];
        if(current_pipe->in_use) { // Only accessing the pipe if its currently being used in the game
            bool top_pipe_collision_result = simple_check_collision(p_flappy->flappy_dimensions, current_pipe->pipe_dimensions_top); 
            bool bottom_pipe_collision_result = simple_check_collision(p_flappy->flappy_dimensions, current_pipe->pipe_dimensions_bottom);

            // Checking the results of the collision checks
            if(top_pipe_collision_result || bottom_pipe_collision_result) {
                return true; // A collision was found
            }
        }
    }
    return false; // Did not find a collision
}

// Acting on a valid score
void flappy_score_func(FLAPPY_OBJ *p_flappy, PIPE_OBJ **pipe_ptr_arr, int pipe_arr_size, int *p_current_game_score) {
    // Check each pipe --> See if a point should be given
    for(int index=0; index<pipe_arr_size; index++) {
        PIPE_OBJ *p_current_pipe = pipe_ptr_arr[index];

        if(p_current_pipe->in_use) { // Avoid checking pipes that aren't being used
            // NOTE: Potential bug where float is type casted to the value just after flappy x location
            if(p_flappy->loc_x > ((int)p_current_pipe->f_loc_x + p_current_pipe->size_x)) { 
                if(!p_current_pipe->point_given) {
                    *p_current_game_score += 1; // Adding one to the variable saved in the pointer address
                    p_current_pipe->point_given = true;
                }
            }
        }
    }
} 

void render_score(SDL_Renderer *p_renderer, OBJS_TO_CLOSE *p_close_objs, int *p_current_game_score, bool *p_running_flag) {
    char current_game_score_str[10]; // 10 chars used for max int size (10 chars)

    snprintf(current_game_score_str, 10, "%d", *p_current_game_score); // Saving score to string --> snprintf(str, max_size, format_style, format_var);
    int num_digits = strlen(current_game_score_str);
    if(num_digits > 3) { // Stopping if score past 999
      *p_running_flag = false; 
    } else {
      char first_digit_char = 0x0;
      char second_digit_char = 0x0;
      char third_digit_char = 0x0;

      if(num_digits > 0) { 
      first_digit_char = current_game_score_str[num_digits - 1];
        if(num_digits > 1) { 
          second_digit_char = current_game_score_str[num_digits - 2]; 
          if(num_digits > 2) { 
            third_digit_char = current_game_score_str[num_digits - 3];
          }
        }
      }

      SDL_Texture *first_digit_texture = select_image_from_passed_char(p_close_objs->p_num_images, first_digit_char);
      if(first_digit_texture != NULL) {
        SDL_Rect first_digit_rect = (SDL_Rect){50*WINDOW_SCALE, 20*WINDOW_SCALE, 20*WINDOW_SCALE, 20*WINDOW_SCALE}; // {x, y, size_x, size_y}
        SDL_RenderCopy(p_renderer, first_digit_texture, NULL, &first_digit_rect); 
      }

      SDL_Texture *second_digit_texture = select_image_from_passed_char(p_close_objs->p_num_images, second_digit_char);
      if(second_digit_texture != NULL) {
        SDL_Rect second_digit_rect = (SDL_Rect){30*WINDOW_SCALE, 20*WINDOW_SCALE, 20*WINDOW_SCALE, 20*WINDOW_SCALE}; // {x, y, size_x, size_y}
        SDL_RenderCopy(p_renderer, second_digit_texture, NULL, &second_digit_rect); 
      }

      SDL_Texture *third_digit_texture = select_image_from_passed_char(p_close_objs->p_num_images, third_digit_char);
      if(third_digit_texture != NULL) {
        SDL_Rect third_digit_rect = (SDL_Rect){10*WINDOW_SCALE, 20*WINDOW_SCALE, 20*WINDOW_SCALE, 20*WINDOW_SCALE}; // {x, y, size_x, size_y}
        SDL_RenderCopy(p_renderer, third_digit_texture, NULL, &third_digit_rect); // Rendering the bird 
      }
    }
}
