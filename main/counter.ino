#include "main.hpp"

void count(int sec, struct side *side) {
        displayNumber(sec / 10, 1, &(side->disp));
        displayNumber(sec % 10, 0, &(side->disp));
        side->disp.pixels.show();
}