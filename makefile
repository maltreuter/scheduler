CC	   = g++
FLAGS  = -Wall -std=c++14

all: scheduler

scheduler: scheduler.cpp
	$(CC) scheduler.cpp $(FLAGS) -o scheduler

clean:
	rm scheduler
