#include "screen.h"

int main() {
    G screen;
    while (true) {
        screen.input();
        screen.clearpixels();
        screen.drawpixel(100, 100);
        screen.update();
    }
    return 0;
}
