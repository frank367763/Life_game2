#include "screen.h"
#include <vector>

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


int main() {
    G screen;
    std::vector<int> display(GAME_WIDTH * GAME_HEIGHT, 0);
    std::vector<int> swap(GAME_WIDTH * GAME_HEIGHT, 0);

    while (true) {
        screen.input();
        screen.clearpixels();
        screen.update();
    }
    return 0;
}
