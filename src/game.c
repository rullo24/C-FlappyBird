#include "game.h"

// Looping the game starting screen until the space bar is pressed
void pre_start_waiting_screen_render_loop(SDL_Renderer *renderer, bool *p_running_flag, SDL_Event *p_event, struct flappy_obj *p_flappy_bird, SDL_Texture *flappy_texture, SDL_Texture *background_texture) {
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
void change_bird_position(flappy_obj *p_flappy_bird, int game_window_height, bool *p_running_flag) {
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
void *create_pipe(int game_window_width, SDL_Texture *p_up_pipe_texture, SDL_Texture *p_down_pipe_texture, pipe_obj **p_pipe_ptr_arr, int *p_current_num_pipes) {
    if(*p_current_num_pipes >= MAX_NUM_PIPES) {
        fprintf(stderr, "\nERROR: Tried to add one too many pipes");
        return NULL;
    }

    pipe_obj *p_pipe = malloc(sizeof(pipe_obj)); // Allocating the memory for the pipe
    if(p_pipe == NULL) {
        fprintf(stderr, "\nERROR: Failed to assign memory for a pipe");
        return NULL;
    }

    p_pipe->f_loc_x = game_window_width; // Start off the screen
    p_pipe->f_loc_y_mid = 100; // Random number to assigned --> Needs to be randomised
    p_pipe->size_x = 50 * WINDOW_SCALE;
    p_pipe->size_y = 50 * WINDOW_SCALE; // Temporary value --> To be dynamically changed based on rand choice
    p_pipe->p_up_pipe_texture = p_up_pipe_texture;
    p_pipe->p_down_pipe_texture = p_down_pipe_texture;
    // NOTE: p_pipe_pipe_dimensions set dynamically when moving the pipe

    for(int pipe_index=0; pipe_index < MAX_NUM_PIPES; pipe_index++) {
        if(p_pipe_ptr_arr[pipe_index] != NULL) {
            continue; // Move along if occupied
        }
        p_pipe->pipe_arr_index = pipe_index;
        p_pipe_ptr_arr[pipe_index] = p_pipe; // Setting the available array location to the newly formed pointer
    }

    *p_current_num_pipes += 1;

    return p_pipe;
}

// Destory the memory allocated to a pipe
bool destroy_pipe(pipe_obj *p_pipe, pipe_obj **p_pipe_ptr_arr, int *p_current_num_pipes) {
    if(p_pipe == NULL) {
        fprintf(stderr, "\nERROR: Tried to destroy a memoryless pipe");
        return false;
    }
    p_pipe_ptr_arr[p_pipe->pipe_arr_index] = NULL;

    *p_current_num_pipes -= 1;
    free(p_pipe);

    return true;
}

// Changing the positioning of a pipe --> Returns: -1=ERROR; 0=SUCCESS; 1=DestroyedPipe
int change_pipe_position(pipe_obj *p_pipe, pipe_obj **p_pipe_ptr_arr, int *p_current_num_pipes) {
    if(p_pipe == NULL) {
        fprintf(stderr, "\nERROR: Tried to change the position of a non-existent pipe");
        return -1;
    }

    // Checking if the pipe has moved completed out of view --> despawn
    if(((float)p_pipe->f_loc_x) <= (0 - p_pipe->size_x)) { 
        bool destroy_result = destroy_pipe(p_pipe, p_pipe_ptr_arr, p_current_num_pipes); // Destroying the pipe's memory --> Should not be a failed pipe
        if(destroy_result == false) {
            fprintf(stderr, "\nERROR: Failed to destroy the pipe as it DNE");
            return -1;
        }
        return 1; // Destroy pipe
    }

    float pipe_x_speed = PIPE_STARTING_SPEED * WINDOW_SCALE;
    p_pipe->f_loc_x += pipe_x_speed; // Shifting the pipe ever so slightly

    SDL_Rect local_pipe_dimensions = {p_pipe->f_loc_x, p_pipe->f_loc_y_mid, p_pipe->size_x, p_pipe->size_y}; // Creating the SDL dimensions for texture print
    p_pipe->pipe_dimensions = local_pipe_dimensions;

    return 0;
}

// Rendering the pipe(s)
bool render_pipes(SDL_Renderer *p_renderer, pipe_obj **p_pipe_ptr_arr, int num_arr_elements, SDL_Texture *p_texture) {
    // int size_of_pipe_obj_ptr = sizeof(pipe_obj*);

    for(int pipe_index=0; pipe_index < num_arr_elements; pipe_index++) {
        if(p_pipe_ptr_arr[pipe_index] == NULL) {
            continue; // Skip NULL pointers
        }

        SDL_RenderCopy(p_renderer, p_texture, NULL, &(p_pipe_ptr_arr[pipe_index]->pipe_dimensions));
    }

    return true;
}

// Checking for an SDL event and acting on it --> Keypress
void handle_keypress_in_game(SDL_Event *p_event, bool *p_running_flag, struct flappy_obj *p_flappy_bird) {
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