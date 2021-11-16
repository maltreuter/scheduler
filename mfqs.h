#ifndef MFQS_H
#define MFQS_H

#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

#include "Queue.h"

using namespace std;

class mfqs {
	public:
		int n_queues;
		int time_quantum;
		int io_time;
		int aging_time;
		vector<Process> processes;
		vector<Queue> queues;

		mfqs(int n_queues, int time_quantum, int io_time, int aging_time, vector<Process> processes) {
			this->n_queues = n_queues;
			this->time_quantum = time_quantum;
			this->io_time = io_time;
			this->aging_time = aging_time;
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

			vector<Process> io;
			int p_io = io_time;

			// stats
			int ran = 0;
			int pp_size = processes.size();

			cout << "Scheduling " << pp_size << " processes..." << endl;
			chrono::milliseconds timespan(5000);
			this_thread::sleep_for(timespan);

			// while processes or things in queue or cpu occupied or processes still doing io
			while(processes.size() || n_empty >= 0 || occupied || io.size()) {
				int i = 0;
				// cout << "clock: " << clock << " n_empty: " << n_empty << endl;

				// add new processes to first queue
				while(processes[i].arrival == clock && processes.size()) {
					int pid = add_to_queue_n(processes[i], 0);
					// cout << "pid added: " << pid << endl;
					processes.erase(processes.begin());
				}

				// Move processes from last queue to first queue at aging interval
				if(clock % aging_time == 0) {
					while(!queues[n_queues - 1].q.empty()) {
						Process tmp = queues[n_queues - 1].q.front();
						queues[n_queues - 1].q.pop();
						queues[0].q.push(tmp);
					}
				}

				// Check processes doing io
				for(i = 0; i < io.size(); i++) {
					// if 0, io finished, return to queue 0
					if(io[i].io == 0) {
						int pid = add_to_queue_n(io[i], 0);
						io.erase(io.begin() + i);
					} else {
						// decrement process io
						io[i].io--;
					}
				}

				// if no process in cpu, add one
				if(!occupied) {
					if(n_empty < 0) {
						// cout << "no process to run, continue" << endl;
					} else {
						running = queues[n_empty].q.front().clone();
						queues[n_empty].q.pop();

						if(queues[n_empty].rr) {
							cpu = queues[n_empty].time_quantum;
						} else {
							cpu = running->burst;
						}

						occupied = true;
						// cout << "added pid: " << running->pid << " to cpu" << endl;
					}
				} else {
					// check if cpu == 0, if not decrement cpu and process burst
					if(cpu == 0) {
						occupied = false;
						if(n_empty + 1 == n_queues) {
							queues[n_empty].q.push(*running);
							cout << "time quantum over pid: " << running->pid << " added back to queue " << n_empty << endl;
						} else {
							queues[n_empty + 1].q.push(*running);
							cout << "time quantum over pid: " << running->pid << " added back to queue " << n_empty + 1 << endl;
						}
					} else {
						running->burst--;
						p_io--;

						// check io time, if 0, add to io queue and reset p_io
						if(p_io == 0) {
							io.push_back(*running);
							occupied = false;
							p_io = io_time;
						}

						// check if processes burst is done
						if(running->burst == 0) {
							occupied = false;
							// cout << "finished pid: " << running->pid << endl;
							ran++;
						}

						cpu--;
					}
				}

				n_empty = queues_empty();
				clock++;
			}

			cout << "Scheduled " << ran << " processes out of " << pp_size << " in " << clock << " clock ticks" << endl;

			return 0;
		}
};

#endif
