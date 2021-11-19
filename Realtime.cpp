#include "Realtime.h"

using namespace std;

Realtime::Realtime(bool hard, vector<Process> processes) {
	this->hard = hard;
	this->processes = processes;
}

Realtime::~Realtime() {

}

int Realtime::find_earliest_deadline(Process *running, int clock, int &not_finished) {
	int min_deadline = -1;
	int min_index = -1;

	//loop through process list sorted by arrival time
	for(size_t i = 0; i < processes.size(); i++) {
		//if arrival time > clock, stop loop
		if(processes[i].arrival > clock) {
			break;
		}
		//if clock + burst > deadline, process will never complete, remove it
		//is this correct or should we still run it even though we know it won't complete?
		if(clock + processes[i].burst > processes[i].deadline) {
			cout << "process not finished" << endl;
			processes.erase(processes.begin() + i);
			not_finished++;
			if(hard) {
				return -2;
			}
		} else {
			//if min_deadline has not been set, or there is a process with an earlier deadline, set new min_deadline
			if(min_deadline == -1 || processes[i].deadline < min_deadline) {
				min_deadline = processes[i].deadline;
				min_index = i;
			}
		}
	}

	return min_index;
}

int Realtime::schedule() {
	int size = processes.size();
	cout << "scheduling " << size << " processes."  << endl;
	int not_finished = 0;
	int finished = 0;

	int clock = 0;
	bool occupied = false;
	Process *running = processes[0].clone();

	while(processes.size() || occupied) {
		//check this first so that we can put a new process in the cpu this clock tick
		if(occupied && clock > running->deadline) {
			not_finished++;
			cout << "process not finished" << endl;
			occupied = false;
			if(hard) {
				break;
			}
		}

		int min_index = find_earliest_deadline(running, clock, not_finished);

		if(min_index == -2) {
			break;
		}

		if(min_index >= 0) {
			if(!occupied) {
				//cpu is empty, so put min_index in it
				cout << "nothing in cpu, adding process" << endl;
				running = processes[min_index].clone();
				processes.erase(processes.begin() + min_index);
				occupied = true;
			} else {
				//min_index has earlier deadline, so put it in the cpu
				if(processes[min_index].deadline < running->deadline) {
					cout << "swapping processes" << endl;
					processes.push_back(*running);
					running = processes[min_index].clone();
					processes.erase(processes.begin() + min_index);
				}
			}
		} else {
			//nothing has changed, do nothing
		}


		if(occupied) {
			running->burst--;
			if(running->burst == 0) {
				occupied = false;
				finished++;
				cout << "pid " << running->pid << " finished" << endl;
			}
		}
		clock++;
	}

	cout << "number of processes: " << size << endl;
	cout << "number of processes finished: " << finished << endl;
	cout << "number of processes not finished: " << not_finished << endl;
	cout << "finished + not finished: " << finished + not_finished << endl;

	return 0;
}
