#define SDL_MAIN_HANDLED
#include "screen.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 480;

inline int idx(int x, int y) {
    return y * GAME_WIDTH + x;
}

bool isAlive(const std::vector<int>& game, const int x, const int y)
{
    int alive = 0;

    if (x > 0 && game[idx(x - 1, y)] == 1) alive += 1;
    if (x < GAME_WIDTH - 1 && game[idx(x + 1, y)] == 1) alive += 1;
    if (y > 0 && game[idx(x, y - 1)] == 1) alive += 1;
    if (y < GAME_HEIGHT - 1 && game[idx(x, y + 1)] == 1) alive += 1;

    if (y > 0 && x > 0 && game[idx(x - 1, y - 1)] == 1) alive += 1;
    if (y > 0 && x < GAME_WIDTH - 1 && game[idx(x + 1, y - 1)] == 1) alive += 1;
    if (y < GAME_HEIGHT - 1 && x > 0 && game[idx(x - 1, y + 1)] == 1) alive += 1;
    if (y < GAME_HEIGHT - 1 && x < GAME_WIDTH - 1 && game[idx(x + 1, y + 1)] == 1) alive += 1;

    if (game[idx(x, y)] == 1 && alive < 2) return false;
    if (game[idx(x, y)] == 1 && (alive == 2 || alive == 3)) return true;
    if (alive > 3) return false;
    if (game[idx(x, y)] == 0 && alive == 3) return true;

    return false;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(0)));

    G screen;
    std::vector<int> display(GAME_WIDTH * GAME_HEIGHT, 0);
    std::vector<int> swap(GAME_WIDTH * GAME_HEIGHT, 0);

    // Очистим поле
    std::fill(display.begin(), display.end(), 0);

    // --- Выбор режима инициализации
    char choice;
    std::cout << "Выберите режим инициализации:\n";
    std::cout << "r - случайное заполнение\n";
    std::cout << "m - рисовать мышью\n";
    std::cout << "Ваш выбор (r/m): ";
    std::cin >> choice;

    if (choice == 'r' || choice == 'R') {
        std::generate(display.begin(), display.end(), []() { return rand() % 10 == 0 ? 1 : 0; });
    }
    else {
        bool editing = true;
        SDL_Event event;
        while (editing) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    SDL_Quit();
                    return 0;
                }
                else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    editing = false;
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mx, my;
                    SDL_GetMouseState(&mx, &my);
                    mx /= 4;
                    my /= 4;
                    if (mx >= 0 && mx < GAME_WIDTH && my >= 0 && my < GAME_HEIGHT) {
                        int id = idx(mx, my);
                        display[id] = display[id] ? 0 : 1;
                    }
                }
            }

            screen.clearpixels();
            for (int y = 0; y < GAME_HEIGHT; y++) {
                for (int x = 0; x < GAME_WIDTH; x++) {
                    if (display[idx(x, y)]) {
                        screen.drawpixel(x, y);
                    }
                }
            }
            screen.update();
        }
    }

    // --- Основной цикл игры
    while (true) {
        for (int y = 0; y < GAME_HEIGHT; y++) {
            for (int x = 0; x < GAME_WIDTH; x++) {
                swap[idx(x, y)] = isAlive(display, x, y) ? 1 : 0;
            }
        }

        for (int y = 0; y < GAME_HEIGHT; y++) {
            for (int x = 0; x < GAME_WIDTH; x++) {
                if (swap[idx(x, y)]) {
                    screen.drawpixel(x, y);
                }
            }
        }

        std::copy(swap.begin(), swap.end(), display.begin());

        screen.update();
        SDL_Delay(20);
        screen.input();
        screen.clearpixels();
    }
}
