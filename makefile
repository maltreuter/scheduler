CC	   = g++
FLAGS  = -Wall -std=c++14 -O2

all: scheduler

scheduler: menu.o mfqs.o realtime.o process.o
	$(CC) $(FLAGS) Menu.o Mfqs.o Realtime.o Process.o -o scheduler

menu.o: menu.cpp
	$(CC) $(FLAGS) -c menu.cpp

mfqs.o: Mfqs.cpp
	$(CC) $(FLAGS) -c Mfqs.cpp

realtime.o: Realtime.cpp
	$(CC) $(FLAGS) -c Realtime.cpp

process.o: Process.cpp
	$(CC) $(FLAGS) -c Process.cpp

clean:
	rm *.o scheduler
