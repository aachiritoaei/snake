#include "game_events.h"

/* 
 > SNAKE < 
 */

/**  
 * Initialize snake at position (row, col).
 * Tail starts at the left of the head.
 * Keeps track of snake's parts.
 */
void snake_init(struct coordinates snake[], int length, int row, int col){
	int i;
	//Keep track of its body parts.
	for(i = 0; i < length; i++){
		snake[i].row = row;
		snake[i].col = col - i;
	}
	//Draw head
	move(row,col);
	addch('@');
	//Draw body
	for(i = 1; i < length; i++){
		move(snake[i].row, snake[i].col);
		addch('0');
	}
}
/**
 * Redraw snake after a change is made.
 */
void snake_redraw(struct coordinates snake[], int length, int new_row, int new_col){
	int i;
	//Keep track of its body parts.
		//tail
	for(i = length - 1; i > 0; i--){
		snake[i].row = snake[i - 1].row;
		snake[i].col = snake[i - 1].col;
	}
		//head
	snake[0].row = new_row;
	snake[0].col = new_col;

	//Move head
	move(new_row, new_col);
	addch('@');
		
	//Replace last head position with body part.
	move(snake[1].row, snake[1].col);
	addch('0');
}

/** 
 * Check snake collision with borders. 
 */
int check_border_collision(struct gameBorder box, int new_row, int new_col){
	if(new_row < box.initrow + 1 || new_row > box.initrow + box.height - 2 || new_col < box.initcol +1 || new_col > box.initcol + box.width - 2)
		return 1;
	return 0;
}

/** 
 * Check snake collision with himself. 
 */
int check_collision(struct coordinates snake[], int length, int new_row, int new_col){
	int i;
	for(i = 1; i < length; i++)
		if(new_row == snake[i].row && new_col == snake[i].col)
			return 1;
	return 0;
}


/* 
 > FOOD < 
 */

 /**
 * Generate two random coordinates on which food will be placed.
 */
void grow_food(struct coordinates snake[], struct gameBorder box, int *randomrow, int *randomcol){
	(*randomrow) = 1 + box.initrow + rand() % (box.initrow + box.height - 2 - box.initrow);
	(*randomcol) = 1 + box.initcol + rand() % (box.initcol + box.width - 2 - box.initcol);
}

/** 
 * Check generated coordinates availability(they are not allowed to be on the snake)
 */
int check_on_snake(struct coordinates snake[], int length, int randomrow, int randomcol){
	int i;
	for(i = 0; i < length; i++)
		if(snake[i].row == randomrow && snake[i].col == randomcol)
			return 1;
	return 0;
}


/* 
 > OBSTACLES <
 */

/**
* Generate two random coordinates on which an obstacle will be placed.
*/
void obstacle_grow(struct coordinates snake[], struct gameBorder box, int *randomrow, int *randomcol){
	(*randomrow) = 2 + box.initrow + rand() % (box.initrow + box.height - 4 - box.initrow);
	(*randomcol) = 2 + box.initcol + rand() % (box.initcol + box.width - 4 - box.initcol);
}
/** 
 * Verifies available position for an obstacle.
 * Verifies a paradoxal positioning of the obstacles.
 *     # x #
 *     x # x
 *     # x #
 * Paradoxal cases : snake's head on an 'x' position, impossible to avoid colliding with '#' obstacle.
 */
int obstacle_check(int randomRowObs, int randomColObs, struct coordinates obstacles[], int numberOfObstacles){
	int i, max_near_obstacle = 0;
	//max_near_obstacle allows for a single adjacent obstacle in a corner
	for(i = 0; i < numberOfObstacles; i++){
		if(obstacles[i].row == randomRowObs - 1 && obstacles[i].col == randomColObs - 1){
			if(max_near_obstacle == 1)
				return 1;
			else 
				max_near_obstacle++;
		}
		if(obstacles[i].row == randomRowObs - 1 && obstacles[i].col == randomColObs + 1){
			if(max_near_obstacle == 1)
				return 1;
			else 
				max_near_obstacle++;
		}
		if(obstacles[i].row == randomRowObs + 1 && obstacles[i].col == randomColObs - 1){
			if(max_near_obstacle == 1)
				return 1;
			else 
				max_near_obstacle++;
		}
		if(obstacles[i].row == randomRowObs + 1 && obstacles[i].col == randomColObs + 1){
			if(max_near_obstacle == 1)
				return 1;
			else 
				max_near_obstacle++;
		}
		if(obstacles[i].row == randomRowObs - 2 && obstacles[i].col == randomColObs && max_near_obstacle == 1)
				return 1;
		if(obstacles[i].row == randomRowObs + 2 && obstacles[i].col == randomColObs && max_near_obstacle == 1)
				return 1;
		if(obstacles[i].row == randomRowObs  && obstacles[i].col == randomColObs + 2 && max_near_obstacle == 1)
				return 1;
		if(obstacles[i].row == randomRowObs  && obstacles[i].col == randomColObs - 2 && max_near_obstacle == 1)
				return 1;
	}
	return 0;
}
/** 
 * Random obstacle initializing.
 * For each generated obstacle, it verifies if the selected position is available.
 */
void obstacle_init(int numberOfObstacles, struct coordinates obstacles[], struct gameBorder box, struct coordinates snake[], int length){
	int randomRowObs, randomColObs, i;
	for(i = 0; i < numberOfObstacles; i++){
		do{
			obstacle_grow(snake, box, &randomRowObs, &randomColObs);
		}while(check_on_snake(snake, length, randomRowObs, randomColObs) == 1 || obstacle_check(randomRowObs, randomColObs, obstacles, numberOfObstacles) == 1);
		obstacles[i].row = randomRowObs;
		obstacles[i].col = randomColObs;
		move(randomRowObs, randomColObs);
		addch(ACS_CKBOARD);
	}
}