#ifndef PROCESS_H
#define PROCESS_H

using namespace std;

class Process {
	public:
		int pid;
		int burst;
		int arrival;
		int priority;
		int deadline;
		int io;
		int start;

		Process() = default;

		Process(int pid, int burst, int arrival, int priority, int deadline, int io) {
			this->pid = pid;
			this->burst = burst;
			this->arrival = arrival;
			this->priority = priority;
			this->deadline = deadline;
			this->io = io;
		}

		~Process() {

		}
};

#endif
