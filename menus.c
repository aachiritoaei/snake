#include "menus.h"

/**
 * Displays initial menu.
 */
void initial_menu(WINDOW *win){
	//clear window
	wclear(win);
	//create window
	wattron(win, A_BOLD);
	box(win, 0, 0);
	display_logo(win);
	//set white for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));
	mvwaddstr(win, 15, 2, "                       1) Start game - with obstacles");
	mvwaddstr(win, 16, 2, "                       2) Start game - without obstacles");
	mvwaddstr(win, 17, 2, "                       3) Instructions");
	mvwaddstr(win, 18, 2, "                       4) Highscores");
	mvwaddstr(win, 19, 2, "                       5) Quit");
	//intial colors
	wattron(win, COLOR_PAIR(1));
	wrefresh(win);
}
/** 
 * Displays instructions.
 */
void instructions(WINDOW *win){
	//clear window
	wclear(win);
	//create window
	wattron(win, A_BOLD);
	box(win, 0, 0);
	display_logo(win);
	//set white for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));
	mvwaddstr(win, 14, 2, "                            Instructions");
	mvwaddstr(win, 16, 2, "                           Control keys:");
	mvwaddstr(win, 17, 2, "                              W - Up");
	mvwaddstr(win, 18, 2, "                              A - Left");
	mvwaddstr(win, 19, 2, "                              S - Down");
	mvwaddstr(win, 20, 2, "                              D - Right");
	mvwaddstr(win, 21, 2, "                              P - Pause");
	mvwaddstr(win, 22, 2, "                    Press 'Q' to leave this meu.");
	//initial color
	wattron(win, COLOR_PAIR(1));
	wrefresh(win);
	//wait until q is pressed
	char c;
	do{
		c = getchar();
	}while(c != 'q');
}

/** 
 * Display the highscores.
 */
void display_highscores(WINDOW *win){
	wclear(win);
	clear();
	char c;
	int i;
	struct highscore highscore[5];
	FILE *f = fopen("highscore.dat", "r");
	
	win = initscr();
	wattron(win, A_BOLD);
	attron(COLOR_PAIR(1));
	box(win, 0, 0);
	display_logo(win);
	//set white for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));

	mvwaddstr(win, 14, 2, "                                 HIGHSCORES");
	//show first 5 highscores
	for(i = 0; i < 5; i++){
		fscanf(f, "%s %f", highscore[i].nume, &highscore[i].scor);
		mvwprintw(win, 16 + i, 22, "%s", highscore[i].nume);
		mvwprintw(win, 16 + i, 52, "%.2f", highscore[i].scor);
	}
	mvwaddstr(win, 22, 2, "                    Press 'Q' to leave this meu.");
	//initial color
	wattron(win, COLOR_PAIR(1));
	wrefresh(win);
	fclose(f);
	do{
		c = getchar();
	}while(c != 'q');
}

/** 
 * Displays the pause submenu.
 */
void pause_submenu(WINDOW *win){
	win = newwin(8, 25, BOX_ROW + 10, BOX_WIDTH);
	start_color();
	wattron(win, A_BOLD);
	//set green color for window
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	box(win, 0, 0);
	//set white color for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));

	mvwaddstr(win, 2, 1, "    Game is paused. ");
	mvwaddstr(win, 3, 1, "       Press 'P'   ");
	mvwaddstr(win, 4, 1, "     to continue.  ");
	refresh();
	wrefresh(win);
}
/** 
 * Displays the unpause submenu.
 */
void unpause_submenu(WINDOW *win){
	win = newwin(8, 25, BOX_ROW + 10, BOX_WIDTH);
	start_color();
	wattron(win, A_BOLD);
	//set green color for window
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	box(win, 0, 0);
	//set white color for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));

	mvwaddstr(win, 2, 1, "     Game unpaused.   ");
	mvwaddstr(win, 3, 1, "     Press W/A/S/D    ");
	mvwaddstr(win, 4, 1, "       to move.       ");
	refresh();
	wrefresh(win);
}
/**
 * Displays final menu.
 */
void final_menu(WINDOW *win, int length){
	char c, nume[20];
	int i = 35, j = 0;
	float scor = length * M_PI * 10;
	win = initscr();
	wclear(win);
	//set green color for window
	wattron(win, A_BOLD);
	attron(COLOR_PAIR(1));

	box(win, 0, 0);
	display_logo(win);
	//set white color for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));

	mvwaddstr(win, 14, 2, "                              Game over!");
	mvwaddstr(win, 15, 2, "                         	   Score:");
	mvwprintw(win, 15, 42, "%.2f", scor);
	//set yellow for player's name
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	wattron(win, COLOR_PAIR(3));
	mvwaddstr(win, 16, 2, "                         	   Insert name here");
	wattroff(win, COLOR_PAIR(3));

	mvwaddstr(win, 16, 2, "                          Name:");
	mvwaddstr(win, 18, 1, "                         Press 'Q' to quit.");
	mvwaddstr(win, 17, 1, "                         Press 'H' to view highscores.");

	wrefresh(win);
	echo();
	mvwaddstr(win, 16, 2, "                          Name:                         ");
	while(FOREVER){
		wmove(win, 16, i);
		scanf("%c", &c);
		if(c == '\r' || c == ' ')
			break;
		//set yellow color for player's name
		attron(A_BOLD);
		wattron(win, COLOR_PAIR(3));
		mvwprintw(win, 16, i, "%c", c);
		attroff(A_BOLD);
		wattroff(win, COLOR_PAIR(3));
		nume[j] = c;
		j++;
		i++;
		wrefresh(win);
	}
	noecho();
	//Se ordoneaza scorurile
	highscore_order(nume, scor);
}
/** 
 * Orders highscores.
 */
