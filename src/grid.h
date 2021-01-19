#ifndef GRID_H
#define GRID_H

#include <stdbool.h>

#define GRID_COLS 29
#define GRID_ROWS 11

#define GRID_CELL_NONE 0
#define GRID_CELL_BLOCK 1
#define GRID_CELL_BOMB 2

void Grid_setCell(int col, int row, short value);

short Grid_getCell(int col, int row);

bool Grid_hasCollision(int col, int row);

bool Grid_hasBlock(int col, int row);

bool Grid_hasPillar(int col, int row);

float Grid_coordToGrid(float coord);

int Grid_gridToCoord(int cell);

void Grid_clear();

#endif
