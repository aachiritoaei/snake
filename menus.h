#ifndef _MENUS_H_
#define _MENUS_H_

#include "snake.h"

void initial_menu(WINDOW *win);
void instructions(WINDOW *win);
void final_menu(WINDOW *win, int length);
void display_highscores(WINDOW *win);
void pause_submenu(WINDOW *win);
void unpause_submenu(WINDOW *win);
void highscore_order(char nume[], float scor);
void display_highscore_submenu(WINDOW *win);
void display_current_score_submenu(WINDOW *win, float scor);
void display_logo(WINDOW *win);
void display_snake(WINDOW *win);

#endif