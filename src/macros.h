#ifndef MACROS_H
#define MACROS_H

// Defining all MACROS
#define MAX_NUM_PIPES 1
#define PIPE_SPAWN_INTERVAL 1.0
#define PIXEL_SPACE_BETWEEN_PIPES 110 * WINDOW_SCALE
#define PIPE_PIXEL_SPAWN_BOUND PIPE_SPACE_BETWEEN_PIPES + 30
#define WINDOW_SCALE 2 // Multiples the stock 320x480 screen by the scale
#define MAX_PATH 4096
#define GRAVITY_MILLISECOND_MMS2 0.03f
#define JUMP_INSTANT_SPEED_MILLISECOND -2.3f // 0 defined at bottom
#define PIPE_STARTING_SPEED -0.9f // 0 defined on left

#endif // !MACROS_H