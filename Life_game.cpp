#define SDL_MAIN_HANDLED
#include "screen.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>

// ==================== КЛАСС ДЛЯ ХРАНЕНИЯ СОСТОЯНИЯ ====================
class LifeGrid {
private:
    int width, height;
    std::vector<int> current;
    std::vector<int> next;

    inline int idx(int x, int y) const {
        return y * width + x;
    }

    int countAliveNeighbors(int x, int y) const {
        int alive = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                    alive += current[idx(nx, ny)];
                }
            }
        }
        return alive;
    }

public:
    LifeGrid(int w, int h) : width(w), height(h), current(w* h, 0), next(w* h, 0) {}

    void clear() {
        std::fill(current.begin(), current.end(), 0);
    }

    void randomize() {
        for (auto& cell : current) {
            cell = (rand() % 10 == 0) ? 1 : 0;
        }
    }

    void setCell(int x, int y, int val) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            current[idx(x, y)] = val;
        }
    }

    int getCell(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return current[idx(x, y)];
        }
        return 0;
    }

    void update() {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int alive = countAliveNeighbors(x, y);
                int id = idx(x, y);
                if (current[id] == 1) {
                    next[id] = (alive == 2 || alive == 3) ? 1 : 0;
                }
                else {
                    next[id] = (alive == 3) ? 1 : 0;
                }
            }
        }
        current.swap(next);
    }

    const std::vector<int>& getData() const {
        return current;
    }
};

// ==================== ОСНОВНОЙ КОД ====================

const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 480;

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(0)));

    G screen;
    LifeGrid grid(GAME_WIDTH, GAME_HEIGHT);

    // Очистим поле
    grid.clear();

    // Выбор режима инициализации
    char choice;
    std::cout << "Выберите режим инициализации:\n";
    std::cout << "r - случайное заполнение\n";
    std::cout << "m - рисовать мышью\n";
    std::cout << "Ваш выбор (r/m): ";
    std::cin >> choice;

    if (choice == 'r' || choice == 'R') {
        grid.randomize();
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
                    grid.setCell(mx, my, grid.getCell(mx, my) ? 0 : 1);
                }
            }

            screen.clearpixels();
            const auto& data = grid.getData();
            for (int y = 0; y < GAME_HEIGHT; ++y) {
                for (int x = 0; x < GAME_WIDTH; ++x) {
                    if (data[y * GAME_WIDTH + x]) {
                        screen.drawpixel(x, y);
                    }
                }
            }
            screen.update();
        }
    }

    // Основной цикл игры
    while (true) {
        grid.update();

        screen.clearpixels();
        const auto& data = grid.getData();
        for (int y = 0; y < GAME_HEIGHT; ++y) {
            for (int x = 0; x < GAME_WIDTH; ++x) {
                if (data[y * GAME_WIDTH + x]) {
                    screen.drawpixel(x, y);
                }
            }
        }

        screen.update();
        SDL_Delay(20);
        screen.input();
    }
}

