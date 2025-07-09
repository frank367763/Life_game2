#include "screen.h"
#include <vector>

const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 480;

inline int idx(int x, int y) {
    return y * GAME_WIDTH + x;
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
