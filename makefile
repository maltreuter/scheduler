CC	   = g++
FLAGS  = -Wall -std=c++14

all: menu

menu: menu.cpp
	$(CC) menu.cpp $(FLAGS) -o menu

clean:
	rm menu
