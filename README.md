# C-FlappyBird

C-FlappyBird is a clone of the popular Flappy Bird game, written entirely in C using the SDL2 library for rendering. All physics and game logic are implemented manually.

## Features

- Classic Flappy Bird gameplay
- Simple 2D graphics rendered using SDL2
- Manual implementation of all game physics and logic

## Requirements
- C compiler (e.g., `gcc`)
- CMake
- Make
- SDL2 core and image libraries

## Installation

1. **Clone the repository:**
    ```sh
    git clone https://github.com/rullo24/C-FlappyBird.git
    cd C-FlappyBird
    ```

2. **Install SDL2:**
    Depending on your operating system, follow the appropriate instructions to install SDL2.

    **On Debian-based systems (e.g., Ubuntu):**

    ```sh
    sudo apt-get install libsdl2-dev
    sudo apt-get install libsdl2-image-dev
    ```

    **On Windows:**

    Download the development libraries from the [SDL2 website](https://www.libsdl.org/download-2.0.php) and follow the instructions for setting them up with your compiler.

3. **Compile the project:**

    ```sh
    cd C-FlappyBird
    cmake .
    make
    ```

## Usage
To run the game, execute the following command:

```sh
./flappybird
```

To clean the game when recompiling use:
```sh
make clean
```