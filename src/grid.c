#include "grid.h"
#include "constants.h"
#include <string.h>

static short grid[GRID_COLS * GRID_ROWS] = {GRID_CELL_NONE};

void Grid_setCell(int col, int row, short value)
{
    if (col < 0 || row < 0 || col >= GRID_COLS || row >= GRID_ROWS)
        return;
    grid[row * GRID_COLS + col] = value;
}

short Grid_getCell(int col, int row)
{
    if (col < 0 || row < 0 || col >= GRID_COLS || row >= GRID_ROWS)
        return GRID_CELL_NONE;
    return grid[row * GRID_COLS + col];
}

bool Grid_hasPillar(int col, int row)
{
    return (col < 0 || row < 0 || col >= GRID_COLS || row >= GRID_ROWS) || (col % 2 != 0 && row % 2 != 0);
}

bool Grid_hasCollision(int col, int row)
{
    return Grid_hasPillar(col, row) || Grid_getCell(col, row) != GRID_CELL_NONE;
}

bool Grid_hasBlock(int col, int row)
{
    return Grid_getCell(col, row) == GRID_CELL_BLOCK;
}

float Grid_coordToGrid(float coord)
{
    return coord / TILE_SIZE - 1;
}

int Grid_gridToCoord(int cell)
{
    return TILE_SIZE * (cell + 1);
}

void Grid_clear()
{
    memset(grid, 0, sizeof(grid));
}
