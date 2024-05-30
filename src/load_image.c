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
  
  //SDL_Texture *background_texture = load_texture_from_file(renderer, image_file_location);
  
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