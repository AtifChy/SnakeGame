# Snake Game

This is a simple implementation of the classic Snake Game in C++ using the raylib library.

## Controls

- Use the Arrow keys to move the snake.
- Press Space to pause the game.

## Rules

- Eat the fruit to grow your snake.
- Don't run into yourself, or the game will be over.

## Building the Project

This project uses CMake for building. To build the project, follow these steps:

1. Create a build directory and navigate into it:

```sh
mkdir build
cd build
```

2. Run CMake to generate the build files:

```sh
cmake ..
```

3. Build the project:

```sh
cmake --build . --config Release
```

## Running the Game

After building the project, you can run the game with:

```sh
cd Release
./SnakeGame
```
## License

This project is open source and available under [Apache License](LICENSE).