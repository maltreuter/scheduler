#include "Realtime.h"

using namespace std;

Realtime::Realtime(bool hard, vector<Process> processes) {
	this->hard = hard;
	this->processes = processes;
}

Realtime::~Realtime() {

}

int Realtime::find_earliest_deadline(int clock, int &not_finished) {
	int min_deadline = -1;
	int min_index = -1;

	//loop through the run queue
	for(size_t i = 0; i < run_queue.size(); i++) {
		if(hard) {
			//if min_deadline has not been set, or there is a process with an earlier deadline, set new min_deadline
			if(min_deadline == -1 || run_queue[i].deadline < min_deadline) {
				min_deadline = run_queue[i].deadline;
				min_index = i;
			}
		} else {
			//if clock + burst > deadline, process will never complete, remove it
			if(clock + run_queue[i].burst > run_queue[i].deadline) {
				_DEBUG2(cout << "pid " << run_queue[i].pid << " not finished" << endl);
				run_queue.erase(run_queue.begin() + i);
				not_finished++;
			} else {
				if(min_deadline == -1 || run_queue[i].deadline < min_deadline) {
					min_deadline = run_queue[i].deadline;
					min_index = i;
				}
			}
		}
	}

	//either the index of the process in the queue with the earliest deadline
	//or -1, meaning there is nothing in the queue
	return min_index;
}

vector<tuple<int, int, int>> Realtime::schedule() {
	// ofstream gantt;
	// gantt.open("gantt.txt");

	// PID, cpu start clock, cpu end clock
	vector<tuple<int, int, int>> gantt_list;
	tuple<int, int, int> gantt_p;

	int size = processes.size();
	int not_finished = 0;
	int ran = 0;

	int clock = 0;
	bool occupied = false;
	Process *running = processes.back().clone();
	size_t avg_tt = 0;

	cout << "Scheduling " << size << " processes."  << endl;
	chrono::milliseconds timespan(2000);
	this_thread::sleep_for(timespan);


	//while processes or things in run queue or cpu occupied
	while(processes.size() || run_queue.size() || occupied) {
		//add all new processes to the run queue
		while(processes.size() && processes.back().arrival == clock) {
			run_queue.push_back(processes.back());
			processes.pop_back();
		}

		//find index of process with earliest deadline
		int min_index = find_earliest_deadline(clock, not_finished);

		//if something has changed
		if(min_index >= 0) {
			if(!occupied) {
				//cpu is empty, so put run_queue[min_index] in it
				_DEBUG2(cout << "nothing in cpu, adding process" << endl);
				delete running;
				running = run_queue[min_index].clone();
				run_queue.erase(run_queue.begin() + min_index);
				occupied = true;

				//write to gantt file - beginning of process
				gantt_p = make_tuple(running->pid, clock, 0);
			} else {
				//if run_queue[min_index] has earlier deadline, swap it with what's in the cpu
				if(run_queue[min_index].deadline < running->deadline) {
					_DEBUG2(cout << "swapping processes" << endl);
					run_queue.push_back(*running);
					delete running;
					running = run_queue[min_index].clone();
					run_queue.erase(run_queue.begin() + min_index);

					//write to gantt file - end of process and beginning of new process
					get<2>(gantt_p) = clock;
					gantt_list.push_back(gantt_p);
					gantt_p = make_tuple(running->pid, clock, 0);
				}
			}
		}

		//if there's something in the cpu, run it
		if(occupied) {
			running->burst--;

			//if process has reached its deadline and has not finished
			if(hard && clock == running->deadline && running->burst > 0) {
				_DEBUG2(cout << "pid " << running->pid << " not finished; arrival " << running->arrival << "; deadline " << running->deadline << "; burst " << running->burst << "; clock " << clock << endl);
				not_finished++;

				//write to gantt file - end of process
				get<2>(gantt_p) = clock + 1;
				gantt_list.push_back(gantt_p);

				break;
			} else {
				//process finished
				if(running->burst == 0) {
					_DEBUG2(cout << "ran pid " << running->pid << "; arrival " << running->arrival << "; deadline " << running->deadline << "; clock " << clock << endl);
					occupied = false;
					ran++;
					avg_tt += (clock - running->arrival);

					//write to gantt file - end of processes
					get<2>(gantt_p) = clock + 1;
					gantt_list.push_back(gantt_p);

					_DEBUG2(cout << "pid " << running->pid << " finished" << endl);
				}
			}
		}
		clock++;
	}

	delete running;


	cout << "Number of processes: " << size << endl;
	cout << "Number of processes ran: " << ran << endl;
	cout << "Number of processes not finished: " << not_finished << endl;
	if(ran > 0) {
		cout << "Average turn around time: " << avg_tt / ran << endl;
	}
	cout << "Number of clock ticks: " << clock << endl;

	return gantt_list;
}
