#ifndef PROCESS_H
#define PROCESS_H

class Process {
	public:
		int pid;
		int burst;
		int arrival;
		int priority;
		int deadline;
		int io;

		int og_burst;
		int queue;

		Process();
		Process(int pid, int burst, int arrival, int priority, int deadline, int io);
		~Process();
		Process* clone();
};

#endif
