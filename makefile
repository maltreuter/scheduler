CC	   = g++
FLAGS  = -Wall -std=c++14
MIKECC = g++-11

all: scheduler

mike: scheduler.cpp
	$(MIKECC) scheduler.cpp $(FLAGS) -o scheduler

scheduler: scheduler.cpp
	$(CC) scheduler.cpp $(FLAGS) -o scheduler

clean:
	rm scheduler
