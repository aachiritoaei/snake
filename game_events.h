#ifndef _GAME_EVENTS_H_
#define _GAME_EVENTS_H_

#include "snake.h"

/* Snake events */
void snake_init(struct coordinates snake[], int length, int row, int col);
void snake_redraw(struct coordinates snake[], int length, int new_row, int new_col);
int check_border_collision(struct gameBorder box, int new_row, int new_col);
int check_collision(struct coordinates snake[], int length, int new_row, int new_col);
/* Food events */
void grow_food(struct coordinates snake[], struct gameBorder box, int *randomrow, int *randomcol);
int check_on_snake(struct coordinates snake[], int length, int randomrow, int randomcol);
/* Obstacle events */
void obstacle_grow(struct coordinates snake[], struct gameBorder box, int *randomrow, int *randomcol);
int obstacle_check(int randomRowObs, int randomColObs, struct coordinates obstacles[], int numberOfObstacles);
void obstacle_init(int numberOfObstacles, struct coordinates obstacles[], struct gameBorder box, struct coordinates snake[], int length);

#endif