#include "load_image.h"

// Creates a 'drawable' texture from an image file
SDL_Texture *load_texture_from_image_file(SDL_Renderer* renderer, char *exec_dir_path_buffer, const char* file_name_only) {
  // Creating all necessary textures prior to the program run
  char *images_dir_string = "/images/"; 
  char *image_file_location = malloc(sizeof(char) * MAX_PATH); // Allocating memory for the background file location
  strncpy(image_file_location, exec_dir_path_buffer, MAX_PATH); // Copying the executables directory location string for relative import
  if(image_file_location == NULL) {
    fprintf(stderr, "\nERROR: Failed to copy the executable's directory to the string variable");
    return NULL;
  }
  strncat(image_file_location, images_dir_string, MAX_PATH); // Adding the images location dir to the location string
  strncat(image_file_location, file_name_only, MAX_PATH); // Adding the actual file name to the location string
  
  // Load the image as a surface --> areas of "graphical" memory that can be drawn
  SDL_Surface *image_surface = IMG_Load(image_file_location);
  free(image_file_location); // Deallocating the memory that was assigned to the image location string
  if(image_surface == NULL) {
    fprintf(stderr, "\nERROR: Failed to retrieve a texture from the provided file path.");
    return NULL;
  }
  
  // Create a texture from a surface
  SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);
  SDL_FreeSurface(image_surface); // Freeing the memory that was used for the surface --> No longer in use
  if(image_texture == NULL) {
    fprintf(stderr, "\nERROR: Failed to create an SDL texture from the Surface that was created");
    return NULL;
  }

  return image_texture;
}

// Creating a 'drawable' texture for each of the score numbers
NUMBER_IMAGES *load_number_image_textures_from_file(SDL_Renderer *p_renderer, char *exec_dir_path_buffer) {
  NUMBER_IMAGES *p_num_images = calloc(1, sizeof(NUMBER_IMAGES));
  p_num_images->zero = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "0_text_24x36.png");
  p_num_images->one = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "1_text_16x36.png");
  p_num_images->two = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "2_text_24x36.png");
  p_num_images->three = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "3_text_24x36.png");
  p_num_images->four = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "4_text_24x36.png");
  p_num_images->five = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "5_text_24x36.png");
  p_num_images->six = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "6_text_24x36.png");
  p_num_images->seven = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "7_text_24x36.png");
  p_num_images->eight = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "8_text_24x36.png");
  p_num_images->nine = load_texture_from_image_file(p_renderer, exec_dir_path_buffer, "9_text_24x36.png");

  return p_num_images;
}

// Freeing all memory assigned to the score number textures
void free_all_number_texture_mem(NUMBER_IMAGES *p_num_images) {
  // Checking that the passed pointer is valid
  if(p_num_images != NULL) {
    // Freeing the memory assigned to each of the numbers
    if(p_num_images->zero != NULL) { SDL_DestroyTexture(p_num_images->zero); }
    if(p_num_images->one != NULL) { SDL_DestroyTexture(p_num_images->one); }
    if(p_num_images->two != NULL) { SDL_DestroyTexture(p_num_images->two); }
    if(p_num_images->three != NULL) { SDL_DestroyTexture(p_num_images->three); }
    if(p_num_images->four != NULL) { SDL_DestroyTexture(p_num_images->four); }
    if(p_num_images->five != NULL) { SDL_DestroyTexture(p_num_images->five); }
    if(p_num_images->six != NULL) { SDL_DestroyTexture(p_num_images->six); }
    if(p_num_images->seven != NULL) { SDL_DestroyTexture(p_num_images->seven); }
    if(p_num_images->eight != NULL) { SDL_DestroyTexture(p_num_images->eight); }
    if(p_num_images->nine != NULL) { SDL_DestroyTexture(p_num_images->nine); }

    // Freeing the memory to the number-holding struct
    free(p_num_images);
  }
}

// Select number image to use
SDL_Texture *select_image_from_passed_char(NUMBER_IMAGES *p_num_images, char passed_char) {
  SDL_Texture *selected_num_image; 

  switch(passed_char) {
    case '0': 
      selected_num_image = p_num_images->zero;
      break;
    case '1': 
      selected_num_image = p_num_images->one;
      break;
    case '2':  
      selected_num_image = p_num_images->two;
      break;
    case '3': 
      selected_num_image = p_num_images->three;
      break;
    case '4':
      selected_num_image = p_num_images->four;
      break;
    case '5': 
      selected_num_image = p_num_images->five;
      break;
    case '6': 
      selected_num_image = p_num_images->six;
      break;
    case '7': 
      selected_num_image = p_num_images->seven;
      break;
    case '8': 
      selected_num_image = p_num_images->eight;
      break;
    case '9': 
      selected_num_image = p_num_images->nine;
      break;
    case 0x0:
      selected_num_image = NULL;
      break;
  }

  return selected_num_image;
}




