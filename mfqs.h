#ifndef MFQS_H
#define MFQS_H

#include "Queue.h"

using namespace std;

class mfqs {
	public:
		int n_queues;
		int time_quantum;
		vector<Process> processes;
		vector<Queue> queues;

		mfqs(int n_queues, int time_quantum, vector<Process> processes) {
			this->n_queues = n_queues;
			this->time_quantum = time_quantum;
			this->processes = processes;

			this->queues = vector<Queue>(n_queues);
			for(int i = 0; i < n_queues - 1; i++) {
				queues[i].time_quantum = time_quantum;
				time_quantum = time_quantum * 2;
			}
			queues[n_queues - 1].rr = 0;
		}

		int queues_empty() {
			for(int i = 0; i < n_queues; i++) {
				if(queues[i].q.size() > 0) {
					return i;
				}
			}
			return -1;
		}

		int schedule() {
			int clock = 0;
			int cpu;
			bool occupied = 0;
			Process *running;
			int n_empty = 0;

			cout << "p[0]\t" << running->pid << endl;

			// while processes and !queues_empty
			while(processes.size() && n_empty >= 0) {
				int i = 0;
				cout << "clock: " << clock << endl;

				// add new processes to first queue
				while(processes[i].arrival == clock) {
					queues[0].q.push(processes[i]);
					processes.erase(processes.begin() + i);
					i++;
				}

				// if no process in cpu, add one
				if(!occupied) {
					running = queues[n_empty].q.front();
					queues[n_empty].q.pop();
					cpu = queues[n_empty].time_quantum;
					occupied = true;
					cout << "added pid: " << running.pid << " to cpu" << endl;
				} else {
					// check if cpu == 0, if not decrement cpu and process burst
					if(cpu == 0) {
						cout << "time quantum over pid: " << running.pid << endl;
						occupied = false;
						if(n_empty + 1 > n_queues) {
							queues[n_empty].q.push(running);
						} else {
							queues[n_empty + 1].q.push(running);
						}
					} else {
						running.burst--;
						if(running.burst == 0) {
							occupied = false;
							cout << "finished pid: " << running.pid << endl;
						}
						cpu--;
					}
				}

				n_empty = queues_empty();
				clock++;
			}
			return 0;
		}
};

#endif
