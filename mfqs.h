#ifndef MFQS_H
#define MFQS_H

#include <iostream>
#include <vector>

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

		int add_to_queue_n(Process p, int n) {
			queues[n].q.push(p);
			return p.pid;
		}

		int schedule() {
			int clock = 0;
			int cpu;
			bool occupied = false;
			Process *running = processes[0].clone();
			int n_empty = queues_empty();

			// while processes or things in queue or cpu occupied
			while(processes.size() || n_empty >= 0 || occupied) {
				int i = 0;
				cout << "clock: " << clock << " n_empty: " << n_empty << endl;

				// add new processes to first queue
				// cout << "adding new processes" << endl;
				while(processes[i].arrival == clock && processes.size()) {
					int pid = add_to_queue_n(processes[i], 0);
					cout << "pid added: " << pid << endl;
					processes.erase(processes.begin());
					//i++;
				}

				// cout << "finished adding processes" << endl;

				// if no process in cpu, add one
				if(!occupied) {
					if(n_empty < 0) {
						cout << "no process to run, continue" << endl;
					} else {
						// cout << "occupied is false, add process to cpu" << endl;
						// cout << "n_empty = " << n_empty << endl;
						// cout << "size of queues[n_empty] = " << queues[n_empty].q.size() << endl;
						running = queues[n_empty].q.front().clone();
						queues[n_empty].q.pop();
						cpu = queues[n_empty].time_quantum;
						occupied = true;
						cout << "added pid: " << running->pid << " to cpu" << endl;
					}
				} else {
					// check if cpu == 0, if not decrement cpu and process burst
					if(cpu == 0) {
						occupied = false;
						if(n_empty + 1 > n_queues) {
							queues[n_empty].q.push(*running);
							cout << "time quantum over pid: " << running->pid << " added back to queue " << n_empty << endl;
						} else {
							queues[n_empty + 1].q.push(*running);
							cout << "time quantum over pid: " << running->pid << " added back to queue " << n_empty + 1 << endl;
						}
					} else {
						running->burst--;
						cout << "pid: " << running->pid << " burst: " << running->burst << endl;
						cout << "cpu: " << cpu << endl;
						if(running->burst == 0) {
							occupied = false;
							cout << "finished pid: " << running->pid << endl;
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
