/* 2nd Homework 
 * Computer programming
 * Achiritoaei Alexandru-Adrian
 * 315CC
 */

#include "snake.h"
#include "menus.h"
#include "game_events.h"

 int main(void)
{
	int row = INIT_ROW, col = INIT_COL, new_row, new_col;
	int nrows, ncols;
	char c, dir;

	/* Set window size */
	system("resize -s 24 80");

	/* Select variables */
	int nfds, sel, k;
	fd_set read_descriptors;
	struct timeval timeout;

	/* Initialize screen */
	WINDOW *score_win, *highscore_win, *pause, *win = initscr();
	
	/* Retain max number of rows and columns */
	getmaxyx(win, nrows, ncols);

	/* Clear screen */
	clear();
	/* Pressed characters won't be seen on the screen */
	noecho();
	/* Typed characters are immediatly read, without buffering */
	cbreak();
	/* Hide cursor */	
	curs_set(0);
	/* Set color pair */
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));

	/* Display initial menu */
	MENU:initial_menu(win);
	
	/** 
	 * Choose an option from the menu
	 */
	int obstacole = 0;
	char optiune;
	OPTIUNE:
	optiune = getchar();
		switch(optiune){
			case '1':
				obstacole = 1;
				break;
			case '2':
				obstacole = 0;
				break;
			case '3':
				instructions(win);
				goto MENU;
			case '4':
				display_highscores(win);
				goto MENU;
			case '5':
				clear();
				refresh();
				endwin();
				return 0;
			case 'q':
				clear();
				refresh();
				endwin();
				return 0;
			default:
				goto OPTIUNE;
		}
	wclear(win);

	/* Game window */
	display_snake(win);
	//window dimensions;
	struct gameBorder box; 
	box.initrow = BOX_ROW;
	box.initcol = BOX_COL;
	box.height = BOX_HEIGHT;
	box.width = BOX_WIDTH;
	//draw border
	win = newwin(box.height, box.width, box.initrow, box.initcol);
	//color green
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	wattron(win, A_BOLD);
	attron(A_BOLD);
	//wborder(win, ACS_VLINE, ACS_VLINE, '_', ACS_S1,'_','_',ACS_S1,ACS_S1);
	box(win, 0, 0);
		
	/* Current score submenu */
	float scor = 5 * M_PI * 10;
	display_current_score_submenu(score_win, scor);

	/* Current highscore submenu */
	display_highscore_submenu(highscore_win);
		
	//refresh
	refresh();
	wrefresh(win);
	
	/* Initialize snake */
	struct coordinates snake[box.height * box.width];
	int length = 5;
	snake_init(snake, length, row, col);
	
	/* Initialize obstacles */
	//color blue
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	attron(COLOR_PAIR(2));
	srand(time(NULL));
	int numberOfObstacles = 50;
	struct coordinates obstacles[numberOfObstacles];
	if(obstacole == 1)
		obstacle_init(numberOfObstacles, obstacles, box, snake, length);
	attroff(COLOR_PAIR(2));
	//back to initial color
	attron(COLOR_PAIR(1));

	/* Initialize food */
	int randomrow, randomcol;
	do{
		grow_food(snake, box, &randomrow, &randomcol);
	} while (check_on_snake(snake, length, randomrow, randomcol) == 1 || check_on_snake(obstacles, numberOfObstacles, randomrow, randomcol) == 1);
	move(randomrow, randomcol);
	//color red
	init_pair(3, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(3));
	addch('*');
	//back to initial colors
	attron(COLOR_PAIR(1));

	/* Select defines */
	nfds = 1;
	FD_ZERO(&read_descriptors);
	FD_SET(KEYBOARD, &read_descriptors);
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000000;
	k = 10000;
	refresh();

	/* Wait for a key to be pressed, in order to start snake's movement */
	while(FOREVER){
		c = getchar();
		if(tolower(c) == 'w' || tolower(c) == 'a' || tolower(c) == 's' || tolower(c) == 'd' || tolower(c) == 'q')
			break;
	}
	
	/* Play until Q is pressed */
	while (FOREVER) {
		//maintain moving direction until another key is pressed
		sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
		switch(sel){
		case SELECT_EVENT:
			c = getchar();
			break;
		}

		FD_SET(KEYBOARD, &read_descriptors);
		timeout.tv_sec = 0;
		//speed increases as more food is eaten; there's a maximum speed allowed;
		if(k > 900000)
			timeout.tv_usec = 100000;
		else
			timeout.tv_usec = 1000000 - k;

		//move in the set direction, after unpause
		READY:
		
		if((tolower(c) == 'w' && dir != 's')|| (tolower(c) == 'a' && dir != 'd') || (tolower(c) == 's' && dir != 'w') || (tolower(c) == 'd' && dir != 'a') || tolower(c) == 'q' || tolower(c) == 'p')
			dir = c;
		if (tolower(dir) == 'q') {
			break;
		}
		
		/* Determine new position bassed on the key pressed */
		switch (tolower(dir)) {
		case 'a':
			new_row = row;
			if (col > 0)
				new_col = col - 1;
			break;
						
		case 'd':
			new_row = row;
			if (col + 1 < ncols)
				new_col = col + 1;
			break;
			
		case 'w':
			if (row > 0)
				new_row = row - 1;
			new_col = col;
			break;

		case 's':
			if (row + 1 < nrows)
				new_row = row + 1;
			new_col = col;
			break;

		case 'p':
		//pause option;
		//wait for a direction
			pause_submenu(pause);
			do{
				c = getchar();
			}while(c != 'p');
			unpause_submenu(pause);
			c = getchar();
			goto READY;
		}

		/* Reposition snake */
		if(new_row == randomrow && new_col == randomcol){
			length++;
			randomrow = 0;
			randomcol = 0;
			k = k + 10000;
		}
		else{
			move(snake[length - 1].row, snake[length - 1].col);
			addch(' ');
		}
		snake_redraw(snake,length, new_row, new_col);

		/* Generate new food if the current one was eaten */
		if(randomrow == 0 && randomcol == 0){
			do{
				grow_food(snake, box, &randomrow, &randomcol);
			} while (check_on_snake(snake, length, randomrow, randomcol) == 1 || check_on_snake(obstacles, numberOfObstacles, randomrow, randomcol) == 1);
			move(randomrow, randomcol);
			attron(COLOR_PAIR(3));
			addch('*');
			attron(COLOR_PAIR(1));
		}
			
		/* Verify snake position */
		if(check_border_collision(box, new_row, new_col) == 1)
			break;
		if(check_collision(snake, length, new_row, new_col) == 1)
			break;
		if(check_collision(obstacles, numberOfObstacles, new_row, new_col) == 1)
			break;

		row = new_row;
		col = new_col;
		
		/* Refresh current score */
		scor = length * M_PI * 10;
		display_current_score_submenu(score_win, scor);

		refresh();
	}
	clear();
	/* Final menu */
	final_menu(win, length);

	//Loop until Q or H is pressed 
	while(FOREVER){
		c = getchar();
		if(tolower(c) == 'q')
			break;
		if(tolower(c) == 'h'){
			display_highscores(win);
			break;
		}
	}
	delwin(win);
	endwin();
	return 0;
}