void highscore_order(char nume[], float scor){
	FILE *f = fopen("highscore.dat", "r");
	struct highscore highscore[6], aux;
	int i, j;
	for(i = 0; i < 5; i++)
		fscanf(f, "%s %f", highscore[i].nume, &highscore[i].scor);
	fclose(f);
	strcpy(highscore[5].nume, nume);
	highscore[i].scor = scor;
	for(i = 0; i < 5; i++)
		for(j = i +1; j < 6; j++)
			if(highscore[i].scor < highscore[j].scor){
				aux = highscore[i];
				highscore[i] = highscore[j];
				highscore[j] = aux;
			}
	f = fopen("highscore.dat", "w");
	for(i = 0; i < 5; i++)
		fprintf(f, "%s %.2f", highscore[i].nume, highscore[i].scor);
	fclose(f);
}
/**
 * Displays a submenu which shows the highest score.
 */
void display_highscore_submenu(WINDOW *win){
	FILE *f = fopen("highscore.dat", "r");
	char nume[30];
	float scor;
	fscanf(f, "%s %f", nume, &scor);
	win = newwin(5, 25, BOX_ROW, BOX_WIDTH);
	start_color();

	wattron(win, A_BOLD);
	attron(A_BOLD);
	//set green color for window
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	box(win, 0, 0);
	//set white color for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));

	mvwaddstr(win, 1, 1, "       HIGHSCORE");
	mvwprintw(win, 3, 1, "        %.2f", scor);
	fclose(f);
	refresh();
	wrefresh(win);
}
/**
 * Displays current score. 
 */
void display_current_score_submenu(WINDOW *win, float scor){
	win = newwin(5, 25, BOX_ROW +5, BOX_WIDTH);
	start_color();
	wattron(win, A_BOLD);
	attron(A_BOLD);
	//set green color for window
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	box(win, 0, 0);
	//set white color for text
	init_pair(5, COLOR_WHITE, COLOR_BLACK);
	wattron(win, COLOR_PAIR(5));

	mvwaddstr(win, 1, 1, "      SCOR CURENT");
	mvwprintw(win, 3, 1, "        %.2f", scor);
	refresh();
	wrefresh(win);
}
/**
 * Displays game's logo 
 */

void display_logo(WINDOW *win){
	mvwaddstr(win, 1, 1, "           ______     __   __     ______     __  __     ______       ");
	mvwaddstr(win, 2, 1, "          /\\  ___\\   /\\  -.\\ \\   /\\  __ \\   /\\ \\/ /    /\\  ___\\      ");
	mvwaddstr(win, 3, 1, "          \\ \\___  \\  \\ \\ \\-.  \\  \\ \\  __ \\  \\ \\  _-.   \\ \\  __\\      ");
	mvwaddstr(win, 4, 1, "           \\/\\_____\\  \\ \\_\\\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\   ");
	mvwaddstr(win, 5, 1, "            \\/_____/   \\/_/ \\/_/   \\/_/\\/_/   \\/_/\\/_/   \\/_____/    ");
	mvwaddstr(win, 6, 1, "                           __    __    __    __ ");
	mvwaddstr(win, 7, 1, "                          /  \\  /  \\  /  \\  /  \\");
	mvwaddstr(win, 8, 1, "_________________________/  __\\/  __\\/  __\\/  __\\_____________________________");
	mvwaddstr(win, 9, 1, "________________________/  /__/  /__/  /__/  /________________________________");
	mvwaddstr(win, 10, 1, "                        | / \\   / \\   / \\   / \\  \\____");
	mvwaddstr(win, 11, 1, "                        |/   \\_/   \\_/   \\_/   \\    o \\");
	mvwaddstr(win, 12, 1, "                                                \\_____/--<)");
	wrefresh(win);
}
/**
 * Displays snake ASCII ART
 */
void display_snake(WINDOW *win){
	mvwaddstr(win, 1, 1, "           ______     __   __     ______     __  __     ______       ");
	mvwaddstr(win, 2, 1, "          /\\  ___\\   /\\  -.\\ \\   /\\  __ \\   /\\ \\/ /    /\\  ___\\      ");
	mvwaddstr(win, 3, 1, "          \\ \\___  \\  \\ \\ \\-.  \\  \\ \\  __ \\  \\ \\  _-.   \\ \\  __\\      ");
	mvwaddstr(win, 4, 1, "           \\/\\_____\\  \\ \\_\\\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\   ");
	mvwaddstr(win, 5, 1, "            \\/_____/   \\/_/ \\/_/   \\/_/\\/_/   \\/_/\\/_/   \\/_____/    ");
	wrefresh(win);
}