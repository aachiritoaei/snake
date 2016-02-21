/* Tema 2 
 * Programarea calculatoarelor
 * Achiritoaei Alexandru-Adrian
 * Grupa 315CC
 */

#include "snake.h"
#include "menus.h"
#include "game_events.h"

 int main(void)
{
	int row = INIT_ROW, col = INIT_COL, new_row, new_col;
	int nrows, ncols;
	char c, dir;

	/* Setam dimensiunea ferestrei de joc */
	system("resize -s 24 80");

	/* Variabile pentru functia select */
	int nfds, sel, k;
	fd_set read_descriptors;
	struct timeval timeout;

	/* Se inițializează ecranul; initscr se va folosi în mod obligatoriu; se definesc variabilele WINDOW necesare */
	WINDOW *score_win, *highscore_win, *pause, *win = initscr();
	
	/* getmaxyx - este un macro, nu o funcție, așă că nu avem adresă la parametri */
	/* Se va reține în nrows și ncols numărul maxim de linii și coloane */
	getmaxyx(win, nrows, ncols);

	/* Se șterge ecranul */
	clear();
	/* Se inhibă afișarea caracterelor introduse de la tastatură */
	noecho();
	/* Caracterele introduse sunt citite imediat - fără 'buffering' */
	cbreak();
	/* Se ascunde cursorul */	
	curs_set(0);

	/* Se seteaza culorile */
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));

	/* Se afiseaza meniul intial */

	MENU:initial_menu(win);
	
	/* Se alege o optiune din meniu 
	 * Folosind 'goto' exista posibilitatea de intoarcere la meniul initial, din meniurile selectate ulterior
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

	/* Fereastra jocului*/

	//Se afiseaza logo-ul 'snake'
	display_snake(win);
	//Definire dimensiuni fereastra joc;
	struct gameBorder box; 
	box.initrow = BOX_ROW;
	box.initcol = BOX_COL;
	box.height = BOX_HEIGHT;
	box.width = BOX_WIDTH;
	//Se deseneaza chenarul in care se va misca sarpele
	win = newwin(box.height, box.width, box.initrow, box.initcol);
	//Se coloreaza verde, se seteaza atributul BOLD
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	wattron(win, A_BOLD);
	attron(A_BOLD);
	//wborder(win, ACS_VLINE, ACS_VLINE, '_', ACS_S1,'_','_',ACS_S1,ACS_S1);
	box(win, 0, 0);
		
	/* Submeniu in fereastra jocului care afiseaza scorul curent */
	float scor = 5 * M_PI * 10;
	display_current_score_submenu(score_win, scor);

	/* Submeniu in fereastra jocului care afiseaza highscore */
	display_highscore_submenu(highscore_win);
		
	//Reflectare schimbari
	refresh();
	wrefresh(win);
	
	/* Se initializeaza sarpele */
	struct coordinates snake[box.height * box.width];
	int length = 5;
	snake_init(snake, length, row, col);
	
	/* Se initializeaza obstacolele, daca este cazul */
	//Se vor colora albastru
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	attron(COLOR_PAIR(2));

	srand(time(NULL));
	int numberOfObstacles = 50;
	struct coordinates obstacles[numberOfObstacles];
	if(obstacole == 1)
		obstacle_init(numberOfObstacles, obstacles, box, snake, length);
	attroff(COLOR_PAIR(2));
	//Se revine la culoarea initiala
	attron(COLOR_PAIR(1));

	/* Se initializeaza hrana 
	 * Se vor genera coordinates random pentru hrana pana se va gasi o pozitie valida
	 * Pozitie valida inseamna ca aceasta sa nu se afle peste sarpe sau peste obstacole
	 */
	int randomrow, randomcol;
	do{
		grow_food(snake, box, &randomrow, &randomcol);
	} while (check_on_snake(snake, length, randomrow, randomcol) == 1 || check_on_snake(obstacles, numberOfObstacles, randomrow, randomcol) == 1);
	move(randomrow, randomcol);
	//Se va colora in rosu
	init_pair(3, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(3));
	addch('*');
	//Se revine la culoarea initiala
	attron(COLOR_PAIR(1));

	/* Predefiniri select */
	nfds = 1;
	FD_ZERO(&read_descriptors);
	FD_SET(KEYBOARD, &read_descriptors);
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000000;
	k = 10000;

	/* Se reflectă schimbările pe ecran */
	refresh();
	

	/* Sarpele ramane pe loc pana la primirea unei directii */
	while(FOREVER){
		c = getchar();
		if(tolower(c) == 'w' || tolower(c) == 'a' || tolower(c) == 's' || tolower(c) == 'd' || tolower(c) == 'q')
			break;
	}
	
	/* Rămânem în while până când se primește tasta q */
	while (FOREVER) {
		//Sarpele va pastra directia de miscare pana la primirea unei altei directii (W/A/S/D)
		sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
		switch(sel){
		case SELECT_EVENT:
			c = getchar();
			break;
		}

		FD_SET(KEYBOARD, &read_descriptors);
		timeout.tv_sec = 0;
		//Viteza sarpelui creste de fiecare data cand mananca o 'hrana'; exista un prag maxim;
		if(k > 900000)
			timeout.tv_usec = 100000;
		else
			timeout.tv_usec = 1000000 - k;

		//Dupa ce s-a iesit din pauza, se seteaza miscarea in directia indicata
		READY:
		
		if((tolower(c) == 'w' && dir != 's')|| (tolower(c) == 'a' && dir != 'd') || (tolower(c) == 's' && dir != 'w') || (tolower(c) == 'd' && dir != 'a') || tolower(c) == 'q' || tolower(c) == 'p')
			dir = c;
		if (tolower(dir) == 'q') {
			break;
		}
		
		/* Se determină noua poziție, în funcție de tasta apăsată
		 * Nu putem depași nrows și ncols, sau linia 0/coloana 0.
		 */
		
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
		//optiune de pauza;
		//dupa incheierea pauzei, se asteapta indicarea unei directii de mers;
			pause_submenu(pause);
			do{
				c = getchar();
			}while(c != 'p');
			unpause_submenu(pause);
			c = getchar();
			goto READY;
		}

		/* Se repozitioneaza sarpele */
			//Stergerea ultimei parti a corpului sau adaugarea uneia in cazul in care capul trece peste hrana
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

		/* Se repozitioneaza hrana, daca aceasta a fost mancata
		 * Hrana se genereaza astfel incat sa nu fie pozitionata peste obstacole sau peste sarpe
		 * Generam pozitii random, pana la intalnirea uneia care satisface conditiile cerute
		 */
		if(randomrow == 0 && randomcol == 0){
			do{
				grow_food(snake, box, &randomrow, &randomcol);
			} while (check_on_snake(snake, length, randomrow, randomcol) == 1 || check_on_snake(obstacles, numberOfObstacles, randomrow, randomcol) == 1);
			move(randomrow, randomcol);
			attron(COLOR_PAIR(3));
			addch('*');
			attron(COLOR_PAIR(1));
		}
			
		/* Se verifica pozitia sarpelui; daca se loveste de chenarul jocului, de obstacole sau de sine, jocul se va incheia */
		if(check_border_collision(box, new_row, new_col) == 1)
			break;
		if(check_collision(snake, length, new_row, new_col) == 1)
			break;
		if(check_collision(obstacles, numberOfObstacles, new_row, new_col) == 1)
			break;

		row = new_row;
		col = new_col;
		
		/* Refresh meniu scor curent */
		scor = length * M_PI * 10;
		display_current_score_submenu(score_win, scor);

		refresh();
	}
	clear();
	/* Se afiseaza meniul final */
	final_menu(win, length);

	//Ramanem in meniul final pana la apasarea tastei 'q' pentru iesire sau 'h' pentru afisarea highscore 
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