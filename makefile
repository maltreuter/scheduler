CC	  =	g++
FLAGS = -Wall

all: scheduler

scheduler: scheduler.cpp
	$(CC) scheduler.cpp $(FLAGS) -o scheduler

clean:
	rm scheduler
