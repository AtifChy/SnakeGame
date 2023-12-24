#include <deque>
#include <iostream>
#include <raylib.h>

/*
 * Snake Game
 *
 * Controls:
 *   - Arrow keys to move
 *   - Space to pause
 *
 * Rules:
 *   - Eat the fruit to grow
 *   - Don't run into yourself
 *
 * Made by: Atif
 */

/*
 *  TODO:
 *   - Implement highest score counter
 *   - Store highest score in file
 *   - Implement welcome screen
 */

bool gameOver;
const int screenWidth = 800;
const int screenHeight = 600;
const int blockSize = 20;

struct Position {
    int x;
    int y;
};

int score;
Position fruit;
std::deque<Position> snake;

Position step;
enum eDirection {
    Up,
    Down,
    Left,
    Right,
    STOP
};
eDirection dir;

void newFruit() {
    srand(time(NULL));

    Position nextFruit;
    bool onSnake;

    do {
        onSnake = false;

        nextFruit = {
            (rand() % (screenWidth / blockSize)) * blockSize,
            (rand() % (screenHeight / blockSize)) * blockSize,
        };

        for (const Position& segment : snake) {
            if (segment.x == nextFruit.x && segment.y == nextFruit.y) {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);

    fruit = nextFruit;
}

void initGame() {
    gameOver = false;
    score = 0;
    dir = Right;
    step = {blockSize, 0};

    snake = {
        {
            screenWidth / 2,
            screenHeight / 2,
        },
    };
    newFruit();
}

void input() {
    switch (GetKeyPressed()) {
        case KEY_RIGHT:
            if (dir != Left) dir = Right;
            break;
        case KEY_LEFT:
            if (dir != Right) dir = Left;
            break;
        case KEY_UP:
            if (dir != Down) dir = Up;
            break;
        case KEY_DOWN:
            if (dir != Up) dir = Down;
            break;
        case KEY_SPACE:
            dir = STOP;
            break;
        case KEY_ENTER:
            if (gameOver) initGame();
            break;
        default:
            break;
    }
}

void logic() {
    switch (dir) {
        case Right:
            step = {blockSize, 0};
            break;
        case Left:
            step = {-blockSize, 0};
            break;
        case Up:
            step = {0, -blockSize};
            break;
        case Down:
            step = {0, blockSize};
            break;
        case STOP:
            return;
        default:
            break;
    }

    Position next = {
        (snake.front().x + step.x + screenWidth) % screenWidth,
        (snake.front().y + step.y + screenHeight) % screenHeight,
    };

    for (const Position& segment : snake) {
        if (segment.x == next.x && segment.y == next.y) {
            gameOver = true;
            break;
        }
    }

    snake.push_front(next);

    if (next.x == fruit.x && next.y == fruit.y) {
        newFruit();
        score += 10;
    } else {
        snake.pop_back();
    }
}

void drawScore(int posX, int posY) {
    const char* scoreText = TextFormat("Score: %i", score);
    int scoreTextWidth = MeasureText(scoreText, 20);

    Color color = LIGHTGRAY;

    for (const Position& segment : snake) {
        if (segment.x < posX + scoreTextWidth && segment.x + blockSize > posX) {
            if (segment.y < posY + blockSize && segment.y + blockSize > posY) {
                color = DARKGRAY;
            }
        }
    }

    DrawText(scoreText, posX, posY, 20, color);
}

void draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (!gameOver) {
        drawScore(10, 10);

        for (const Position& segment : snake) {
            if (segment.x == snake.front().x && segment.y == snake.front().y) {
                DrawRectangle(segment.x, segment.y, blockSize, blockSize, GREEN);
                continue;
            }
            DrawRectangle(segment.x, segment.y, blockSize, blockSize, DARKGREEN);
        }

        DrawRectangle(fruit.x, fruit.y, blockSize, blockSize, RED);

        if (dir == STOP) {
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
        }
    } else {
        DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
    }

    EndDrawing();
}

int main() {
    initGame();

    InitWindow(screenWidth, screenHeight, "Snake Game");
    SetTargetFPS(10);

    while (!WindowShouldClose()) {
        input();
        logic();
        draw();
    }

    return 0;
}