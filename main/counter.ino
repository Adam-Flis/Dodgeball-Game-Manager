#include "main.hpp"

void count(int num, struct side *side) {
        int sec = getValue(num, side);
        displayNumber(sec / 10, 1, &(side->disp));
        displayNumber(sec % 10, 0, &(side->disp));
        side->disp.pixels.show();
}

int getValue(int num, struct side *side) {
        int ret = num;
        if (side->countUp) { // Count up or down
                ret = side->curMax - num;
        }
        return ret;
}