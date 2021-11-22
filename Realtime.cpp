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
				// cout << "pid " << run_queue[i].pid << " not finished" << endl;
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

int Realtime::schedule() {
	ofstream gantt;
	gantt.open("gantt.txt");

	int size = processes.size();
	int not_finished = 0;
	int ran = 0;

	int clock = 0;
	bool occupied = false;
	Process *running = processes.back().clone();
	size_t avg_tt = 0;

	cout << "scheduling " << size << " processes."  << endl;
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
				// cout << "nothing in cpu, adding process" << endl;
				delete running;
				running = run_queue[min_index].clone();
				run_queue.erase(run_queue.begin() + min_index);
				occupied = true;

				//write to gantt file - beginning of process
				gantt << running->pid << "," << clock << ",";
			} else {
				//if run_queue[min_index] has earlier deadline, put it in the cpu
				//if deadlines are equal should we check burst?
				if(run_queue[min_index].deadline < running->deadline) {
					// cout << "swapping processes" << endl;
					run_queue.push_back(*running);
					delete running;
					running = run_queue[min_index].clone();
					run_queue.erase(run_queue.begin() + min_index);

					//write to gantt file - end of process and beginning of new process
					gantt << clock << endl << running->pid << "," << clock << ",";
				}
			}
		}

		//if there's something in the cpu, run it
		if(occupied) {
			running->burst--;
			if(hard && clock == running->deadline && running->burst > 0) {
				cout << "pid " << running->pid << " not finished; arrival " << running->arrival << "; deadline " << running->deadline << "; burst " << running->burst << "; clock " << clock << endl;
				not_finished++;

				//write to gantt file - end of process
				gantt << clock << endl;

				break;
			} else {
				if(running->burst == 0) {
					cout << "ran pid " << running->pid << "; arrival " << running->arrival << "; deadline " << running->deadline << "; clock " << clock << endl;
					occupied = false;
					ran++;
					avg_tt += (clock - running->arrival);

					//write to gantt file - end of processes
					gantt << clock << endl;

					// cout << "pid " << running->pid << " finished" << endl;
				}
			}
		}
		clock++;
	}

	delete running;

	gantt.close();


	cout << "number of processes: " << size << endl;
	cout << "number of processes ran: " << ran << endl;
	cout << "number of processes not finished: " << not_finished << endl;
	cout << "ran + not finished: " << ran + not_finished << endl;
	cout << "avg tt: " << avg_tt << endl;
	if(ran > 0) {
		cout << "Average turn around time: " << avg_tt / ran << endl;
	}
	cout << "clock ticks: " << clock << endl;

	return 0;
}
