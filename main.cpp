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

const int blockSize = 26;
int screenWidth = blockSize * 36;
int screenHeight = blockSize * 28;
const int fps = 10;

enum GameScreen {
    NONE,
    WELCOME,
    SETTINGS,
    GAME,
    GAMEOVER
};
GameScreen currentScreen = NONE;

struct Position {
    int x;
    int y;
};

int score;
int highestScore;
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

bool wallCollision;
bool speedUp;
int speedUpFPS;

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
    }

    score = 0;
    highestScore = Config::get<int>({"highestScore"});
    dir = RIGHT;
    step = {blockSize, 0};

    snake = {
        {
            screenWidth / 2,
            screenHeight / 2,
        },
    };
    newFruit();

    wallCollision = Config::get<bool>({"mode", "wall"});
    speedUp = Config::get<bool>({"mode", "speed"});

    SetTargetFPS(fps);
    speedUpFPS = fps;
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
            } else {
                break;
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
        snake.front().x + step.x,
        snake.front().y + step.y,
    };

    for (const Position& segment : snake) {
        if (segment.x == next.x && segment.y == next.y) {
            currentScreen = GAMEOVER;
            break;
        }
    }

    if (wallCollision) {
        if (next.x < 0 || next.x >= screenWidth || next.y < 0 || next.y >= screenHeight) {
            currentScreen = GAMEOVER;
            return;
        }
    } else {
        next.x = (next.x + screenWidth) % screenWidth;
        next.y = (next.y + screenHeight) % screenHeight;
    }

    snake.push_front(next);

    if (next.x == fruit.x && next.y == fruit.y) {
        newFruit();
        score += 10;
        if (score > highestScore) highestScore = score;
        if (speedUp) SetTargetFPS(++speedUpFPS);
    } else {
        snake.pop_back();
    }
}

void DrawGameText(const char* text, int posX, int posY, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;

    if (posX + textWidth > screenWidth) {
        posX -= textWidth;
    }

    for (const Position& segment : snake) {
        if (segment.x < posX + textWidth && segment.x + blockSize > posX) {
            if (segment.y < posY + textHeight && segment.y + blockSize > posY) {
                color = DARKGRAY;
                break;
            }
        }
    }

    DrawText(text, posX, posY, fontSize, color);
}

void DrawCenteredText(const char* text, int verticalOffset, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;
    int textPosX = screenWidth / 2 - textWidth / 2;
    int textPosY = screenHeight / 2 - textHeight / 2 + verticalOffset;
    DrawText(text, textPosX, textPosY, fontSize, color);
}

Rectangle DrawButton(const char* text, int posX, int posY, int fontSize, Color color) {
    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;

    Rectangle button = {
        (float)(posX - textWidth / 2),
        (float)(posY - textHeight / 2),
        (float)(textWidth),
        (float)(textHeight),
    };

    DrawText(text, button.x, button.y, fontSize, color);

    return button;
}

Rectangle DrawToggleText(const char* text, int posX, int posY, int fontSize, Color color, bool toggle) {
    if (toggle) {
        text = TextFormat("%s ON", text);
    } else {
        text = TextFormat("%s OFF", text);
    }

    int textWidth = MeasureText(text, fontSize);
    int textHeight = fontSize;

    Rectangle button = {
        (float)(posX - textWidth / 2),
        (float)(posY - textHeight / 2),
        (float)(textWidth),
        (float)(textHeight),
    };

    DrawText(text, button.x, button.y, fontSize, color);

    return button;
}

void draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    // DrawFPS(10, screenHeight - 30);

    if (currentScreen == WELCOME) {
        DrawCenteredText("SNAKE GAME", -20, 40, GRAY);
        DrawCenteredText("Press ENTER to start", 30, 20, GRAY);

        Rectangle settingButton = DrawButton("Settings", screenWidth / 2, screenHeight - 100, 20, GRAY);
        if (CheckCollisionPointRec(GetMousePosition(), settingButton)) {
            DrawText("Settings", settingButton.x, settingButton.y, 20, LIGHTGRAY);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SETTINGS;
            }
        }
    } else if (currentScreen == SETTINGS) {
        DrawCenteredText("Settings", -100, 40, GRAY);

        Rectangle speedButton = DrawToggleText("Speed Up", screenWidth / 2, screenHeight / 2, 20, GRAY, speedUp);
        Rectangle wallButton = DrawToggleText("Wall Collision", screenWidth / 2, screenHeight / 2 + 40, 20, GRAY, wallCollision);
        Rectangle backButton = DrawButton("Back", screenWidth / 2, screenHeight - 100, 20, GRAY);

        if (CheckCollisionPointRec(GetMousePosition(), speedButton)) {
            DrawText("Speed Up", speedButton.x, speedButton.y, 20, LIGHTGRAY);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                speedUp = !speedUp;
                Config::set<bool>({"mode", "speed"}, speedUp);
                Config::save();
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), wallButton)) {
            DrawText("Wall Collision", wallButton.x, wallButton.y, 20, LIGHTGRAY);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                wallCollision = !wallCollision;
                Config::set<bool>({"mode", "wall"}, wallCollision);
                Config::save();
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), backButton)) {
            DrawText("Back", backButton.x, backButton.y, 20, LIGHTGRAY);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = WELCOME;
            }
        }
    } else if (currentScreen == GAME) {
        DrawGameText(TextFormat("Score: %i", score), 10, 10, blockSize, LIGHTGRAY);
        DrawGameText(TextFormat("Highest Score: %i", highestScore), screenWidth - 10, 10, blockSize, LIGHTGRAY);

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
        DrawCenteredText("GAME OVER", -20, 40, GRAY);
        if (score == highestScore) {
            DrawCenteredText("NEW HIGH SCORE!", 30, 20, GRAY);
            Config::set<int>({"highestScore"}, score);
            Config::save();
        }
    }

    EndDrawing();
}

int main() {
    initGame();

    InitWindow(screenWidth, screenHeight, "Snake Game");

    while (!WindowShouldClose()) {
        input();
        logic();
        draw();
    }

    return 0;
}