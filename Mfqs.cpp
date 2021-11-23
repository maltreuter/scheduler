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

/**
 * @brief finds the first queue that is not empty, or returns -1 if all are empty
 * 
 * @return int index of the queue
 */
int Mfqs::queues_empty() {
	for(int i = 0; i < n_queues; i++) {
		if(queues[i].q.size() > 0) {
			return i;
		}
	}
	return -1;
}

/**
 * @brief adds a process to the given queue
 * 
 * @param p the process to be added
 * @param n the queue to be added to
 * @return int the pid of the added process
 */
int Mfqs::add_to_queue_n(Process p, int n) {
	// Check if queue exists, if not default to last queue
	if(n >= n_queues) {
		p.queue = n_queues - 1;
		queues[n_queues - 1].q.push(p);
	} else {
		p.queue = n;
		queues[n].q.push(p);
	}
	return p.pid;
}

/**
 * @brief add processes that are arriving to the first queue
 * 
 * @param clock the current clock tick
 * @param num_io the number of processes that have io
 */
void Mfqs::add_processes(int clock, int &num_io) {
	while(processes.size() && processes.back().arrival == clock) {
		if(processes.back().io > 0) {
			num_io++;
		}
		add_to_queue_n(processes.back(), 0);
		processes.pop_back();
	}
}

/**
 * @brief checks if the aging interval has been hit in the last queue and moves all of the processes to the first queue
 * 
 * @param clock the current clock tick
 */
void Mfqs::check_aging(int clock) {
	if(clock % aging_time == 0) {
		while(!queues[n_queues - 1].q.empty()) {
			Process tmp = queues[n_queues - 1].q.front();
			queues[n_queues - 1].q.pop();
			queues[0].q.push(tmp);
		}
	}
}

/**
 * @brief runs the io for processes in the io queue
 * 
 */
void Mfqs::do_io() {
	size_t i;
	for(i = 0; i < io.size(); i++) {
		// If 0, io finished, return to queue 0
		if(io[i].io == 0) {
			add_to_queue_n(io[i], 0);
			_DEBUG2(cout << "io finished for pid: " << io[i].pid << endl);
			io.erase(io.begin() + i);
		} else {
			// Decrement process io
			io[i].io--;
		}
	}
}

/**
 * @brief schedules the processes using MFQS
 * 
 * @return vector<tuple<int, int, int>> cpu enter and exit times for processes for the Gantt chart
 */
vector<tuple<int, int, int>> Mfqs::schedule() {
	int clock = 0;
	int cpu = -1;
	bool occupied = false;
	Process *running = processes.back().clone();
	int n_empty = queues_empty();

	// Gantt chart - pid, cpu start, cpu end
	vector<tuple<int, int, int>> gantt_list;
	tuple<int, int, int> gantt_p;

	// Stats
	int ran = 0;
	int pp_size = processes.size();
	size_t avg_tt = 0;
	size_t avg_wait = 0;
	int num_io = 0;
	int sent_io = 0;

	cout << "Scheduling " << pp_size << " processes..." << endl;
	chrono::milliseconds timespan(2000);
	this_thread::sleep_for(timespan);

	// While processes or things in queue or cpu occupied or processes still doing io
	while(processes.size() || n_empty >= 0 || occupied || io.size()) {
		// Add new processes to first queue
		add_processes(clock, num_io);

		// Move processes from last queue to first queue at aging interval
		check_aging(clock);

		// Check processes doing io
		do_io();

		n_empty = queues_empty();

		// If no process in cpu, add one
		if(!occupied) {
			if(n_empty < 0) {
				_DEBUG2(cout << "no process to run, continue" << endl);
			} else {
				// Get next process in line
				delete running;

				while(!occupied) {
					running = queues[n_empty].q.front().clone();
					if(running->burst == 1 && running->io > 0) {
						io.push_back(*running);
						sent_io++;
					} else {
						occupied = true;
					}
					queues[n_empty].q.pop();
				}

				// Set time slice for cpu
				if(queues[n_empty].rr) {
					cpu = queues[n_empty].time_quantum;
				} else {
					cpu = running->burst;
				}

				// Process added to cpu - gantt
				gantt_p = make_tuple(running->pid, clock, 0);
			}
		}

		if(occupied) {
			// CPU running...
			running->burst--;
			cpu--;

			if(running->burst == 1 && running->io > 0) {
				occupied = false;
				io.push_back(*running);
				sent_io++;

				// Removed from cpu to io - add to gantt list
				get<2>(gantt_p) = clock + 1;
				gantt_list.push_back(gantt_p);

				_DEBUG2(cout << "pid " << running->pid << " added to io list" << endl);
			} else if(running->burst == 0) {
				// Check if processes burst is done or if its time for io
				occupied = false;

				ran++;
				avg_tt += (clock - running->arrival);
				avg_wait += (clock - running->arrival - running->og_burst);

				// Process finished in cpu - add to gantt list
				get<2>(gantt_p) = clock + 1;
				gantt_list.push_back(gantt_p);

				_DEBUG2(cout << "finished pid: " << running->pid << endl);
			} else {
				// Check if time quantum is up, if not, continue running
				if(cpu == 0) {
					occupied = false;

					// Add to next queue
					add_to_queue_n(*running, running->queue + 1);
					_DEBUG2(cout << "time quantum over pid: " << running->pid << " added to queue " << running->queue + 1 << endl);

					// Time quantum over - add to gantt list
					get<2>(gantt_p) = clock + 1;
					gantt_list.push_back(gantt_p);
				}
			}

		}

		clock++;
	}

	delete running;

	cout << "\nScheduled " << ran << " processes out of " << pp_size << " in " << clock << " clock ticks" << endl;
	cout << "Average turn around time: " << avg_tt / ran << " clock ticks" << endl;
	cout << "Average wait time: " << avg_wait / ran << " clock ticks" << endl;
	cout << "Processes with io: " << num_io << endl;
	cout << "Processes that did io: " << sent_io << endl;

	return gantt_list;
}
