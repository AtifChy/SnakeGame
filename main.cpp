#include "config.h"

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
 */

const int screenWidth = 800;
const int screenHeight = 600;
const int blockSize = 20;

enum GAMEScreen {
    NONE,
    WELCOME,
    GAME,
    GAMEOVER,
};
GAMEScreen currentScreen = NONE;

struct Position {
    int x;
    int y;
};

int score;
Position fruit;
std::deque<Position> snake;
Position step;

enum eDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};
eDirection dir;
eDirection lastDir;

Config config;
int highestScore;

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
    if (currentScreen == NONE) {
        currentScreen = WELCOME;
        return;
    }

    score = 0;
    highestScore = config.getInt("highestScore");
    dir = RIGHT;
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
            if (dir != LEFT) {
                dir = RIGHT;
                step = {blockSize, 0};
            }
            break;
        case KEY_LEFT:
            if (dir != RIGHT) {
                dir = LEFT;
                step = {-blockSize, 0};
            }
            break;
        case KEY_UP:
            if (dir != DOWN) {
                dir = UP;
                step = {0, -blockSize};
            }
            break;
        case KEY_DOWN:
            if (dir != UP) {
                dir = DOWN;
                step = {0, blockSize};
            }
            break;
        case KEY_SPACE:
            if (dir != STOP) {
                lastDir = dir;
                dir = STOP;
            } else if (dir == STOP) {
                dir = lastDir;
            }
            break;
        case KEY_ENTER:
            if (currentScreen == GAMEOVER) {
                currentScreen = WELCOME;
            } else if (currentScreen == WELCOME) {
                currentScreen = GAME;
            }
            initGame();
            break;
        default:
            break;
    }
}

void logic() {
    if (currentScreen != GAME || dir == STOP) return;

    Position next = {
        (snake.front().x + step.x + screenWidth) % screenWidth,
        (snake.front().y + step.y + screenHeight) % screenHeight,
    };

    for (const Position& segment : snake) {
        if (segment.x == next.x && segment.y == next.y) {
            currentScreen = GAMEOVER;
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

void drawHighestScore(int posX, int posY) {
    int displayScore = highestScore;
    if (score > highestScore) {
        displayScore = score;
    }
    const char* highestScoreText = TextFormat("Highest Score: %i", displayScore);
    int highestScoreTextWidth = MeasureText(highestScoreText, 20);

    Color color = LIGHTGRAY;

    for (const Position& segment : snake) {
        if (segment.x < posX + highestScoreTextWidth && segment.x + blockSize > posX) {
            if (segment.y < posY + blockSize && segment.y + blockSize > posY) {
                color = DARKGRAY;
            }
        }
    }

    DrawText(highestScoreText, posX - highestScoreTextWidth, posY, 20, color);
}

void SaveScore() {
    if (score > highestScore) {
        config.set("highestScore", score);
        config.save();
    }
}

void DrawCenteredText(const char* text, int verticalOffset, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;
    int textPosX = screenWidth / 2 - textWidth / 2;
    int textPosY = screenHeight / 2 - textHeight / 2 + verticalOffset;
    DrawText(text, textPosX, textPosY, fontSize, color);
}

void draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (currentScreen == WELCOME) {
        DrawCenteredText("SNAKE GAME", -20, 40, GRAY);
        DrawCenteredText("Press ENTER to start", 40, 20, GRAY);
    } else if (currentScreen == GAME) {
        drawScore(10, 10);
        drawHighestScore(screenWidth - 10, 10);

        for (const Position& segment : snake) {
            if (segment.x == snake.front().x && segment.y == snake.front().y) {
                DrawRectangle(segment.x, segment.y, blockSize, blockSize, GREEN);
                continue;
            }
            DrawRectangle(segment.x, segment.y, blockSize, blockSize, DARKGREEN);
        }

        DrawRectangle(fruit.x, fruit.y, blockSize, blockSize, RED);

        if (dir == STOP) {
            DrawCenteredText("GAME PAUSED", 0, 40, GRAY);
        }
    } else if (currentScreen == GAMEOVER) {
        DrawCenteredText("GAME OVER", 0, 40, GRAY);
        SaveScore();
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