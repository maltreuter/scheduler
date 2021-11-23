#include "Mfqs.h"

using namespace std;

Mfqs::Mfqs(int n_queues, int time_quantum, int aging_time, vector<Process> processes) {
	this->n_queues = n_queues;
	this->time_quantum = time_quantum;
	this->aging_time = aging_time;
	this->processes = processes;

	this->queues = vector<Queue>(n_queues);
	for(int i = 0; i < n_queues - 1; i++) {
		queues[i].time_quantum = time_quantum;
		time_quantum = time_quantum * 2;
	}
	queues[n_queues - 1].rr = 0;
}

Mfqs::~Mfqs() {

}

int Mfqs::queues_empty() {
	for(int i = 0; i < n_queues; i++) {
		if(queues[i].q.size() > 0) {
			return i;
		}
	}
	return -1;
}

int Mfqs::add_to_queue_n(Process p, int n) {
	// check if queue exists, if not default to last queue
	if(n >= n_queues) {
		p.queue = n_queues - 1;
		queues[n_queues - 1].q.push(p);
	} else {
		p.queue = n;
		queues[n].q.push(p);
	}
	return p.pid;
}

void Mfqs::add_processes(int clock, int &num_io) {
	while(processes.size() && processes.back().arrival == clock) {
		if(processes.back().io > 0) {
			num_io++;
		}
		add_to_queue_n(processes.back(), 0);
		processes.pop_back();
	}
}

void Mfqs::check_aging(int clock) {
	if(clock % aging_time == 0) {
		while(!queues[n_queues - 1].q.empty()) {
			Process tmp = queues[n_queues - 1].q.front();
			queues[n_queues - 1].q.pop();
			queues[0].q.push(tmp);
		}
	}
}

void Mfqs::do_io() {
	size_t i;
	for(i = 0; i < io.size(); i++) {
		// if 0, io finished, return to queue 0
		if(io[i].io == 0) {
			add_to_queue_n(io[i], 0);
			_DEBUG2(cout << "io finished for pid: " << io[i].pid << endl);
			io.erase(io.begin() + i);
		} else {
			// decrement process io
			io[i].io--;
		}
	}
}

vector<tuple<int, int, int>> Mfqs::schedule() {
	int clock = 0;
	int cpu = -1;
	bool occupied = false;
	Process *running = processes.back().clone();
	int n_empty = queues_empty();

	// gantt chart - pid, cpu start, cpu end
	vector<tuple<int, int, int>> gantt_list;
	tuple<int, int, int> gantt_p;

	// stats
	int ran = 0;
	int pp_size = processes.size();
	size_t avg_tt = 0;
	int num_io = 0;
	int sent_io = 0;

	cout << "Scheduling " << pp_size << " processes..." << endl;
	chrono::milliseconds timespan(2000);
	this_thread::sleep_for(timespan);

	// while processes or things in queue or cpu occupied or processes still doing io
	while(processes.size() || n_empty >= 0 || occupied || io.size()) {
		// add new processes to first queue
		add_processes(clock, num_io);

		// Move processes from last queue to first queue at aging interval
		check_aging(clock);

		// Check processes doing io
		do_io();

		n_empty = queues_empty();

		// if no process in cpu, add one
		if(!occupied) {
			if(n_empty < 0) {
				_DEBUG2(cout << "no process to run, continue" << endl);
			} else {
				// get next process in line
				delete running;
				running = queues[n_empty].q.front().clone();
				occupied = true;
				queues[n_empty].q.pop();

				// set time slice for cpu
				if(queues[n_empty].rr) {
					cpu = queues[n_empty].time_quantum;
				} else {
					cpu = running->burst;
				}

				// process added to cpu - gantt
				gantt_p = make_tuple(running->pid, clock, 0);
			}
		}

		if(occupied) {
			// cpu running...
			if(running->burst == 1 && running->io > 0) {
				occupied = false;
				io.push_back(*running);
				sent_io++;

				//again, should we add a new process to the cpu here?

				// removed from cpu to io - add to gantt list
				get<2>(gantt_p) = clock;
				gantt_list.push_back(gantt_p);

				_DEBUG2(cout << "pid " << running->pid << " added to io list" << endl);
			} else {
				running->burst--;
				cpu--;

				if(running->burst == 0) {
					// check if processes burst is done or if its time for io
					occupied = false;

					ran++;
					avg_tt += (clock - running->arrival);

					// process finished in cpu - add to gantt list
					get<2>(gantt_p) = clock;
					gantt_list.push_back(gantt_p);

					_DEBUG2(cout << "finished pid: " << running->pid << endl);
				} else {
					// check if time quantum is up, if not, continue running
					if(cpu == 0) {
						occupied = false;

						//add to next queue
						add_to_queue_n(*running, running->queue + 1);
						_DEBUG2(cout << "time quantum over pid: " << running->pid << " added to queue " << running->queue << endl);

						// time quantum over - add to gantt list
						get<2>(gantt_p) = clock;
						gantt_list.push_back(gantt_p);
					}
				}
			}
		}

		clock++;
	}

	delete running;

	cout << "Scheduled " << ran << " processes out of " << pp_size << " in " << clock << " clock ticks" << endl;
	cout << "Average turn around time: " << avg_tt / ran << " clock ticks" << endl;
	cout << "Processes with io: " << num_io << endl;
	cout << "Processes that did io: " << sent_io << endl;

	return gantt_list;
}
