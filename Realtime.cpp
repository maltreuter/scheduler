#include "Realtime.h"
#include <algorithm>

using namespace std;

Realtime::Realtime(bool hard, vector<Process> processes) {
	this->hard = hard;
	this->processes = processes;
}

Realtime::~Realtime() {

}

// bool Realtime::sort_deadline(Process x, Process y) const {
// 	return x.deadline < y.deadline;
// }

int Realtime::schedule() {
	cout << "scheduling" << endl;
	int not_finished = 0;
	int clock = 0;
	bool occupied = false;
	Process *running = processes[0].clone();

	while(processes.size() || !run_queue.empty() || occupied) {
		cout << "got here" << endl;
		bool arrived = false;
		// processes arrived
		if(processes[0].arrival == clock) {
			cout << "processes arriving" << endl;
			arrived = true;
		}
		while(processes[0].arrival == clock) {
			run_queue.push_back(processes[0]);
			processes.erase(processes.begin());
		}

		if(arrived) {
			sort(run_queue.begin(), run_queue.end(), [ ] ( const auto& lhs, const auto& rhs)
			{
				cout << "sorting" << endl;
				return lhs.deadline < rhs.deadline;
			});

			if (occupied && running->deadline > run_queue.front().deadline) {
				run_queue.push_back(*running);
				running = run_queue.front().clone();
			}
		}

		if(!occupied) {
			// process running in cpu
			running = run_queue.front().clone();
			run_queue.erase(run_queue.begin());
			occupied = true;
			cout << "add process to cpu" << endl;
		} else {
			if(running->burst == 0) {
				// process finished burst
				occupied = false;
				cout << "process finished" << endl;
			} else if(running->deadline == clock) {
				// didnt finish before deadline
				cout << "process didn't finish before deadline" << endl;
				occupied = false;
				not_finished++;
			} else {
				running->burst--;
			}
		}

		clock++;
	}

	cout << "number of processes not finished: " << not_finished << endl;

	return 0;
}
