#include "Realtime.h"

using namespace std;

Realtime::Realtime(bool hard, vector<Process> processes) {
	this->hard = hard;
	this->processes = processes;
}

Realtime::~Realtime() {

}

int Realtime::schedule() {
	int clock = 0;
	int cpu;
	bool occupied;
	Process *running = processes[0].clone();

	while(processes.size() || !run_queue.empty() || occupied) {
		// processes arrived
		while(processes[0].arrival == clock) {
			run_queue.push(processes[0]);
			processes.erase(processes.begin());
		}

		if(!occupied) {
			// process running in cpu
			running = run_queue.front().clone();
			run_queue.pop();
			cpu = running->burst;
			occupied = true;
		} else {
			if(cpu == 0) {
				// process finished burst
				occupied = false;
			} else if(running->deadline == 0) {
				// didnt finish before deadline
				occupied = false;
			} else {
				cpu--;
				running->deadline--;
			}
		}

		clock++;
	}

	return 0;
}
