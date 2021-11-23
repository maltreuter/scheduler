CC	   = g++
FLAGS  = -Wall -std=c++14 -O2

all: scheduler

debug: menu.o mfqs_d.o realtime_d.o process.o
	$(CC) $(FLAGS) Menu.o Mfqs.o Realtime.o Process.o -o scheduler

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

mfqs_d.o:
	$(CC) $(FLAGS) -D DEBUG -c Mfqs.cpp

realtime_d.o:
	$(CC) $(FLAGS) -D DEBUG -c Realtime.cpp

clean:
	rm *.o gantt.txt scheduler
