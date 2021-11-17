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

	while(processes.size() || occupied) {
		if(!occupied) {
			running = processes[0].clone();
			processes.erase(processes.begin());
			occupied = true;
		} else {
			
		}

		clock++;
	}

	return 0;
}
