# Programarea Calculatoarelor, seria CC
# Tema2 - Snake
.PHONY: build

build: snake

snake: snake.c game_events.c menus.c
	gcc -Wall snake.c game_events.c menus.c -o snake -lcurses

clean:
	rm -f snake

run:
	./snake

