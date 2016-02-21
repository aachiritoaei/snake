#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <ctype.h>
#include <time.h>
#include <sys/select.h>
#include <math.h>

#define FOREVER 	1
#define INIT_ROW 	20
#define INIT_COL 	20

#define BOX_ROW 6
#define BOX_COL 0
#define BOX_HEIGHT 18
#define BOX_WIDTH 55

#define SELECT_EVENT 	1
#define SELECT_NO_EVENT 0
#define KEYBOARD 0

/** 
 * Coordinates 
 * row - row index
 * col - column index
 */
typedef struct coordinates{
	int row;
	int col;
}coordinates;

/** 
 * Game border
 * Keeps top left corner coordinates and border's width and height.
 */
typedef struct gameBorder{
	int initrow;
	int initcol;
	int width;
	int height;
} gameBorder;

/** 
 * Highschore entry
 * Player's name and score.
 */
typedef struct highscore{
	char nume[30];
	float scor;
} highscore;

#endif