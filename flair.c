#include <stdlib.h>
#include <stdio.h>
#include "finalProjectLib.h"
#include "mode0.h"
#include "print.h"

void moveUnderline(int row) {

    int tileIndexLose = 111;
    int tileIndexPause = 123; 
    int tileIndexErase = 0;

    if (row == 8) {
        for (int i = 3; i < UNDERLINE_LENGTH + 3; i++) {
            SCREENBLOCK[16].tilemap[OFFSET(i, row, 32)] = tileIndexLose;
            SCREENBLOCK[16].tilemap[OFFSET(i, 14, 32)] = tileIndexErase;
        }
    } else if (row == 14) {
        for (int i = 3; i < UNDERLINE_LENGTH + 3; i++) {
            SCREENBLOCK[16].tilemap[OFFSET(i, row, 32)] = tileIndexLose;
            SCREENBLOCK[16].tilemap[OFFSET(i, 8, 32)] = tileIndexErase;
        }
    } else if (row == 11) {
        for (int i = 7; i < UNDERLINE_LENGTH + 7; i++) {
            SCREENBLOCK[16].tilemap[OFFSET(i, row, 32)] = tileIndexPause;
            SCREENBLOCK[16].tilemap[OFFSET(i, 15, 32)] = tileIndexErase;
        }
    } else if (row == 15) {
        for (int i = 7; i < UNDERLINE_LENGTH + 7; i++) {
            SCREENBLOCK[16].tilemap[OFFSET(i, row, 32)] = tileIndexPause;
            SCREENBLOCK[16].tilemap[OFFSET(i, 11, 32)] = tileIndexErase;
        }
    }
}